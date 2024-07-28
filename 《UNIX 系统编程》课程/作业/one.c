#include <stdio.h>
int main()
{
    //定义变量
    short shortVar = 0;
    int intVar = 1;
    unsigned unsignedVar = 2; //单独使用为unsigned int
    long longVar = 3;
    float floatVar = 4;
    double doubleVar = 5;
    int *p = &intVar; //地址字节数，与操作系统位数有关

    //查看变量首地址、数值和类型长度
    printf("short类型变量的首地址：%p 数值：%hd 类型长度：%d\n", &shortVar, shortVar, sizeof(short));
    printf("int类型变量的首地址：%p 数值：%d 类型长度：%d\n", &intVar, intVar, sizeof(int));
    printf("unsigned类型变量的首地址：%p 数值：%u 类型长度：%d\n", &unsignedVar, unsignedVar, sizeof(unsigned));
    printf("long类型变量的首地址：%p 数值：%ld 类型长度：%d\n", &longVar, longVar, sizeof(long));
    printf("float类型变量的首地址：%p 数值：%f 类型长度：%d\n", &floatVar, floatVar, sizeof(float));
    printf("double类型变量的首地址：%p 数值：%lf 类型长度：%d\n", &doubleVar, doubleVar, sizeof(double));
    printf("指针类型变量的首地址：%p 数值：%d 类型长度：%d\n", &p, *p, sizeof(int *));

    return 0;
}