#include <stdio.h>
int main()
{
	int n,i,j,k;
	printf("Please enter a integer number:");
	scanf("%d",&n);
	if(n%2==0)
		printf("输入为偶数，不符合要求！");
	else
		for(i=1;i<=(n+1)/2;i++)//行数
		{
			for(j=1;j<=(n+1)/2-i;j++)//空格
				printf(" ");
				for(k=1;k<=2*i-1;k++)//星星	
					printf("*");
					printf("\n");
		}
		for(i=1;i<=n/2;i++)
		{
			for(j=1;j<=i;j++)
				printf(" ");
				for(k=1;k<=n-2*i;k++)	
					printf("*");
					printf("\n");	
		}
		
	return 0;
}