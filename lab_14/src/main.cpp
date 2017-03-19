#include "matrix.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstddef>

using namespace std;

int main() {
    Matrix registers[10];
    while (true) {
        try {
            string command;
            cin >> command;
            if (command == "load") {
                string reg, filename;
                cin >> reg >> filename;
                size_t k = reg[1] - '0';
                ifstream fin(filename);
                size_t n, m;
                fin >> n >> m;
                registers[k] = Matrix(n, m);
                for (size_t i = 0; i < n; i++)
                    for (size_t j = 0; j < m; j++)
                        fin >> registers[k][i][j];
            } else if (command == "print") {
                string reg;
                cin >> reg;
                size_t k = reg[1] - '0';
                for (size_t i = 0; i < registers[k].rows(); i++) {
                    for (size_t j = 0; j < registers[k].columns(); j++)
                        cout << registers[k][i][j] << " ";
                    cout << endl;
                }
            } else if (command == "add") {
                string reg1, reg2;
                cin >> reg1 >> reg2;
                size_t i1 = reg1[1] - '0';
                size_t i2 = reg2[1] - '0';
                registers[i1] += registers[i2];
            } else if (command == "mul") {
                string reg1, reg2;
                cin >> reg1 >> reg2;
                size_t i1 = reg1[1] - '0';
                size_t i2 = reg2[1] - '0';
                registers[i1] *= registers[i2];
            } else if (command == "elem") {
                string reg; size_t i, j;
                cin >> reg >> i >> j;
                size_t k = reg[1] - '0';
                cout << registers[k][i][j] << endl;
            } else if (command == "exit") {
                break;
            }
        } catch(MatrixException e) {
            cout << e.reason() << endl;
        }
    }
    return 0;
}
