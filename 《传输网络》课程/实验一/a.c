#include <stdio.h>

//寄存器状态取值
int d1 = 1;
int d2 = 0;
int d3 = 1;
int d4 = 0;
int x[32] = {0};                                                                                               //输入序列全0
int y[32] = {0};                                                                                               //加扰序列  多个函数用到，放全局变量
int x1[32] = {0};                                                                                              //解扰序列
int sn[32] = {1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1}; //输入序列学号

void structOneScrambling(int x[]);            //结构1加扰器
void structOneDescrambling(int x[], int y[]); //结构1解扰器
void structTwoScrambling(int x[]);            //结构2加扰器
void structTwoDescrambling(int x[], int y[]); //结构2解扰器
int moduleTwoPlus(int i1, int i2);            //模二加
void compareSequence(int i1[], int i2[]);     //比较序列
void sequenceError(int y[]);                  //序列取反

//系统函数
void structOneScrambling(int x[]) //结构1加扰器
{
    printf("输入序列X：");
    for (int j = 0; j < 32; j++)
    {
        printf("%d", x[j]);
    }
    printf("\n");
    printf("寄存器初始状态：%d    %d    %d    %d\n\n", d1, d2, d3, d4);
    printf("加扰器D1~D4的状态变化过程:\n");

    //计算结果中间值
    int t1 = d1;
    int t2 = d2;
    int t3 = d3;
    int t4 = d4;
    int yCycle = 0;     //记录输出序列Y的周期
    int yCycleFlag = 0; //记录是否已记录输出序列Y的周期

    for (int i = 0; i < 32; i++) //输出加扰器D1~D4的状态变化过程
    {
        int temp1 = 0; //中间值
        int temp2 = 0;

        temp1 = moduleTwoPlus(t3, t4);      //计算d3、d4
        temp2 = moduleTwoPlus(temp1, x[i]); //计算d3、d4、x 使用字符数组x[i]有结束符

        //加入扰码输出序列y
        y[i] = temp2;

        //移位
        t4 = t3;
        t3 = t2;
        t2 = t1;
        t1 = temp2;

        //打印输出
        printf("第%d次：%d  %d  %d  %d\n", i + 1, t1, t2, t3, t4);
        if ((i + 1) % 5 == 0) //每五个换行
        {
            printf("\n");
        }

        if (t1 == d1 && t2 == d2 && t3 == d3 && t4 == d4) //到达一个周期
        {
            if (yCycleFlag == 0) //未记录过周期
            {
                yCycle = i + 1; //记录输出序列Y的周期 数组从0开始，+1
                yCycleFlag = 1; //已记录过周期
            }
            else //已记录过周期
            {
                if (i + 1 != 2 * yCycle) //下一个“周期到来”，判断是否已过一个周期   不是
                {
                    yCycle = 0; //周期置0
                }
            }
        }
    }

    printf("\n输出序列Y:"); //输出序列Y
    for (int j = 0; j < 32; j++)
    {
        printf("%d", y[j]);
    };

    if (yCycle == 0) //无周期
    {
        printf("\n输出序列Y无周期\n\n");
    }
    else //有周期
    {
        printf("\n输出序列Y的周期是:%d\n\n", yCycle);
    }
}
void structOneDescrambling(int x[], int y[]) //结构1解扰器
{
    printf("输入序列Y：");
    for (int j = 0; j < 32; j++)
    {
        printf("%d", y[j]);
    }
    printf("\n");
    printf("寄存器初始状态：%d    %d    %d    %d\n\n", d1, d2, d3, d4);
    printf("解扰器D1~D4的状态变化过程:\n");

    //计算中间值
    int t1 = d1;
    int t2 = d2;
    int t3 = d3;
    int t4 = d4;

    for (int i = 0; i < 32; i++) //输出解扰器D1~D4的状态变化过程
    {
        int temp1 = 0; //中间值
        int temp2 = 0;

        temp1 = moduleTwoPlus(t3, t4);      //计算d3、d4
        temp2 = moduleTwoPlus(temp1, y[i]); //计算d3、d4、y

        //加入扰码输出序列y
        x1[i] = temp2;

        //移位
        t4 = t3;
        t3 = t2;
        t2 = t1;
        t1 = y[i]; //这是解扰器和加扰器不同的点 送进去的是y

        //打印输出
        printf("第%d次：%d  %d  %d  %d\n", i + 1, t1, t2, t3, t4);
        if ((i + 1) % 5 == 0) //每五个换行
        {
            printf("\n");
        }
    }

    printf("\n输出序列x1:"); //输出序列X1
    for (int j = 0; j < 32; j++)
    {
        printf("%d", x1[j]);
    };
    printf("\n");

    compareSequence(x, x1); //验证序列X和X1
}
void structTwoScrambling(int x[]) //结构2加扰器
{
    printf("输入序列X：");
    for (int j = 0; j < 32; j++)
    {
        printf("%d", x[j]);
    }
    printf("\n");
    printf("寄存器初始状态：%d    %d    %d    %d\n\n", d1, d2, d3, d4);
    printf("加扰器D1~D4的状态变化过程:\n");

    //计算结果中间值
    int t1 = d1;
    int t2 = d2;
    int t3 = d3;
    int t4 = d4;
    int yCycle = 0;     //记录输出序列Y的周期
    int yCycleFlag = 0; //记录是否已记录输出序列Y的周期

    for (int i = 0; i < 32; i++) //输出加扰器D1~D4的状态变化过程
    {
        int temp1 = 0; //中间值
        int temp2 = 0;

        temp1 = moduleTwoPlus(t3, t4);      //计算d3、d4
        temp2 = moduleTwoPlus(temp1, x[i]); //计算d3、d4、x 使用字符数组x[i]有结束符

        //加入扰码输出序列y
        y[i] = temp2;

        //移位
        t4 = t3;
        t3 = t2;
        t2 = t1;
        t1 = temp1;

        //打印输出
        printf("第%d次：%d  %d  %d  %d\n", i + 1, t1, t2, t3, t4);
        if ((i + 1) % 5 == 0) //每五个换行
        {
            printf("\n");
        }

        if (t1 == d1 && t2 == d2 && t3 == d3 && t4 == d4) //到达一个周期
        {
            if (yCycleFlag == 0) //未记录过周期
            {
                yCycle = i + 1; //记录输出序列Y的周期 数组从0开始，+1
                yCycleFlag = 1; //已记录过周期
            }
            else //已记录过周期
            {
                if (i + 1 != 2 * yCycle) //下一个“周期到来”，判断是否已过一个周期   不是
                {
                    yCycle = 0; //周期置0
                }
            }
        }
    }

    printf("\n输出序列Y:"); //输出序列Y
    for (int j = 0; j < 32; j++)
    {
        printf("%d", y[j]);
    };

    if (yCycle == 0) //无周期
    {
        printf("\n输出序列Y无周期\n\n");
    }
    else //有周期
    {
        printf("\n输出序列Y的周期是:%d\n\n", yCycle);
    }
}
void structTwoDescrambling(int x[], int y[]) //结构2解扰器
{
    printf("输入序列Y：");
    for (int j = 0; j < 32; j++)
    {
        printf("%d", y[j]);
    }
    printf("\n");
    printf("寄存器初始状态：%d    %d    %d    %d\n\n", d1, d2, d3, d4);
    printf("解扰器D1~D4的状态变化过程:\n");

    //计算中间值
    int t1 = d1;
    int t2 = d2;
    int t3 = d3;
    int t4 = d4;

    for (int i = 0; i < 32; i++) //输出解扰器D1~D4的状态变化过程
    {
        int temp1 = 0; //中间值
        int temp2 = 0;

        temp1 = moduleTwoPlus(t3, t4);      //计算d3、d4
        temp2 = moduleTwoPlus(temp1, y[i]); //计算d3、d4、y

        //加入扰码输出序列y
        x1[i] = temp2;

        //移位
        t4 = t3;
        t3 = t2;
        t2 = t1;
        t1 = temp1; //这是解扰器和加扰器不同的点 送进去的是y

        //打印输出
        printf("第%d次：%d  %d  %d  %d\n", i + 1, t1, t2, t3, t4);
        if ((i + 1) % 5 == 0) //每五个换行
        {
            printf("\n");
        }
    }

    printf("\n输出序列x1:"); //输出序列X1
    for (int j = 0; j < 32; j++)
    {
        printf("%d", x1[j]);
    };
    printf("\n");

    compareSequence(x, x1); //验证序列X和X1
}

