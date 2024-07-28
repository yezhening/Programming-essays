// 头文件------------------------------
#include <stdio.h>
#include <string.h> //strlen() strcmp() strcpy() strcat() memset()
#include <stdlib.h> //malloc() free() atoi() itoa() exit()
#include <ctype.h>  // isdigit()

// 结构体------------------------------
// 符号表
struct SymbolTable
{
    char varName[10]; // 变量名，最大长度为十个字符
    char *varAddress; // 变量地址
    char varType[10]; // 变量类型，最大长度为十个字符
};
// 栈区表
struct StackArea
{
    char *varAddress;  // 变量地址
    char varValue[10]; // 变量值，最大长度为十个字符
};
// 全局区表
struct GlobalArea
{
    char *varAddress;  // 变量地址
    char varValue[10]; // 变量值，最大长度为十个字符
};
//函数符号表
struct FunctionSymbolTable
{
    char varName[10]; // 函数名，最大长度为十个字符
    char *varAddress; // 函数地址
    char varType[20]; // 函数类型，最大长度为二十个字符
};

//全局变量------------------------------
struct SymbolTable st[10]; // 符号表结构体数组
int stNum = 0;             // 符号表结构体数组序号

char *stackArea = NULL;        // 模拟栈 首地址静态不变
char *stackAreaPointer = NULL; // 栈指针   会动态改变
//函数定义时会记录地址，记录预先分配一定栈帧空间
char *stackAreaFunctionHead = NULL; //头
char *stackAreaFunctionTail = NULL; //尾
char *stackAreaAssignment = NULL;   //记录在一个函数栈帧中压栈后的各个地址
struct StackArea sa[10];            //栈区表结构体数组
int saNum = 0;                      // 栈区表结构体数组序号

char *globalArea = NULL;        // 模拟全局区 首地址静态不变
char *globalAreaPointer = NULL; // 全局区地址指针   会动态改变
struct GlobalArea ga[20];       //全局区结构体数组
int gaNum = 0;                  // 全局区结构体数组序号

char (*codeArea)[30] = NULL;        //模拟代码区 首地址静态不变 指向指针的指针/指针数组 指向一个 大小为30的字符数组/列指针 指向五个行指针的首地址
char (*codeAreaPointer)[30] = NULL; //使用其来操作代码区
int codeAreaStatementNum = 0;       //记录存入代码区语句数

// 函数符号表结构体数组
struct FunctionSymbolTable fst[10];
// 函数符号表结构体数组序号
int fstNum = 0;

int areaTypeFlag = 0; //记录操作区域类型 0是符号表，全局区，默认 1是函数符号表，栈区

const int intByte = 4;       //int类型变量分配字节数
const int stringByte = 10;   //string类型变量分配字节数 一个字符一个字节，一个字符串最多十个字节
const int functionByte = 20; //函数类型分配字节数

// 函数------------------------------
//函数声明
//系统函数
void openUpMemory();                       //开辟内存空间
void freeMemory();                         // 释放内存空间
void statementProcessing(char sentence[]); // 语句处理

//功能函数
void varDeclaration(char type[], char name[]); //变量声明
void varAssignment(char sentence[]);           //变量赋值
void varGetValue(char sentence[]);             //变量取值
void varGetAddress(char sentence[]);           // 变量取址
void varCalculation(char sentence[]);          //变量计算
void getStringSize(char sentence[]);           //字符串取长度
void ifStatement(char sentence[]);             //if语句
void list(char part2[]);                       // 打印符号表、函数符号表、内存
void whileStatement(char sentence[]);          // while语句

//函数有关
void functionDefinition(char name[]);                                                  //函数定义
void functionCall(char sentence[], char type[]);                                       //函数调用
void functionStatementProcessing(char argument1[], char argument2[], char sentence[]); //函数语句处理
void functionReturn(char sentence[]);                                                  //函数返回
void functionClear();                                                                  // 函数擦除

//辅助函数
//取值 数字、字符串和变量
char *getValue(char argument[]);
void calculationStorage(char type[], char part1[], char result[]); //计算存储
int conditionalJudgment(char judgement[]);                         //条件判断函数

// 函数定义
// 系统函数
// 开辟内存空间
void openUpMemory()
{
    // 栈区
    stackArea = (char *)malloc(sizeof(char) * 80); //申请实际内存空间  80+16字节：内存对齐
    //判断是否分配成功
    if (stackArea == NULL) //失败
    {
        printf("开辟栈区内存空间失败\n");
        exit(1);
    }
    else //成功
    {
        memset(stackArea, 0, sizeof(char) * 80); //初始化栈区空间

        stackAreaPointer = stackArea; //栈区地址指针赋值
    }

    // 全局区
    globalArea = (char *)malloc(sizeof(char) * 80); //申请实际内存空间  80+16字节：内存对齐
    //判断是否分配成功
    if (globalArea == NULL) //失败
    {
        printf("开辟全局区内存空间失败\n");
        exit(1);
    }
    else //成功
    {
        memset(globalArea, 0, sizeof(char) * 80); //初始化全局区空间

        globalAreaPointer = globalArea; //全局区地址指针赋值
    }

    //代码区
    char(*codeArea)[30] = (char(*)[30])malloc(sizeof(char) * 5 * 30); //五行三十列的二维字符数组指针 理解
    //判断是否分配成功
    if (codeArea == NULL) //失败
    {
        printf("开辟代码区内存空间失败\n");
        exit(1);
    }
    else //成功
    {
        memset(codeArea, 0, sizeof(char) * 5 * 30); //初始化代码区空间

        codeAreaPointer = codeArea; //代码区地址指针赋值
    }
}
// 释放内存空间
void freeMemory()
{
    //释放栈区空间
    free(stackArea);
    stackArea = NULL;

    //释放全局区空间
    free(globalArea);
    globalArea = NULL;

    //释放全局区空间
    free(codeArea);
    codeArea = NULL;
}
// 语句处理
void statementProcessing(char sentence[])
{
    // 一.判断整条语句
    // 转退出函数 最优先级
    if (strcmp(sentence, "exit;") == 0)
    {
        exit(1); //退出程序
    }

    //二.判断语句第一部分
    // 接收语句部分
    char part1[15] = {0}; //动态数组不好跟踪数组内变量
    char part2[35] = {0}; //part1用于判断，part2存取剩余的，在if语句中会很多字符

    //判断存取部分变量
    int part1I = 0;
    int part2I = 0;

    int partFlag = 1; //存取标志 数字代表第几部分

    //扫描语句
    int sentenceLength = strlen(sentence); //语句长度
    for (int sentenceI = 0; sentenceI < sentenceLength; sentenceI++)
    {
        //判断分隔符
        if (sentence[sentenceI] == ' ' || sentence[sentenceI] == '(') //空格：声明 左括号：字符串取长度,if语句,while语句,函数调用
        {
            partFlag = 2;
            sentenceI++; //跳过空格、左括号
        }
        //判断结束符
        if (sentence[sentenceI] == ';')
        {
            partFlag = 0;
        }

        //存语句段
        if (partFlag == 1)
        {
            part1[part1I] = sentence[sentenceI];
            part1I++;
        }
        if (partFlag == 2)
        {
            part2[part2I] = sentence[sentenceI];
            part2I++;
        }
    }

    // 判断part1
    if (strcmp(part1, "int") == 0 || strcmp(part1, "string") == 0) //动态数组指针只有首地址的值 int a;
    {
        varDeclaration(part1, part2); // 转变量声明函数
    }

    //转函数定义函数
    if (strcmp(part1, "def") == 0) //define func
    {
        functionDefinition(part2); //传函数名即可
    }

    //转函数调用函数
    //查函数符号表
    for (int fstI = 0; fstI < fstNum; fstI++)
    {
        if (strcmp(fst[fstI].varName, part1) == 0) //是函数名 func(a,b);
        {
            functionCall(sentence, fst[fstI].varType); //传语句，函数类型
        }
    }

    //转字符串取长度函数
    if (strcmp(part1, "size") == 0)
    {
        getStringSize(sentence); //size(a);
    }

    //转if语句函数
    if (strcmp(part1, "if") == 0)
    {
        ifStatement(sentence); //if(a==b) a=s s else b=d d endif;
    }

    //转打印符号表、函数符号表、内存函数
    if (strcmp(part1, "list") == 0)
    {
        list(part2); //all
    }

    //三.扫描语句第一部分具体内容
    int part1Length = strlen(part1); //第一部分语句长度

    int assignmentFlag = 0;  //判断是否有赋值符
    int getValueFlag = 0;    //判断是否有取值符
    int getAddressFlag = 0;  //判断是否有取地址符
    int calculationFlag = 0; //判断是否有计算符

    int bracketsFlag = 0; //判断是否有括号符

    for (int judgeI = 0; judgeI < part1Length; judgeI++)
    {
        if (part1[judgeI] == '=')
        {
            assignmentFlag = 1;
        }
        if (part1[judgeI] == '?') //取值符
        {
            getValueFlag = 1;
        }
        if (part1[judgeI] == '&') //取址符
        {
            getAddressFlag = 1;
        }
        if (part1[judgeI] == '+' || part1[judgeI] == '-' || part1[judgeI] == '*' || part1[judgeI] == '/' || part1[judgeI] == '|' || part1[judgeI] == '^')
        {
            calculationFlag = 1;
        }
    }

    // 转变量赋值函数
    if (assignmentFlag == 1 && calculationFlag == 0) //calculationFlag == 0：避免a=a+b变量计算情况 a=3;
    {
        varAssignment(sentence); //传";"
    }

    // 转变量取值函数
    if (getValueFlag == 1)
    {
        varGetValue(part1); //?a
    }

    // 转变量取址函数
    if (getAddressFlag == 1)
    {
        varGetAddress(part1); //&a
    }

    //转变量计算函数
    if (calculationFlag == 1) //无赋值符判断
    {
        varCalculation(part1); //a=a+b
    }

    int whileFlag = 0;  //while语句标志
    if (whileFlag == 1) //while声明后全转while语句函数
    {
        whileStatement(sentence);
    }

    // 转while语句函数
    if (strcmp(part1, "while") == 0)
    {
        whileFlag = 1;
        whileStatement(sentence);
    }
}

