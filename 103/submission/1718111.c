#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define P 131
int main(){
	char *A,*B;
	int count,LA,LB,i,j;
	unsigned long long *hash,h,*power;
	A=(char *)malloc((1e6+2)*sizeof(char));
	B=(char *)malloc((1e6+2)*sizeof(char));
	hash=(unsigned long long *)malloc((1e6+1)*sizeof(unsigned long long));
	power=(unsigned long long *)malloc((1e6+1)*sizeof(unsigned long long));
	gets(A+1);
	gets(B+1);
	LA=strlen(A+1);
	LB=strlen(B+1);
	power[0]=1;
	for(i=1;i<=LA;i++) power[i]=power[i-1]*P;
	hash[0]=0;
	for(i=1;i<=LA;i++) hash[i]=hash[i-1]*P+A[i]-'0';
	h=0;
	for(i=1;i<=LB;i++) h=h*P+B[i]-'0';
	count=0;
	for(i=0;i<=LA-LB;i++){ 
		if(h==hash[i+LB]-hash[i]*power[LB]) count++;
	}
	printf("%d\n",count);
	free(A);
	free(B);
	free(hash);
	free(power);
	return 0;
}
