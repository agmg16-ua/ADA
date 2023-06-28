//Alejandro Guillén Merino      DNI: 48790456G
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>
#include <climits>
#include <ctime>

using namespace std;

int minDist = INT_MAX-1;
vector<int> moves, minMoves;

struct Estadisticas {
    int visita;
    int explorado;
    int hoja;
    int no_factible;
    int no_prometedor;
    int prometedor_rechazado;
    int mejor_solucion_act_hoja;
    int mejor_solucion_act_pesimista;
} estadisticas;

struct Posicion {
    int minDist;
    int optDist;
    bool factible;
    int visitado;
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

int distOp(vector<vector<int>> maze, int row, int col) {
    int distanciaOp = 0;
    
    while(true) {
        if(row == maze.size()-1 && col == maze[0].size()-1) {
            distanciaOp++;
            break;
        }


        if(row < maze.size()-1 && col < maze[0].size()-1) {
            row++;
            col++;
            distanciaOp++;
        } else {
            if(row == maze.size()-1) {
                col++;
                distanciaOp++;
            } else if(col == maze[0].size()-1) {
                row++;
                distanciaOp++;
            } else {
                break;
            }
        }
    }

    return distanciaOp;
}

void maze_bb(vector<vector<int>> matrix, int x, int y, int distancia, vector<vector<Posicion>> &datosPosiciones) {
    estadisticas.visita++; //CONTADOR
    int n = matrix.size();
    int m = matrix[0].size();


    if(datosPosiciones[x][y].minDist <= distancia) {
        estadisticas.prometedor_rechazado++; //Contador
        return;
    } else {
        datosPosiciones[x][y].minDist = distancia;
    }

    if(datosPosiciones[x][y].optDist == -1) {
        datosPosiciones[x][y].optDist = distOp(matrix, x, y);
    }

    if(matrix[0][0] == 0 || matrix[n-1][m-1] == 0) {
        return;
    }
    if(x == n-1 && y == m-1) {
        estadisticas.hoja++; //CONTADOR
        if(distancia < minDist) {
            minDist = distancia;
            minMoves = moves;
        }
        if(datosPosiciones[0][0].optDist == datosPosiciones[n-1][m-1].minDist) {
            return;
        }
        return;
    }

    if(datosPosiciones[0][0].optDist == datosPosiciones[n-1][m-1].minDist) {
        estadisticas.mejor_solucion_act_hoja++; //CONTADOR
        return;
    }

    if(datosPosiciones[x][y].optDist + distancia > minDist){
        estadisticas.no_factible++;
        return;
    }

    if(distancia >= minDist) {
        estadisticas.no_prometedor++; //CONTADOR
        return;
    }

    int dx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
    int dy[] = {0, 1, 1, 1, 0, -1, -1, -1};
    int orden[] = {3, 2, 4, 0, 1, 5, 6, 7};
    for (int j = 0; j < 8; j++) {  // probamos las 8 direcciones
        int i = orden[j];
        int nx = x + dx[i];
        int ny = y + dy[i];
        if(datosPosiciones[0][0].optDist == datosPosiciones[n-1][m-1].minDist) {
            break;
        }
        if (nx >= 0 && nx < n && ny >= 0 && ny < m && matrix[nx][ny] == 1 && !datosPosiciones[nx][ny].visitado) {
            estadisticas.explorado++; //CONTADOR
            datosPosiciones[nx][ny].visitado = 1;
            moves.push_back(i+1);
            maze_bb(matrix, nx, ny, distancia+1, datosPosiciones);
            datosPosiciones[nx][ny].visitado = 0;
            moves.pop_back();
        } else {
            estadisticas.no_factible++; //CONTADOR
        }
    }

}

void print_result(vector<vector<int>> matrix, double time, int argc, char* argv[]) {
    if(minDist > matrix.size() * matrix[0].size()) {
        cout << 0 << endl;
    } else {
        cout << minDist << endl;
    }
    cout << estadisticas.visita << ' ' << estadisticas.explorado << ' ' << estadisticas.hoja << ' ' << estadisticas.no_factible << ' ' << estadisticas.no_prometedor << ' ' << estadisticas.prometedor_rechazado << ' ' << estadisticas.mejor_solucion_act_hoja << ' ' << estadisticas.mejor_solucion_act_pesimista << endl;
    cout << time << endl;

    if(param_position(argc, argv, "--p2D") != -1) {
        if((minDist <= 0 && matrix[0][0] == 0)|| minDist > matrix.size() * matrix[0].size()) {
            cout << "NO EXIT" << endl;
        } else {
            print_matrix(matrix);
        }
    }

    if(param_position(argc, argv, "-p") != -1) {
        cout << '<';
        if((minDist <= 0 && matrix[0][0] == 0) || minDist > matrix.size() * matrix[0].size()) {
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

    estadisticas.visita = 0;
    estadisticas.explorado = 0;
    estadisticas.hoja = 0;
    estadisticas.mejor_solucion_act_hoja = 0;
    estadisticas.mejor_solucion_act_pesimista = 0;
    estadisticas.no_factible = 0;
    estadisticas.no_prometedor = 0;
    estadisticas.prometedor_rechazado = 0;

    vector<vector<Posicion>> datosPosiciones(matrix.size(), vector<Posicion>(matrix[0].size()));
    for(int i=0; i<matrix.size(); i++) {
        for(int j=0; j<matrix[0].size(); j++) {
            datosPosiciones[i][j].minDist = INT_MAX - 1;
            datosPosiciones[i][j].optDist = -1;
            datosPosiciones[i][j].visitado = 0;
            datosPosiciones[i][j].factible = true;
        }
    }

    

    datosPosiciones[0][0].visitado = 1;

    maze_bb(matrix, 0, 0, 1, datosPosiciones);
    auto end = clock();

    double time = 1000.0 * (end-start)/CLOCKS_PER_SEC;

    print_result(matrix, time, argc, argv);

    return 0;

}

