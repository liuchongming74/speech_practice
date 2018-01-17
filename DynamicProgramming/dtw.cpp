#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

const int MAXN = 999999;
const int col = 4;
const int row = 6;
char R[] = {'A', 'B', 'C', 'D', 'E', 'F'};
int T[] = {1, 2, 3, 4};

void print(int dp[][col + 1]) {
    for (int i = 1; i <= 6; ++i) {
        for (int j = 1; j <= 4; ++j) {
            cout << dp[i][j] << "\t";
        }
        cout << endl;
    }
}

void printPath(int d[][col], int dp[][col + 1], int i, int j) {
    if (i == 1 && j == 1) {
        cout << R[i - 1] << " - " << T[j - 1] << endl;
        return;
    }

    if (dp[i][j] == dp[i - 1][j - 1] + 2 * d[i - 1][j - 1]) {
        printPath(d, dp, i - 1, j - 1);
    } else if (dp[i][j] == dp[i][j - 1] + d[i - 1][j - 1]) {
        printPath(d, dp, i, j - 1);
    } else {
        printPath(d, dp, i - 1, j);
    }

    cout << R[i - 1] << " - " << T[j - 1] << endl;
}

int boundaryJudge(int d[][col], int dp[][col + 1], int i, int j) {
    int exp = MAXN;
    if (i - 1 == 0 && j - 1 == 0) {
        //起始点
        dp[i][j] = 2 * d[i - 1][j - 1];
    } else if (dp[i - 1][j] == MAXN) {
        //位于第一行
        dp[i][j] = dp[i - 1][j - 1] + 2 * d[i - 1][j - 1];
        exp = dp[i][j - 1] + d[i - 1][j - 1];
    } else if (dp[i][j - 1] == MAXN) {
        //位于第一列
        dp[i][j] = dp[i - 1][j - 1] + 2 * d[i - 1][j - 1];
        exp = dp[i - 1][j] + d[i - 1][j - 1];
    } else {
        dp[i][j] = dp[i - 1][j - 1] + 2 * d[i - 1][j - 1];
        exp = (dp[i][j - 1] > dp[i - 1][j] ? dp[i - 1][j] : dp[i][j - 1]) + d[i - 1][j - 1];
    }
    return exp;
}

/**
 * dp[i][j] = min{ dp[i-1][j-1] + 2d[i][j]; dp[i][j] = dp[i-1][j] + d[i][j]; dp[i][j] = dp[i][j-1] + d[i][j] }
 */
void match(int d[][col], int dp[][col + 1]) {
    for (int i = 0; i <= 6; ++i) {
        dp[i][0] = MAXN;
    }
    for (int i = 0; i <= 4; ++i) {
        dp[0][i] = MAXN;
    }
    for (int i = 1; i <= 6; ++i) {
        for (int j = 1; j <= 4; ++j) {
            int exp = boundaryJudge(d, dp, i, j);
            if (exp < dp[i][j]) {
                dp[i][j] = exp;
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    int dp[row + 1][col + 1];
    int d[row][col] = {{2, 1, 5, 1},
                       {3, 4, 8, 2},
                       {5, 2, 4, 3},
                       {4, 7, 2, 4},
                       {1, 5, 1, 6},
                       {2, 1, 7, 5}};
    match(d, dp);
    cout << "相似度为：" << dp[row][col] << endl;
    printPath(d, dp, row, col);
    return 0;
}
