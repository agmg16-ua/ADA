#include <iostream>
#include <math.h>

using namespace std;

long pasos = 0;

unsigned long pow2_1(unsigned n) {
    pasos++;

    if(n != 0) {
        return 2 * pow2_1(n - 1);
    } else {
        return 1;
    }
}

unsigned long pow2_2(unsigned n) {
    pasos++;

    if (n == 0) {
        return 1;
    } else {

        int aux = pow2_2(n/2);
        
        if (n % 2 == 0) {
            return aux * aux;
        } else { 
            return 2 * aux * aux;
        }
    }
    
}

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
    //Con intencion de hacer la gráfica, se van a utilizar los exponentes 10, 25, 50 y 100
    cout << "#n    pow2_1  pow2_2   pow2_3" << endl
        << "#--------------------------------------" << endl;
    for(n = 1; n<=25; n++) {

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