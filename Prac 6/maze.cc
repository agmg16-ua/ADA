//Alejandro Guillén Merino      DNI: 48790456G
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>

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
bool checkParams(int argc, char* argv[]) {
    bool p = false, t = false, ignore_misive = false, f = false;
    string filename = "";

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0) {
            p = true;
        } else if (strcmp(argv[i], "-t") == 0) {
            t = true;
        } else if (strcmp(argv[i], "--ignore-misive") == 0) {
            ignore_misive = true;
        } else if (strcmp(argv[i], "-f") == 0) {
            if(i < argc - 1) {
                f = true;
                filename = argv[i+1];
                i++;
            } else {
                cerr << "ERROR: missing filename." << endl
                << "Usage: " << endl
                << "maze [-p] [-t] [--ignore-misive] -f file" << endl;
                exit(EXIT_FAILURE);
            }
           
        } else {
            cerr << "ERROR: unknown option " << argv[i] << '.' << endl
            << "Usage: " << endl
            << "maze [-p] [-t] [--ignore-misive] -f file" << endl;
        exit(EXIT_FAILURE);
        }
    }

    if (!f || filename == "") {
        cerr << "Usage: " << endl
            << "maze [-p] [-t] [--ignore-misive] -f file" << endl;
        exit(EXIT_FAILURE);
    }

    ifstream infile(filename);
    if (!infile.good()) {
        cerr << "ERROR: can't open file: " << filename << '.' << endl
            << "Usage: " << endl
            << "maze [-p] [-t] [--ignore-misive] -f file" << endl;
        exit(EXIT_FAILURE);
    }

    return true;
}


//------------------------------------------------------------------------

int main(int argc, char* argv[]) {

    if(checkParams(argc, argv)) {
        cout << "It works!" << endl;
    }

    return 0;
}