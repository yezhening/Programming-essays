//头文件————————————————————
#include <stdio.h>  //FILE
#include <stdlib.h> ///malloc()、
#include <string.h>
#include <math.h> //pow()

//常量————————————————————
//所读取的每行字符串长度——100
//图灵机结构体成员长度——50、20
//转换函数结构体成员长度——5
//检查图灵机属性所分割的字符串——5

//宏
#define TAPE_SIZE 20 //纸带大小/符号数

//结构体————————————————————
//转换函数结构体
struct transfer_function
{
    char start_state[5]; //开始状态
    char start_char;     //开始字符
    char end_state[5];   //结束状态
    char end_char;       //结束字符
    char end_operator;   //结束操作符
};

//图灵机
struct tm
{
    char state_set[50];                                 //状态集
    char tape_set[50];                                  //纸带集
    char initial_state[50];                             //初始状态
    char accept_state[50];                              //接受状态
    char refuse_state[50];                              //拒绝状态
    struct transfer_function transfer_function_set[50]; // 转换函数集

    int transfer_count;    //转换函数计数  定义时不能初始化
    char operator_set[50]; //操作符号集
};

//函数声明————————————————————
void read_tm_file(struct tm *concrete_tm);
void create_tm(char line_str[], struct tm *concrete_tm);
void check_tm(struct tm *concrete_tm);
int check_repeated_str(char str[], char sub_str[]);
int check_repeated_char(char str[], char sub_char);
void show_tm(struct tm *concrete_tm);

void read_tape_file(char tape[]);
void check_tape(struct tm *concrete_tm, char tape[]);
void get_next(struct tm *concrete_tm, char tape[]);
void odd_even_judgement(struct tm *concrete_tm, char tape[]);
void string_flip(struct tm *concrete_tm, char tape[]);

//函数定义————————————————————
//读取tm.txt文件
void read_tm_file(struct tm *concrete_tm)
{
    int length = 100; //所读取的每行字符串长度
    char *line_str = NULL;
    line_str = (char *)malloc(sizeof(char) * length); //所读取的每行字符串 动态数组
    FILE *fp = NULL;                                  //文件指针

    if ((fp = fopen("./tm.txt", "r")) == NULL)
    {
        fprintf(stderr, "Failed to open 'tm.txt' file\n");
        exit(1);
    }

    while (fgets(line_str, length, fp) != NULL) //读取文件内容，构造具体图灵机
    {
        create_tm(line_str, concrete_tm);
    }

    fclose(fp);

    return;
}

