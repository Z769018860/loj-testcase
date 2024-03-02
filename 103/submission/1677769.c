#include<stdio.h>
#include<string.h>
char a[2333333],b[2333333];
int main(){
    gets(a);
    gets(b);
    int n=strlen(a),m=strlen(b),i;
    if(m>n){puts("0");return 0;}
    long long hs=0,fu=1,sb=0;
    for(i=0;i<m;i++)hs=hs*233+a[i],sb=sb*233+b[i],fu*=233;
    int ans=(hs==sb);
    for(i=m;i<n;i++){
        hs=hs*233+a[i]-fu*a[i-m];
        ans+=(hs==sb);
    }
    printf("%d", ans);
}