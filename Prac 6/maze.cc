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

int maze_it_vector(vector<vector<int>> matriz, vector<vector<int>> &it_matrix) {
    int filas = matriz.size();
    int columnas = matriz[0].size();

    int inf = INT_MAX-1;
    int row = 0;

    vector<int> fila_anterior(columnas, inf);
    vector<int> fila_actual = matriz[row];

    if(fila_actual[0] == 0) {
        return 0;
    }

    for(int i=0; i<filas; i++) {
        row++;
        for(int j=0; j<columnas; j++) {

            //Casos especiales de la primera fila. Solo se puede acceder desde la izquierda
            if(i==0 && j > 0) {
                if(fila_actual[j-1] == inf) {
                    fila_actual[j] = inf;
                } else if(fila_actual[j] == 0) {
                    fila_actual[j] = inf;
                } else {
                    fila_actual[j] = fila_actual[j-1] + 1;
                }
            }

            //Casos especiales de la primera columna. Solo se puede acceder desde arriba
            if(j==0 && i > 0) {
                if(fila_anterior[j] == inf) {
                    fila_actual[j] = inf;
                } else if(fila_actual[j] == 0) {
                    fila_actual[j] = inf;
                } else {
                    fila_actual[j] = fila_anterior[j] + 1;
                }
            }

            //Para el resto de casos intermedios
            if(i > 0 && j > 0) {
                if(fila_actual[j] == 0) {
                    fila_actual[j] = inf;
                } else {
                    int valor = min(fila_actual[j-1], min(fila_anterior[j-1], fila_anterior[j]));

                    if(valor == inf) {
                        fila_actual[j] = inf;
                    } else {
                        fila_actual[j] = valor + 1;
                    }

                }
            }
        }

        it_matrix[i] = fila_actual;

        //Control. Si todas las posiciones de la fila actual son inf, no hay salida
        bool continuar = false;

        for(int k=0; k<columnas; k++) {
            if(fila_actual[k] != inf) {
                continuar = true;
                break;
            }
        }

        if(continuar == false) {
            break;
        }

        if(row != filas) {
            fila_anterior = fila_actual;
            fila_actual = matriz[row];
        }        
    }

    return fila_actual[columnas-1];
}

void maze_parser(vector<vector<int>> matriz, vector<vector<int>> matriz_it) {
    int m = matriz.size()-1;
    int n = matriz[0].size()-1;

    if(!(matriz_it[m-1][n-1] > 0 && matriz_it[m-1][n-1] < (n*m))){
        return;
    }
    
    //Comprobar que m y n no sea menor que el tamaño
    while(matriz_it[m][n] != 1){

        if(m==0) {
            if(matriz_it[m][n-1] == matriz_it[m][n] - 1){
                matriz_it[m][n] = -1;
                n--;
            } else {
                break;
            }
        } else if(n==0) { 
            if(matriz_it[m-1][n] == matriz_it[m][n] - 1) {
                matriz_it[m][n] = -1;
                m--;
            } else {
                break;
            }
        }else if(matriz_it[m-1][n-1] == matriz_it[m][n] -1) {
            matriz_it[m][n] = -1;
            n--;
            m--;
        } else if(matriz_it[m-1][n] == matriz_it[m][n] -1) {
            matriz_it[m][n] = -1;
            m--;
        } else if(matriz_it[m][n-1] == matriz_it[m][n] -1) {
            matriz_it[m][n] = -1;
            n--;
        } else {
            cout << "NO HAY SALIDA" << endl;
            break;
        }
    }

    for(unsigned i=0; i<matriz.size(); i++) {
        for(unsigned j=0; j<matriz[0].size(); j++) {
            if(matriz_it[i][j] == -1) {
                cout << "*";
            } else if(i == 0 && j == 0) {
                cout << "*";
            }else {
                cout << matriz[i][j];
            }
        }

        cout << endl;
    }
}

//-------------------------------------------IMPRIMIR TODO-----------------------------------------------------------------------------

//Se usa para imprimir los datos en pantalla, y tambien calculará el camino con '*' utilizando una funcion auxiliar
void print_all(int argc, char* argv[], vector<vector<int>> file_matrix, int naive, vector<vector<int>> memo_table, int memo, vector<vector<int>> it_matrix, int it_vector, vector<vector<int>> it_vector_table) {
    memo = memo_table[0][0];
    int it1 = it_matrix[0][0]+1;

    //Control de naive
    if(naive == -1) {
        cout << "- ";
    } else {
        if(naive < 0) {
            cout << 0 << " ";
        } else {
            cout << naive << " ";
        }
    }

    //Control de memo
    if(memo == INT_MAX-1) {
        cout << 0 << " ";
    } else {
        cout << memo << " ";
    }

    int max = it_matrix.size() * it_matrix[0].size();

    //Control de it_matrix
    if(it1 >= max) {
        cout << 0 << " ";
    } else {
        cout << it1 << " ";
    }

    //Control de it_vector
    if(it_vector == INT_MAX-1) {
        cout << 0 << endl;
    } else {
        cout << it_vector << endl;
    }

    //Control para mostrar el camino (maze_parser)
    if(param_position(argc, argv, "-p") != -1) {
        if(it_vector == INT_MAX -1) {
            cout << "NO EXIT" << endl;
        } else {
            maze_parser(file_matrix, it_vector_table);
        }
    }

    //Control para mostrar las tablas
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
                if(it_vector_table[i][j] > it_vector || it_vector_table[i][j] == INT_MAX-1 || it_vector_table[i][j] == 0) {
                    cout << setw(5) << "X";
                } else {
                    cout << setw(5) << it_vector_table[i][j];
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
    vector<vector<int>> it_table(file_matrix.size(), vector<int>(file_matrix[0].size()));

    int it_vector = maze_it_vector(file_matrix, it_table);
    
    //print-------------------------------------------------------------------------------------

    print_all(argc, argv, file_matrix, naive, memo_table, memo, it_matrix, it_vector, it_table);

    return 0;
}