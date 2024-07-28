#include <stdio.h>  //printf()
#include <stdlib.h> //getenv()

int main()
{
    printf("PATH : %s\n", getenv("PATH")); //shell解释器搜索用户执行命令、程序的路径
    printf("HOME : %s\n", getenv("HOME")); //当前用户的主工作目录（家目录）
    printf("SHELL : %s\n", getenv("SHELL")); //当前用户使用的shell解释器名称/类型，值通常是/bin/bash

    printf("HOSTNAME : %s\n", getenv("HOSTNAME")); //主机名称
    printf("LOGNAME : %s\n", getenv("LOGNAME"));   //当前用户的登录名
    printf("PS1 : %s\n", getenv("PS1"));           // 基本提示符，对root用户是#，对普通用户是$

    printf("HISTSIZE : %s\n", getenv("HISTSIZE")); //历史记录数
    printf("LANGUGE : %s\n", getenv("LANGUGE"));               //语言相关的环境变量
    printf("MAIL : %s\n", getenv("MAIL"));                  // 当前用户的邮件存放目录

    printf("D_LIBRARY_PATH : %s\n", getenv("D_LIBRARY_PATH")); // 程序运行时，库文件的搜索路径的环境变量
    printf("LIBRARY_PATH : %s\n", getenv("LIBRARY_PATH"));     // 程序编译时，库文件的搜索路径和环境变量

    return 0;
}