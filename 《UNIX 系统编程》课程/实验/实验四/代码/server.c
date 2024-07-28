//头文件————————————————————
#include <sys/socket.h>   //网络编程相关
#include <stdio.h>        //perror()
#include <netinet/in.h>   //sockaddr_in、htonl()、htons()
#include <unistd.h>       //close()
#include <pthread.h>      //线程相关
#include <string.h>       //strerror()、memset()、strcmp()
#include <time.h>         //时间相关
#include <stdlib.h>       //exit()
#include <sys/resource.h> //getrlimit()
#include <sys/stat.h>     //open()、umask()
#include <fcntl.h>        //open()

//宏————————————————————
#define BUFSIZE 1024 //传递消息缓冲区大小

//线程处理请求
void *handle(void *conn_fd)
{
    int hand_fd;                                                    //已建立连接并用来处理请求的套接字文件描述符
    char msg_recv[BUFSIZE];                                         //接收客户端消息缓冲区
    const char *msg_send = "The server has received your messages"; //发送客户端响应消息
    time_t time_msg;                                                //系统时间，秒数为单位
    char *date_msg;                                                 //系统日期，一定日期格式

    hand_fd = *((int *)conn_fd); //类型转换 处理套接字文件描述符

    //处理请求
    while (1) //一个线程与一个客户端绑定进行通信
    {
        memset(msg_recv, 0, BUFSIZE); //初始化缓冲区    全0

        if (read(hand_fd, msg_recv, BUFSIZE) == -1) //客户端发送、服务端接收消息
        {
            perror("Failed to receive messages from the client");
            // return 1;
            //报警告   warning: return makes pointer from integer without a cast [enabled by default
            //在函数中出错，直接退出程序
            exit(1);
        }

        if (strcmp(msg_recv, "quit") == 0) //断开连接
        {
            printf("The client requested to disconnect\n");
            break;
        }
        else if (strcmp(msg_recv, "hello") == 0)
        {
            printf("client's messages：%s\n", msg_recv);

            msg_send = "hello";
        }
        else if (strcmp(msg_recv, "date") == 0)
        {
            printf("client's messages：%s\n", msg_recv);

            time_msg = time(NULL);       //系统时间
            date_msg = ctime(&time_msg); //日期时间
            msg_send = date_msg;
        }
        else
        {
            printf("client's messages：%s\n", msg_recv);

            msg_send=msg_recv;
        }

        if (write(hand_fd, msg_send, strlen(msg_send)) == -1) //服务端发送、客户端接收消息
        {
            perror("Failed to send messages to the client");
            exit(1);
        }
    }

    close(hand_fd);     //关闭套接字文件描述符
    pthread_exit(NULL); // 线程退出
}

