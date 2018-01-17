#include <iostream>
#include <cstdio>

using namespace std;

int f[2][100];
int cost[2][100];
int l[2][100];
int t[2][100];
int e1, e2, x1, x2;

void printPath(int line, int station) {
    if (station > 1) {
        printPath(l[line - 1][station], station - 1);
    }
    cout << "Line " << line << ", Station " << station << endl;
}

int main() {
    int count;
    cin >> count;
    cin >> e1 >> e2 >> x1 >> x2;
    for (int i = 1; i <= count; i++) {
        cin >> cost[0][i];
    }
    for (int i = 1; i <= count; i++) {
        cin >> cost[1][i];
    }
    for (int i = 1; i < count; i++) {
        cin >> t[0][i];
        cin >> t[1][i];
    }

    f[0][1] = e1 + cost[0][1];
    f[1][1] = e2 + cost[1][1];
    for (int i = 2; i <= count; i++) {
        if ((f[0][i - 1] + cost[0][i]) >= (f[1][i - 1] + t[1][i - 1] + cost[0][i])) {
            l[0][i] = 2;
            f[0][i] = f[1][i - 1] + t[1][i - 1] + cost[0][i];
        } else {
            l[0][i] = 1;
            f[0][i] = f[0][i - 1] + cost[0][i];
        }

        if ((f[0][i - 1] + t[0][i - 1] + cost[1][i]) >= (f[1][i - 1] + cost[1][i])) {
            l[1][i] = 2;
            f[1][i] = f[1][i - 1] + cost[1][i];
        } else {
            l[1][i] = 1;
            f[1][i] = f[0][i - 1] + t[0][i - 1] + cost[1][i];
        }
    }

    int line = -1;
    if (f[0][count] + x1 >= f[1][count] + x2) {
        line = 2;
        cout << "Output from Line 2: " << f[1][count] + x2 << endl;
    } else {
        line = 1;
        cout << "Output from Line 1: " << f[0][count] + x1 << endl;
    }

    printPath(line, count);
    
    return 0;
}

/**
6
2 4 3 2
7 9 3 4 8 4
8 5 6 4 5 7
2 2 3 1 1 2 3 2 4 1
 */