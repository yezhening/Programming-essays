#include <stdio.h>
int main()
{
	int a[101]={0},n,x,i;//桶排序，初始化为0
	for(n=1;;n++)
	{
		scanf("%d",&x);
		if(x==-1)
			break;
		if(n==101)
			break;
		a[x]++;//扫描到就在其中放个小旗子即赋值	 
	}
	for(i=1;i<=n;i++)
	{
		if(a[i]!=0)//从头开始，有小旗子就输出小旗子个数，即为排序数
		{
			printf("%d",i);//数字
			printf(" ");
			printf("%d",a[i]);//数字个数
			printf("\n");
			
		}
	}
	return 0;
}