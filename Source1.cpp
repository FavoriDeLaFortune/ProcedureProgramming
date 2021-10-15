#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <ctime>
using namespace std;
// вводится с клавиатуры число прямых, генерируемых в файл в формате kx+b=0 n,n-вес графа. все коэффициенты рандомятся в диапазоне +-3n
// n-вес прямой (стоимость прохода по ней). найти 2 самые дорогие по стоимости перемещения друг к другу точки при условии испеользования каждой прямой не более одного раза. точки образуются на пересечении прямых

int getRandom(int n) {
    return -3 * n + (rand() % (6 * n + 1));
}


bool inputCheck(string input) {
    if (input[0] == '0') {
        return true;
    }
    for (int i = 0; i < input.size(); ++i) {
        if (input[i] < 48 || input[i] > 57) {
            return true;
        }
    }
    return false;
}

int convertToInt(string input) {
    int num = 0;
    for (int i = 0; i < input.size(); ++i) {
        num = num * 10 + (int)(input[i] - 48);
    }
    return num;
}

struct line {
    int x_k;
    int free_k;
    int cost;
};

int main() {
    string input;
    int countOfLines;
    cout << "Input count of linear equations (positive number): ";
    while (true) {
        cin >> input;
        if (inputCheck(input)) {
            cout << "INVALID INPUT: type positive number without lead zero digit\nInput count of linear equations (positive number): ";
        } else {
            countOfLines = convertToInt(input);
            break;
        }
    }
    srand(time(0));
    ofstream write_f("input.txt");
    vector<line> MassOfLines(countOfLines);
    for (int i = 0; i < countOfLines; ++i) {
        int n;
        n = rand() % 12 + 3;
        int k, b;
        k = getRandom(n);
        b = getRandom(n);
        line equat;
        equat.x_k = k;
        equat.free_k = k;
        equat.cost = k;
        write_f << k << 'x' << b << "=0 " << n << '\n';
    }
    write_f.close();
    ifstream read_f("input.txt");
    cout << countOfLines << "linear equations is written in file:\n";
    for ()
    return 0;
}