//根据tm.txt文件描述构造图灵机
void create_tm(char line_str[], struct tm *concrete_tm)
{
    //判断并存属性
    int i = 0;          //循环变量
    int store_flag = 0; //存标志 0不可存1可存
    int char_count = 0; //所读取字符串的字符计数

    int argument_flag = 0; //判断转换函数的第几个参数

    switch (line_str[0])
    {
    case 'Q':
        for (i = 0; i < strlen(line_str); i++)
        {
            if (line_str[i] == ':') //可存
            {
                store_flag = 1;
                i++;
                continue;
            }

            if (line_str[i] == ';') //不可存
            {
                concrete_tm->state_set[char_count] = '\0'; //必需
                break;
            }

            if (store_flag == 1 && line_str[i] != ' ') //存
            {
                concrete_tm->state_set[char_count] = line_str[i];
                char_count++;
            }
        }
        break;
    case 'S':
        for (i = 0; i < strlen(line_str); i++)
        {
            if (line_str[i] == ':') //可存
            {
                store_flag = 1;
                i++;
                continue;
            }

            if (line_str[i] == ';') //不可存
            {
                concrete_tm->tape_set[char_count] = '\0';
                break;
            }

            if (store_flag == 1 && line_str[i] != ' ') //存 纸带集无空格符
            {
                concrete_tm->tape_set[char_count] = line_str[i];
                char_count++;
            }
        }
        break;
    case 'q':
        for (i = 0; i < strlen(line_str); i++)
        {
            if (line_str[i] == ':') //可存
            {
                store_flag = 1;
                i++;
                continue;
            }

            if (line_str[i] == ';') //不可存
            {
                concrete_tm->initial_state[char_count] = '\0';
                break;
            }

            if (store_flag == 1 && line_str[i] != ' ') //存
            {
                concrete_tm->initial_state[char_count] = line_str[i];
                char_count++;
            }
        }
        break;
    case 'A':
        for (i = 0; i < strlen(line_str); i++)
        {
            if (line_str[i] == ':') //可存
            {
                store_flag = 1;
                i++;
                continue;
            }

            if (line_str[i] == ';') //不可存
            {
                concrete_tm->accept_state[char_count] = '\0';
                break;
            }

            if (store_flag == 1 && line_str[i] != ' ') //存
            {
                concrete_tm->accept_state[char_count] = line_str[i];
                char_count++;
            }
        }
        break;
    case 'R':
        for (i = 0; i < strlen(line_str); i++)
        {
            if (line_str[i] == ':') //可存
            {
                store_flag = 1;
                i++;
                continue;
            }

            if (line_str[i] == ';') //不可存
            {
                concrete_tm->refuse_state[char_count] = '\0';
                break;
            }

            if (store_flag == 1 && line_str[i] != ' ') //存
            {
                concrete_tm->refuse_state[char_count] = line_str[i];
                char_count++;
            }
        }
        break;
    case 'D': //第一行
        for (i = 0; i < strlen(line_str); i++)
        {
            if (argument_flag == 0 && line_str[i] == '(') //第一个左括号
            {
                argument_flag = 1;
                continue;
            }
            else if (argument_flag == 1 && line_str[i] == ',') //第一个逗号
            {
                concrete_tm->transfer_function_set[concrete_tm->transfer_count].start_state[char_count] = '\0';
                char_count = 0; //置位

                argument_flag = 2;
                continue;
            }
            else if (argument_flag == 2 && line_str[i] == ')') //第一个右括号
            {
                i = i + 2;
                continue;
            }
            else if (argument_flag == 2 && line_str[i] == '(') //第二个左括号
            {
                argument_flag = 3;
                continue;
            }
            else if (argument_flag == 3 && line_str[i] == ',') //第二个逗号
            {
                concrete_tm->transfer_function_set[concrete_tm->transfer_count].end_state[char_count] = '\0';
                char_count = 0;

                argument_flag = 4;
                continue;
            }
            else if (argument_flag == 4 && line_str[i] == ',') //第三个逗号
            {
                argument_flag = 5;
                continue;
            }
            else if (argument_flag == 5 && line_str[i] == ')') //第二个右括号
            {
                break;
            }

            //存
            if (argument_flag == 1)
            {
                concrete_tm->transfer_function_set[concrete_tm->transfer_count].start_state[char_count] = line_str[i];
                char_count++;
            }
            else if (argument_flag == 2)
            {
                concrete_tm->transfer_function_set[concrete_tm->transfer_count].start_char = line_str[i];
            }
            else if (argument_flag == 3)
            {
                concrete_tm->transfer_function_set[concrete_tm->transfer_count].end_state[char_count] = line_str[i];
                char_count++;
            }
            else if (argument_flag == 4)
            {
                concrete_tm->transfer_function_set[concrete_tm->transfer_count].end_char = line_str[i];
            }
            else if (argument_flag == 5)
            {
                concrete_tm->transfer_function_set[concrete_tm->transfer_count].end_operator = line_str[i];
            }
        }
        concrete_tm->transfer_count++;
        break;
    case '(': //其他行
        for (i = 0; i < strlen(line_str); i++)
        {
            if (argument_flag == 0 && line_str[i] == '(') //第一个左括号
            {
                argument_flag = 1;
                continue;
            }
            else if (argument_flag == 1 && line_str[i] == ',') //第一个逗号
            {
                concrete_tm->transfer_function_set[concrete_tm->transfer_count].start_state[char_count] = '\0';
                char_count = 0; //置位

                argument_flag = 2;
                continue;
            }
            else if (argument_flag == 2 && line_str[i] == ')') //第一个右括号
            {
                i = i + 2;
                continue;
            }
            else if (argument_flag == 2 && line_str[i] == '(') //第二个左括号
            {
                argument_flag = 3;
                continue;
            }
            else if (argument_flag == 3 && line_str[i] == ',') //第二个逗号
            {
                concrete_tm->transfer_function_set[concrete_tm->transfer_count].end_state[char_count] = '\0';
                char_count = 0;

                argument_flag = 4;
                continue;
            }
            else if (argument_flag == 4 && line_str[i] == ',') //第三个逗号
            {
                argument_flag = 5;
                continue;
            }
            else if (argument_flag == 5 && line_str[i] == ')') //第二个右括号
            {
                break;
            }

            //存
            if (argument_flag == 1)
            {
                concrete_tm->transfer_function_set[concrete_tm->transfer_count].start_state[char_count] = line_str[i];
                char_count++;
            }
            else if (argument_flag == 2)
            {
                concrete_tm->transfer_function_set[concrete_tm->transfer_count].start_char = line_str[i];
            }
            else if (argument_flag == 3)
            {
                concrete_tm->transfer_function_set[concrete_tm->transfer_count].end_state[char_count] = line_str[i];
                char_count++;
            }
            else if (argument_flag == 4)
            {
                concrete_tm->transfer_function_set[concrete_tm->transfer_count].end_char = line_str[i];
            }
            else if (argument_flag == 5)
            {
                concrete_tm->transfer_function_set[concrete_tm->transfer_count].end_operator = line_str[i];
            }
        }
        concrete_tm->transfer_count++;
        break;
    default:
        fprintf(stderr, "Failed to construct the turing machine: 'tm.txt' file contains unknown contents");
        exit(1);
    }

    return;
}

