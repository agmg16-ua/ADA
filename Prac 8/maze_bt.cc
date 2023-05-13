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
int visita = 0, explorado = 0, hoja = 0, no_factible = 0, no_prometedor = 0;

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

void maze_bt(vector<vector<int>> matrix, int x, int y, int dist, vector<vector<int>> &visitado, vector<vector<int>> &almacen) {
    visita++; //CONTADOR
    int n = matrix.size();
    int m = matrix[0].size();
    if(almacen[x][y] <= dist) {
        no_factible++;//Contador
        return;
    } else {
        almacen[x][y] = dist;
    }

    if(matrix[0][0] == 0) {
        return;
    }

    if(x == n-1 && y == m-1) {
        hoja++; //CONTADOR
        if(dist < minDist) {
            minDist = dist;
            minMoves = moves;
        }
        return;
    }

    if(dist >= minDist) {
        no_prometedor++; //CONTADOR
        return;
    }
    int dx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
    int dy[] = {0, 1, 1, 1, 0, -1, -1, -1};
    for (int i = 0; i < 8; i++) {  // probamos las 8 direcciones
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (nx >= 0 && nx < n && ny >= 0 && ny < m && matrix[nx][ny] == 1 && !visitado[nx][ny]) {
            explorado++; //CONTADOR
            visitado[nx][ny] = 1;
            moves.push_back(i+1);
            maze_bt(matrix, nx, ny, dist+1, visitado, almacen);
            visitado[nx][ny] = 0;
            moves.pop_back();
        } else {
            no_factible++; //CONTADOR
        }
    }

}

void print_result(vector<vector<int>> matrix, double time, int argc, char* argv[]) {
    if(minDist > matrix.size() * matrix[0].size()) {
        cout << 0 << endl;
    } else {
        cout << minDist << endl;
    }
    cout << visita << ' ' << explorado << ' ' << hoja << ' ' << no_factible << ' ' << no_prometedor << endl;
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

    vector<vector<int>> visitado(matrix.size(), vector<int>(matrix[0].size()));

    vector<vector<int>> almacen(matrix.size(), vector<int>(matrix[0].size()));

    for(int i=0; i<almacen.size(); i++) {
        for(int j=0; j<almacen[0].size(); j++) {
            almacen[i][j] = INT_MAX;
        }
    }

    visitado[0][0] = 1;

    maze_bt(matrix, 0, 0, 1, visitado, almacen);
    auto end = clock();

    double time = 1.0 * (end-start)/CLOCKS_PER_SEC;

    print_result(matrix, time, argc, argv);

    return 0;

}

