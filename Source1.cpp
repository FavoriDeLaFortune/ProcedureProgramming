﻿#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <ctime>
#include <string>
#include <algorithm>
#include <set>

using namespace std;
// вводится с клавиатуры число прямых, генерируемых в файл в формате kx+b=0 n,n-вес графа. все коэффициенты рандомятся в диапазоне +-3n
// n-вес прямой (стоимость прохода по ней). найти 2 самые дорогие по стоимости перемещения друг к другу точки при условии испеользования каждой прямой не более одного раза. точки образуются на пересечении прямых

int getRandom(const int n) {
    return -3 * n + (rand() % (6 * n + 1));
}

bool inputCheck(const string input) {
    if (input[0] == '0') {
        return true;
    }
    for (int i = 0; i < input.size(); ++i) {
        if (input[i] < '0' || input[i] > '9') {
            return true;
        }
    }
    return false;
}

int convertToInt(const string input) {
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

struct dot {
    pair<double, double> coordinates;
    set<int> indexOfLines;
};

int binSearch(const vector<dot> massOfDots, const pair<double, double> coordOfDot) {
    if (massOfDots.size() == 0) {
        return -1;
    }
    int l = 0;
    int r = massOfDots.size();
    while (r - l > 1) {
        int m = (r + l) / 2;
        if (massOfDots[m].coordinates >= coordOfDot) {
            l = m;
        } else {
            r = m;
        }
    }
    if (massOfDots[l].coordinates == coordOfDot) {
        return l;
    } else {
        return -1;
    }
}

int main() {
    string input;
    int countOfLines;
    while (true) {
        cout << "Input count of linear equations (positive integer number withoout lead zero digit): ";
        cin >> input;
        if (inputCheck(input)) {
            cout << "INVALID INPUT: you have written incorrect data\n";
        } else {
            countOfLines = convertToInt(input);
            break;
        }
    }
    srand(time(0));
    ofstream write_f("input.txt");
    vector<line> massOfLines(countOfLines);
    for (int i = 0; i < countOfLines; ++i) {
        int n;
        n = rand() % 12 + 3;
        int k, b;
        k = 0;
        while (k == 0) {
            k = getRandom(n);
        }
        b = getRandom(n);
        line equat;
        equat.x_k = k;
        equat.free_k = b;
        equat.cost = n;
        massOfLines[i] = equat;
        if (k == 1) {
            write_f << 'x';
        } else if (k == -1) {
            write_f << '-x';
        } else {
            write_f << k << 'x';
        }
        if (b == 0) {
            write_f << "=0 " << n << '\n';
        } else if (b > 0) {
            write_f << '+' << b << "=0 " << n << '\n';
        } else {
            write_f << b << "=0 " << n << '\n';
        }
    }
    cout << '\n';
    write_f.close();
    ifstream read_f("input.txt");
    cout << countOfLines << " linear equations is written in file:\n";
    for (int i = 0; i < countOfLines; ++i) {
        string s;
        getline(read_f, s);
        cout << s << '\n';
    }
    read_f.close();
    vector<dot> massOfDots;
    vector<vector<int>> dotsOnLines(countOfLines);
    for (int i = 0; i < countOfLines - 1; ++i) {
        for (int j = i + 1; j < countOfLines; ++j) {
            if (massOfLines[i].x_k == massOfLines[j].x_k) {
                continue;
            }
            pair<double, double> coordOfDot;
            double x, y;
            x = (massOfLines[j].free_k - massOfLines[i].free_k) / (massOfLines[i].x_k - massOfLines[j].x_k);
            y = massOfLines[i].x_k * x + massOfLines[i].free_k;
            coordOfDot = make_pair(x, y);
            int pos;
            pos = binSearch(massOfDots, coordOfDot);
            if (pos == -1) {
                dot newDot;
                newDot.coordinates = coordOfDot;
                set<int> linesOfDot;
                linesOfDot.insert(i);
                linesOfDot.insert(j);
                newDot.indexOfLines = linesOfDot;
                massOfDots.push_back(newDot);
                sort(massOfDots.begin(), massOfDots.end());
            } else {
                massOfDots[pos].indexOfLines.insert(i);
                massOfDots[pos].indexOfLines.insert(j);
            }
        }
    }
    for (int i = 0; i < massOfDots.size(); ++i) {
        for (int line: massOfDots[i].indexOfLines) {
            dotsOnLines[line].push_back(i);
        }
    }
    vector<vector<pair<int, int>>> graph(massOfDots.size());
    for (int i = 0; i < massOfDots.size(); ++i) {
        for (int j: massOfDots[i].indexOfLines) {
            for (int l = 0; l < dotsOnLines[j].size(); ++l) {
                if (i == dotsOnLines[j][l]) {
                    continue;
                }
                pair<int, int> pointAndLine;
                pointAndLine = make_pair(l, j);
                graph[i].push_back(pointAndLine);
            }
        }
    }
    for (int i = 0; i < graph.size(); ++i) {
        for (int j = 0; j < graph[i].size(); ++j) {
            cout << '{' << graph[i][j].first << ', ' << graph[i][j].second << "} ";
        }
        cout << '\n';
    }
    return 0;
}
