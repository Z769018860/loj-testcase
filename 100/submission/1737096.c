#include <stdio.h>
#include <stdlib.h>
#define MOD 1000000007
#define N 500
long long a[N][N], b[N][N], c[N][N];
int main(void)
{
	int n, p, m;
	scanf("%d %d %d", &n, &p, &m);
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < p; ++j)
			scanf("%lld", &a[i][j]);
	for (int i = 0; i < p; ++i)
		for (int j = 0; j < m; ++j)
			scanf("%lld", &b[i][j]);
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j)
			for (int k = 0; k < p; ++k)
				c[i][j] = (c[i][j] + (a[i][k] * b[k][j] + MOD) % MOD + MOD) % MOD;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j)
			printf("%lld ", c[i][j]);
		putchar('\n');
	}
	return 0;
}