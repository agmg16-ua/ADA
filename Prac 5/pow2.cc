#include <iostream>
#include <math.h>

using namespace std;

long pasos = 0;

//Se realizan n llamadas recursivas
unsigned long pow2_1(unsigned n) {
    unsigned long resultado = 1;
    pasos++;

    if(n != 0) {
        resultado = pow2_1(n - 1) * 2;
    } else {
        resultado = 1;
    }

    return resultado;
}

unsigned long pow2_2(unsigned n) {
    unsigned long resultado;

    pasos++;

    if(n != 0) {
        resultado = pow2_2(n - 1) + pow2_2(n - 1);
    } else {
        resultado = 1;
    }
    
    return resultado;
}

unsigned long pow2_3(unsigned n) {

    pasos++;
    if (n == 0) {
        return 1;
    } else {

        // Calculamos la potencia para la mitad del exponente
        int aux = pow2_3(n/2);
        
        // Si el exponente es par, devolvemos el cuadrado de la potencia para la mitad
        if (n % 2 == 0) {
            return aux * aux;
        } else {  // Si el exponente es impar, devolvemos la potencia para la mitad por 2
            return 2 * aux * aux;
        }
    }
    


}

int main() {
    unsigned long n;
    //Con intencion de hacer la gráfica, se van a utilizar los exponentes 10, 25, 50 y 100
    cout << "n    pow2_1  pow2_2   pow2_3" << endl
        << "--------------------------------------" << endl;
    for(n = 0; n<=20; n++) {

        cout << n << "    \t";

        //pow2_1
        pasos = 0;
        if(pow2_1(n) != pow(2, n)) {
            cerr << "Panic, diferent result of pow(2, " << n << ')' << endl;
            exit(EXIT_FAILURE);
        }
        cout << pasos << "\t";

        //pow2_2
        pasos = 0;
        if(pow2_2(n) != pow(2, n)) {
            cerr << "Panic, diferent result of pow(2, " << n << ')' << endl;
            exit(EXIT_FAILURE);
        }
        cout << pasos << "\t";

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