//查看图灵机属性
void show_tm(struct tm *concrete_tm)
{
    printf("[图灵机]\n");
    printf("状态集：%s\n", concrete_tm->state_set);
    printf("纸带集：%s\n", concrete_tm->tape_set);
    printf("初始状态：%s\n", concrete_tm->initial_state);
    printf("接受状态：%s\n", concrete_tm->accept_state);
    printf("拒绝状态：%s\n", concrete_tm->refuse_state);
    printf("转换函数：开始状态 开始字符 结束状态 结束字符 结束操作符\n");
    for (int i = 0; i < concrete_tm->transfer_count; i++)
    {
        printf("%s ", concrete_tm->transfer_function_set[i].start_state);
        printf("%c ", concrete_tm->transfer_function_set[i].start_char);
        printf("%s ", concrete_tm->transfer_function_set[i].end_state);
        printf("%c ", concrete_tm->transfer_function_set[i].end_char);
        printf("%c\n", concrete_tm->transfer_function_set[i].end_operator);
    }

    return;
}

//检查图灵机属性
void check_tm(struct tm *concrete_tm)
{
    //各状态集无重复状态
    char str[5] = {'\0'};   //所分割的字符串
    int char_count = 0;     //字符计数
    int repeated_count = 0; //重复字符数量

    for (int i = 0; i < strlen(concrete_tm->state_set); i++)
    {
        if (concrete_tm->state_set[i] != ',')
        {
            str[char_count] = concrete_tm->state_set[i];
            char_count++;
        }

        if ((concrete_tm->state_set[i] == ',') || (i == strlen(concrete_tm->state_set) - 1))
        {
            repeated_count = check_repeated_str(concrete_tm->state_set, str); //处理思路：遍历，存在该字符串个数>1
            if (repeated_count > 1)
            {
                fprintf(stderr, "Failed to construct the turing machine: 'tm.txt' file's state_set contains repeated string");
                exit(1);
            }

            char_count = 0;                 //置位
            memset(str, '\0', sizeof(str)); //清空
        }
    }

    for (int i = 0; i < strlen(concrete_tm->tape_set); i++)
    {
        if (concrete_tm->tape_set[i] != ',')
        {
            str[char_count] = concrete_tm->tape_set[i];
            char_count++;
        }

        if ((concrete_tm->tape_set[i] == ',') || (i == strlen(concrete_tm->tape_set) - 1))
        {
            repeated_count = check_repeated_str(concrete_tm->tape_set, str); //处理思路：遍历，存在该字符串个数>1
            if (repeated_count > 1)
            {
                fprintf(stderr, "Failed to construct the turing machine: 'tm.txt' file's tape_set contains repeated string");
                exit(1);
            }

            char_count = 0;                 //置位
            memset(str, '\0', sizeof(str)); //清空
        }
    }

    for (int i = 0; i < strlen(concrete_tm->accept_state); i++)
    {
        if (concrete_tm->accept_state[i] != ',')
        {
            str[char_count] = concrete_tm->accept_state[i];
            char_count++;
        }

        if ((concrete_tm->accept_state[i] == ',') || (i == strlen(concrete_tm->accept_state) - 1))
        {
            repeated_count = check_repeated_str(concrete_tm->accept_state, str); //处理思路：遍历，存在该字符串个数>1
            if (repeated_count > 1)
            {
                fprintf(stderr, "Failed to construct the turing machine: 'tm.txt' file's accept_state contains repeated string");
                exit(1);
            }

            char_count = 0;                 //置位
            memset(str, '\0', sizeof(str)); //清空
        }
    }

    for (int i = 0; i < strlen(concrete_tm->refuse_state); i++)
    {
        if (concrete_tm->accept_state[i] != ',')
        {
            str[char_count] = concrete_tm->refuse_state[i];
            char_count++;
        }

        if ((concrete_tm->refuse_state[i] == ',') || (i == strlen(concrete_tm->refuse_state) - 1))
        {
            repeated_count = check_repeated_str(concrete_tm->refuse_state, str); //处理思路：遍历，存在该字符串个数>1
            if (repeated_count > 1)
            {
                fprintf(stderr, "Failed to construct the turing machine: 'tm.txt' file's refuse_state contains repeated string");
                exit(1);
            }

            char_count = 0;                 //置位
            memset(str, '\0', sizeof(str)); //清空
        }
    }

    //初始状态只有一个  可扩展
    for (int i = 0; i < strlen(concrete_tm->initial_state); i++)
    {
        if (concrete_tm->initial_state[i] == ',')
        {
            fprintf(stderr, "Failed to construct the turing machine: 'tm.txt' file's initial state contains redundant string");
            exit(1);
        }
    }

    //接受状态和拒绝状态在状态集合中
    for (int i = 0; i < strlen(concrete_tm->accept_state); i++)
    {
        if (concrete_tm->accept_state[i] != ',')
        {
            str[char_count] = concrete_tm->accept_state[i];
            char_count++;
        }

        if ((concrete_tm->accept_state[i] == ',') || (i == strlen(concrete_tm->accept_state) - 1))
        {
            repeated_count = check_repeated_str(concrete_tm->state_set, str); //不同点
            if (repeated_count == 0)
            {
                fprintf(stderr, "Failed to construct the turing machine: Existing accept state is not in state set");
                exit(1);
            }

            char_count = 0;                 //置位
            memset(str, '\0', sizeof(str)); //清空
        }
    }

    for (int i = 0; i < strlen(concrete_tm->refuse_state); i++)
    {
        if (concrete_tm->refuse_state[i] != ',')
        {
            str[char_count] = concrete_tm->refuse_state[i];
            char_count++;
        }

        if ((concrete_tm->refuse_state[i] == ',') || (i == strlen(concrete_tm->refuse_state) - 1))
        {
            repeated_count = check_repeated_str(concrete_tm->state_set, str);
            if (repeated_count == 0)
            {
                fprintf(stderr, "Failed to construct the turing machine: Existing refuse state is not in state set");
                exit(1);
            }

            char_count = 0;                 //置位
            memset(str, '\0', sizeof(str)); //清空
        }
    }

    //接受状态和拒绝状态无交集
    for (int i = 0; i < strlen(concrete_tm->accept_state); i++)
    {
        if (concrete_tm->accept_state[i] != ',')
        {
            str[char_count] = concrete_tm->accept_state[i];
            char_count++;
        }

        if ((concrete_tm->accept_state[i] == ',') || (i == strlen(concrete_tm->accept_state) - 1))
        {
            repeated_count = check_repeated_str(concrete_tm->refuse_state, str); //不同点
            if (repeated_count != 0)
            {
                fprintf(stderr, "Failed to construct the turing machine: accept state and refuse state have the same element");
                exit(1);
            }

            char_count = 0;                 //置位
            memset(str, '\0', sizeof(str)); //清空
        }
    }

    //转换函数集各参数在各状态集中存在
    for (int i = 0; i < concrete_tm->transfer_count; i++)
    {
        repeated_count = check_repeated_str(concrete_tm->state_set, concrete_tm->transfer_function_set[i].start_state); //不同点
        if (repeated_count == 0)
        {
            fprintf(stderr, "Failed to construct the turing machine: start state is not in state set");
            exit(1);
        }

        repeated_count = 0; //置位
        repeated_count = check_repeated_char(concrete_tm->tape_set, concrete_tm->transfer_function_set[i].start_char);
        if (repeated_count == 0)
        {
            fprintf(stderr, "Failed to construct the turing machine: start char is not in tape set");
            exit(1);
        }

        repeated_count = 0;
        repeated_count = check_repeated_str(concrete_tm->state_set, concrete_tm->transfer_function_set[i].end_state);
        if (repeated_count == 0)
        {
            fprintf(stderr, "Failed to construct the turing machine: end state is not in state set");
            exit(1);
        }

        repeated_count = 0;
        repeated_count = check_repeated_char(concrete_tm->tape_set, concrete_tm->transfer_function_set[i].end_char);
        if (repeated_count == 0)
        {
            fprintf(stderr, "Failed to construct the turing machine: end char is not in tape set");
            exit(1);
        }

        repeated_count = 0;
        repeated_count = check_repeated_char(concrete_tm->operator_set, concrete_tm->transfer_function_set[i].end_operator);
        if (repeated_count == 0)
        {
            fprintf(stderr, "Failed to construct the turing machine: end operator is not in operator set");
            exit(1);
        }
    }
}

