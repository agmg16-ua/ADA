//Alejandro Guillén Merino      DNI:48790456G
#include <iostream>
#include <math.h>

using namespace std;

long pasos = 0;

//Coste: \Tetha(n)
unsigned long pow2_1(unsigned n) {
    pasos++;

    if(n != 0) {
        return 2 * pow2_1(n - 1);
    } else {
        return 1;
    }
}

//Coste: \Tetha(log2(n))
unsigned long pow2_2(unsigned n) {
    pasos++;

    if (n == 0) {
        return 1;
    } else {

        unsigned long aux = pow2_2(n/2);
        
        if (n % 2 == 0) {
            return aux * aux;
        } else { 
            return 2 * aux * aux;
        }
    }
    
}

//Coste: \Tetha(2^n)
unsigned long pow2_3(unsigned n) {
    pasos++;

    if(n != 0) {
        return pow2_3(n - 1) + pow2_3(n - 1);
    } else {
        return 1;
    }
}

int main() {
    unsigned long n;
    //Se ha probado en los ordenadores de la eps y con n=20 tarda menos de 5 segundos, aunque se puede alcanzar n=29 en menos de 5 segundos en mi portatil
    cout << "#n    pow2_1  pow2_2   pow2_3" << endl
        << "#--------------------------------------" << endl;
    for(n = 1; n<=20; n++) {

        cout << n << "    \t";

        //pow2_1
        pasos = 0;
        if(pow2_1(n) != pow(2, n)) {
            cerr << "Panic, diferent result of pow(2, " << n << ')' << endl;
            exit(EXIT_FAILURE);
        }
        cout << pasos << "\t\t" << std::flush;

        //pow2_2
        pasos = 0;
        if(pow2_2(n) != pow(2, n)) {
            cerr << "Panic, diferent result of pow(2, " << n << ')' << endl;
            exit(EXIT_FAILURE);
        }
        cout << pasos << "\t\t" << std::flush;

        //pow2_3
        pasos = 0;
        if(pow2_3(n) != pow(2, n)) {
            cerr << "Panic, diferent result of pow(2, " << n << ')' << endl;
            exit(EXIT_FAILURE);
        }       
        cout << pasos << endl;

    }

    return 0;

}