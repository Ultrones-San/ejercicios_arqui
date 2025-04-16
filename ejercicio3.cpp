#include<iostream>
#include<iomanip>
#include<bitset>
#include<cmath>
#include<cstdint>
using namespace std;

union FloatBits{
    float value;
    uint32_t bits;
};

// imprimir el valor en float y su representacion en bits
void imprimeFloatBits(const FloatBits& fb,const string& nombre){
    cout<<nombre<<"="<<fb.value<<"|bits:"<<bitset<32>(fb.bits)<<endl;
}
int main(){
    FloatBits x,y,a;
    cout<<"ingrese x:";
    cin>>x.value;
    cout<<"ingrese y:";
    cin>>y.value;

    imprimeFloatBits(x,"x");
    imprimeFloatBits(y,"y");

    // separacion de signo, exponente y significando
    uint32_t signox=(x.bits>>31)&0x1;
    uint32_t expox=(x.bits>>23)&0xFF;
    uint32_t sigx=x.bits&0x7FFFFF;
    uint32_t signoy=(y.bits>>31)&0x1;
    uint32_t expoy=(y.bits>>23)&0xFF;
    uint32_t sigy=y.bits&0x7FFFFF;

    // agregar bit implicito si no es denormalizado
    if(expox!=0)sigx|=(1<<23);
    if(expoy!=0)sigy|=(1<<23);

    // revisa ceros
    if(x.value==0.0f){
        a.bits=(signox^signoy)<<31;
        imprimeFloatBits(a,"resultado");
        return 0;
    }
    if(y.value==0.0f){
        a.bits=((signox^signoy)<<31)|0x7F800000; // infinito
        imprimeFloatBits(a,"resultado(infinito)");
        return 0;
    }

    // resta de exponentes en biased
    int32_t bias=127;
    int32_t expa=expox-expoy+bias;

    // verificacion de overflow/underflow
    if(expa>=255){
        cout<<"overflowdelexponente."<<endl;
        a.bits=((signox^signoy)<<31)|0x7F800000;
        imprimeFloatBits(a,"resultado");
        return 0;
    }
    else if(expa<=0){
        cout<<"underflowdelexponente."<<endl;
        a.bits=(signox^signoy)<<31;
        imprimeFloatBits(a,"resultado");
        return 0;
    }

    // division de significandos
    uint64_t siga=((uint64_t)sigx<<23)/sigy;

    // normalizacion
    while((siga&(1<<23))==0){
        siga<<=1;
        expa--;
    }
    siga&=0x7FFFFF;

    // ensamblaje del resultado final
    a.bits=((signox^signoy)<<31)|((expa&0xFF)<<23)|(siga&0x7FFFFF);
    imprimeFloatBits(a,"resultado");

    // comparacion con el resultado real del lenguaje
    FloatBits real={x.value/y.value};
    imprimeFloatBits(real,"resultado_real");

    return 0;
}
