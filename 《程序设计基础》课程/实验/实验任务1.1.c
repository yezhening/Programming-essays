#include <stdio.h>
int main()
{
	double a,b,c;
	printf("Please enter three numbers:");
	scanf("%lf,%lf,%lf",&a,&b,&c);
	if(a>0&&b>0&&c>0)
		if(a==b&&a+b>c||a==c&&a+c>b||b==c&&b+c>a)
			printf("这三个数可以构成等腰三角形。\n");
		else
			printf("虽然这三个数都大于0，但是不能构成等腰三角形。\n");
	else
		printf("三角形的边长必须大于0。\n");
	return 0;
}