//检查重复字符串
int check_repeated_str(char str[], char sub_str[])
{
    char split_str[5] = {'\0'}; //所分割的字符串
    int char_count = 0;         //字符计数
    int sub_str_count = 0;      //重复字符串计数

    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] != ',')
        {
            split_str[char_count] = str[i];
            char_count++;
        }

        if ((str[i] == ',') || (i == strlen(str) - 1))
        {
            if ((strcmp(split_str, sub_str)) == 0) //比较
            {
                sub_str_count++;
            }

            char_count = 0;                             //置位
            memset(split_str, '\0', sizeof(split_str)); //清空
        }
    }

    return sub_str_count;
}

//检查重复字符
int check_repeated_char(char str[], char sub_char)
{
    int sub_char_count = 0; //重复字符计数

    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] == sub_char) //可不管分隔符','
        {
            sub_char_count++;
        }
    }

    return sub_char_count;
}

//读取tape.txt文件
void read_tape_file(char tape[])
{
    FILE *fp = NULL; //文件指针

    if ((fp = fopen("./tape.txt", "r")) == NULL)
    {
        fprintf(stderr, "Failed to open 'tape.txt' file\n");
        exit(1);
    }

    if (fgets(tape, TAPE_SIZE, fp) == NULL) //读取文件内容，构造输入纸带
    {
        fprintf(stderr, "The tape file has no content\n");
        exit(1);
    }

    printf("\n[输入纸带]\n%s\n", tape);

    fclose(fp);

    return;
}

