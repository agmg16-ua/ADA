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

int cotaOptimista(int row, int col, int m, int n) {

    return max(m - row, n - col) - 1;

}

int cotaPesimista(vector<vector<int>> matrix, int row, int col) {
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
                return INT_MAX - (filas * columnas);
            }

            matrix[row][col] = -1;
            row++;
            longitud++;
        } else if(row == filas - 1) {
            if(matrix[row][col+1] == 0) {
                matrix[row][col] = -1;
                return INT_MAX - (filas * columnas);
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
                return INT_MAX - (filas * columnas);
            }
        }
    }

    return longitud;

}

Nodo obtenerNodo(vector<vector<int>> maze, int row, int col, int dist, vector<int> moves) {
    Nodo nodo;
    nodo.row = row;
    nodo.col = col;
    nodo.opt = -1;
    nodo.pes = -1;
    nodo.dist = dist;
    nodo.moves = moves;
    return nodo;
}

bool esHoja(Nodo nodo, vector<vector<int>> maze) {
    if(nodo.row == maze.size()-1 && nodo.col == maze[0].size()-1) {
        return true;
    }
    return false;
}

bool esMejor(Nodo nodo, int mejorActual) {
    if(nodo.dist <= mejorActual) {
        return true;
    }

    return false;
}

vector<Nodo> expande(Nodo nodo, vector<vector<int>> maze) {
    vector<Nodo> nodos;
    int dRow[] = {-1, -1, 0, 1, 1, 1, 0, -1};
    int dCol[] = {0, 1, 1, 1, 0, -1, -1, -1};

    
    for(int i=0; i<8; i++) {
        vector<int> newMoves = nodo.moves;
        newMoves.push_back(i+1);
        Nodo ex = obtenerNodo(maze, nodo.row + dRow[i], nodo.col + dCol[i], nodo.dist + 1, newMoves);
        nodos.push_back(ex);
    }

    return nodos;
}

bool dentroMaze(Nodo nodo, vector<vector<int>> maze) {
    if(nodo.row >= 0 && nodo.row <= maze.size()-1 && nodo.col >= 0 && nodo.col <= maze[0].size()-1) {
        return true;
    }
    return false;
}

bool esFactible(Nodo nodo, vector<vector<int>> maze, vector<vector<bool>> visitado) {
    if(!dentroMaze(nodo, maze)) {
        return false;
    }
    
    if(maze[nodo.row][nodo.col] == 0) {
        return false;
    }

    if(visitado[nodo.row][nodo.col] == true) {
        return false;
    }

    return true;
}

bool esPrometedor(Nodo nodo, int mejorActual) {
    /*if(nodo.opt > mejorActual) {
        return false;
    }

    if(nodo.pes < nodo.opt) {
        return false;
    }


    return true;*/

    return nodo.opt <= mejorActual;
}

struct esPeor {
    bool operator()(const Nodo &n1, const Nodo &n2) {
        return n1.opt > n2.opt;
    }
};

int maze_bb(vector<vector<int>> maze, vector<vector<bool>> &visitado) {
    priority_queue<Nodo, vector<Nodo>, esPeor> pq;
    Nodo inicial = obtenerNodo(maze, 0, 0, 1, {});
    inicial.pes = cotaPesimista(maze, 0, 0);
    inicial.opt = cotaOptimista(0, 0, maze.size(), maze[0].size());
    int mejorActual = inicial.pes;
    pq.push(inicial);

    while(!pq.empty()) {
        Nodo actual = pq.top();
        pq.pop();

        if(esHoja(actual, maze)) {
            estadisticas.hoja++;

            if(esMejor(actual, mejorActual)) {
                mejorActual = actual.dist;
                minMoves = actual.moves;
            }
            continue;
        }

        visitado[actual.row][actual.col] = true;

        for(Nodo nodo : expande(actual, maze)) {
            
            if(esFactible(nodo, maze, visitado)) {
                visitado[nodo.row][nodo.col] = true;

                nodo.pes = cotaPesimista(maze, nodo.row, nodo.col);
                nodo.opt = cotaOptimista(nodo.row, nodo.col, maze.size(), maze[0].size());

                if(nodo.dist + nodo.pes < mejorActual) {
                    mejorActual = nodo.dist + nodo.pes;
                }

                if(esPrometedor(nodo, mejorActual)) {
                    pq.push(nodo);
                }
                
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

    vector<vector<bool>> visitado(matrix.size(), vector<bool>(matrix[0].size(), false));
    
    int mejor = maze_bb(matrix, visitado);

    auto end = clock();

    double time = 1000.0 * (end-start)/CLOCKS_PER_SEC;

    print_result(matrix, time, argc, argv, mejor);

    return 0;

}