//功能函数
int moduleTwoPlus(int s1, int s2) //模二加
{
    int result = 0;

    if (s1 == s2) //00或11
    {
        result = 0;
    }
    else //01或10
    {
        result = 1;
    }

    return result;
}
void compareSequence(int s1[], int s2[]) //比较序列
{
    // int length = (sizeof(s1) / sizeof(s1[0])); //序列长度
    int flag = 0; //记录是否有位不同

    for (int i = 0; i < 32; i++)
    {
        if (s1[i] != s2[i])
        {
            flag = 1;                      //有位数不同
            printf("第%d位不同\n", i + 1); //位数应+1
        }
    }

    if (flag == 0) //无位数不同
    {
        printf("无位数不同\n");
    }

    printf("\n");
}
void sequenceError(int y[]) //序列取反
{
    int count = 0;
    printf("请输入取反位数：");
    scanf("%d", &count);
    printf("\n");

    printf("正确输入序列：");
    for (int i = 0; i < 32; i++)
    {
        printf("%d", y[i]);
    }
    printf("\n");

    //取反
    for (int i = 0; i < 32; i++)
    {
        if (i + 1 == count) //i从0开始
        {
            if (y[i] == 0)
            {
                y[i] = 1;
            }
            else
            {
                y[i] = 0;
            }
        }
    }

    printf("错误输入序列：");
    for (int i = 0; i < 32; i++)
    {
        printf("%d", y[i]);
    }
    printf("\n\n");
}

//主函数
int main(void)
{
    int select = 0; //选择操作

    while (1)
    {
        printf("结构1加扰解扰，输入序列全0：1\n");
        printf("结构1加扰解扰，输入序列学号：2\n");
        printf("结构2加扰解扰，输入序列全0：3\n");
        printf("结构2加扰解扰，输入序列学号：4\n");

        printf("请选择操作：\n");
        scanf("%d", &select);

        switch (select)
        {
        case 1: //结构1加扰解扰 输入序列全0
            structOneScrambling(x);
            structOneDescrambling(x, y);
            sequenceError(y);
            structOneDescrambling(x, y);
            break;
        case 2: //结构1加扰解扰 输入序列学号
            structOneScrambling(sn);
            structOneDescrambling(sn, y);
            sequenceError(y);
            structOneDescrambling(sn, y);
            break;
        case 3: //结构2加扰解扰 输入序列全0
            structTwoScrambling(x);
            structTwoDescrambling(x, y);
            sequenceError(y);
            structTwoDescrambling(x, y);
            break;
        case 4: //结构2加扰解扰 输入序列全0
            structTwoScrambling(sn);
            structTwoDescrambling(sn, y);
            sequenceError(y);
            structTwoDescrambling(sn, y);
            break;
        default:
            return 0;
        }
    }

    return 0;
}