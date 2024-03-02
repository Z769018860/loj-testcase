#include <stdio.h>
#define N 500
const unsigned M = 1000000007;
void Input(long a[N][N], int m, int n);
void Calc(long a[N][N], long b[N][N], long c[N][N], int n, int m, int p);
void Output(long c[N][N], int n, int m);

int main()
{
    int n, p, m;
    long a[N][N], b[N][N], c[N][N];

    scanf("%d %d %d", &n, &p, &m);
    Input(a, n, p);
    Input(b, p, m);
    Calc(a, b, c, n, m, p);
    Output(c, n, m);
    return 0;
}

//输入两个矩阵
void Input(long a[N][N], int m, int n)
{
    int i, j;

    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
        {
            scanf("%ld", &a[i][j]);
        }
    }
}

//计算矩阵乘积
void Calc(long a[N][N], long b[N][N], long c[N][N], int n, int m, int p)
{
    int i, j, k;

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            long long sum = 0;
            for (k = 0; k < p; k++)
            {
                sum += (((a[i][k] + M) % M) * ((b[k][j] + M) % M)) % M;
            }
            c[i][j] = sum;
        }
    }
}

//输出结果
void Output(long c[N][N], int n, int m)
{
    int i, j;

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            printf("%ld ", (c[i][j] + M) % M);
        }
        printf("\n");
    }
}