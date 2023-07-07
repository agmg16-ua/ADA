//Alejandro Guillén Merino      DNI: 48790456G
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>
#include <climits>
#include <ctime>
#include <cmath>
#include <queue>

using namespace std;

vector<int> minMoves;

struct Estadisticas {
    int visita = 0;
    int explorado = 0;
    int hoja = 0;
    int no_factible = 0;
    int no_prometedor = 0;
    int prometedor_rechazado = 0;
    int mejor_solucion_act_hoja = 0;
    int mejor_solucion_act_pesimista = 0;
} estadisticas;

struct Nodo {
    int row;
    int col;
    int opt;
    int pes;
    int dist;
    vector<int> moves;
};

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

void print_matrix(vector<vector<int>> matrix) {
    int n = 0;
    int m = 0;
    matrix[n][m] = -1;
    for(int i=0; i<minMoves.size(); i++) {
        int mov = minMoves[i];
        switch (mov) {
            case 1:
                n--;
                matrix[n][m] = -1;
                break;
            case 2:
                n--;
                m++;
                matrix[n][m] = -1;
                break;
            case 3:
                m++;
                matrix[n][m] = -1;
                break;
            case 4:
                n++;
                m++;
                matrix[n][m] = -1;
                break;
            case 5:
                n++;
                matrix[n][m] = -1;
                break;
            case 6:
                n++;
                m--;
                matrix[n][m] = -1;
                break;
            case 7:
                m--;
                matrix[n][m] = -1;
                break;

            case 8:
                n--;
                m--;
                matrix[n][m] = -1;
                break;
        }
    }

    for(int i=0; i<matrix.size(); i++) {
        for(int j=0; j<matrix[0].size(); j++) {
            if(matrix[i][j] == -1) {
                cout << '*';
            } else {
                cout << matrix[i][j];
            }
        }
        cout << endl;
    }
}

//------------------------------------------------------------------------------

struct esPeor {
    bool operator()(const Nodo &n1, const Nodo &n2) {
        return n1.opt + n1.dist > n2.opt + n2.dist;
    }
};

//Obtencion de la cota optimista
int cotaOptimista(int row, int col, int m, int n) {

    return max(m - row, n - col) - 1;

}

//Obtencion de la cota pesimista
int cotaPesimista(vector<vector<int>> &matrix, int row, int col) {
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
            longitud++;
            break;
        }

        //Condicion especial ultima columna
        if(col == columnas - 1) {
            if(matrix[row+1][col] == 0){
                return INT_MAX - (filas * columnas);
            }

            row++;
            longitud++;
        } else if(row == filas - 1) {
            if(matrix[row][col+1] == 0) {
                return INT_MAX - (filas * columnas);
            }

            matrix[row][col] = -1;
            col++;
            longitud++;
        } else {
            if(matrix[row+1][col+1] == 1) {
                row++;
                col++;
                longitud++;
            } else if(matrix[row+1][col] == 1) {
                row++;
                longitud++;
            } else if(matrix[row][col+1] == 1) {
                col++;
                longitud++;
            } else {
                return INT_MAX - (filas * columnas);
            }
        }
    }

    return longitud;

}

//Metodo para crear un nuevo nodo, en cualquier caso
Nodo obtenerNodo(int row, int col, int dist, vector<int> moves) {
    Nodo nodo;
    nodo.row = row;
    nodo.col = col;
    nodo.opt = -1;
    nodo.pes = -1;
    nodo.dist = dist;
    nodo.moves = moves;
    return nodo;
}

//Comprobar si un nodo es hoja
bool esHoja(Nodo nodo, vector<vector<int>> &maze) {
    if(nodo.row == maze.size()-1 && nodo.col == maze[0].size()-1) {
        return true;
    }
    return false;
}

//Comprobar si el nodo actual tiene mejor solución que uno ya calculado
bool esMejor(Nodo nodo, int mejorActual) {
    if(nodo.dist <= mejorActual) {
        return true;
    }

    return false;
}

//Expandir todos los nodos a partir de uno
vector<Nodo> expande(Nodo nodo) {
    vector<Nodo> nodos;
    int dRow[] = {-1, -1, 0, 1, 1, 1, 0, -1};
    int dCol[] = {0, 1, 1, 1, 0, -1, -1, -1};

    
    for(int i=0; i<8; i++) {
        vector<int> newMoves = nodo.moves;
        newMoves.push_back(i+1);
        Nodo ex = obtenerNodo(nodo.row + dRow[i], nodo.col + dCol[i], nodo.dist + 1, newMoves);
        nodos.push_back(ex);
    }

    return nodos;
}

