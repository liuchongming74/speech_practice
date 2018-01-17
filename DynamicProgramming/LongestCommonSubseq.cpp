#include <iostream>

using namespace std;

#define UP 8
#define LEFT_UP 7
#define LEFT 4

int c[100][100];
int b[100][100];

void printLCS(char x[], int i, int j) {
    if (i == 0 || j == 0) {
        return;
    }
    if (b[i][j] == LEFT_UP) {
        printLCS(x, i - 1, j - 1);
        cout << x[i - 1] << " ";
    } else if (b[i][j] == UP) {
        printLCS(x, i - 1, j);
    } else if (b[i][j] == LEFT) {
        printLCS(x, i, j - 1);
    }
}

void printLCSWithoutB(char x[], char y[], int i, int j) {
    if (i == 0 || j == 0)
        return;

    if (x[i - 1] != y[j - 1]) {
        if (c[i - 1][j] == c[i][j]) {
            printLCSWithoutB(x, y, i - 1, j);
        } else {
            printLCSWithoutB(x, y, i, j - 1);
        }
    } else {
        printLCSWithoutB(x, y, i - 1, j - 1);
        cout << x[i - 1] << " ";
    }
}

/**
 * 最长公共子序列问题
 *
 * @param x 序列X
 * @param y 序列Y
 * @param c 维护记录表
 * @param b 序列记录表
 */
void LCS_Length(char x[], char y[]) {
    int m = strlen(x);
    int n = strlen(y);
    for (int i = 0; i < m; ++i) {
        c[i][0] = 0;
    }
    for (int j = 0; j < n; ++j) {
        c[0][j] = 0;
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (x[i] == y[j]) {
                c[i + 1][j + 1] = c[i][j] + 1;
                b[i + 1][j + 1] = LEFT_UP;
            } else if (c[i][j + 1] >= c[i + 1][j]) {
                c[i + 1][j + 1] = c[i][j + 1];
                b[i + 1][j + 1] = LEFT;
            } else {
                c[i + 1][j + 1] = c[i + 1][j];
                b[i + 1][j + 1] = UP;
            }
        }
    }
}

int Memoized_LCS_Length(char x[], char y[], int i, int j) {
    if (c[i][j] > 0) {
        return c[i][j];
    }

    if (i == 0 || j == 0) {
        c[i][j] = 0;
    } else if (x[i - 1] == y[j - 1]) {
        c[i][j] = Memoized_LCS_Length(x, y, i - 1, j - 1) + 1;
        b[i][j] = LEFT_UP;
    } else {
        int up = Memoized_LCS_Length(x, y, i - 1, j);
        int left = Memoized_LCS_Length(x, y, i, j - 1);
        if (up >= left) {
            c[i][j] = up;
            b[i][j] = UP;
        } else {
            c[i][j] = left;
            b[i][j] = LEFT;
        }
    }

    return c[i][j];
}


int initMemo(char x[], char y[]) {
    for (int i = 0; i <= strlen(x); ++i) {
        for (int j = 0; j <= strlen(y); ++j) {
            c[i][j] = -1;
        }
    }
    return Memoized_LCS_Length(x, y, strlen(x), strlen(y));
}