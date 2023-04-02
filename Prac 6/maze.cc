//Alejandro Guillén Merino      DNI: 48790456G
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>
#include <climits>
#include <iomanip>

using namespace std;

//El método recursivo memo_table obtiene correctamente valor del camino más corto. Sin embargo, no he logrado que se genere la tabla de memoizacion correctamente

//-------------------------Métodos personales-----------------------------
//Comprueba que los parametros introducidos sean correctos
// -f file -> archivo con maze
// --ignore-misive -> el valor de maze_misive es -
// -p -> muestra salida en forma de matriz con *
// -t -> muestra la tabla de memoizacion o iterativa
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
                << "maze [-p] [-t] [--ignore-naive] -f file" << endl;
                exit(EXIT_FAILURE);
            }
        } else if (!((strcmp(argv[i], "-p") == 0) || (strcmp(argv[i], "-t") == 0) || (strcmp(argv[i], "--ignore-naive") == 0))) {
            cerr << "ERROR: unknown option " << argv[i] << '.' << endl
            << "Usage: " << endl
            << "maze [-p] [-t] [--ignore-naive] -f file" << endl;
            exit(EXIT_FAILURE); 
        }
    }

    if (!f || filename == "") {
        cerr << "Usage: " << endl
            << "maze [-p] [-t] [--ignore-naive] -f file" << endl;
        exit(EXIT_FAILURE);
    }

    ifstream infile(filename);
    if (!infile.good()) {
        cerr << "ERROR: can't open file: " << filename << '.' << endl
            << "Usage: " << endl
            << "maze [-p] [-t] [--ignore-naive] -f file" << endl;
        exit(EXIT_FAILURE);
    }

    return true;
}

//Devuelve la posicion de un parametro. Si es -1, no existe
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

//Lee el archivo y devuelve la matriz que contiene
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

//En desarrollo: Se utilizará para imprimir el camino marcado con '*'
void print_path(vector<vector<int>> matriz, vector<vector<int>> memo, int contador) {

    for(int i=matriz.size()-1; i>=0; i--) {
        for(int j=matriz[0].size()-1; j>=0; j--) {
            if(memo[i][j] == contador) {
                cout << '*';
                contador--;
            } else {
                cout << matriz[i][j];
            }
        }
        cout << endl;
    }
}

//------------------------------------------------------------------------

int maze_naive(vector<vector<int>>& matriz, unsigned row, unsigned col) {

    if(matriz.size()-1 == row && matriz[0].size()-1 == col-1) {
        return 0;
    }

    if(row >= matriz.size() || col >= matriz[0].size() || matriz[row][col] == 0) { 
        return INT_MAX-1;
    }
    
    matriz[row][col] = 0;

    int derecha = maze_naive(matriz, row, col+1);
    int abajo = maze_naive(matriz, row+1, col);
    int diag = maze_naive(matriz, row+1, col+1);

    matriz[row][col] = 1;

    
    return min(min(derecha, abajo), diag) + 1;
} 

vector<vector<int>> maze_memo(vector<vector<int>> &matriz) {
    int n = matriz.size();
    int m = matriz[0].size();
    vector<vector<int>> auxiliar(n, vector<int>(m, -1));
    const int inf = 2 * n * m;
    if (matriz[n-1][m-1] == 0) {
        return auxiliar;
    }
    auxiliar[n-1][m-1] = 0;
    for (int i = n-2; i >= 0; i--) {
        if (matriz[i][m-1] == 1 && auxiliar[i+1][m-1] != -1 && matriz[i+1][m-1] == 1) {
            auxiliar[i][m-1] = auxiliar[i+1][m-1] + 1;
        } else {
            auxiliar[i][m-1] = inf;
        }
    }
    for (int j = m-2; j >= 0; j--) {
        if (matriz[n-1][j] == 1 && auxiliar[n-1][j+1] != -1 && matriz[n-1][j+1] == 1) {
            auxiliar[n-1][j] = auxiliar[n-1][j+1] + 1;
        } else {
            auxiliar[n-1][j] = inf;
        }
    }
    for (int i = n-2; i >= 0; i--) {
        for (int j = m-2; j >= 0; j--) {
            if (matriz[i][j] == 0) {
                auxiliar[i][j] = -2;
            } else {
                int abajo = inf;
                int derecha = inf;
                int diag = inf;
                if(auxiliar[i+1][j] != -1 && matriz[i+1][j] == 1) {
                    abajo = auxiliar[i+1][j];
                }

                if(auxiliar[i][j+1] != -1 && matriz[i][j+1] == 1) {
                    derecha = auxiliar[i][j+1];
                }

                if(auxiliar[i+1][j+1] != -1 && matriz[i+1][j+1] == 1) {
                    diag = auxiliar[i+1][j+1];
                }
                
                auxiliar[i][j] = min(min(abajo, derecha), diag) + 1;
            }
        }
    }
    return auxiliar;
}

void maze_it_matrix() {

}

void maze_it_vector() {

}

//Se usa para imprimir los datos en pantalla, y tambien calculará el camino con '*' utilizando una funcion auxiliar
void maze_parser(int argc, char* argv[], vector<vector<int>> file_matrix, int naive, vector<vector<int>> memo_table) {
    int memo = memo_table[0][0] + 1;

    if(naive == -1) {
        cout << '-';
    } else {
        cout << naive;
    }

    cout << ' ';

    cout << memo << " ? ?" << endl;

    if(param_position(argc, argv, "-p") != -1) {
        /*int contador = memo-1;
        print_path(file_matrix, memo_table, contador);*/
        cout << "?" << endl;
    }

    if(param_position(argc, argv, "-t") != -1) {
        cout << "Memoization table:" << endl;
        for(unsigned i = 0; i < file_matrix.size(); i++) {
            for(unsigned j = 0; j < file_matrix[0].size(); j++) {
                if(memo_table[i][j] >= 0 && memo_table[i][j] < memo) {
                    cout << setw(5) << memo - memo_table[i][j];
                } else if(memo_table[i][j] < 0) {
                    cout << setw(5) << "X";
                } else {
                    cout << setw(5) << "-";
                }
            
            }
            cout << endl;
        }

        cout << "Iterative table:" << endl;
        cout << "?" << endl;
    } 


    cout << endl;

}



int main(int argc, char* argv[]) { 

    checkParams(argc, argv); //Se comprueba que los parámetros son correctos

    int naive;
    int file_position = param_position(argc, argv, "-f") + 1;

    vector<vector<int>> file_matrix = get_data_file(argv[file_position]);

    //maze_naive-------------------------------------------------------------------------------
    if(param_position(argc, argv, "--ignore-naive") == -1) {
        naive = maze_naive(file_matrix, 0, 0);
    } else {
        naive = -1;
    }

    //maze_mamo-------------------------------------------------------------------------------
    vector<vector<int>> aux_memo(file_matrix.size(), vector<int>(file_matrix[0].size()));

    for(unsigned i=0; i<aux_memo.size(); i++) {
        for(unsigned j=0; j<aux_memo[0].size(); j++) {
            aux_memo[i][j] = file_matrix[i][j];
        }
    }
    
    vector<vector<int>> memo_table = maze_memo(aux_memo);
    
    //print-------------------------------------------------------------------------------------

    maze_parser(argc, argv, file_matrix, naive, memo_table);

    return 0;
}