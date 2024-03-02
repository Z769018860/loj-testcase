#include<stdio.h>
#define MOD 1000000007
#define MAXN 510

long long a[MAXN][MAXN], b[MAXN][MAXN], res[MAXN][MAXN];
int n, p, m;

void solve() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            res[i][j] = 0;
            for (int k = 0; k < p; k++)
                res[i][j] = (res[i][j] + a[i][k] * b[k][j]) % MOD;
            long long ans = res[i][j];
            if (ans < 0) ans += MOD;
            printf("%lld ", ans);
        }
        printf("\n");
    }
}

int main() {
    scanf("%d %d %d", &n, &p, &m);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < p; j++)
            scanf("%lld", &a[i][j]);
    for (int i = 0; i < p; i++)
        for (int j = 0; j < m; j++)
            scanf("%lld", &b[i][j]);
    solve();
    return 0;
}
