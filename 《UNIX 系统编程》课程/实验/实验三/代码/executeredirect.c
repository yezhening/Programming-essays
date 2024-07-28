//头文件————————————————————
#include <string.h> //strchr()
#include <stdlib.h> //NULL
#include <stdio.h>  //perror()、stderr
#include <unistd.h> //execvp()
#include <errno.h>

//相关函数声明————————————————————
int parseandredirectin(char *s);
int parseandredirectout(char *s);
int makeargv(const char *s, const char *delimiters, char ***argvp);

//对可能带有重定向的单个命令进行处理
void executeredirect(char *s, int in, int out) //命令，标准输入重定向标志位，标准输出重定向标志位:非0为需重定向
{
    char *pin; //记录重定向符第一次出现的位置
    char *pout;
    char **chargv; //参数数组

    if (in && ((pin = strchr(s, '<')) != NULL) &&
        out && ((pout = strchr(s, '>')) != NULL) &&
        (pin > pout)) //标准输入重定向符号在标准输出重定向后出现
    {
        if (parseandredirectin(s) == -1) //先处理符号在后的重定向
        {
            perror("Fail to redirect input");
            return;
        }

        in = 0; //置位，避免可能的重复操作
    }

    if (out && (parseandredirectout(s) == -1)) //1.再处理符号在前的重定向    2.先处理符号在后的重定向   漏"out &&"
    {
        perror("Fail to redirect output");
    }
    else if (in && (parseandredirectin(s) == -1)) //再处理符号在前的重定向  漏"in &&"
    {
        perror("Fail to redirect input");
    }
    else if (makeargv(s, " \t", &chargv) <= 0) //获得参数数组 漏打，chargv未赋值，导致执行execvp产生段错误
    {
        fprintf(stderr,"Fail to parse command line\n");
    }
    else //执行命令
    {
        printf("这是子进程");
        execvp(chargv[0], chargv);           //一级指针，二级指针
        perror("Failed to execute command"); //成功执行后不会再往下执行
    }

    exit(1);
}