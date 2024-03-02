#include <stdio.h>
int main() {
    int n, p, m;
    scanf("%d %d %d", &n, &p, &m);
    int a[600][600], b[600][600];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < p; j++)
            scanf("%d", &a[i][j]);
    for (int i = 0; i < p; i++)
        for (int j = 0; j < m; j++)
            scanf("%d", &b[i][j]);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            long long tmp = 0;
            for (int k = 0; k < p; k++) {
                tmp += (long long)a[i][k] * (long long)b[k][j];
                tmp %= 1000000007;
            }
            if(tmp<0) tmp+=1000000007;
            printf("%d ", (int)tmp);
        }
        printf("\n");
    }
}