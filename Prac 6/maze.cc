#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;

void maze_naive() {

}

void maze_memo() {

}

void maze_it_matrix() {

}

void maze_it_vector() {

}

void maze_parser() {

}

//-------------------------Métodos personales-----------------------------
bool existe_parametro(int argc, char* argv[], const char* parametro) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], parametro) == 0) { // si se encuentra el parámetro
            if (strcmp(argv[i], "-f") == 0 && i + 1 < argc && argv[i + 1][0] != '-') {
                // si es el parámetro -f, comprobar que el siguiente no empieza por '-'
                return true;
            } else if (strcmp(argv[i], "-f") != 0) {
                // si no es el parámetro -f, devolver true directamente
                return true;
            }
        }
    }
    return false; // si no se encuentra el parámetro
}


//------------------------------------------------------------------------

int main(int argc, char* argv[]) {

    if(!existe_parametro(argc, argv, "-f")) {
        cerr << "ERROR: -f <file_name> needed" << endl;
        exit(EXIT_FAILURE);
    }

    int pos_file = 0;
    for(int i=0; i<argc; i++) {
        if(strcmp(argv[i], "-f") == 0) {
            pos_file = i+1;
        }
    }

    return 0;
}