#include<stdio.h>
#include<string.h>

const int N =1e6+10;

char s[1000010],p[1000010];
int ne[1000010];
int slen,plen;
//void getnext()
//{
//	slen=strlen(s);
//	tlen=strlen(t);
//	int j=0,k=-1;
//	next[0]=-1;
//	while(j<tlen)
//	{
//		if(k==-1||t[j]==t[k])
//			next[++j]=++k;
//		else
//			k=next[k];
//	}
//}

int KMP()
{
	plen=strlen(p+1);
	slen=strlen(s+1);
	int ans=0;
	for (int i = 2, j = 0; i <= plen; i ++ )
	{
	    while (j && p[i] != p[j + 1]) j = ne[j];
	    if (p[i] == p[j + 1]) j ++ ;
	    ne[i] = j;
	}
	
	// 匹配
	for (int i = 1, j = 0; i <= slen; i ++ )
	{
	    while (j && s[i] != p[j + 1]) j = ne[j];
	    if (s[i] == p[j + 1]) j ++ ;
	    if (j == plen)
	    {
	        j = ne[j];
	        ans++;
	        // 匹配成功后的逻辑
	    }
	}
	return ans;
}
//
//int kmp()
//{
//	int ans=0,i,j=0;
//	getnext();
//	for(i=0;i<slen;i++)
//	{
//		while(j>=0&&s[i]!=t[j])
//		{
//			j=next[j];
//		}
//		if(j==-1||s[i]==t[j])
//			j++;
//		if(j==tlen)
//		{
//			ans++;
//			j=next[j];
//		}
//	}
//	return ans;
//}
int main()
{
	while(~scanf("%s%s",s+1,p+1))
	{
		printf("%d\n",KMP());	
	}
	return 0;	
} 