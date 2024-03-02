#include<stdio.h>
#include<string.h>
char s[1000010],t[1000010];
int next[1000010];
int slen,tlen;
void getnext()
{
	slen=strlen(s);
	tlen=strlen(t);
	int j=0,k=-1;
	next[0]=-1;
	while(j<tlen)
	{
		if(k==-1||t[j]==t[k])
			next[++j]=++k;
		else
			k=next[k];
	}
}
int kmp()
{
	int ans=0,i,j=0;
	getnext();
	for(i=0;i<slen;i++)
	{
		while(j>=0&&s[i]!=t[j])
		{
			j=next[j];
		}
		if(j==-1||s[i]==t[j])
			j++;
		if(j==tlen)
		{
			ans++;
			j=next[j];
		}
	}
	return ans;
}
int main()
{
	while(~scanf("%s%s",s,t))
	{
		printf("%d\n",kmp());	
	}
	return 0;	
} 