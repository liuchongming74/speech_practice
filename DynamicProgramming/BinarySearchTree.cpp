#include <iostream>

using namespace std;

const int MAXN = 999999;
const int n = 5;

double p[n + 1] = {-1, 0.15, 0.1, 0.05, 0.1, 0.2};
double q[n + 1] = {0.05, 0.1, 0.05, 0.05, 0.05, 0.1};

int root[n + 1][n + 1];
double w[n + 2][n + 2];
double e[n + 2][n + 2];


void optimalBST(double *p, double *q, int n) {
    for (int i = 1; i <= n + 1; ++i) {
        e[i][i - 1] = q[i - 1];
        w[i][i - 1] = q[i - 1];
    }
    for (int l = 1; l <= n; l++) {
        for (int i = 1; i <= n - l + 1; i++) {
            int j = i + l - 1;
            e[i][j] = MAXN;
            w[i][j] = w[i][j - 1] + p[j] + q[j];
            for (int r = i; r <= j; r++) {
                double temp = e[i][r - 1] + e[r + 1][j] + w[i][j];
                if (temp < e[i][j]) {
                    e[i][j] = temp;
                    root[i][j] = r;
                }
            }
        }
    }
}

void printOptimalBST(int i, int j, int r) {
    int rootChild = root[i][j];//子树根节点
    if (rootChild == root[1][n]) {
        cout << "k" << rootChild << "是根" << endl;
        printOptimalBST(i, rootChild - 1, rootChild);
        printOptimalBST(rootChild + 1, j, rootChild);
        return;
    }

    if (j < i - 1) {
        return;
    } else if (j == i - 1) {
        //遇到虚拟键
        if (j < r) {
            cout << "d" << j << "是" << "k" << r << "的左孩子" << endl;
        } else
            cout << "d" << j << "是" << "k" << r << "的右孩子" << endl;
        return;
    } else {
        //遇到内部结点
        if (rootChild < r) {
            cout << "k" << rootChild << "是" << "k" << r << "的左孩子" << endl;
        } else
            cout << "k" << rootChild << "是" << "k" << r << "的右孩子" << endl;
    }

    printOptimalBST(i, rootChild - 1, rootChild);
    printOptimalBST(rootChild + 1, j, rootChild);
}