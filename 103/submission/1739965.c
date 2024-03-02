#include<stdio.h>
#include<string.h>

char a[1000000];
char b[1000000];
	
int main()
{
	
	scanf("%s%s",a,b);
	int lena = strlen(a);
	int lenb = strlen(b);
	int i,sum=0;
	for(i=0;i<lena-lenb+1;i++)
		{
			if(strncmp(&a[i],b,lenb)==0)
				sum++;
		}
	printf("%d",sum);
}