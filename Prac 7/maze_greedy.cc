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
                << "maze_greedy [-p] -f file" << endl;
                exit(EXIT_FAILURE);
            }
        } else if (!(strcmp(argv[i], "-p") == 0)) {
            cerr << "ERROR: unknown option " << argv[i] << '.' << endl
            << "Usage: " << endl
            << "maze_greedy [-p] -f file" << endl;
            exit(EXIT_FAILURE); 
        }
    }

    if (!f || filename == "") {
        cerr << "Usage: " << endl
            << "maze_greedy [-p] -f file" << endl;
        exit(EXIT_FAILURE);
    }

    ifstream infile(filename);
    if (!infile.good()) {
        cerr << "ERROR: can't open file: " << filename << '.' << endl
            << "Usage: " << endl
            << "maze_greedy [-p] -f file" << endl;
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

//-----------------------------------------------------------------------------------------------

int maze_greedy(vector<vector<int>> &matrix, int row, int col) {
    int longitud = 0;
    bool seguir = true;
    int filas = matrix.size();
    int columnas = matrix[0].size();

    if(matrix[row][col] == 0) {
        return longitud;
    }

    while(seguir) {
        //Condición especial última posicion
        if(row == filas-1 && col == columnas-1) {
            matrix[row][col] = -1;
            longitud++;
            break;
        }

        //Condicion especial ultima columna
        if(col == columnas - 1) {
            if(matrix[row+1][col] == 0){
                matrix[row][col] = -1;
                return 0;
            }

            matrix[row][col] = -1;
            row++;
            longitud++;
        } else if(row == filas - 1) {
            if(matrix[row][col+1] == 0) {
                matrix[row][col] = -1;
                return 0;
            }

            matrix[row][col] = -1;
            col++;
            longitud++;
        } else {
            if(matrix[row+1][col+1] == 1) {
                matrix[row][col] = -1;
                row++;
                col++;
                longitud++;
            } else if(matrix[row+1][col] == 1) {
                matrix[row][col] = -1;
                row++;
                longitud++;
            } else if(matrix[row][col+1] == 1) {
                matrix[row][col] = -1;
                col++;
                longitud++;
            } else {
                matrix[row][col] = -1;
                return 0;
            }
        }
    }

    return longitud;
}

//Si no se puede mover en diagonal, se comprueba primero si se puede desplazar abajo y si tampoco puede a la derecha.
int main(int argc, char* argv[]) {

    checkParams(argc, argv);

    int file_position = param_position(argc, argv, "-f") + 1;

    vector<vector<int>> matrix = get_data_file(argv[file_position]);

    cout << maze_greedy(matrix, 0, 0) << endl;

    if(param_position(argc, argv, "-p") != -1) {
        for(unsigned i = 0; i<matrix.size(); i++) {
            for(unsigned j=0; j<matrix[0].size(); j++) {
                if(matrix[i][j]==-1) {
                    cout << '*';
                } else {
                    cout << matrix[i][j];
                }
            }
            cout << endl;
        }
    }



    return 0;
}