// 功能函数
// 变量声明
void varDeclaration(char type[], char name[]) //int a
{
    if (areaTypeFlag == 0) //符号表，全局区
    {
        // 符号表项记录
        strcpy(st[stNum].varName, name);          //变量名
        st[stNum].varAddress = globalAreaPointer; //变量地址
        strcpy(st[stNum].varType, type);          // 变量类型

        //判断类型  全局区地址指针增加
        if (strcmp(type, "int") == 0)
        {
            globalAreaPointer = globalAreaPointer + intByte; //全局区指针递增 一个十六进制地址代表一个字节 最多分配四个字节，四个字符
        }
        else //string类型
        {
            globalAreaPointer = globalAreaPointer + stringByte; //全局区指针递增一个十六进制地址代表一个字节  最多分配十个字节，十个字符
        }

        stNum++; //符号表项序号递增
    }
    else //函数符号表，栈区
    {
        //记录函数符号表项
        strcpy(fst[fstNum].varName, name);         //变量名
        fst[fstNum].varAddress = stackAreaPointer; //记录当前栈指针
        strcpy(fst[fstNum].varType, type);         // 变量类型

        if (strcmp(fst[fstNum].varType, "int") == 0)
        {
            stackAreaPointer = stackAreaPointer + intByte; //栈区指针递增 为其地址预留四字节空间
        }
        else //string类型
        {
            stackAreaPointer = stackAreaPointer + stringByte; //栈区指针递增 为其地址预留十字节空间
        }

        fstNum++; //函数符号表项序号递增
    }
}
// 变量赋值
void varAssignment(char sentence[]) //a="sdf";
{
    // 接收语句部分
    char part1[10] = {0};
    char part2[10] = {0};

    // 判断存取部分变量
    int part1I = 0; //动态数组不好跟踪数组内变量
    int part2I = 0;

    int partFlag = 1; //存储标志

    // 扫描语句
    int sentenceLength = strlen(sentence); //语句长度
    for (int sentenceI = 0; sentenceI < sentenceLength; sentenceI++)
    {
        //判断分隔符
        if (sentence[sentenceI] == '=')
        {
            partFlag = 2;
            sentenceI++; //跳过赋值符
        }
        if (sentence[sentenceI] == ';') //判断结束符
        {
            partFlag = 0;
        }

        //存语句段
        if (partFlag == 1)
        {
            part1[part1I] = sentence[sentenceI];
            part1I++;
        }
        if (partFlag == 2)
        {
            part2[part2I] = sentence[sentenceI];
            part2I++;
        }
    }

    // 取值
    char *value = NULL; //存储值
    value = getValue(part2);

    // 赋值
    if (areaTypeFlag == 0) //符号表，全局区
    {
        // 查符号表
        int stI = 0;
        for (stI = 0; stI < stNum; stI++)
        {
            // 先查变量声明
            if (strcmp(st[stI].varName, part1) == 0) //part1是变量名
            {
                //再判断变量类型
                if (strcmp(st[stI].varType, "int") == 0) //int类型
                {
                    // 后取地址赋值
                    for (int assignI = 0; assignI < intByte; assignI++)
                    {
                        st[stI].varAddress[assignI] = value[assignI]; //一字节一字符存内存，最多四字节  即字符数组循环赋值 part2是值 这里的地址即为全局区的值
                    }
                }
                else //string类型
                {
                    // 后取地址赋值
                    for (int assignI = 0; assignI < stringByte; assignI++)
                    {
                        st[stI].varAddress[assignI] = value[assignI]; //一字节一字符存内存，最多十字节  即字符数组循环赋值
                    }
                }

                // 记录全局区项
                ga[gaNum].varAddress = st[stI].varAddress; //变量地址
                strcpy(ga[gaNum].varValue, value);         //变量值
                gaNum++;                                   // 全局区项递增

                break; //查到跳出循环
            }
        }

        //未查到报错
        if (stI == stNum)
        {
            printf("左变量%s未声明，不允许赋值操作\n", part1);
        }
    }
    else //函数符号表，栈区
    {
        //查函数符号表
        int fstI = 0;
        for (fstI = 0; fstI < fstNum; fstI++)
        {
            //查声明
            if (strcmp(fst[fstI].varName, part1) == 0) //查到
            {
                //查类型
                if (strcmp(fst[fstI].varType, "int") == 0) //int类型
                {
                    //创建当前被调用函数的栈帧 存放局部变量
                    for (int assignI = 0; assignI < intByte; assignI++)
                    {
                        fst[fstI].varAddress[assignI] = value[assignI];
                    }
                }
                else //string类型
                {
                    //创建当前被调用函数的栈帧 存放局部变量
                    for (int assignI = 0; assignI < stringByte; assignI++)
                    {
                        fst[fstI].varAddress[assignI] = value[assignI];
                    }
                }

                // 记录栈区项
                sa[saNum].varAddress = fst[fstI].varAddress; //变量地址
                strcpy(sa[saNum].varValue, value);           //变量值
                saNum++;                                     // 栈区项递增

                break; //查到跳出循环
            }
        }

        //未查到报错
        if (fstI == fstNum)
        {
            printf("左变量%s未声明，不允许赋值操作\n", part1);
        }
    }
}
// 变量取值
void varGetValue(char sentence[]) //?a
{
    char varName[5] = {0}; //变量名 五个字符

    // 取变量名
    int sentenceLength = strlen(sentence);
    for (int getI = 1; getI < strlen(sentence); getI++) //跳过取值符
    {
        varName[getI - 1] = sentence[getI]; //数组从0开始
    }

    //所取值
    char varIntValue[4] = {0}; //静态数组只能给具体数字
    char varStringValue[10] = {0};

    if (areaTypeFlag == 0) //符号表，全局区
    {
        // 查符号表
        int stI = 0;
        for (stI = 0; stI < stNum; stI++)
        {
            // 先查变量名
            if (strcmp(st[stI].varName, varName) == 0) //查到
            {
                //判断是否已赋值
                if (*st[stI].varAddress == '\0') //根据第一个字符判断
                {
                    printf("变量%s未赋值，不允许取值操作\n", varName); //提示

                    return; //退出该函数
                }

                //再判断变量类型
                if (strcmp(st[stI].varType, "int") == 0) //int类型
                {
                    // 后从地址取值
                    for (int readI = 0; readI < intByte; readI++)
                    {
                        varIntValue[readI] = st[stI].varAddress[readI]; //一字节一字符读内存，最多四字节  即字符数组循环取值
                    }

                    // 打印值
                    printf("变量%s值为：%s\n", varName, varIntValue); //使用字符数组显示
                }
                else //string类型
                {
                    // 后从地址取值
                    for (int readI = 0; readI < stringByte; readI++)
                    {
                        varStringValue[readI] = st[stI].varAddress[readI]; //一字节一字符读内存，最多十字节  即字符数组循环取值
                    }

                    // 打印值
                    printf("变量%s值为：%s\n", varName, varStringValue); //使用字符数组显示
                }

                break; //查到跳出循环
            }
        }

        //未查到报错
        if (stI == stNum)
        {
            printf("变量未定义，不允许取值操作\n");
        }
    }
    else //函数符号表，栈区
    {
        // 查函数符号表
        int fstI = 0;
        for (fstI = 0; fstI < fstNum; fstI++)
        {
            // 先查变量名
            if (strcmp(fst[fstI].varName, varName) == 0) //查到
            {
                //判断是否已赋值
                if (*fst[fstI].varAddress == '\0')
                {
                    printf("变量%s未赋值，不允许取值操作\n", varName); //提示

                    return; //退出该函数
                }

                //再判断变量类型
                if (strcmp(fst[fstI].varType, "int") == 0) //int类型
                {
                    // 后从地址取值
                    for (int readI = 0; readI < intByte; readI++)
                    {
                        varIntValue[readI] = fst[fstI].varAddress[readI]; //一字节一字符读内存，最多四字节  即字符数组循环取值
                    }

                    // 打印值
                    printf("变量%s值为：%s\n", varName, varIntValue); //使用字符数组显示
                }
                else //string类型
                {
                    // 后从地址取值
                    for (int readI = 0; readI < stringByte; readI++)
                    {
                        varStringValue[readI] = fst[fstI].varAddress[readI]; //一字节一字符读内存，最多十字节  即字符数组循环取值
                    }

                    // 打印值
                    printf("变量%s值为：%s\n", varName, varStringValue); //使用字符数组显示
                }

                break; //查到跳出循环
            }
        }

        //未查到报错
        if (fstI == fstNum)
        {
            printf("变量未定义，不允许取值操作\n");
        }
    }
}
// 变量取址
void varGetAddress(char sentence[]) //&a
{
    char varName[5] = {0};   //存储变量名
    char *varAddress = NULL; //变量地址

    //取变量名
    int sentenceLength = strlen(sentence);                           //语句长度
    for (int sentenceI = 1; sentenceI < sentenceLength; sentenceI++) //跳过取址符
    {
        varName[sentenceI - 1] = sentence[sentenceI]; //注意-1操作
    }

    //取地址
    if (areaTypeFlag == 0) //符号表，全局区
    {
        // 查符号表
        int stI = 0;
        for (stI = 0; stI < stNum; stI++)
        {
            // 先查变量名
            if (strcmp(st[stI].varName, varName) == 0) //查到
            {
                // 后取地址
                varAddress = st[stI].varAddress;

                // 打印值
                printf("变量%s地址为：%p\n", varName, varAddress);

                break; //查到跳出循环
            }
        }

        //未查到报错
        if (stI == stNum)
        {
            printf("变量%s未声明，不允许取地址操作\n", varName);
        }
    }
    else //函数符号表，栈区
    {
        // 查符号表
        int fstI = 0;
        for (fstI = 0; fstI < fstNum; fstI++)
        {
            // 先查变量名
            if (strcmp(fst[fstI].varName, varName) == 0) //查到
            {
                // 后取地址
                varAddress = fst[fstI].varAddress;

                // 打印值
                printf("变量%s地址为：%p\n", varName, varAddress);

                break; //查到跳出循环
            }
        }

        //未查到报错
        if (fstI == stNum)
        {
            printf("变量%s未声明，不允许取地址操作\n", varName);
        }
    }
}
// 变量计算
void varCalculation(char sentence[]) //a=a+1
{
    // 接收语句部分 最多三部分，最少两部分（使用临时变量）
    char part1[10] = {0}; //一部分五个字符
    char part2[10] = {0};
    char part3[10] = {0}; //开得小了导致没有结束符

    //存取变量
    int part1I = 0;
    int part2I = 0;
    int part3I = 0;

    int assignmentFlag = 0; //记录是否有赋值符 0无1有
    int partFlag = 1;       //存取标志 默认从第一部分开始存
    char operator;          //记录操作符

    // 扫描语句 判断是否有赋值符
    int sentenceLength = strlen(sentence); //语句长度 没有分号，给予分号判断结束
    sentence[sentenceLength] = ';';
    sentenceLength = strlen(sentence); //重新取长度

    int sentenceI = 0;
    for (sentenceI = 0; sentenceI < sentenceLength; sentenceI++)
    {
        if (sentence[sentenceI] == '=') //有赋值符
        {
            assignmentFlag = 1;

            break; //跳出循环不再扫描
        }
    }

    if (sentenceI == sentenceLength) //没有赋值符
    {
        partFlag = 2; //在part2和part3存两个操作数 有赋值符时才用part1
    }

    //扫描语句 取部分内容
    sentenceI = 0;
    for (sentenceI = 0; sentenceI < sentenceLength; sentenceI++)
    {
        //判断分隔符
        if (sentence[sentenceI] == '=')
        {
            partFlag = 2; //在有赋值符时置位
            sentenceI++;  //跳过赋值符
        }
        //判断操作符
        if (sentence[sentenceI] == '+' || sentence[sentenceI] == '-' || sentence[sentenceI] == '*' || sentence[sentenceI] == '/' || sentence[sentenceI] == '|' || sentence[sentenceI] == '^') //判断操作符
        {
            partFlag = 3;

            operator= sentence[sentenceI]; //记录操作符 先记录再跳

            sentenceI++; //跳过操作符
        }
        //判断结束符
        if (sentence[sentenceI] == ';')
        {
            break; //跳出循环
        }

        //存语句段
        if (partFlag == 1)
        {
            part1[part1I] = sentence[sentenceI];
            part1I++;
        }
        if (partFlag == 2)
        {
            part2[part2I] = sentence[sentenceI];
            part2I++;
        }
        if (partFlag == 3)
        {
            part3[part3I] = sentence[sentenceI];
            part3I++;
        }
    }

    //一.取计算值
    char *leftValue = NULL;
    char *rightValue = NULL;

    leftValue = getValue(part2);
    rightValue = getValue(part3);

    if (*leftValue == '\0' || *rightValue == '\0')
    {
        return; //未取到变量值，不可操作直接返回
    }

    //二.判断操作类型
    int operationType = 0; //记录运算类型 int为0，默认,string为1

    int intLeftOperand = 0;  //左值
    int intRightOperand = 0; //右值

    //判断使用的是字符数字、字符/字符串
    if (isdigit(leftValue[0])) //左值是字符数字
    {
        intLeftOperand = atoi(leftValue); //将字符数字转换为数字

        operationType = 0; //int类型运算
    }
    else //是字符/字符串
    {
        operationType = 1; //string类型运算
    }

    if (isdigit(rightValue[0])) //右值是字符数字
    {
        intRightOperand = atoi(rightValue); //将字符数字转换为数字

        operationType = 0; //int类型运算
    }
    else //是字符/字符串
    {
        operationType = 1; //string类型运算
    }

    // 三.根据操作类型进行相应判断操作
    int intOperationResult = 0;           //int类型操作结果
    char stringOperationResult[10] = {0}; //string类型操作结果

    if (operationType == 0) //int类型
    {
        //1.计算
        if (operator== '+') //取判断符判断
        {
            intOperationResult = intLeftOperand + intRightOperand;
        }
        else if (operator== '-')
        {
            intOperationResult = intLeftOperand - intRightOperand;
        }
        else if (operator== '*')
        {
            intOperationResult = intLeftOperand * intRightOperand;
        }
        else // '/'除号
        {
            intOperationResult = intLeftOperand / intRightOperand;
        }

        // 将int型数字变量转换为字符数组存取
        char result[4] = {0};
        itoa(intOperationResult, result, 10); //第三个参数是转移数字时所用的基数,十进制

        //2.存储
        calculationStorage("int", part1, result);
    }
    else //string类型
    {
        // 取最大操作数字符串长度
        int leftOperandLength = strlen(leftValue); //操作数长度
        int rightOperandLength = strlen(rightValue);
        int operandLength = 0; //判断长度，取大循环

        if (leftOperandLength >= rightOperandLength) //第一个字符串长
        {
            operandLength = leftOperandLength;
        }
        else //第二个字符串长
        {
            operandLength = rightOperandLength;
        }

        //1.计算
        if (operator== '+') //拼接
        {
            strcat(stringOperationResult, leftValue);
            strcat(stringOperationResult, rightValue);
        }
        else if (operator== '|') //按位取大
        {
            for (int compareI = 0; compareI < operandLength; compareI++)
            {
                if (leftValue[compareI] >= rightValue[compareI]) //取第一个操作数字符
                {
                    stringOperationResult[compareI] = leftValue[compareI];
                }
                else //取第二个操作数字符
                {
                    stringOperationResult[compareI] = rightValue[compareI];
                }
            }
        }
        else //按位取小
        {
            for (int compareI = 0; compareI < operandLength; compareI++)
            {
                if (leftValue[compareI] <= rightValue[compareI]) //取第一个操作数字符
                {
                    stringOperationResult[compareI] = leftValue[compareI];
                }
                else //取第二个操作数字符
                {
                    stringOperationResult[compareI] = rightValue[compareI];
                }
            }
        }

        //2.存储
        calculationStorage("string", part1, stringOperationResult);
    }
}
//字符串取长度
void getStringSize(char sentence[]) //size(ab);
{
    //取括号中内容
    char content[10] = {0}; //括号中内容

    int bracketsFlag = 0; //左右括号标志

    // 扫描语句
    int sentenceLength = strlen(sentence); //语句长度，有分号
    for (int sentenceI = 0; sentenceI < sentenceLength; sentenceI++)
    {
        if (sentence[sentenceI] == '(')
        {
            bracketsFlag = 1; //读到左括号

            sentenceI++; //跳过左括号
        }
        if (sentence[sentenceI] == ')')
        {
            break; //读到右括号 不再查
        }

        // 取变量名
        if (bracketsFlag == 1) //左括号后右括号前为变量名
        {
            content[sentenceI - 5] = sentence[sentenceI]; //"size("总占据五个字符
        }
    }

    //取值
    char *value = NULL; //值 数字，字符/字符串，变量
    value = getValue(content);

    //字符指针取长度
    int stringSize = 0;
    while (*value)
    {
        value++; //漏
        stringSize++;
    }

    printf("%s的字符串长度为：%d\n", content, stringSize); //漏
}
//if语句
void ifStatement(char sentence[]) //if(a==b) a=s s else b=d d endif;
{
    // 分隔语句部分
    char judgement[20] = {0};  //判断语句
    char statement1[10] = {0}; //操作语句1
    char statement2[10] = {0}; //操作语句2

    int partFlag = 1; //记录各部分读取标志
    int part1I = 0;   //记录各部分存取下标变量
    int part2I = 0;
    int part3I = 0;

    int bracketsFlag = 0; //左右括号标志

    //一.取语句部分
    // 扫描语句
    int sentenceLength = strlen(sentence); //语句长度
    for (int sentenceI = 0; sentenceI < sentenceLength; sentenceI++)
    {
        if (partFlag == 1) //读第一部分
        {
            if (sentence[sentenceI] == '(')
            {
                bracketsFlag = 1; //读到左括号

                sentenceI++; //跳过左括号
            }
            if (sentence[sentenceI] == ')')
            {
                bracketsFlag = 0; //读到右括号 不再查

                partFlag = 2;

                sentenceI = sentenceI + 1; //跳过右括号，++后在下一个循环跳过空格
            }

            // 取判断语句
            if (bracketsFlag == 1) //左括号后右括号前为变量名
            {
                judgement[part1I] = sentence[sentenceI]; //"if("总占据三个字符
                part1I++;
            }
        }
        else if (partFlag == 2) //读第二部分 操作语句1
        {
            statement1[part2I] = sentence[sentenceI];
            part2I++;

            if (sentence[sentenceI + 1] == ' ' && sentence[sentenceI + 2] == 'e' && sentence[sentenceI + 3] == 'l' && sentence[sentenceI + 4] == 's' && sentence[sentenceI + 5] == 'e' && sentence[sentenceI + 6] == ' ') //读第二部分结束
            {
                partFlag = 3;

                sentenceI = sentenceI + 6; //跳过空格，else，++后在下一个循环跳过空格
            }
        }
        else //partFlag == 3 读第三部分 操作语句2
        {
            statement2[part3I] = sentence[sentenceI];
            part3I++;

            if (sentence[sentenceI + 1] == ' ' && sentence[sentenceI + 2] == 'e' && sentence[sentenceI + 3] == 'n' && sentence[sentenceI + 4] == 'd' && sentence[sentenceI + 5] == 'i' && sentence[sentenceI + 6] == 'f' && sentence[sentenceI + 7] == ';') //读第三部分结束
            {
                break; //读取结束
            }
        }
    }

    int operationResult = -1; //记录操作结果 正确为1错误为0

    operationResult = conditionalJudgment(judgement); //传判断语句

    //五.执行操作语句
    if (operationResult == 1) //执行操作语句1
    {
        statementProcessing(statement1); //转语句处理函数
    }
    else //执行操作语句2
    {
        statementProcessing(statement2); //转语句处理函数
    }
}
// 打印符号表、函数符号表、内存
void list(char part2[])
{
    if (strcmp(part2, "st") == 0) //符号表
    {
        // 打印符号表
        printf("【符号表】\n");
        printf("变量名，变量地址，变量类型\n");
        for (int stI = 0; stI < stNum; stI++)
        {
            printf("%s,%p,%s\n", st[stI].varName, st[stI].varAddress, st[stI].varType);
        }
    }
    else if (strcmp(part2, "fst") == 0) //函数符号表
    {
        // 打印函数符号表
        printf("【函数符号表】\n");
        printf("变量名，变量地址，变量类型\n");
        for (int fstI = 0; fstI < fstNum; fstI++)
        {
            printf("%s,%p,%s\n", fst[fstI].varName, fst[fstI].varAddress, fst[fstI].varType);
        }
    }
    else if (strcmp(part2, "mem") == 0) //内存
    {
        // 打印栈区
        printf("【栈区表】\n");
        printf("变量地址，变量值\n");
        for (int saI = 0; saI < saNum; saI++)
        {
            printf("%p,%s\n", sa[saI].varAddress, sa[saI].varValue);
        }

        printf("\n");

        char *stackAreaListPointer = stackArea; //遍历栈区指针

        printf("【栈区】\n");
        printf("变量地址，变量值\n");
        for (stackAreaListPointer; stackAreaListPointer < stackAreaPointer; stackAreaListPointer++) //打印已存值的栈区
        {
            printf("%p,%s\n", stackAreaListPointer, stackAreaListPointer); //使用%s显示值而不是%c
        }

        printf("\n");

        // 打印全局区
        printf("【全局区表】\n");
        printf("变量地址，变量值\n");
        for (int gaI = 0; gaI < gaNum; gaI++)
        {
            printf("%p,%s\n", ga[gaI].varAddress, ga[gaI].varValue);
        }

        printf("\n");

        char *globalAreaListPointer = globalArea; //遍历全局区指针

        printf("【全局区】\n");
        printf("变量地址，变量值\n");
        for (globalAreaListPointer; globalAreaListPointer < globalAreaPointer; globalAreaListPointer++) //打印已存值的全局区
        {
            printf("%p,%s\n", globalAreaListPointer, globalAreaListPointer); //使用%s显示值而不是%c
        }

        printf("\n");

        char lineI = 0; //行指针计数

        printf("【代码区】\n");
        printf("代码地址，代码值\n");
        for (lineI = 0; lineI < codeAreaStatementNum; lineI++) //打印已存值的代码区
        {
            printf("%p,%s\n", codeAreaPointer[lineI], codeAreaPointer[lineI]); //使用%s显示值而不是%c
        }
    }
    else //全打印 all
    {
        // 打印符号表
        printf("【符号表】\n");
        printf("变量名，变量地址，变量类型\n");
        for (int stI = 0; stI < stNum; stI++)
        {
            printf("%s,%p,%s\n", st[stI].varName, st[stI].varAddress, st[stI].varType);
        }

        printf("\n");

        // 打印函数符号表
        printf("【函数符号表】\n");
        printf("变量名，变量地址，变量类型\n");
        for (int fstI = 0; fstI < fstNum; fstI++)
        {
            printf("%s,%p,%s\n", fst[fstI].varName, fst[fstI].varAddress, fst[fstI].varType);
        }

        printf("\n");

        // 打印栈区
        printf("【栈区表】\n");
        printf("变量地址，变量值\n");
        for (int saI = 0; saI < saNum; saI++)
        {
            printf("%p,%s\n", sa[saI].varAddress, sa[saI].varValue);
        }

        printf("\n");

        char *stackAreaListPointer = stackArea; //遍历栈区指针

        printf("【栈区】\n");
        printf("变量地址，变量值\n");
        for (stackAreaListPointer; stackAreaListPointer < stackAreaPointer; stackAreaListPointer++) //打印已存值的栈区
        {
            printf("%p,%s\n", stackAreaListPointer, stackAreaListPointer); //使用%s显示值而不是%c
        }

        printf("\n");

        // 打印全局区
        printf("【全局区表】\n");
        printf("变量地址，变量值\n");
        for (int gaI = 0; gaI < gaNum; gaI++)
        {
            printf("%p,%s\n", ga[gaI].varAddress, ga[gaI].varValue);
        }

        printf("\n");

        char *globalAreaListPointer = globalArea; //遍历全局区指针

        printf("【全局区】\n");
        printf("变量地址，变量值\n");
        for (globalAreaListPointer; globalAreaListPointer < globalAreaPointer; globalAreaListPointer++) //打印已存值的全局区
        {
            printf("%p,%s\n", globalAreaListPointer, globalAreaListPointer); //使用%s显示值而不是%c
        }

        printf("\n");

        char lineI = 0; //行指针计数

        printf("【代码区】\n");
        printf("代码地址，代码值\n");
        for (lineI = 0; lineI < codeAreaStatementNum; lineI++) //打印已存值的代码区
        {
            printf("%p,%s\n", codeAreaPointer[lineI], codeAreaPointer[lineI]); //使用%s显示值而不是%c
        }
    }
}
// while语句
void whileStatement(char sentence[]) //while(a<5) a=a+2; a=a-1; endwhile;
{
    int statementTypeFlag = 0; //语句类型标志 0为声明，1为执行，2为结束

    int sentenceLength = 0; //语句长度

    //取判断语句
    char judgement[20] = {0}; //判断语句

    if (statementTypeFlag == 0) //while声明
    {
        statementTypeFlag = 1; //语句类型标志,等待接收执行语句

        // 取判断语句
        int judgementI = 0; //记录判断语句存取下标变量

        int bracketsFlag = 0; //左右括号标志

        // 扫描语句
        sentenceLength = strlen(sentence); //语句长度
        for (int sentenceI = 0; sentenceI < sentenceLength; sentenceI++)
        {

            if (sentence[sentenceI] == '(')
            {
                bracketsFlag = 1; //读到左括号

                sentenceI++; //跳过左括号
            }
            if (sentence[sentenceI] == ')')
            {
                bracketsFlag = 0; //读到右括号 不再取
            }

            // 取判断语句
            if (bracketsFlag == 1) //左括号后右括号前为变量名
            {
                judgement[judgementI] = sentence[sentenceI];
                judgementI++;
            }
        }
    }

    //取执行语句
    char sentence1[10] = {0}; //在while语句函数中继续接收需要的语句

    char statement[10][10] = {0}; //执行语句 存储执行语句，最多十条

    int statementNum = 0; //记录执行语句个数

    if (statementTypeFlag == 1) //while操作
    {
        for (int statementI = 0; statementI < 10; statementI++) //最多接收十条语句
        {
            gets(sentence1); //继续接收执行语句

            //如果又是while语句
            char part1[10] = {0}; //存取内容

            for (int readI = 0; readI < 5; readI++) //读语句while
            {
                part1[readI] = sentence1[readI];
            }

            // 判断part1
            if (strcmp(part1, "while") == 0)
            {
                statementProcessing(sentence1); // 转嵌套while

                statementI--; //循环变量减少一
                continue;     //函数返回不让while存储，接收下一条
            }

            if (strcmp(sentence1, "endwhile;") == 0)
            {
                statementTypeFlag = 2; //已接收到while结束语句
                break;                 //退出接收
            }

            strcpy(statement[statementI], sentence1); //二维字符数组的使用
            statementNum++;                           //记录执行语句个数 始终多一个
        }
    }

    for (int statementI = 0; statementI < statementNum; statementI++) //最多接收十条语句
    {
        printf("执行语句：%s\n", statement[statementI]);
    }

    int judgementResult = -1;
    judgementResult = conditionalJudgment(judgement); //取判断结果

    //执行操作
    while (judgementResult == 1) //真实/底层实现
    {
        for (int statementI = 0; statementI < statementNum; statementI++) //最多执行十条语句
        {
            if (areaTypeFlag == 0)
            {
                statementProcessing(statement[statementI]); //转语句处理函数
            }
            else
            {
                // functionStatementProcessing(statement[statementI]); //转函数语句处理函数
            }
        }

        judgementResult = conditionalJudgment(judgement); //再取判断结果
    }
}

