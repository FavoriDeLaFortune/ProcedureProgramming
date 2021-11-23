#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <ctime>
#include <string>
#include <algorithm>
#include <set>
#include <GLFW/glfw3.h>

using namespace std;

// вводится с клавиатуры число прямых, генерируемых в файл в формате kx+b=0 n,n-вес графа. все коэффициенты рандомятся в диапазоне +-3n
// n-вес прямой (стоимость прохода по ней). найти 2 самые дорогие по стоимости перемещения друг к другу точки при условии использования каждой прямой не более одного раза. точки образуются на пересечении прямых
// сделать графический интерфейс пересечения линий. при ответе показать путь (линия->линия->линия.....)

const int SCREEN_HEIGHT = 600;
const int SCREEN_WIDTH = 800;
long long SUM_OF_WEIGHTS = 0;
vector<pair<int, vector<int>>> dotsMaxPath;
vector<bool> usedLines;
bool flag;

int getRandom(const int& n) { //получение случайного целого числа в диапазоне [-3 * n; 3 * n]
    return -3 * n + (rand() % (6 * n + 1));
}

bool inputCheck(const string& input) { //проверка правильности ввода
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

int convertToInt(const string& input) { // перевод числа из типа string в тип int
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

void pathBuild(int c, const vector<dot>& massOfDots, const vector<line>& massOfLines, const vector<vector<int>>& dotsOnLines, int costPath, vector<int>& linePath) {
    for (int line : massOfDots[c].indexOfLines) {
        if (!usedLines[line]) {
            linePath.push_back(line + 1);
            for (int v : dotsOnLines[line]) {
                if (flag) {
                    return;
                }
                if (v == c) {
                    continue;
                }
                if (costPath + massOfLines[line].cost > dotsMaxPath[v].first) {
                    dotsMaxPath[v].first = costPath + massOfLines[line].cost;
                    dotsMaxPath[v].second = linePath;
                }
                if (dotsMaxPath[v].first == SUM_OF_WEIGHTS) {
                    flag = true;
                    return;
                }
                costPath += massOfLines[line].cost;
                usedLines[line] = true;
                pathBuild(v, massOfDots, massOfLines, dotsOnLines, costPath, linePath);
                costPath -= massOfLines[line].cost;
                usedLines[line] = false;
            }
            linePath.erase(linePath.end() - 1);
        }
    }
}

int binSearch(const vector<dot>& massOfDots, const pair<double, double>& coordOfDot) { //поиск точки в структуре massOfDots за O(log(n))
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

bool cmp(const dot& a, const dot& b) { // компаратор к сортировке
    if (a.coordinates <= b.coordinates) {
        return true;
    } else {
        return false;
    }
}

void drawGraphic() {

}

int main() {
    string input; //кол-во линейных уравнений в типе string
    int countOfLines; //кол-во линейных уравнений в типе int
    while (true) { //защищенный ввод кол-ва линейных уравнений
        cout << "Input count of linear equations (positive integer number without lead zero digit): ";
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
    vector<line> massOfLines(countOfLines); //вектор линий
    for (int i = 0; i < countOfLines; ++i) {
        int n;
        n = rand() % 12 + 3;
        SUM_OF_WEIGHTS += n;
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
            write_f << "-x";
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
        cout << i + 1 << ") " << s << '\n';
    }
    read_f.close();
    vector<dot> massOfDots; //массив уникальных точек, образованных при пересечении прямых
    double y_max, y_min, x_max, x_min;
    y_max = -10000;
    y_min = 10000;
    x_max = -10000;
    x_min = 10000;
    for (int i = 0; i < countOfLines - 1; ++i) { //перебор всех возможных пересечений линий
        for (int j = i + 1; j < countOfLines; ++j) {
            if (massOfLines[i].x_k == massOfLines[j].x_k) {
                continue;
            }
            pair<double, double> coordOfDot;
            double x, y;
            x = ((double)massOfLines[j].free_k - massOfLines[i].free_k) / (massOfLines[i].x_k - massOfLines[j].x_k);
            y = massOfLines[i].x_k * x + massOfLines[i].free_k;
            x_max = max(x_max, x);
            y_max = max(y_max, y);
            y_min = min(y_min, y);
            x_min = min(x_min, x);
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
                sort(massOfDots.begin(), massOfDots.end(), cmp);
            } else {
                massOfDots[pos].indexOfLines.insert(i);
                massOfDots[pos].indexOfLines.insert(j);
            }
        }
    }
    vector<vector<int>> dotsOnLines(countOfLines); // двумерный массив, где индекс массива-предка значит номер линии, а индекс во вложенном массиве означает номер вершины
    for (int i = 0; i < massOfDots.size(); ++i) {
        for (int line : massOfDots[i].indexOfLines) {
            dotsOnLines[line].push_back(i);
        }
    }

    /*
    vector<vector<int>> graph(massOfDots.size()); //список смежности, где индекс массива-предка значит номер вершины, а индекс во вложенном означает номер вершины
    for (int i = 0; i < massOfDots.size(); ++i) {
        for (int j : massOfDots[i].indexOfLines) {
            for (int l = 0; l < dotsOnLines[j].size(); ++l) {
                if (i == dotsOnLines[j][l]) {
                    continue;
                }
                graph[i].push_back(dotsOnLines[j][l]);
            }
        }
    }
    for (int i = 0; i < graph.size(); ++i) {
        cout << i << " = ";
        for (int j = 0; j < graph[i].size(); ++j) {
            cout << graph[i][j] << ", ";
        }
        cout << '\n';
    }
    */

    int maxDistPath = 0;
    vector<int> maxLinePath;
    pair<int, int> maxDistPair;
    for (int i = 0; i < massOfDots.size(); ++i) {
        dotsMaxPath.resize(massOfDots.size());
        usedLines.resize(countOfLines, false);
        flag = false;
        vector<int> linePath;
        pathBuild(i, massOfDots, massOfLines, dotsOnLines, 0, linePath);
        for (int j = 0; j < dotsMaxPath.size(); ++j) {
            if (maxDistPath <= dotsMaxPath[j].first) {
                maxDistPath = dotsMaxPath[j].first;
                maxDistPair = make_pair(i, j);
                maxLinePath = dotsMaxPath[j].second;
            }
        }
        if (maxDistPath == SUM_OF_WEIGHTS) {
            break;
        }
    }
    cout << "maximum path cost between (" << massOfDots[maxDistPair.first].coordinates.first << ", " << massOfDots[maxDistPair.first].coordinates.second 
        <<  ") and (" << massOfDots[maxDistPair.second].coordinates.first << ", " << massOfDots[maxDistPair.second].coordinates.second << "). Path cost is equal to " << maxDistPath << "\nPath: ";
    for (int i = 0; i < maxLinePath.size(); ++i) {
        cout << maxLinePath[i];
        if (i != maxLinePath.size() - 1) {
            cout << " -> ";
        }
    }

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
      
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "graphic of linear equations", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glViewport(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_LINES);
        glColor3f(1, 1, 1);
        glVertex2f(800, 300);
        glVertex2f(0, 300);
        glVertex2f(400, 600);
        glVertex2f(400, 0);
        glEnd();

        glBegin(GL_LINES);
        glColor3f(0, 1, 0);
        for (int i = 0; i < countOfLines; ++i) {
            glVertex2f(0, massOfLines[i].x_k* (-SCREEN_WIDTH / 2) + massOfLines[i].free_k + SCREEN_HEIGHT / 2);
            glVertex2f(SCREEN_WIDTH, massOfLines[i].x_k* (SCREEN_WIDTH / 2) + massOfLines[i].free_k + SCREEN_HEIGHT / 2);
        }
        glEnd();


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