//检查输入纸带
void check_tape(struct tm *concrete_tm, char tape[])
{
    for (int i = 0; i < strlen(tape); i++) //对每输入纸带字符
    {
        int j = 0;
        for (j = 0; j < strlen(concrete_tm->tape_set); j++) //对具体图灵机纸带集
        {
            if (tape[i] == concrete_tm->tape_set[j]) //可不管分隔符
            {
                break;
            }
        }

        if (j == strlen(concrete_tm->tape_set))
        {
            fprintf(stderr, "Invalid characters exist in the tape\n");
            exit(1);
        }
    }
}

//获取后继
void get_next(struct tm *concrete_tm, char tape[])
{
    char current_state[5]; //记录当前状态
    strncpy(current_state, concrete_tm->initial_state, strlen(concrete_tm->initial_state));
    current_state[strlen(concrete_tm->initial_state)] = '\0'; //注意    手动加结束符

    int tape_pos = 0; //输入纸带读写头位置
    char operator;    //输入纸带操作符

    char tape1[TAPE_SIZE]; //记录原输入纸带
    strncpy(tape1, tape, strlen(tape));

    printf("\n[运行过程]\n");
    printf("当前状态 读取字符 转换状态 转换字符 操作符 读写头位置\n");

    while (1) //循环操作 知道接受/拒绝状态才退出
    {
        printf("%s %c ", current_state, tape[tape_pos]);

        int i = 0;                                        //循环变量
        for (i = 0; i < concrete_tm->transfer_count; i++) //对每一个转换函数
        {
            if ((strncmp(concrete_tm->transfer_function_set[i].start_state, current_state, strlen(current_state)) == 0) &&
                (concrete_tm->transfer_function_set[i].start_char == tape[tape_pos])) //匹配
            {
                //转换
                strncpy(current_state, concrete_tm->transfer_function_set[i].end_state, strlen(concrete_tm->transfer_function_set[i].end_state));
                current_state[strlen(concrete_tm->transfer_function_set[i].end_state)] = '\0';

                tape[tape_pos] = concrete_tm->transfer_function_set[i].end_char;

                operator= concrete_tm->transfer_function_set[i].end_operator;

                printf("%s %c %c ", current_state, tape[tape_pos], operator);

                break;
            }
        }

        if (i == concrete_tm->transfer_count)
        {
            fprintf(stderr, "Failed to find transfer function\n");
            exit(1);
        }

        for (int i = 0; i <= tape_pos; i++)
        {
            printf("%c", tape1[i]);
        }
        printf("\n");

        //根据操作符移动输入纸带读写头位置
        if (operator== 'L')
        {
            tape_pos--;
        }
        else if (operator== 'R')
        {
            tape_pos++;
        }

        //停机
        if (strncmp(current_state, concrete_tm->accept_state, strlen(concrete_tm->accept_state)) == 0) //接受
        {
            printf("\n[图灵机接受状态]");
            printf("\n[输出纸带]\n");
            printf("%s\n", tape);

            break;
        }
        else if (strncmp(current_state, concrete_tm->accept_state, strlen(concrete_tm->refuse_state)) == 0) //拒绝
        {
            printf("\n[图灵机拒绝状态]");
            printf("\n[输出纸带]\n");
            printf("%s\n", tape);

            break;
        }
    }

    return;
}

