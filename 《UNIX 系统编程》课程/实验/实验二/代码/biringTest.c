//头文件————————————————————
#include <stdio.h>
//fprintf()、perror()、stderr
#include <stdlib.h>    //atoi()、malloc()
#include <unistd.h>    //pipe()、dup2()、close()、getpid()、getppid()、STDIN_FILENO、STDOUT_FILENO
#include <string.h>    //strerror()
#include <errno.h>     //errno
#include <sys/types.h> //pid_t
#include <sys/wait.h>  //wait()

//主函数————————————————————
int main(int argc, char *argv[])
{
    //变量定义
    int nprocs = 0; //记录进程总数    splint改
    int fd[5];      //创建管道使用的两个文件描述符记录      进程环另一个方向记录
    int i;          //创建其余进程计数，从1开始，0是初始进程
    pid_t childpid; //记录所创建子进程的进程号
    int error;      //其余进程重定向连接管道时调用dup2()的返回值

    long *processIdArray;  //记录环上所有进程的id
    long *processIdArray1; //记录另一个方向环上所有进程的id
    long next_ID;          //记录当前进程的进程id，用于构建进程id数组

    //检查命令行：一个输入参数，创建进程的数量为有效数字
    if ((argc != 2) || ((nprocs = atoi(argv[1])) <= 0)) //atoi()：字符串转换为整数
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

    processIdArray = (long *)malloc((nprocs + 1) * sizeof(long));  //创建动态数组  每个进程都有/独立   每个“子进程”继承动态数组
    processIdArray1 = (long *)malloc((nprocs + 1) * sizeof(long)); //创建动态数组  每个进程都有/独立   每个“子进程”继承动态数组

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

        //另一个进程环方向记录  先记录再后续的重定向
        //重定向,进程连接管道
        if (childpid > 0) //父进程
        {
            fd[4] = dup(STDOUT_FILENO);
            fd[3] = dup(fd[0]);
            // error = dup2(STDOUT_FILENO, fd[4]);  //dup2会失败：Bad file descriptor
            // error = dup2(fd[0], fd[3]);
        }
        else //子进程
        {
            fd[3] = dup(STDIN_FILENO);
            fd[4] = dup(fd[1]);
            // error = dup2(STDIN_FILENO, fd[3]);
            // error = dup2(fd[1], fd[4]);
        }
        if ((fd[3] == -1) || (fd[4] == -1)) //检错
        {
            fprintf(stderr, "[%ld] :fail to dup the other pipes for iteration %d: %s\n", (long)getpid(), i, strerror(errno));
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

        //传数据
        //重定向,进程连接管道
        if (childpid > 0) //父进程
        {
            processIdArray[0] = getpid();
            next_ID = getpid();

            write(fd[4], &next_ID, sizeof(next_ID));
            read(fd[3], &next_ID, sizeof(next_ID)); //接收的是前一个进程id号
            processIdArray[i] = next_ID;
        }
        else //子进程
        {
            if (i == nprocs - 1)
            {
                processIdArray[0] = getpid();
                next_ID = getpid();

                write(fd[4], &next_ID, sizeof(next_ID));
                read(fd[3], &next_ID, sizeof(next_ID)); //接收的是前一个进程id号

                processIdArray[i + 1] = next_ID; //记录它接收的
            }
        }

        //父进程成功创建子进程，父进程跳出循环不再执行，子进程继续执行
        if (childpid)
        {
            break;
        }
    }

    //wait()调用
    pid_t childpid1;
    childpid1 = wait(NULL);
    if (childpid1 != -1)
    {
        fprintf(stderr, "Waited for child with pid %ld\n", childpid1); //漏打1    标准输出已被重定向！
    }

    //打印判断是否成功创建进程、连接管道，nprocs数量的进程环
    fprintf(stderr, "This is process %d whit ID %ld and parent id %ld\n", i, (long)getpid(), (long)getppid());

    //不能用printf()，因为已被重定向    用标准错误的输出显示信息
    fprintf(stderr, "This is process's id array value: ");
    for (int j = 0; j <= nprocs; j++) //数组个数应多1
    {
        fprintf(stderr, "%ld ", processIdArray[j]);
    }
    fprintf(stderr, "\n\n");

    return 0;
}