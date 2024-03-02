#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Consts

#define RN 11005
#define RM 11005

// Defs

typedef int I;
typedef char C;
typedef double F;
typedef long long L;

// Utility

#define SWAP(T, a, b) ({T t = a; a = b; b = t;})
#define EPS 1e-10

// Simplex

I sn, sm;
I id[RN + RM];
F mat[RN][RM];
F ans[RN + RM];

void Pivot(int x,int y)
{
    SWAP(I, id[x + sm], id[y]);
    F fac = -mat[x][y];
    mat[x][y] = -1.0;

    for (I i = 0; i <= sm; i++)
        mat[x][i] /= fac;

    for (I i = 0; i <= sn; i++)
    {
        fac = mat[i][y];
        if(i == x || (fac > -EPS && fac < EPS)) continue;

        for (I j = 0; j <= sm; j++)
            mat[i][j] += fac * mat[x][j];
    }
}
char simplex()
{
    for (I i = 1; i <= sn + sm; i++)
        id[i] = i;
    while (1)
    {
        I r = 0, c = 0;
        F rv = 0, cv = 0;
        for (I i = 1; i <= sn; i++)
        {
            if(mat[i][0] < rv)
                rv = mat[i][0], r = i;
        }
        if (!r) break;

        for (I i = 1; i <= sm; i++)
        {
            if (mat[r][i] > cv)
                cv = mat[r][i], c = i;
        }
        if(!c) return 0;
        Pivot(r, c);
    }
    while (1)
    {
        I r = 0, c = 0;
        F rv = 1e20, cv = 0;

        for (I i = 1; i <= sm; i++)
        {
            if (mat[0][i] > cv)
                cv = mat[0][i], c = i;
        }
        if (!c) break;

        for (I i = 1; i <= sn; i++)
        {
            F theta = -mat[i][0] / mat[i][c];
            if(theta < rv && mat[i][c] < -EPS)
                rv = theta, r = i;
        }
        if(!r) return 1;

        Pivot(r, c);
    }
    for (I i = sm + 1; i <= sn + sm; i++)
        ans[id[i]] = mat[i - sm][0];
    return 2;
}
// Main

int main()
{
    I n, m, s, t;
    scanf("%d%d%d%d", &n, &m, &s, &t);
    sm = m + 1;
    sn = n + m;
    for (I i = 1; i <= m; i++)
    {
        I u, v, w, c;
        scanf("%d%d%d", &u, &v, &w);
        mat[u][i] = 1.0;
        mat[v][i] = -1.0;
        mat[n + i][i] = -1.0;
        mat[n + i][0] = w;
    }
    mat[0][m + 1] = 1.0;
    mat[s][m + 1] = -1.0;
    mat[t][m + 1] = 1.0;

    simplex();

    L ans = mat[0][0];
    printf("%lld", ans);

    return 0;
}