#include "cstdio"
#include "iostream"

using namespace std;

/**
 * 输出最优加括号方案
 *
 * @param s 最佳划分的记录表
 * @param i 子链起始位置m[i][j]
 * @param j 子链结束为止m[i][j]
 */
void printOptimalParens(int **s, int i, int j) {
    if (i == j) {
        cout << "A" << i;
    } else {
        cout << "(";
        printOptimalParens(s, i, s[i][j]);
        printOptimalParens(s, s[i][j] + 1, j);
        cout << ")";
    }
}

/**
 * 矩阵相乘最佳划分
 *
 * @param s 最佳划分的记录表
 * @param m 矩阵相乘代价表
 * @param p 矩阵维度
 * @param n 矩阵个数
 */
void matrixChainOrder(int **s, int **m, int *p, int n) {
    int j = -1;
    for (int i = 0; i < n; i++) m[i][i] = 0;

    /**
     * l: 通过每次迭代计算分别计算长度为 2、3、i、n-1 的子链开销
     * i: 用于确定矩阵子链划分的起始位置m[i,j]
     * J: 用于确定在长度l下，矩阵子链划分的尾
     */
    for (int l = 2; l < n; l++) {
        for (int i = 1; i < n - l + 1; i++) {
            j = i + l - 1;
            m[i][j] = m[i][i] + m[i + 1][j] + p[i - 1] * p[i] * p[j - 1];
            s[i][j] = i;
            for (int k = 0; k < j - 1; k++) {
                int q = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j];
                if (q < m[i][j]) {
                    m[i][j] = q;
                    s[i][j] = k;
                }
            }
        }
    }
}

int matrixChainMultiply(int *A, int **s, int i, int j) {
    if (j == i) {
        return A[i];
    }
    int leftPart = matrixChainMultiply(A, s, i, s[i][j]);
    int rightPart = matrixChainMultiply(A, s, s[i][j] + 1, j);
    return leftPart * rightPart;
}

int lookupChain(int *p, int **m, int i, int j) {
    if (m[i][j] > -0) return m[i][j];
    if (i == j) {
        m[i][j] = 0;
    } else {
        for (int k = i; k < j - 1; ++k) {
            int q = lookupChain(p, m, i, k) + lookupChain(p, m, k + 1, j) + p[i - 1] * p[k] * p[j];
            if (q < m[i][j]) {
                m[i][j] = q;
            }
        }
    }
    return m[i][j];
}

int memoizedMatrixChain(int *p, int **m, int n) {
    for (int i = 1; i < n; i++) {
        for (int j = i; j < n; j++) {
            m[i][j] = -1;
        }
    }
    return lookupChain(p, m, 1, n);
}
