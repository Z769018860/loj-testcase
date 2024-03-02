#include<stdio.h>
#include<math.h>
#include<string.h>
int ne[1000100],len1,len2;
char a[1000100],b[1000100];
void get_next()
{
    int  j=-1,i=0;
    ne[0]=-1;
   while(i<len2)
   {
        if(j==-1||b[i]==b[j])
        {
            i++;         //i仍然是0
            j++;         //j仍然是-1
            ne[i]=j;     //在这个里面 i已经变成1 ，j已经变成·0   ,这与ne[i++]=j++ 不一样
        }
         else
        j=ne[j];
   }
}
void kmp()
{
    int  j=0,i=0;
     int ans=0;
     // get_next();
     while(i<len1)
     {
         if(j==-1||a[i]==b[j])
         {
             ++i;
             ++j;
         }
         else
            j=ne[j];
         if(j==len2)
         {

             j=ne[j];    //题目说可以叠加  ，如果不能叠加 j=0 
               ans++;
         }
     }
     printf("%d\n",ans);
}
int main()
{
    scanf("%s %s",a,b);
    len1=strlen(a);    //主
    len2=strlen(b);     //子
    if(len2>len1)
        printf("0\n");
     get_next();
     kmp();
    return 0;
}