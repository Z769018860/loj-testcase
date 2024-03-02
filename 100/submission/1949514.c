/*分别给定 n \times p 和 p \times m 的两个矩阵 A 和 B，求 A \times B。
输入格式
第一行三个正整数 n、p、m，表示矩阵的长宽。
之后的 n 行，每行 p 个整数，表示矩阵 A。
之后的 p 行，每行 m 个整数，表示矩阵 B。
输出格式
输出 n 行，每行 m 个整数，表示矩阵 A \times B，每个数模 10 ^ 9 + 7 输出。 
输入
3 4 5
-2 -8 -9 8
-10 0 6 -8
-10 -6 6 9
4 -7 5 -5 9
10 -2 -10 5 5
-3 -7 -3 8 -2
-6 7 7 3 -2
输出
999999898 149 153 999999929 999999951
999999997 999999979 999999883 74 999999921
999999835 103 55 95 999999857     
*/
#include <stdio.h>
#define MOD 1000000007 // 模数 10^9 + 7
// 将负数 x 取模为正数
int positiveMod(int x, int mod) {
    return (x % mod + mod) % mod;
}

void matrixMultiply(int n, int p, int m, long long A[n][p], long long B[p][m], long long result[n][m]) {
	int i,j,k;
    for (i = 0; i < n; ++i) {
        for (j = 0; j < m; ++j) {
            result[i][j] = 0;
            for (k = 0; k < p; ++k) {
                result[i][j] = positiveMod(result[i][j] + ((A[i][k] % MOD) * (B[k][j] % MOD)) % MOD, MOD);
            }
        }
    }
}


int main() {
    int n, p, m;
    scanf("%d %d %d", &n, &p, &m);
    long long A[n][p], B[p][m], result[n][m];
    // 输入矩阵 A
    int i,j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < p; j++) {
            scanf("%lld", &A[i][j]);
        }
    }
    // 输入矩阵 B
    for (i = 0; i < p; i++) {
        for (j = 0; j < m; j++) {
            scanf("%lld", &B[i][j]);
        }
    }
    // 矩阵乘法
    matrixMultiply(n, p, m, A, B, result);
    // 输出结果
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            printf("%lld ", result[i][j]);
        }
        printf("\n");
    }
    return 0;
}