//数字的奇偶判定
void odd_even_judgement(struct tm *concrete_tm, char tape[])
{
    int number = 0;           //十进制数字
    int base_number = 2;      //基数
    int transform_number = 0; //每二进制数转换的十进制数

    for (int i = 0; i < strlen(tape); i++)
    {
        if (tape[i] != 'B')
        {
            base_number = pow(2, i - 1); //考虑第一个空白起始字符
            if (tape[i] == '1')
            {
                transform_number = 1 * base_number;
            }
            else
            {
                transform_number = 0;
            }
            number = number + transform_number;
        }
    }

    printf("\n[数字的奇偶判定]\n");
    printf("数字：%d，", number);

    if (number % 2 != 0)
    {
        printf("为奇数\n", number);
    }
    else
    {
        printf("为偶数\n", number);
    }

    return;
}

//字符串翻转
void string_flip(struct tm *concrete_tm, char tape[])
{
    char current_state[5]; //记录当前状态
    strncpy(current_state, concrete_tm->initial_state, strlen(concrete_tm->initial_state));
    current_state[strlen(concrete_tm->initial_state)] = '\0'; //注意    手动加结束符

    int tape_pos = 0; //输入纸带读写头位置
    char operator;    //输入纸带操作符

    char tape1[TAPE_SIZE]; //记录原输入纸带
    strncpy(tape1, tape, strlen(tape));

    printf("\n[运行过程]\n");
    printf("当前状态 读取字符 转换状态 转换字符 操作符 读写头位置   输出纸带\n");

    while(1) //循环操作 知道接受/拒绝状态才退出
    {
        printf("%-7s %-7c ", current_state, tape[tape_pos]);

        int i = 0;                                        //循环变量
        for (i = 0; i < concrete_tm->transfer_count; i++) //对每一个转换函数
        {
            if ((strncmp(concrete_tm->transfer_function_set[i].start_state, current_state, strlen(current_state)) == 0) &&
                (concrete_tm->transfer_function_set[i].start_char == tape[tape_pos])) //匹配
            {
                //转换
                strncpy(current_state, concrete_tm->transfer_function_set[i].end_state, strlen(concrete_tm->transfer_function_set[i].end_state));
                current_state[strlen(concrete_tm->transfer_function_set[i].end_state)] = '\0';

                tape[tape_pos] = concrete_tm->transfer_function_set[i].end_char;

                operator= concrete_tm->transfer_function_set[i].end_operator;

                printf("%-7s %-7c %-7c ", current_state, tape[tape_pos], operator);

                break;
            }
        }

        if (i == concrete_tm->transfer_count)
        {
            fprintf(stderr, "Failed to find transfer function\n");
            exit(1);
        }

        for (int i = 0; i <= tape_pos; i++) //读写头位置
        {
            printf("%c", tape1[i]);
        }

        for (int i = tape_pos; i < 12; i++) //输出空格   美化显示
        {
            printf(" ");
        }

        printf("%s\n", tape); //输出纸带

        //根据操作符移动输入纸带读写头位置
        if (operator== 'L')
        {
            tape_pos--;
        }
        else if (operator== 'R')
        {
            tape_pos++;
        }

        //停机
        if (strncmp(current_state, concrete_tm->accept_state, strlen(concrete_tm->accept_state)) == 0) //接受
        {
            printf("\n[图灵机接受状态]");
            printf("\n[输出纸带]\n");
            printf("%s\n", tape);

            break;
        }
        else if (strncmp(current_state, concrete_tm->accept_state, strlen(concrete_tm->refuse_state)) == 0) //拒绝
        {
            printf("\n[图灵机拒绝状态]");
            printf("\n[输出纸带]\n");
            printf("%s\n", tape);

            break;
        }
    }

    return;
}

//主函数————————————————————
int main()
{
    //具体图灵机初始化
    struct tm concrete_tm1 = //具体图灵机    必需，否则结构体指针变量为空
        {
            transfer_count : 0,        //转换函数计数
            operator_set : "L,R,S,+,-" //定义操作符号集 可扩展
        };
    struct tm *concrete_tm = NULL;
    concrete_tm = &concrete_tm1;

    read_tm_file(concrete_tm); //读取tm.txt文件,根据tm.txt文件描述构造图灵机   传地址

    check_tm(concrete_tm); //检查图灵机属性

    show_tm(concrete_tm); //查看图灵机属性

    char tape[TAPE_SIZE]; //纸带
    read_tape_file(tape); //读取tape.txt文件,根据tape.txt文件描述得输入纸带

    check_tape(concrete_tm, tape); //检查输入纸带

    // get_next(concrete_tm, tape); //获取后继

    // odd_even_judgement(concrete_tm, tape); //数字的奇偶判定

    string_flip(concrete_tm, tape); //字符串翻转

    return 0;
}