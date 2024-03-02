#include <stdio.h>
long long a[501][501],b[501][501],tmp,n,m,p,N=1000000007;
int main(void){scanf("%lld %lld %lld",&n,&m,&p);for(int i=1;i<=n;i++){for(int j=1;j<=m;j++){scanf("%lld",&a[i][j]);a[i][j]+=N;a[i][j]%=N;}}for(int i=1;i<=m;i++){for(int j=1;j<=p;j++){scanf("%lld",&b[i][j]);b[i][j]+=N;b[i][j]%=N;}}for(int i=1;i<=n;i++){for(int j=1;j<=p;j++){tmp=0;for(int k=1;k<=m;k++){tmp+=a[i][k]%N*b[k][j]%N;}printf("%lld ",tmp%N);}printf("\n");}return 0;}
