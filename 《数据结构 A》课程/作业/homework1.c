#include <stdio.h>
int main()
{
	int m = 0, n = 0, i;
	printf("第一个数组长度多少:");
	scanf_s("%d", &m);
	int a[1000] = {0};
	for (i = 1; i <= m; i++)
	{
		scanf_s("%d", &a[i]);
	}
	printf("第一个数组:\n");
	for (i = 1; i <= m; i++)
	{
		printf("%d ", a[i]);
	}
	printf("\n");
	printf("第二个数组长度多少:");
	scanf_s("%d", &n);
	int b[1000] = { 0 };
	for (i = 1; i <= n; i++)
	{
		scanf_s("%d", &b[i]);
	}
	printf("第二个数组:\n");
	for (i = 1; i <= n; i++)
	{
		printf("%d ", b[i]);
	}
	printf("\n");//初始化

	int c[1000] = { 0 };
	int d[1000] = {0};
	int p=0,q=0,j=1,k=1;
	p=a[1];
	q=b[1];
	c[j] = p;
	d[k] = q;
	int count = 0;
	for (i = 2; i <= m; i++)
	{
		if (a[i] > p)
		{
			j++;
			c[j] = a[i];
			p = a[i];
			count++;
		}
	}
	printf("去重数组长度：%d\n", j);
	printf("去重数组:");
	for (i = 1; i <= j; i++)
	{
		printf("%d ",c[i]);
	}
	printf("\n");
	for (i = 2; i <= n; i++)
	{
		if (b[i]>q)
		{
			k++;
			d[k] = b[i];
			q = b[i];
			count++;
		}
	}
	printf("去重数组长度：%d\n", k);
	printf("去重数组:");
	for (i = 1; i <= k; i++)
	{
		printf("%d ", d[i]);
	}
	printf("\n");//去重数组

	int e,f,h=1;
	int g[1000] = { 0 };
	for (e=1,f=1;;)
	{
		if (c[e] == d[f])
		{
			g[h] = c[e];
			count++;
			e++;
			if (e > j)
			{
				break;
			}
			f++;
			if (f > k)
			{
				break;
			}
			h++;
		}
		else if (c[e] < d[f])
		{
			count++;
			e++;
			if (e > j)
			{
				break;
			}
		}
		else
		{
			count++;
			f++;
			if (f > k)
			{
				break;
			}
		}
	}
	printf("重复元素为：");
	for (i = 1; i <= h; i++)
	{
		printf("%d ",g[i]);
	}
	printf("\n");
	printf("所需运行次数：");
	printf("%d\n", count);//算法
	return 0;
}