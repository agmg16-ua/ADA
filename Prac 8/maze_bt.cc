//Alejandro Guillén Merino      DNI: 48790456G
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>

using namespace std;

//----------------------------------Metodos Personales----------------------------------------

bool checkParams(int argc, char* argv[]) {
    bool f = false;
    string filename = "";

    for (int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-f") == 0) {
            if(i < argc - 1) {
                f = true;
                filename = argv[i+1];
                i++;
            } else {
                cerr << "ERROR: missing filename." << endl
                << "Usage: " << endl
                << "maze_bt [-p] [--p2D] -f fichero_entrada" << endl;
                exit(EXIT_FAILURE);
            }
        } else if (!((strcmp(argv[i], "-p") == 0) || (strcmp(argv[i], "--p2D") == 0))) {
            cerr << "ERROR: unknown option " << argv[i] << '.' << endl
            << "Usage: " << endl
            << "maze_bt [-p] [--p2D] -f fichero_entrada" << endl;
            exit(EXIT_FAILURE); 
        }
    }

    if (!f || filename == "") {
        cerr << "Usage: " << endl
        << "maze_bt [-p] [--p2D] -f fichero_entrada" << endl;
        exit(EXIT_FAILURE);
    }

    ifstream infile(filename);
    if (!infile.good()) {
        cerr << "ERROR: can't open file: " << filename << '.' << endl
            << "Usage: " << endl
            << "maze_bt [-p] [--p2D] -f fichero_entrada" << endl;
        exit(EXIT_FAILURE);
    }

    return true;
}

int param_position(int argc, char* argv[], const char* param) {
    int position = -1;

    for (int i = 1; i < argc; i++) {
        if (strcmp(param, argv[i]) == 0) {
            position = i;
            break;
        }
    }

    return position;
}

vector<vector<int>> get_data_file(string fileName) {
    ifstream file(fileName);
    int rows, cols;
    file >> rows >> cols; // read first line for dimensions

    vector<vector<int>> file_matrix(rows, vector<int>(cols));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            file >> file_matrix[i][j];
        }
    }

    file.close();

    return file_matrix;
}

//------------------------------------------------------------------------------



void print_result() {

}

int main(int argc, char* argv[]) {

    checkParams(argc, argv);

    int file_position = param_position(argc, argv, "-f") + 1;

    vector<vector<int>> matrix = get_data_file(argv[file_position]);



    print_result();

    return 1;

}

