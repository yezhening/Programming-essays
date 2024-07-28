#include <stdio.h>

//递归计数
int count = 1;

//记录函数function“函数激活”地址
int *address1 = NULL;
int *address2 = NULL;

//定义函数
void function()
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
    printf("指针类型变量的首地址：%p 数值：%d 类型长度：%d\n\n", &p, *p, sizeof(int *));

    //记录函数function“函数激活”地址
    if (count == 1)
    {
        address1 = &intVar;
    }
    else
    {
        address2 = &intVar;
    }

    //递归调用
    if (count != 0)
    {
        count--;
        //查看“函数激活”大小
        function();
    }
}

void function1()
{
}

int main()
{
    //查看函数地址
    printf("函数function的地址：%p\n\n", &function);

    //查看函数大小
    printf("函数function的地址：%p\n", &function);
    printf("函数function1的地址：%p\n", &function1);
    printf("函数function大小：%d\n\n", &function1 - &function);

    //查看“函数激活”大小
    function();
    printf("函数functionint变量地址：%p\n", address1);
    printf("函数function递归int变量地址：%p\n", address2);
    printf("函数function“函数激活”大小：%d\n", address1 - address2);

    //查看赋值语句大小，不会

    return 0;
}