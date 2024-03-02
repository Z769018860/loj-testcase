#include <stdio.h>
#include <stdlib.h>
#define MOD 1000000007
#define N 501
long long a[N][N], b[N][N], c[N][N];
int main(void)
{
	int n, p, m;
	scanf("%d %d %d", &n, &p, &m);
	for (long long i = 0; i < n; ++i)
		for (long long j = 0; j < p; ++j)
			scanf("%lld", &a[i][j]);
	for (long long i = 0; i < p; ++i)
		for (long long j = 0; j < m; ++j)
			scanf("%lld", &b[i][j]);
	for (long long i = 0; i < n; ++i)
		for (long long j = 0; j < m; ++j)
			for (long long k = 0; k < p; ++k)
				c[i][j] = ((c[i][j] + (a[i][k] * b[k][j] + MOD) % MOD) + MOD) % MOD;
	for (long long i = 0; i < n; ++i) {
		for (long long j = 0; j < m; ++j)
			printf("%lld ", c[i][j]);
		putchar('\n');
	}
	return 0;
}