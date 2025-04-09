#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

// Convierte un binario a decimal (con signo)
int binario_decimal(const vector<int>& bin) {
    int n=bin.size();
    int resultado=0;
    for(int i=0;i<n;++i)
        resultado+=bin[i]<<(n-i-1);
    if(bin[0]==1)
        resultado-=(1<<n);
    return resultado;
}

// Convierte un entero a binario en complemento a 2 con n bits
vector<int> decimal_binario(int num,int n) {
    vector<int> bin(n);
    if(num<0) num=pow(2,n)+num;
    for(int i=n-1;i>=0;--i) {
        bin[i]=num%2;
        num/=2;
    }
    return bin;
}

// Suma binaria de dos vectores (n bits)
vector<int> sumarBinarios(const vector<int>& primerNumero,const vector<int>& segundoNumero) {
    int cantidadBits=primerNumero.size();
    vector<int> resultado(cantidadBits);
    int a=0;
    for(int i=cantidadBits-1;i>=0;--i) {
        int suma=primerNumero[i]+segundoNumero[i]+a;
        resultado[i]=suma%2;
        a=suma/2;
    }
    return resultado;
}

// Complemento a 2
vector<int> complementoDos(const vector<int>& binario) {
    int n=binario.size();
    vector<int> uno(n,0);
    uno[n-1]=1;
    vector<int> inverso(n);
    for(int i=0;i<n;++i) {
        inverso[i]=(binario[i]==0)?1:0;
    }
    vector<int> complemento=sumarBinarios(inverso,uno);
    return complemento;
}

// Corrimiento aritmético a la derecha sobre A, Q y Q-1
void corrimientoDerecha(vector<int>& registroA,vector<int>& registroQ,int& bitQ_1) {
    int bitMSB_A=registroA[0];
    bitQ_1=registroQ.back();
    for(int i=registroQ.size()-1;i>0;--i) {
        registroQ[i]=registroQ[i-1];
    }
    registroQ[0]=registroA.back();
    for(int i=registroA.size()-1;i>0;--i) {
        registroA[i]=registroA[i-1];
    }
    registroA[0]=bitMSB_A;
}

// Imprime binario
void imprimir(const vector<int>& bin) {
    for(int bit:bin) cout<<bit;
}

// Algoritmo de Booth
void multiplicacionBooth(int M,int Q,int n) {
    vector<int> A(n,0);
    vector<int> Q_bin=decimal_binario(Q,n);
    int Q_1=0;
    vector<int> M_bin=decimal_binario(M,n);
    vector<int> M_negativo=decimal_binario(-M,n);

    cout<<"Paso inicial: A=";
    for(int bit:A) cout<<bit;
    cout<<", Q=";
    for(int bit:Q_bin) cout<<bit;
    cout<<", Q-1="<<Q_1<<endl;

    for(int i=0;i<n;++i) {
        int Q0=Q_bin.back();
        if(Q0==1&&Q_1==0) {
            A=sumarBinarios(A,M_negativo);
            cout<<"A=A-M  -> A=";
            for(int bit:A) cout<<bit;
            cout<<endl;
        }
        else if(Q0==0&&Q_1==1) {
            A=sumarBinarios(A,M_bin);
            cout<<"A=A+M  -> A=";
            for(int bit:A) cout<<bit;
            cout<<endl;
        }
        corrimientoDerecha(A,Q_bin,Q_1);
        cout<<"Corrimiento -> A=";
        for(int bit:A) cout<<bit;
        cout<<", Q=";
        for(int bit:Q_bin) cout<<bit;
        cout<<", Q-1="<<Q_1<<endl;
    }

    vector<int> resultado=A;
    resultado.insert(resultado.end(),Q_bin.begin(),Q_bin.end());

    cout<<"\nResultado binario: ";
    for(int bit:resultado) cout<<bit;
    cout<<"\nResultado decimal: "<<binario_decimal(resultado)<<endl;
}

int main() {
    int M,Q,n;
    cout<<"Ingrese el multiplicando (M): "; cin>>M;
    cout<<"Ingrese el multiplicador (Q): "; cin>>Q;
    cout<<"Ingrese el número de bits (n): "; cin>>n;

    multiplicacionBooth(M,Q,n);

    return 0;
}
