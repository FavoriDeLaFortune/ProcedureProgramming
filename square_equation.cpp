#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <cmath>
#include <cstdlib>
using namespace std;

// SAMPLE OF CORRECT EQUATION: x^2-2x+1; -x^2+0.0x^2-1.44
double to_double(string s) {
    long long sum = 0;
    int pos_dot = 0;
    bool t = false;
    for (int i = 0; i < s.size(); ++i) {
        if (s[i] == '.') {
            t = true;
            continue;
        }
        sum = (sum * 10) + int(s[i]) - 48;
        if (t) {
            ++pos_dot;
        }
    }
    return sum / pow(10, pos_dot);
}
int main() {
    ifstream f("input.txt");
    string s;
    while (getline(f, s)) {
        int i = 0;
        char pred = '+';
        double free_k, x2_k, x_k;
        free_k = 0;
        x2_k = 0;
        x_k = 0;
        double sum_free_k, sum_x2_k, sum_x_k;
        sum_free_k = sum_x2_k = sum_x_k = 0;
        bool t = true;
        while (i < s.size()) {
            if (s[i] >= 48 && s[i] <= 57) {
                if (pred != '-' && pred != '+') {
                    t = false;
                    break;
                }
                string num = "";
                num += s[i];
                ++i;
                while (s[i] >= 48 && s[i] <= 57 || s[i] == '.' || s[i] == ',') {
                    if (s[i] == ',' || s[i] == '.') {
                        num += '.';
                    }
                    else {
                        num += s[i];
                    }
                    ++i;
                    if (i >= s.size()) {
                        break;
                    }
                }
                if (i + 2 < s.size()) {
                    if (s[i] == 'x' && s[i + 1] == '^' && s[i + 2] == '2') {
                        x2_k = to_double(num);
                        if (pred == '-') {
                            x2_k = -x2_k;
                        }
                        sum_x2_k += x2_k;
                        pred = s[i + 2];
                        i += 3;
                    }
                    else if (s[i] == 'x') {
                        x_k = to_double(num);
                        if (pred == '-') {
                            x_k = -x_k;
                        }
                        sum_x_k += x_k;
                        pred = s[i];
                        ++i;
                    }
                    else {
                        free_k = to_double(num);
                        if (pred == '-') {
                            free_k = -free_k;
                        }
                        sum_free_k += free_k;
                        pred = s[i];
                        ++i;
                    }
                    continue;
                }
                if (s[i] == 'x') {
                    x_k = to_double(num);
                    if (pred == '-') {
                        x_k = -x_k;
                    }
                    sum_x_k += x_k;
                    pred = s[i];
                    ++i;
                }
                else {
                    free_k = to_double(num);
                    if (pred == '-') {
                        free_k = -free_k;
                    }
                    sum_free_k += free_k;
                    pred = s[i];
                    ++i;
                }
            }
            else if (s[i] == 'x') {
                if (pred == '-' || pred == '+') {
                    if (i + 2 < s.size()) {
                        if (s[i + 1] == '^' && s[i + 2] == '2') {
                            if (pred == '-') {
                                x2_k = -1;
                                sum_x2_k += x2_k;
                                pred = s[i + 2];
                                i += 3;
                            }
                            else {
                                x2_k = 1;
                                sum_x2_k += x2_k;
                                pred = s[i + 2];
                                i += 3;
                            }
                        }
                    }
                    else {
                        if (pred == '-') {
                            x_k = -1;
                            sum_x_k += x2_k;
                            pred = s[i];
                            ++i;
                        }
                        else {
                            x_k = 1;
                            sum_x_k += x2_k;
                            pred = s[i];
                            ++i;
                        }
                    }
                }
            }
            else if (s[i] == '-' || s[i] == '+') {
                if (pred == '-' || pred == '+') {
                    t = false;
                    break;
                }
                pred = s[i];
                ++i;
                if (i == s.size()) {
                    t = false;
                    break;
                }
            }
            else {
                t = false;
                break;
            }
        }
        if (t) {
            cout << "NORMAS\n";
        }
        else {
            cout << "NENORMAS\n";
        }
    }
}