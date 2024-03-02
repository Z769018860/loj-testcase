#include<stdio.h>
#define maxn 500 + 10
const int mod = 1e9 + 7;
int a[maxn][maxn],b[maxn][maxn],ans[maxn][maxn];
int main(){
	int n,p,m;
	scanf("%d%d%d",&n,&p,&m);
	for(int i = 1;i <= n;i++)
		for(int j = 1;j <= p;j++)
			scanf("%d",a[i]+j);
	for(int i = 1;i <= p;i++)
		for(int j = 1;j <= m;j++)
			scanf("%d",b[i]+j);
	for(int i = 1;i <= n;i++)
		for(int j = 1;j <= m;j++)
			for(int k = 1;k <= p;k++)
				ans[i][j] = (ans[i][j] + 1ll * a[i][k] * b[k][j] % mod + mod) % mod;
	for(int i = 1;i <= n;i++){
		for(int j = 1;j <= m;j++)
			printf("%d ",ans[i][j]);
		puts("");
	}
}