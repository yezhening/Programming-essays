//头文件————————————————————
#include <stdio.h>  //fprintf()、perror()
#include <stdlib.h> //exit()
#include <unistd.h> //pipe()、fork()、dup2()、close()
#include <errno.h>
#include <string.h>

//相关函数声明————————————————————
int makeargv(const char *s, const char *delimiters, char ***argvp);
void executeredirect(char *s, int in, int out);

//报错并退出————————————————————
static void perror_exit(char *s)
{
    perror(s);
    exit(1);
}

//处理流水线的命令执行————————————————————
void executecmd(char *cmds) //一行命令集
{
    int count;       //记录单个命令个数
    char **pipelist; //记录单个命令的命令“数组”，指向参数数组
    int i;           //命令“数组”循环变量
    int fds[2];      //构建管道的文件描述符
    int child;       //“父”创建进程后判断“父子”进程的进程号

    count = makeargv(cmds, "|", &pipelist); //取命令个数
    if (count <= 0)
    {
        fprintf(stderr, "Failed to find any commands\n");
        exit(1);
    }

    for (i = 0; i < count - 1; i++) //处理除最后一个命令
    {
        if (pipe(fds) == -1) //创建管道
        {
            perror_exit("Failed to create pipes");
        }
        else if ((child = fork()) == -1) //“父”创建“子”进程
        {
            perror_exit("Failed to create process to run command");
        }
        else if (child) //“父”进程
        {
            if (dup2(fds[1], STDOUT_FILENO) == -1) //重定向标准输出到管道   流水线/过滤器的重定向
            {
                perror_exit("Failed to connect pipeline");
            }
            if (close(fds[0]) || close(fds[1])) //关闭管道文件描述符
            {
                perror_exit("Failed to close needed files");
            }
            executeredirect(pipelist[i], i == 0, 0); //处理单个命令的重定向  重定向  只对第一条命令的标准输入重定向，指明源
            exit(1);                                 //处理完退出
        }

        //“子”进程
        if (dup2(fds[0], STDIN_FILENO) == -1)
        {
            perror_exit("Failed to connect last component");
        }
        if (close(fds[0]) || close(fds[1])) //关闭管道文件描述符
        {
            perror_exit("Failed to do final close");
        }
    }

    executeredirect(pipelist[i], i == 0, 1); //处理最后一条命令  只对标准输出重定向，指明目的    若只有一条命令，自适应对标准输入也重定向
    exit(1);
}