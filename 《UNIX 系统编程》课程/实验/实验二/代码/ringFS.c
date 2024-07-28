//头文件————————————————————
#include <stdio.h>
//fprintf()、perror()、stderr
#include <stdlib.h>    //atoi()、sand()、srand()
#include <unistd.h>    //pipe()、dup2()、close()、getpid()、getppid()、STDIN_FILENO、STDOUT_FILENO
#include <string.h>    //strerror()、strcpy()
#include <errno.h>     //errno
#include <sys/types.h> //pid_t

#include <sys/wait.h> //wait()
#include <time.h>     //time()

//主函数————————————————————
int main(int argc, char *argv[])
{
    //变量定义
    int nprocs = 0; //记录进程总数    splint改
    int fd[2];      //创建管道使用的两个文件描述符记录
    int i;          //创建其余进程计数，从1开始，0是初始进程
    pid_t childpid; //记录所创建子进程的进程号
    int error;      //其余进程重定向连接管道时调用dup2()的返回值

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

    //实现Fibonacci数计算
    char receivedFiString[50]; //记录接收fibonacci字符串
    char fiStringNum1[23];     //记录fibonacci字符数值
    char fiStringNum2[23];
    long long fiNum1; //记录fibonacci数字
    long long fiNum2;
    long long fiResult;      //记录fibonacci计算结果数字
    char fiStringResult[23]; //记录fibonacci计算结果字符数值
    char sendFiString[50];   //记录发送fibonacci字符串

    if (i == 1) //原始父进程
    {
        strcpy(sendFiString, "1 1");
        write(STDOUT_FILENO, &sendFiString, sizeof(sendFiString)); //先写后读
        read(STDIN_FILENO, &receivedFiString, sizeof(receivedFiString));

        fprintf(stderr, "Process %d whit PID %ld and parent PID %ld received %s and sent %s\n", i, (long)getpid(), (long)getppid(), receivedFiString, sendFiString); //打印消息
    }
    else //其他进程
    {
        read(STDIN_FILENO, &receivedFiString, sizeof(receivedFiString)); //先读后写

        //取值  接收字符串转换为字符串数值
        int flag = 0;
        int k = 0; //记录第二个字符的下标
        for (int j = 0; j < sizeof(receivedFiString); j++)
        {
            if (receivedFiString[j] == '\0') //结束符
            {
                break;
            }
            if (receivedFiString[j] == ' ') //分隔符
            {
                fiStringNum1[j] = '\0'; //必须有结束符
                j++;                    //第二个字符不读空格
                flag = 1;
            }

            if (flag == 0)
            {
                fiStringNum1[j] = receivedFiString[j];
            }
            else
            {
                fiStringNum2[k] = receivedFiString[j];
                k++;
            }
        }

        //转换  字符串数值转换为数字
        fiNum1 = (long long)atoi(fiStringNum1); //atoi()返回值是int
        fiNum2 = (long long)atoi(fiStringNum2);

        fiResult = fiNum1 + fiNum2; //计算

        //检测溢出
        if (fiResult < fiNum1)
        {
            strcat(sendFiString, "0 0");
        }
        else
        {
            sprintf(fiStringResult, "%lld", fiResult); //转换   数字转换为字符串数值  linux无itoa()  注意格式化字符串使用%ld

            //转换  字符串数值转换为字符串
            strcat(sendFiString, fiStringNum2);
            strcat(sendFiString, " ");
            strcat(sendFiString, fiStringResult);

            write(STDOUT_FILENO, &sendFiString, sizeof(sendFiString));
        }

        fprintf(stderr, "Process %d whit PID %ld and parent PID %ld received %s and sent %s\n", i, (long)getpid(), (long)getppid(), receivedFiString, sendFiString); //打印消息
    }

    //实现信息检索
    pid_t pid;  //记录当前进程的进程号
    int random; //记录当前进程的随机数/查找信息

    pid = getpid();
    srand((unsigned)time(NULL) + i);
    random = rand() % 10 + 1;

    char pidString[10]; //字符形式
    char randomString[5];

    sprintf(pidString, "%ld", long(pid));
    sprintf(randomString, "%d", random);

    char sendMessage[20];     //发送的信息
    char receivedMessage[20]; //接收的信息

    char searchPid[10];   //接收的查找进程的id号
    char searchRamdom[5]; //接收的随机数字符串

    if (i == 1) //第一个进程
    {
        fprintf(stderr, "Process %d whit PID %ld and parent PID %ld search number: %s\n", i, (long)getpid(), (long)getppid(), randomString); //打印消息

        //生成发送信息
        strcat(sendMessage, pidString);
        strcat(sendMessage, " ");
        strcat(sendMessage, randomString);

        write(STDOUT_FILENO, &sendMessage, sizeof(sendMessage)); //先写后读
        // read(STDIN_FILENO, &receivedMessage, sizeof(receivedMessage));
    }
    else
    {
        read(STDIN_FILENO, &receivedMessage, sizeof(receivedMessage)); //先读后写

        //取值
        int flag = 0;
        int k = 0; //记录第二个字符的下标
        for (int j = 0; j < sizeof(receivedMessage); j++)
        {
            if (receivedMessage[j] == '\0') //结束符
            {
                break;
            }
            if (receivedMessage[j] == ' ') //分隔符
            {
                searchPid[j] = '\0'; //必须有结束符
                j++;                 //第二个字符不读空格
                flag = 1;
            }

            if (flag == 0)
            {
                searchPid[j] = receivedMessage[j];
            }
            else
            {
                searchRamdom[k] = receivedMessage[j];
                k++;
            }
        }

        //比较信息
        if (strcmp(searchRamdom, randomString) == 0) //找到
        {
            fprintf(stderr, "Process %d whit PID %ld and parent PID %ld has number: %s\n", i, (long)getpid(), (long)getppid(), searchRamdom); //打印消息
        }
        else
        {
            fprintf(stderr, "Process %d whit PID %ld and parent PID %ld hasn't number: %s. And has number: %s\n", i, (long)getpid(), (long)getppid(), searchRamdom, randomString); //打印消息
        }

        strcat(sendMessage, receivedMessage);

        write(STDOUT_FILENO, &sendMessage, sizeof(sendMessage));
    }

    //wait()调用    放一个wait()避免孤儿进程
    pid_t childpid1;
    childpid1 = wait(NULL);
    if (childpid1 != -1)
    {
        // fprintf(stderr, "Waited for child with pid %ld\n", childpid1);//不打印
    }

    return 0;
}