//Comprobar si un nodo está dentro del laberinto
bool dentroMaze(Nodo nodo, vector<vector<int>> &maze) {
    if(nodo.row >= 0 && nodo.row <= maze.size()-1 && 
        nodo.col >= 0 && nodo.col <= maze[0].size()-1) {
        return true;
    }
    return false;
}

//Comprobación de si un nodo es factible
bool esFactible(Nodo nodo, vector<vector<int>> &maze, vector<vector<int>> &distancias) {
    if(!dentroMaze(nodo, maze)) {
        return false;
    }
    
    if(maze[nodo.row][nodo.col] == 0) {
        return false;
    }

    if(distancias[nodo.row][nodo.col] <= nodo.dist) {
        return false;
    }
    return true;
}

//Comprobación de si un nodo es prometedor, para añadirlo a la pq
bool esPrometedor(Nodo nodo, int mejorActual) {
    return nodo.opt + nodo.dist <= mejorActual;
}

//Método principal
int maze_bb(vector<vector<int>> maze, vector<vector<int>> &distancias) {
    priority_queue<Nodo, vector<Nodo>, esPeor> pq;
    Nodo inicial = obtenerNodo(0, 0, 1, {});
    inicial.pes = cotaPesimista(maze, 0, 0);
    inicial.opt = cotaOptimista(0, 0, maze.size(), maze[0].size());
    int mejorActual = inicial.pes;
    pq.push(inicial);
    distancias[0][0] = 1;

    while(!pq.empty()) {
        Nodo actual = pq.top();
        pq.pop();

        if(esHoja(actual, maze)) {
            estadisticas.hoja++; //CONTADOR

            if(esMejor(actual, mejorActual)) {
                estadisticas.mejor_solucion_act_hoja++; //CONTADOR
                mejorActual = actual.dist;
                minMoves = actual.moves;
            }
            continue;
        }

        for(Nodo nodo : expande(actual)) {
            estadisticas.visita++; //CONTADOR
            if(esFactible(nodo, maze, distancias)) {
                distancias[nodo.row][nodo.col] = nodo.dist;

                nodo.pes = cotaPesimista(maze, nodo.row, nodo.col);
                nodo.opt = cotaOptimista(nodo.row, nodo.col, maze.size(), maze[0].size());

                if(nodo.dist + nodo.pes < mejorActual) {
                    estadisticas.mejor_solucion_act_pesimista++; //CONTADOR
                    mejorActual = nodo.dist + nodo.pes;
                }

                if(esPrometedor(nodo, mejorActual)) {
                    estadisticas.explorado++; //CONTADOR
                    pq.push(nodo);
                }
                else {
                    estadisticas.no_prometedor++; //CONTADOR
                }
                
            }
            else {
                estadisticas.no_factible++; //CONTADOR
            }
        }
    }

    return mejorActual;
}

void print_result(vector<vector<int>> matrix, double time, int argc, char* argv[], int mejor) {
    if(mejor > matrix.size() * matrix[0].size()) { 
        cout << 0 << endl;
    } else {
        cout << mejor << endl;
    }
    cout << estadisticas.visita << ' ' << estadisticas.explorado << ' ' << estadisticas.hoja << ' ' << estadisticas.no_factible << ' ' << estadisticas.no_prometedor << ' ' << estadisticas.prometedor_rechazado << ' ' << estadisticas.mejor_solucion_act_hoja << ' ' << estadisticas.mejor_solucion_act_pesimista << endl;
    cout << time << endl;

    if(param_position(argc, argv, "--p2D") != -1) {
        if((mejor <= 0 && matrix[0][0] == 0)|| mejor > matrix.size() * matrix[0].size()) {
            cout << "NO EXIT" << endl;
        } else {
            print_matrix(matrix);
        }
    }

    if(param_position(argc, argv, "-p") != -1) {
        cout << '<';
        if((mejor <= 0 && matrix[0][0] == 0) || mejor > matrix.size() * matrix[0].size()) {
            cout << "NO EXIT";
        } else {
            for(int i=0; i<minMoves.size(); i++) {
                cout << minMoves[i];
            }
        }
        cout << '>' << endl;
    }
}

int main(int argc, char* argv[]) {
    auto start = clock();

    checkParams(argc, argv);

    int file_position = param_position(argc, argv, "-f") + 1;

    vector<vector<int>> matrix = get_data_file(argv[file_position]);

    vector<vector<int>> distancias(matrix.size(), vector<int>(matrix[0].size(), INT_MAX));
    
    int mejor = maze_bb(matrix, distancias);

    auto end = clock();

    double time = 1000.0 * (end-start)/CLOCKS_PER_SEC;

    print_result(matrix, time, argc, argv, mejor);

    return 0;

}