//有关函数
//函数定义
void functionDefinition(char name[]) //func
{
    // 取函数名
    printf("变量名：%s\n", name);

    //接收返回值\参数定义语句的部分
    char *part1 = NULL; //使用字符类型指针，不可使用字符数组名
    char *part2 = NULL;
    char *part3 = NULL;

    char returnType[10] = {0};    //记录返回值类型 给够数组长度
    char argument1Type[10] = {0}; //记录参数1类型
    char argument2Type[10] = {0}; //记录参数2类型
    char argument1Name[10] = {0}; //记录参数1名
    char argument2Name[10] = {0}; //记录参数2名

    int statementFlag = 0; //判别，三个参数语句赋值不同

    //一.接收三条参数声明语句
    char sentence[50] = {0}; //继续接收语句
    for (int defineI = 0; defineI < 3; defineI++)
    {
        gets(sentence); //接收语句

        //转擦除函数定义函数
        if (strcmp(sentence, "clear;") == 0)
        {
            functionClear(name); //传入需擦除的函数名

            return; //返回上级语句处理函数
        }

        int sentenceLength = strlen(sentence); //语句长度

        //接收返回值声明
        if (statementFlag == 0) //returnType->reT
        {
            for (int readI = 3 + 1; readI < sentenceLength; readI++) //+1跳过空格
            {
                returnType[readI - 4] = sentence[readI]; //取返回值类型
            }

            statementFlag = 1; //置位，下一次读参数声明

            continue; //跳到下个循环
        }

        int argumentTypeToNameFlag = 0; //判断读取参数声明的类型、名字
        int argumentNameReadI = 0;      //记录语句第三个参数需要的下标

        //第一个参数声明
        if (statementFlag == 1) //argumentType->arT
        {
            for (int readI = 3 + 1; readI < sentenceLength; readI++) //跳过空格
            {
                if (sentence[readI] == ' ') //读到空格分隔 漏等等号
                {
                    argumentTypeToNameFlag = 1; //置位，下一次读参数名

                    readI++; //跳过空格

                    argumentNameReadI = readI; //记录语句第三个参数需要的下标
                }

                if (argumentTypeToNameFlag == 0) //取参数类型
                {
                    argument1Type[readI - 4] = sentence[readI];
                }
                else //取参数名字
                {
                    argument1Name[readI - argumentNameReadI] = sentence[readI];
                }
            }

            statementFlag = 2; //置位，下一次读第二个参数声明 多了个等号
            continue;          //跳到下个循环
        }

        //第二个参数声明
        if (statementFlag == 2)
        {
            for (int readI = 3 + 1; readI < sentenceLength; readI++)
            {
                if (sentence[readI] == ' ') //读到空格分隔 漏等等号
                {
                    argumentTypeToNameFlag = 1; //置位，下一次读参数名

                    readI++; //跳过空格

                    argumentNameReadI = readI; //记录语句第三个参数需要的下标
                }

                if (argumentTypeToNameFlag == 0) //取参数类型
                {
                    argument2Type[readI - 4] = sentence[readI];
                }
                else //取参数名字
                {
                    argument2Name[readI - argumentNameReadI] = sentence[readI];
                }
            }
        }
    }

    printf("函数返回值类型：%s\n", returnType);
    printf("函数参数1类型：%s\n", argument1Type);
    printf("函数参数1名字：%s\n", argument1Name);
    printf("函数参数2类型：%s\n", argument2Type);
    printf("函数参数2名字：%s\n", argument2Name);

    // 取函数类型
    char functionType[30] = {0}; //函数类型 给够数组大小

    strcat(functionType, argument1Type);
    strcat(functionType, ","); //字符串拼接使用双引号
    strcat(functionType, argument2Type);
    strcat(functionType, ",");
    strcat(functionType, returnType);

    printf("函数类型：%s\n", functionType);

    //记录函数符号表项
    strcpy(fst[fstNum].varName, name);
    fst[fstNum].varAddress = stackAreaPointer; //函数存在栈区函数激活/栈帧中
    strcpy(fst[fstNum].varType, functionType);

    fstNum++; //函数符号表项增加

    // 栈区地址指针增加
    stackAreaFunctionHead = stackAreaPointer;               //记录函数栈帧首地址
    stackAreaPointer = stackAreaPointer + functionByte + 1; //一个十六进制地址代表一个字节 最多分配二十个字节，二十个字符 预分配一些空间构成栈帧 指向第二十一个字节
    stackAreaFunctionTail = stackAreaPointer;               //记录函数栈帧尾地址

    //二.接收函数体
    gets(sentence); //接收语句

    while (strcmp(sentence, "enddef;") != 0)
    {
        //转擦除函数定义函数
        if (strcmp(sentence, "clear;") == 0)
        {
            functionClear(name); //传入需擦除的函数名

            return; //返回上级语句处理函数
        }

        strcpy(codeAreaPointer[codeAreaStatementNum], sentence); //将语句存入代码区
        codeAreaStatementNum++;

        gets(sentence); //接收语句
    }

    for (int i = 0; i < codeAreaStatementNum; i++)
    {
        printf("这是代码区：%s\n", codeAreaPointer[i]);
    }
}
//函数调用
void functionCall(char sentence[], char type[]) //func(a,b); int,int,int
{
    //一.取类型
    char functionType[6] = {0}; //存储函数类型 最多为六位string

    int typeLength = strlen(type); //类型长度
    //扫描类型
    for (int typeI = 0; typeI < typeLength; typeI++)
    {
        if (type[typeI] == ',')
        {
            break; //使用第一个参数的类型表示整个函数的操作类型
        }

        functionType[typeI] = type[typeI];
    }

    printf("操作类型：%s\n", functionType);

    //二.取参数
    char argument1[10] = {0}; //存储参数
    char argument2[10] = {0};

    int sentenceLength = strlen(sentence); //语句长度

    int symbolFlag = 0; //记录读取括号、逗号

    int readI = 0; //记录读到左括号的下标

    //扫描语句
    for (int sentenceI = 0; sentenceI < sentenceLength; sentenceI++)
    {
        if (sentence[sentenceI] == '(') //读到左括号
        {
            symbolFlag = 1; //标志置位

            sentenceI++; //跳过左括号

            readI = sentenceI; //记录读到左括号后的下标
        }
        if (sentence[sentenceI] == ',') //读到逗号
        {
            symbolFlag = 2; //标志置位

            sentenceI++; //跳过逗号

            readI = sentenceI; //记录读到逗号后的下标
        }
        if (sentence[sentenceI] == ')') //读到右括号
        {
            symbolFlag = 3; //标志置位
            break;          //结束读取
        }

        if (symbolFlag == 1)
        {
            argument1[sentenceI - readI] = sentence[sentenceI]; //取参数1
        }
        if (symbolFlag == 2)
        {
            argument2[sentenceI - readI] = sentence[sentenceI]; //取参数2
        }
    }

    printf("参数：%s %s\n", argument1, argument2);

    // 三.取参数值
    char *argument1Value = NULL; //存参数值
    char *argument2Value = NULL;

    argument1Value = getValue(argument1);
    argument2Value = getValue(argument2);

    printf("参数值：%s %s\n", argument1Value, argument2Value);

    //四.将参数地址压栈,后从参数地址取值将值压栈 顺序从右向左→简化操作，直接将参数值压栈
    stackAreaAssignment = stackAreaFunctionHead; //记录在一个函数栈帧中压栈后的各个地址

    //判断操作类型压哪个值
    if (strcmp(functionType, "int") == 0) //int类型
    {
        //压栈
        //参数2
        for (int assignI = 0; assignI < intByte; assignI++)
        {
            stackAreaAssignment[assignI] = argument2Value[assignI];
        }

        // 记录栈区表项
        sa[saNum].varAddress = stackAreaAssignment; //地址
        strcpy(sa[saNum].varValue, argument2Value); //值
        saNum++;                                    // 栈区表项递增

        stackAreaAssignment = stackAreaAssignment + intByte; //记录压栈地址，接下来压参数1

        //参数1
        for (int assignI = 0; assignI < intByte; assignI++)
        {
            stackAreaAssignment[assignI] = argument1Value[assignI];
        }

        // 记录栈区表项
        sa[saNum].varAddress = stackAreaAssignment; //地址
        strcpy(sa[saNum].varValue, argument1Value); //值
        saNum++;                                    // 栈区表项递增

        stackAreaAssignment = stackAreaAssignment + intByte; //记录压栈地址，接下来压返回值
    }

    //五.取代码段代码执行
    for (int readI = 0; readI < codeAreaStatementNum; readI++) //取已存值的代码区
    {
        areaTypeFlag = 1; //修改操作的区域类型

        functionStatementProcessing(argument1, argument2, codeAreaPointer[readI]); ////转函数语句处理函数 传参数，执行语句
    }
}
//函数语句处理
void functionStatementProcessing(char argument1[], char argument2[], char sentence[])
{
    //分隔语句部分
    char part1[10] = {0};
    char part2[10] = {0};

    //判断存取部分变量
    int part1I = 0;
    int part2I = 0;

    int partFlag = 1; //存取标志

    // 扫描语句 分隔
    int sentenceLength = strlen(sentence); //语句长度
    for (int sentenceI = 0; sentenceI < sentenceLength; sentenceI++)
    {
        if (sentence[sentenceI] == ' ' || sentence[sentenceI] == '(') //空格：声明 左括号：字符串取长度
        {
            partFlag = 2; //置位，接收第二部分

            sentenceI++; //跳过空格、赋值符、左括号
        }
        if (sentence[sentenceI] == ';') //语句结束符
        {
            break; //直接出循环
        }

        //存取语句
        if (partFlag == 1)
        {
            part1[part1I] = sentence[sentenceI];
            part1I++;
        }
        if (partFlag == 2)
        {
            part2[part2I] = sentence[sentenceI];
            part2I++;
        }
    }

    //二.扫描语句第一部分
    //转变量声明函数
    if (strcmp(part1, "int") == 0 || strcmp(part1, "string") == 0)
    {
        varDeclaration(part1, part2);
    }

    //转字符串取长度函数
    if (strcmp(part1, "size") == 0)
    {
        getStringSize(sentence); //size(a);
    }

    //转if语句函数
    if (strcmp(part1, "if") == 0)
    {
        ifStatement(sentence); //if(a==b) a=s s else b=d d endif;
    }

    //转打印符号表、函数符号表、内存函数
    if (strcmp(part1, "list") == 0)
    {
        list(part2); //all
    }

    int assignmentFlag = 0;  //记录变量赋值标志
    int getValueFlag = 0;    //判断是否有取值符
    int getAddressFlag = 0;  //判断是否有取地址符
    int calculationFlag = 0; //判断是否有计算符

    //扫描语句 判断赋值
    for (int sentenceI = 0; sentenceI < sentenceLength; sentenceI++)
    {
        if (sentence[sentenceI] == '=') //赋值号
        {
            assignmentFlag = 1; //有赋值号
        }
        if (part1[sentenceI] == '?') //取值号
        {
            getValueFlag = 1;
        }
        if (part1[sentenceI] == '&') //取址符
        {
            getAddressFlag = 1;
        }
        if (part1[sentenceI] == '+' || part1[sentenceI] == '-' || part1[sentenceI] == '*' || part1[sentenceI] == '/' || part1[sentenceI] == '|' || part1[sentenceI] == '^')
        {
            calculationFlag = 1;
        }
    }

    //转变量赋值函数
    if (assignmentFlag == 1)
    {
        varAssignment(sentence); //接收整条语句
    }

    // 转变量取值函数
    if (getValueFlag == 1)
    {
        varGetValue(part1); //?a
    }

    // 转变量取址函数
    if (getAddressFlag == 1)
    {
        varGetAddress(part1); //&a
    }

    //转变量计算函数
    if (calculationFlag == 1) //无赋值符判断
    {
        varCalculation(part1); //a=a+b
    }

    //转函数返回函数
    if (strcmp(part1, "return") == 0)
    {
        functionReturn(part2); //字符数组
    }
}
//函数返回函数
void functionReturn(char sentence[]) //c
{
    //取值
    char *value = NULL;
    value = getValue(sentence);

    printf("返回值：%s\n", value);
}
//函数擦除
void functionClear(char functionName[]) //需擦除的函数名
{
    char *clearAddress = NULL; //记录需擦除的栈帧地址

    //擦除函数符号表项
    for (int fstI = 0; fstI < fstNum; fstI++)
    {
        if (strcmp(fst[fstI].varName, functionName) == 0) //查到
        {
            memset(fst[fstI].varName, 0, sizeof(fst[fstI].varName)); //对已定义的数组赋值 函数名

            clearAddress = fst[fstI].varAddress; //记录需擦除的栈帧地址

            fst[fstI].varAddress = NULL;                             //函数地址
            memset(fst[fstI].varType, 0, sizeof(fst[fstI].varType)); //函数类型
        }
    }
    fstNum--; //不考虑到局部变量也压到函数符号表中，不清除表项，继续往下写

    //回收栈区项 主调函数的栈帧 在未调用时还未压栈 清栈区表项
    //判断栈中是否有其他元素
    if (stackAreaFunctionTail == stackAreaPointer) //函数栈帧尾即当前栈区指针指向，函数栈帧为栈顶元素，出栈
    {
        stackAreaPointer = stackAreaPointer - functionByte - 1; //分配时多给了一
    }

    //回收代码区
    for (int i = 0; i < codeAreaStatementNum; i++)
    {
        memset(codeAreaPointer[i], 0, sizeof(codeAreaPointer[i])); //清列
    }
    codeAreaStatementNum = 0;

    areaTypeFlag = 0; //操作区域返回
}

