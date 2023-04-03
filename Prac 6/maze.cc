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

int maze_memo(vector<vector<int>> matriz, vector<vector<int>> &auxiliar, int row, int col) {
    int filas = matriz.size();
    int columnas = matriz[0].size();
    
    if(auxiliar[row][col] != -1) {
        return auxiliar[row][col];
    }

    if(matriz[filas-1][columnas-1] == 0 || matriz[0][0] == 0) {
        auxiliar[row][col] = 0;
        return auxiliar[row][col];
    }

    if(matriz[row][col] == 0) {
        auxiliar[row][col] = INT_MAX-1;
        return auxiliar[row][col];
    }

    if(row == filas-1 && col == columnas-1) {
        auxiliar[row][col] = 1;
        return auxiliar[row][col];
    }

    if(row == filas-1) {
        int der = maze_memo(matriz, auxiliar, row, col+1);
        if(der == INT_MAX-1) {
            auxiliar[row][col] = der;
            return auxiliar[row][col];
        }

        auxiliar[row][col] = der + 1;
        return auxiliar[row][col];
    }

    if(col == columnas-1) {
        int abajo = maze_memo(matriz, auxiliar, row+1, col);
        if(abajo == INT_MAX-1) {
            auxiliar[row][col] = abajo;
            return auxiliar[row][col];
        }

        auxiliar[row][col] = abajo + 1;
        return auxiliar[row][col];
    }

    int der = maze_memo(matriz, auxiliar, row, col+1);
    int abajo = maze_memo(matriz, auxiliar, row+1, col);
    int diag = maze_memo(matriz, auxiliar, row+1, col+1);

    int result = min(der, min(abajo, diag));

    if(result == INT_MAX - 1) {
        auxiliar[row][col] = result;
        return auxiliar[row][col];
    }

    auxiliar[row][col] = result + 1;
    return auxiliar[row][col];
}

vector<vector<int>> maze_it_matrix(vector<vector<int>> &matriz) {
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

int maze_it_vector(vector<vector<int>> matriz, vector<int> anterior, int row) {
    int n = matriz.size();
    int m = matriz[0].size();

    return 0;
}

void maze_parser() {
    cout << "?" <<  endl;
}

//-------------------------------------------IMPRIMIR TODO-----------------------------------------------------------------------------

//Se usa para imprimir los datos en pantalla, y tambien calculará el camino con '*' utilizando una funcion auxiliar
void print_all(int argc, char* argv[], vector<vector<int>> file_matrix, int naive, vector<vector<int>> memo_table, int memo, vector<vector<int>> it_matrix, int it_vector) {
    memo = memo_table[0][0];
    int it1 = it_matrix[0][0]+1;

    if(naive == -1) {
        cout << '-';
    } else {
        cout << naive;
    }

    cout << ' ';

    cout << memo << " " << it1 << " " << it_vector << endl;

    if(param_position(argc, argv, "-p") != -1) {
        maze_parser();
    }

    if(param_position(argc, argv, "-t") != -1) {
        cout << "Memoization table:" << endl;
        for(unsigned i = 0; i < file_matrix.size(); i++) {
            for(unsigned j = 0; j < file_matrix[0].size(); j++) {
                if(memo_table[i][j] >= 0 && memo_table[i][j] <= memo) {
                    cout << setw(5) << memo - memo_table[i][j] + 1;
                } else if(memo_table[i][j] < 0) {
                    cout << setw(5) << "X";
                } else {
                    cout << setw(5) << "-";
                }
            
            }
            cout << endl;
        }

        cout << "Iterative table:" << endl;
        for(unsigned i = 0; i < file_matrix.size(); i++) {
            for(unsigned j = 0; j < file_matrix[0].size(); j++) {
                if(it_matrix[i][j] >= 0 && it_matrix[i][j] < memo) {
                    cout << setw(5) << memo - it_matrix[i][j];
                } else if(it_matrix[i][j] < 0) {
                    cout << setw(5) << "X";
                } else {
                    cout << setw(5) << "-";
                }
            
            }
            cout << endl;
        }
    } 

}

//-----------------------------------------------------------------------------------------------------



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

    //maze_memo-------------------------------------------------------------------------------

    vector<vector<int>> memo_table(file_matrix.size(), vector<int>(file_matrix[0].size()));

    for(unsigned i=0; i<memo_table.size(); i++) {
        for(unsigned j=0; j<memo_table[0].size(); j++) {
            memo_table[i][j] = -1;
        }
    }
    
    int memo = maze_memo(file_matrix, memo_table, 0, 0);

    //maze_it_matrix---------------------------------------------------------------------------

    vector<vector<int>> it_matrix(file_matrix.size(), vector<int>(file_matrix[0].size()));
    
    for(unsigned i=0; i<it_matrix.size(); i++) {
        for(unsigned j=0; j<it_matrix[0].size(); j++) {
            it_matrix[i][j] = file_matrix[i][j];
        }
    }

    it_matrix = maze_it_matrix(it_matrix);

    //maze_it_vector----------------------------------------------------------------------------
    vector<int> anterior = vector<int>(file_matrix[0]);

    int it_vector = maze_it_vector(file_matrix, anterior, 0);
    
    //print-------------------------------------------------------------------------------------

    print_all(argc, argv, file_matrix, naive, memo_table, memo, it_matrix, it_vector);

    return 0;
}