//创建并启动守护进程
void create_daemon()
{
    pid_t child_pid;   //进程号
    pid_t child_pid2;  //避免使用重复变量
    struct rlimit lim; //记录资源软硬件限制的结构体
    int fd_lim;        //进程可打开的最大文件描述符数量
    int fd_i;          //进程可打开的文件描述符（循环计数）
    int fd;            //文件描述符

    if ((child_pid = fork()) < 0) //创建“子”进程
    {
        perror("Failed to fork");
        exit(1);
    }

    if (child_pid > 0) //“父”进程退出，“子”进程是孤儿进程，被Init进程收养，成为后台进程，不是会话、进程组组长
    {
        exit(1);
    }

    //“子”进程操作
    if (setsid() == -1) //创建新会话，为会话组长；创建新进程组，为进程组组长  还可绑定控制终端
    // setsid()出错返回值为pid_t-1并设置errno，有的资料说明返回-1
    // setsid()在调用进程是进程组组长时会失败，前因为fork保证其不是进程组组长
    {
        perror("Failed to create new session");
        exit(1);
    }

    if ((child_pid2 = fork()) < 0) //创建“孙”进程
    {
        perror("Failed to fork");
        exit(1);
    }

    if (child_pid2 > 0) //“子”进程退出，“孙”进程不是会话组组长，无权限重新打开/绑定控制终端
    {
        exit(1);
    }

    //“孙”进程操作
    if ((getrlimit(RLIMIT_NOFILE, &lim)) == -1) //获取进程可打开的最大文件描述符数量
    // RLIMIT_NOFILE：每个进程能打开的最大文件数
    {
        perror("Failed to get the maximum number of file descriptors that a process can open");
        exit(1);
    }

    //获取硬限制 lim.rlim_cur：软限制，lim.rlim_max：硬限制
    //在更改资源限制时，须遵循下列三条规则：
    //任何一个进程都可将一个软限制值更改为小于或等于其硬限制值
    //任何一个进程都可降低其硬限制值，但它必须大于或等于其软限制值。这种降低对普通用户而言是不可逆的
    //只有超级用户进程可以提高硬限制值
    lim.rlim_cur = lim.rlim_max;
    fd_lim = lim.rlim_cur;

    //设置硬限制作为软限制
    if ((setrlimit(RLIMIT_NOFILE, &lim)) == -1)
    {
        {
            perror("Failed to set the maximum number of file descriptors that a process can open");
            exit(1);
        }
    }

    //　“孙”进程从创建它的“父子”进程继承打开的文件描述符，不关闭将会浪费系统资源，造成进程所在的文件系统无法卸下以及引起无法预料的错误
    for (fd_i = 3; fd_i < fd_lim; fd_i++) //关闭文件描述符  是3不是0
    {
        close(fd_i);
    }

    // 重定向三个标准文件描述符到黑洞文件   可直接关闭  后面程序运行需要时再重定向、打开   测试时注释 
    if ((fd = open("/dev/null", O_RDWR)) < 0)
    {
        {
            perror("Failed to open /dev/null");
            exit(1);
        }
    }

    for (fd_i = 0; fd_i < 3; fd_i++)
    {
        if ((dup2(fd, fd_i)) == -1)
        {
            perror("Failed to redirect standard file descriptor");
            exit(1);
        }
    }

    //进程活动时，其工作目录所在的文件系统不能卸下(比如工作目录在一个NFS中,运行一个daemon会导致umount无法成功)
    //一般需要将工作目录改变到根目录。对于需要转储核心、写运行日志的进程将工作目录改变到特定目录如/tmp
    chdir("/"); //改变工作目录为根目录

    //进程从创建它的父进程那里继承了文件创建掩码，它可能修改守护进程所创建的文件的存取位。为防止这一点，将文件创建掩码清除
    umask(0); //清除文件权限掩码
}

//主函数————————————————————
int main()
{
    create_daemon(); //创建并启动守护进程

    int sock_fd;                  //套接字文件描述符
    struct sockaddr_in serv_addr; //服务端网络信息结构体
    struct sockaddr_in cli_addr;  //客户端网络信息结构体
    int addr_len;                 //网络信息结构体长度
    int conn_fd;                  //已建立连接的套接字文件描述符

    //创建套接字并获取套接字文件描述符
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Failed to create server's socket");
        return 1;
    }

    //初始化sockaddr结构体
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons((short)3333); //指定特定端口

    //绑定套接字与网络信息
    if ((bind(sock_fd, (struct sockaddr *)(&serv_addr), sizeof(serv_addr))) == -1)
    {
        perror("Failed to bind the socket to port");
        return 1;
    }

    //套接字设置被动监听状态
    if ((listen(sock_fd, 5)) == -1) //最大连接数五个
    {
        perror("Failed to configure listening");
        return 1;
    }

    //监听客户端请求    使用线程
    while (1)
    {
        //每请求定义
        pthread_t tid; //线程id
        int error;     //记录线程操作错误信息

        //与客户端建立连接
        addr_len = sizeof(struct sockaddr);
        if ((conn_fd = accept(sock_fd, (struct sockaddr *)(&cli_addr), &addr_len)) == -1)
        {
            perror("Failed to accept connection");
            return 1;
        }

        //创建线程
        if (error = pthread_create(&tid, NULL, handle, &conn_fd) == -1)
        {
            fprintf(stderr, "Failed to create thread: %s\n", strerror(error));
            break; //跳出循环
        }
    }

    //关闭套接字文件描述符
    close(conn_fd); //可能重复关闭
    close(sock_fd);

    return 0;
}