//辅助函数
//取值 数字、字符串和变量
char *getValue(char argument[])
{
    char *value = (char *)malloc(sizeof(char) * 10); //存所取值 返回字符指针 应使用动态数组

    if (isdigit(argument[0])) //字符数字
    {
        return argument; //直接返回
    }
    else if (argument[0] == '"') //是字符/字符串
    {
        //取字符/字符串
        for (int assignI = 1; assignI < strlen(argument) - 1; assignI++) //去首尾两个引号
        {
            value[assignI - 1] = argument[assignI]; //数组从0开始
        }

        return value; //返回值
    }
    else //是变量名
    {
        if (areaTypeFlag == 0) //符号表
        {
            // 查符号表
            int stI = 0;
            for (stI = 0; stI < stNum; stI++)
            {
                // 先查变量名
                if (strcmp(st[stI].varName, argument) == 0) //查到
                {
                    //再判断变量类型
                    if (strcmp(st[stI].varType, "int") == 0) //int类型
                    {
                        // 后从地址取值
                        for (int readI = 0; readI < intByte; readI++)
                        {
                            value[readI] = st[stI].varAddress[readI]; //一字节一字符读内存，最多四字节  即字符数组循环取值
                        }
                    }
                    else //string类型
                    {
                        // 后从地址取值
                        for (int readI = 0; readI < stringByte; readI++)
                        {
                            value[readI] = st[stI].varAddress[readI]; //一字节一字符读内存，最多十字节  即字符数组循环取值
                        }
                    }

                    return value; //返回值
                }
            }

            //未查到报错
            if (stI == stNum)
            {
                printf("变量%s未声明，不允许取值操作\n", argument);

                //存结束符
                *value = '\0';
                return value; //返回动态申请的地址，但无值
            }
        }
        else //函数符号表
        {
            // 查函数符号表
            int fstI = 0;
            for (fstI = 0; fstI < fstNum; fstI++)
            {
                // 先查变量名
                if (strcmp(fst[fstI].varName, argument) == 0) //查到
                {
                    //再判断变量类型
                    if (strcmp(fst[fstI].varType, "int") == 0) //int类型
                    {
                        // 后从地址取值
                        for (int readI = 0; readI < intByte; readI++)
                        {
                            value[readI] = fst[fstI].varAddress[readI]; //一字节一字符读内存，最多四字节  即字符数组循环取值
                        }
                    }
                    else //string类型
                    {
                        // 后从地址取值
                        for (int readI = 0; readI < stringByte; readI++)
                        {
                            value[readI] = fst[fstI].varAddress[readI]; //一字节一字符读内存，最多十字节  即字符数组循环取值
                        }
                    }

                    return value; //返回值
                }
            }

            //未查到报错
            if (fstI == fstNum)
            {
                printf("变量%s未声明，不允许取值操作\n", argument);

                //存结束符
                *value = '\0';
                return value; //返回动态申请的地址，但无值
            }
        }
    }
}
//计算存储
void calculationStorage(char type[], char part1[], char result[]) //传计算类型，左值，结果
{
    //2.存储
    if (areaTypeFlag == 0) //符号表，全局区
    {
        if (strcmp(part1, "") == 0) //无左赋值变量 使用栈区临时变量
        {
            int tempFlag = 0; //记录是否有temp表项 0无1有默认无

            // 查符号表
            // 先查变量声明 判断是否已有temp表项
            int stI = 0;
            for (stI = 0; stI < stNum; stI++)
            {
                if (strcmp(st[stI].varName, "temp") == 0) //已有表项 覆盖表项
                {
                    tempFlag = 1; //置位

                    //1.清除原栈区值
                    for (int clearI = 0; clearI < stringByte; clearI++) //按十字节清理
                    {
                        st[stI].varAddress[clearI] = '\0';
                    }

                    //2.清楚栈区表项值
                    for (int saI = 0; saI < saNum; saI++)
                    {
                        if (sa[saI].varAddress = st[stI].varAddress) //地址同
                        {
                            memset(sa[saI].varValue, 0, sizeof(sa[saI].varValue)); //清理值 字符数组赋值

                            break; //跳出循环
                        }
                    }

                    if (strcmp(type, "int") == 0) //存储int类型
                    {
                        // 3.符号表项覆盖
                        st[stI].varAddress = stackAreaPointer; //地址
                        strcpy(st[stI].varType, "int");        //类型

                        // 1.取地址赋值 在栈区
                        for (int assignI = 0; assignI < intByte; assignI++)
                        {
                            st[stI].varAddress[assignI] = result[assignI]; //一字节一字符存内存，最多四字节  即字符数组循环赋值 下标是stI
                        }
                    }
                    else //string类型
                    {
                        // 3.符号表项覆盖
                        st[stI].varAddress = stackAreaPointer; //地址
                        strcpy(st[stI].varType, "string");     //类型

                        // 1.取地址赋值 在栈区
                        for (int assignI = 0; assignI < stringByte; assignI++)
                        {
                            st[stI].varAddress[assignI] = result[assignI]; //一字节一字符存内存，最多四字节  即字符数组循环赋值 下标是stI
                        }
                    }

                    // 3.记录栈区表项
                    sa[saNum].varAddress = st[stI].varAddress;
                    strcpy(sa[saNum].varValue, result);
                    saNum++; // 栈区表项递增
                }

                break; //操作完跳出查符号表循环
            }

            if (tempFlag == 0) //无temp表项
            {
                if (strcmp(type, "int") == 0) //存储int类型
                {
                    // 符号表项记录
                    strcpy(st[stNum].varName, "temp");
                    st[stNum].varAddress = stackAreaPointer;
                    strcpy(st[stNum].varType, "int");

                    // 取地址赋值 在栈区
                    for (int assignI = 0; assignI < intByte; assignI++)
                    {
                        st[stNum].varAddress[assignI] = result[assignI]; //一字节一字符存内存，最多四字节  即字符数组循环赋值 下标是stNum
                    }
                }
                else //string类型
                {
                    // 符号表项记录
                    strcpy(st[stNum].varName, "temp");
                    st[stNum].varAddress = stackAreaPointer;
                    strcpy(st[stNum].varType, "string");

                    // 取地址赋值 在栈区
                    for (int assignI = 0; assignI < stringByte; assignI++)
                    {
                        st[stNum].varAddress[assignI] = result[assignI]; //一字节一字符存内存，最多四字节  即字符数组循环赋值 下标是stNum
                    }
                }

                // 记录栈区表项
                sa[saNum].varAddress = st[stNum].varAddress;
                strcpy(sa[saNum].varValue, result);
                saNum++; // 栈区表项递增

                stNum++; //符号表项序号递增 放在赋值后 放在记录栈区表项后
            }

            // 栈区地址指针增加 考虑到其他元素压栈，不清理栈空间，默认往下下
            stackAreaPointer = stackAreaPointer + stringByte; //默认给temp变量都分配十字节 因为有覆盖因素
        }
        else //有左赋值变量
        {
            //查符号表
            //判断是否已声明
            int stI = 0;
            for (stI = 0; stI < stNum; stI++)
            {
                if (strcmp(st[stI].varName, part1) == 0) //已经声明
                {
                    if (strcmp(type, "int") == 0) //存储int类型
                    {
                        // 后取地址赋值 在全局区
                        for (int assignI = 0; assignI < intByte; assignI++)
                        {
                            st[stI].varAddress[assignI] = result[assignI]; //一字节一字符存内存，最多四字节  即字符数组循环赋值
                        }
                    }
                    else //string类型
                    {
                        // 后取地址赋值 在全局区
                        for (int assignI = 0; assignI < stringByte; assignI++)
                        {
                            st[stI].varAddress[assignI] = result[assignI]; //一字节一字符存内存，最多四字节  即字符数组循环赋值
                        }
                    }

                    // 全局区表项覆盖
                    for (int gaI = 0; gaI < gaNum; gaI++)
                    {
                        if (ga[gaI].varAddress == st[stI].varAddress) //地址同
                        {
                            strcpy(ga[gaI].varValue, result); //覆盖值
                        }
                    }

                    break; //查到跳出循环
                }
            }

            //未查到报错
            if (stI == stNum)
            {
                printf("变量%s未声明，不允许赋值操作\n", part1);
            }
        }
    }
    else //函数符号表，栈区
    {
        if (strcmp(part1, "") == 0) //无左赋值变量 使用栈区临时变量
        {
            int tempFlag = 0; //记录是否有temp表项 0无1有默认无

            // 查函数符号表
            // 先查变量声明 判断是否已有temp表项
            int fstI = 0;
            for (fstI = 0; fstI < fstNum; fstI++)
            {
                if (strcmp(fst[fstI].varName, "temp") == 0) //已有表项 覆盖表项
                {
                    tempFlag = 1; //置位

                    //1.清除原栈区值
                    for (int clearI = 0; clearI < stringByte; clearI++) //按十字节清理
                    {
                        fst[fstI].varAddress[clearI] = '\0';
                    }

                    //2.清楚栈区表项值
                    for (int saI = 0; saI < saNum; saI++)
                    {
                        if (sa[saI].varAddress = fst[fstI].varAddress) //地址同
                        {
                            memset(sa[saI].varValue, 0, sizeof(sa[saI].varValue)); //清理值 字符数组赋值

                            break; //跳出循环
                        }
                    }

                    if (strcmp(type, "int") == 0) //存储int类型
                    {
                        // 3.符号表项覆盖
                        fst[fstI].varAddress = stackAreaPointer; //地址
                        strcpy(fst[fstI].varType, "int");        //类型

                        // 1.取地址赋值 在栈区
                        for (int assignI = 0; assignI < intByte; assignI++)
                        {
                            fst[fstI].varAddress[assignI] = result[assignI]; //一字节一字符存内存，最多四字节  即字符数组循环赋值 下标是stI
                        }
                    }
                    else //string类型
                    {
                        // 3.符号表项覆盖
                        fst[fstI].varAddress = stackAreaPointer; //地址
                        strcpy(fst[fstI].varType, "string");     //类型

                        // 1.取地址赋值 在栈区
                        for (int assignI = 0; assignI < stringByte; assignI++)
                        {
                            fst[fstI].varAddress[assignI] = result[assignI]; //一字节一字符存内存，最多四字节  即字符数组循环赋值 下标是stI
                        }
                    }

                    // 3.记录栈区表项
                    sa[saNum].varAddress = fst[fstI].varAddress;
                    strcpy(sa[saNum].varValue, result);
                    saNum++; // 栈区表项递增
                }

                break; //操作完跳出查符号表循环
            }

            if (tempFlag == 0) //无temp表项
            {
                if (strcmp(type, "int") == 0) //存储int类型
                {
                    // 符号表项记录
                    strcpy(fst[fstNum].varName, "temp");
                    fst[fstNum].varAddress = stackAreaPointer;
                    strcpy(fst[fstNum].varType, "int");

                    // 取地址赋值 在栈区
                    for (int assignI = 0; assignI < intByte; assignI++)
                    {
                        fst[fstNum].varAddress[assignI] = result[assignI]; //一字节一字符存内存，最多四字节  即字符数组循环赋值 下标是stNum
                    }
                }
                else //string类型
                {
                    // 符号表项记录
                    strcpy(fst[fstNum].varName, "temp");
                    fst[fstNum].varAddress = stackAreaPointer;
                    strcpy(fst[fstNum].varType, "string");

                    // 取地址赋值 在栈区
                    for (int assignI = 0; assignI < stringByte; assignI++)
                    {
                        fst[fstNum].varAddress[assignI] = result[assignI]; //一字节一字符存内存，最多四字节  即字符数组循环赋值 下标是stNum
                    }
                }

                // 记录栈区表项
                sa[saNum].varAddress = fst[fstNum].varAddress;
                strcpy(sa[saNum].varValue, result);
                saNum++; // 栈区表项递增

                fstNum++; //符号表项序号递增 放在赋值后 放在记录栈区表项后
            }

            // 栈区地址指针增加 考虑到其他元素压栈，不清理栈空间，默认往下下
            stackAreaPointer = stackAreaPointer + stringByte; //默认给temp变量都分配十字节 因为有覆盖因素
        }
        else //有左赋值变量
        {
            //查函数符号表
            //判断是否已声明
            int fstI = 0;
            for (fstI = 0; fstI < fstNum; fstI++)
            {
                if (strcmp(fst[fstI].varName, part1) == 0) //已经声明
                {
                    if (strcmp(type, "int") == 0) //存储int类型
                    {
                        // 后取地址赋值 在全局区
                        for (int assignI = 0; assignI < intByte; assignI++)
                        {
                            fst[fstI].varAddress[assignI] = result[assignI]; //一字节一字符存内存，最多四字节  即字符数组循环赋值
                        }
                    }
                    else //string类型
                    {
                        // 后取地址赋值 在全局区
                        for (int assignI = 0; assignI < stringByte; assignI++)
                        {
                            fst[fstI].varAddress[assignI] = result[assignI]; //一字节一字符存内存，最多四字节  即字符数组循环赋值
                        }
                    }

                    // 栈区表项记录
                    sa[saNum].varAddress = fst[fstI].varAddress;
                    strcpy(sa[saNum].varValue, result);
                    saNum++; //总是漏这漏那的
                }

                break; //查到跳出循环
            }

            //未查到报错
            if (fstI == fstNum)
            {
                printf("变量%s未声明，不允许赋值操作\n", part1);
            }
        }
    }
}
//条件判断
int conditionalJudgment(char judgement[]) //错误返回0，正确返回1
{
    // 二.判断判断语句 a==b
    char judgementLeftVar[10] = {0};  //左值
    char judgementRightVar[10] = {0}; //右值
    char operator[3] = {0};           //判断符 使用两个字符的数组，在判断符有两个时，未自动赋结束符，会输出operator后的地址数值，如输出judgementRighttValue地址数值

    char judgementFlag = 0; //判断符标志

    int judgementLeftVarI = 0; //存取下标
    int judgementRightVarI = 0;

    // 扫描判断语句 取操作数、操作符
    int judgementLength = strlen(judgement); //判断语句长度
    for (int judgementI = 0; judgementI < judgementLength; judgementI++)
    {
        //判断第一个判断符
        if (judgement[judgementI] == '>' || judgement[judgementI] == '=' || judgement[judgementI] == '<' || judgement[judgementI] == '!') //读到判断符
        {
            judgementFlag = 1; //标志置位

            operator[0] = judgement[judgementI]; //记录第一个判断符

            judgementI++; //跳过第一个判断符

            //判断第二个判断符 在知道第一个判断符的基础上
            if (judgement[judgementI] == '=')
            {
                operator[1] = judgement[judgementI]; //记录第二个判断符

                judgementI++; //跳过第二个判断符
            }
        }

        if (judgementFlag == 0) //取左值
        {
            judgementLeftVar[judgementLeftVarI] = judgement[judgementI];
            judgementLeftVarI++;
        }
        else //取右值
        {
            judgementRightVar[judgementRightVarI] = judgement[judgementI];
            judgementRightVarI++;
        }
    }

    //三.取判断语句中左右值
    char *judgementLeftValue = NULL;  //左值
    char *judgementRightValue = NULL; //右值

    judgementLeftValue = getValue(judgementLeftVar); //字符数组形式 数字，字符/字符串，变量
    judgementRightValue = getValue(judgementRightVar);

    int judgementIntLeftValue = 0;  //左值 存储int类型
    int judgementIntRightValue = 0; //右值
    int operationType = 1;          //记录运算类型 0为int,默认1为string 在判断数字后再变为0

    if (isdigit(judgementLeftValue[0])) //左值是字符数字
    {
        judgementIntLeftValue = atoi(judgementLeftValue); //将字符数字转换为数字 可以传指针

        operationType = 0; //int类型运算
    }
    if (isdigit(judgementRightValue[0])) //左值是字符数字
    {
        judgementIntRightValue = atoi(judgementRightValue); //将字符数字转换为数字

        operationType = 0; //int类型运算
    }

    int operationResult = -1;

    // 四.根据操作类型进行相应判断操作
    if (operationType == 0) //int类型
    {
        if (strcmp(operator, ">=") == 0) //取判断符判断
        {
            if (judgementIntLeftValue >= judgementIntRightValue) //真实/底层判断
            {
                operationResult = 1;
            }
            else
            {
                operationResult = 0;
            }
        }
        else if (strcmp(operator, "==") == 0) //取判断符判断
        {
            if (judgementIntLeftValue == judgementIntRightValue) //真实/底层判断
            {
                operationResult = 1;
            }
            else
            {
                operationResult = 0;
            }
        }
        else if (strcmp(operator, "<=") == 0) //取判断符判断
        {
            if (judgementIntLeftValue <= judgementIntRightValue) //真实/底层判断
            {
                operationResult = 1;
            }
            else
            {
                operationResult = 0;
            }
        }
        else if (strcmp(operator, "!=") == 0) //取判断符判断
        {
            if (judgementIntLeftValue != judgementIntRightValue) //真实/底层判断
            {
                operationResult = 1;
            }
            else
            {
                operationResult = 0;
            }
        }
        else if (strcmp(operator, ">") == 0) //取判断符判断
        {
            if (judgementIntLeftValue > judgementIntRightValue) //真实/底层判断
            {
                operationResult = 1;
            }
            else
            {
                operationResult = 0;
            }
        }
        else //<小于符
        {
            if (judgementIntLeftValue < judgementIntRightValue) //真实/底层判断
            {
                operationResult = 1;
            }
            else
            {
                operationResult = 0;
            }
        }
    }
    else //string类型
    {
        if (strcmp(operator, ">=") == 0) //取判断符判断
        {
            if (strcmp(judgementLeftValue, judgementRightValue) >= 0) //真实/底层判断 可以比较
            {
                operationResult = 1;
            }
            else
            {
                operationResult = 0;
            }
        }
        else if (strcmp(operator, "==") == 0) //取判断符判断
        {
            if (strcmp(judgementLeftValue, judgementRightValue) == 0) //真实/底层判断
            {
                operationResult = 1;
            }
            else
            {
                operationResult = 0;
            }
        }
        else if (strcmp(operator, "<=") == 0) //取判断符判断
        {
            if (strcmp(judgementLeftValue, judgementRightValue) <= 0) //真实/底层判断
            {
                operationResult = 1;
            }
            else
            {
                operationResult = 0;
            }
        }
        else if (strcmp(operator, "!=") == 0) //取判断符判断
        {
            if (strcmp(judgementLeftValue, judgementRightValue) != 0) //真实/底层判断
            {
                operationResult = 1;
            }
            else
            {
                operationResult = 0;
            }
        }
        else if (strcmp(operator, ">") == 0) //取判断符判断
        {
            if (strcmp(judgementLeftValue, judgementRightValue) > 0) //真实/底层判断
            {
                operationResult = 1;
            }
            else
            {
                operationResult = 0;
            }
        }
        else //<小于符
        {
            if (strcmp(judgementLeftValue, judgementRightValue) < 0) //真实/底层判断
            {
                operationResult = 1;
            }
            else
            {
                operationResult = 0;
            }
        }
    }

    return operationResult;
}

//主函数------------------------------
int main()
{
    openUpMemory(); // 开辟内存空间

    // 循环输入
    while (1)
    {
        // 接收语句
        char sentence[50] = {0}; //注意初始化
        gets(sentence);          //接收空格

        statementProcessing(sentence); //转语句处理函数
    }

    freeMemory(); //释放内存空间

    return 0; //程序结束
}