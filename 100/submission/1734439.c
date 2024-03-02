#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
int n = 0, p = 0, m = 0, i = 0, j = 0, k = 0; 
long long sum = 0, A[500][500], B[500][500], C[500][500];
int main()
{
	scanf("%d", &n);
	scanf("%d", &p);
	scanf("%d", &m);
	for (i=0; i < n; i++)
	{
		for (j=0; j < p; j++)
			scanf("%lld", &A[i][j]);
	}
	for (i=0; i < p; i++)
	{
		for (j=0; j < m; j++)
			scanf("%lld", &B[i][j]);
	}
	for (i = 0; i < n; i++)
	{
		for (j = 0,sum=0; j < m; j++)
		{
			for (k = 0; k < p; k++)
			{
				sum = (sum + A[i][k] * B[k][j] + 1000000007) % 1000000007;
			}
			C[i][j] = (sum + 1000000007) % 1000000007;
			sum = 0;
		}
	}
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < m; j++)
			printf("%lld ", C[i][j]);
		printf("\n");
	}
	return 0;
}