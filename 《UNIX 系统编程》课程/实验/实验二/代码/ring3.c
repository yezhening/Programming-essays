//头文件————————————————————
#include <stdio.h>
//fprintf()、perror()、stderr、sprintf()
#include <stdlib.h>    //atoi()
#include <unistd.h>    //pipe()、dup2()、close()、getpid()、getppid()、STDIN_FILENO、STDOUT_FILENO
#include <string.h>    //strerror()
#include <errno.h>     //errno
#include <sys/types.h> //pid_t

void prtastr(const char *s, int fd, int n);
void wastesometime(int n);

void prtastr(const char *s, int fd, int n)
{
    write(fd, s, n);

    wastesometime(n);
}

void wastesometime(int n)
{
    static volatile int dummy = 0;
    int i;

    for (i = 0; i < n; i++)
    {
        dummy++;
    }
}

//主函数————————————————————
int main(int argc, char *argv[])
{
    //变量定义
    int nprocs = 0; //记录进程总数    splint改
    int fd[2];      //创建管道使用的两个文件描述符记录
    int i;          //创建其余进程计数，从1开始，0是初始进程
    pid_t childpid; //记录所创建子进程的进程号
    int error;      //其余进程重定向连接管道时调用dup2()的返回值

    int n = 0; //命令行输入的可选的时延值

    //判断参数个数  必须放在赋值前面先检查
    if (argc == 3)
    {
        n = atoi(argv[2]);
    }
    else if (argc != 2)
    {
        fprintf(stderr, "Usage: %s nprocs\n", argv[0]);
        return 1;
    }

    //检查命令行：创建进程的数量为有效数字
    if ((nprocs = atoi(argv[1])) <= 0) //atoi()：字符串转换为整数
    {
        fprintf(stderr, "Usage: %s nprocs\n", argv[0]);
        return 1;
    }

    //初始进程操作
    //创建管道
    if (pipe(fd) == -1)
    {
        perror("Failed to create starting pipe");
        return 1;
    }

    //重定向,进程连接管道
    if ((dup2(fd[0], STDIN_FILENO) == -1) || (dup2(fd[1], STDOUT_FILENO) == -1))
    {
        perror("Failed to connect pipe");
        return 1;
    }

    //关闭多余文件描述符
    if ((close(fd[0]) == -1) || (close(fd[1]) == -1))
    {
        perror("Failed to close extra descriptors");
        return 1;
    }

    //其余进程操作
    for (i = 1; i < nprocs; i++)
    {
        //创建管道
        if (pipe(fd) == -1)
        {
            fprintf(stderr, "[%ld] :fail to create pipe %d: %s\n", (long)getpid(), i, strerror(errno));
            return 1;
        }

        //创建子进程
        if ((childpid = fork()) == -1)
        {
            fprintf(stderr, "[%ld] :fail to create child %d: %s\n", (long)getpid(), i, strerror(errno));
            return 1;
        }

        //重定向,进程连接管道
        if (childpid > 0) //父进程
        {
            error = dup2(fd[1], STDOUT_FILENO);
        }
        else //子进程
        {
            error = dup2(fd[0], STDIN_FILENO);
        }
        if (error == -1) //检错  漏一个等号
        {
            fprintf(stderr, "[%ld] :fail to dup pipes for iteration %d: %s\n", (long)getpid(), i, strerror(errno));
            return 1;
        }

        //关闭多余文件描述符
        if ((close(fd[0]) == -1) || (close(fd[1]) == -1))
        {
            fprintf(stderr, "[%ld] :fail to close extra descriptors %d: %s\n", (long)getpid(), i, strerror(errno));
            return 1;
        }

        //父进程成功创建子进程，父进程跳出循环不再执行，子进程继续执行
        if (childpid)
        {
            break;
        }
    }

    //打印判断是否成功创建进程、连接管道，nprocs数量的进程环
    char s[100];//使用指针出现段保护错
    sprintf(s, "This is process %d whit ID %ld and parent id %ld\n", i, (long)getpid(), (long)getppid());
    prtastr(s, STDERR_FILENO, n); //C标准的stderr是FIFE*而不是int类型
    return 0;
}