//头文件————————————————————
#include <sys/socket.h> //socket()、connect()、select()、read()、write()
#include <stdio.h>      //perror()
#include <stdlib.h>     //exit()
#include <netinet/in.h> //sockaddr_in、htonl()、htons()
#include <string.h>     //bzero()、strstr()
#include <unistd.h>     //close()
// #include <sys/select.h>
// #include <sys/time.h>

//宏————————————————————
#define SERV_PORT 3333 //服务端端口
#define BUFF_SIZE 32   //缓冲区大小

//全局变量————————————————————
int quit_flag = 0; //退出标志
//当发送QUIT命令请求时，标志从0置1。当接收quit内容响应时，标志为1则客户端主动关闭连接    双重判断
//在服务端未使用多进程、线程技术时，客户端主动关闭连接，服务端会退出程序—————解决

//函数声明————————————————————
//过程函数
void handle(int sock_fd); //处理

//辅助函数
int get_max_fd(int fd1, int fd2); //获取较大的文件描述符

//主函数————————————————————
int main(int argc, char *argv[])
{
    int sock_fd; //套接字文件描述符

    //创建套接字并获取套接字文件描述符
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Failed to create the client's socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serv_addr; //服务端网络信息结构体

    //初始化服务端网络信息结构体
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; // 或htonl(192.168.137.2)
    serv_addr.sin_port = htons(SERV_PORT);

    //与服务端建立连接
    if ((connect(sock_fd, (struct sockaddr *)(&serv_addr), sizeof(serv_addr))) == -1)
    {
        close(sock_fd);

        perror("Failed to establish connection");
        exit(EXIT_FAILURE);
    }

    handle(sock_fd); //处理

    //关闭套接字文件描述符
    close(sock_fd);

    return 0;
}

//函数定义————————————————————
//处理
void handle(int sock_fd)
{
    int max_fd;  //最大文件描述符
    int max_fd1; //最大文件描述符(值)+1

    fd_set read_fd_set;    //读文件描述符集
    FD_ZERO(&read_fd_set); //清空文件描述符集

    int select_result; // select()调用结果

    int n_bytes; //读或写套接字的字节数

    char read_buff[BUFF_SIZE];  //接收服务端缓冲区
    char write_buff[BUFF_SIZE]; //发送服务端缓冲区

    bzero(read_buff, sizeof(read_buff)); //清空缓冲区
    bzero(write_buff, sizeof(write_buff));

    // int quit_flag = 0; //退出标志
    //定义的同时初始化、定义后赋值，程序运行也会失败，真神奇  可以使用全局变量

    //使用select()监听读写文件描述符
    while (1)
    {
        //获取最大文件描述符(值)+1
        //两个文件描述符，比较一次
        //实际上，标准输入文件描述符为0，最大文件描述符为套接字文件描述符
        max_fd = get_max_fd(STDIN_FILENO, sock_fd);
        max_fd1++;

        //设置文件描述符集
        FD_SET(STDIN_FILENO, &read_fd_set); //标准输入可读
        FD_SET(sock_fd, &read_fd_set);      //套接字可读

        select_result = select(max_fd1, &read_fd_set, NULL, NULL, NULL); //监听
        // if (select_result == -1)
        // {
        //     perror("Failed to execute the select()");
        //     exit(EXIT_FAILURE);
        // }
        //常报错：Invalid argument

        //注意时序:先判断套接字可读,标准输入随时可读
        if (FD_ISSET(sock_fd, &read_fd_set)) //套接字可读
        {
            n_bytes = read(sock_fd, read_buff, BUFF_SIZE); //接收数据

            if (n_bytes > 0)
            {
                printf("%s\n", read_buff); //发送标准输出

                if (strstr(read_buff, "quit") != NULL) //是接收quit内容响应
                {
                    if (quit_flag == 1)
                    {
                        return; //客户端主动关闭连接
                    }
                }

                bzero(read_buff, sizeof(read_buff)); //清空接收缓冲区
            }
            else if (n_bytes == 0)
            {
                perror("The server terminated prematurely");
                exit(EXIT_FAILURE);
            }
            else //-1
            {
                perror("Failed to read from the socket");
                exit(EXIT_FAILURE);
            }
            // read()返回值:
            //>0:等于参数中所指定的读出字节数
            //>0:小于参数中所指定的读出字节数
            //可能原因:
            //已经读出部分数据，但遇到了文件末尾EOF/Ctrl+D，返回文件末尾前已经读出的字节数
            //已经读出部分数据，但是被中断信号中断，返回中断前读出的字节数
            //=0:对端关闭套接字或对端关闭套接字的写半部分
            //<0:出错
        }

        if (FD_ISSET(STDIN_FILENO, &read_fd_set)) //标准输入可读
        {
            scanf("%s", write_buff); //接收标准输入

            if (strstr(write_buff, "QUIT:") != NULL) //是发送QUIT命令请求
            {
                if (quit_flag == 0)
                {
                    quit_flag = 1;
                }
            }

            n_bytes = write(sock_fd, write_buff, BUFF_SIZE); //发送数据

            if (n_bytes >= 0)
            {
                bzero(write_buff, sizeof(write_buff)); //清空发送缓冲区
            }
            else //-1
            {
                perror("Failed to write to the socket");
                exit(EXIT_FAILURE);
            }
            // write()返回值:
            //>0:等于参数中所指定的写入字节数
            //>0:小于参数中所指定的写入字节数
            //可能原因:
            //底层物理介质没有足够的空间
            //创建的文件指定了RLIMIT_FSIZE，即指定了文件允许的最大字节数，不能再往其中添加数据
            //已经写入部分数据，但是被中断信号中断，返回中断前写入的字节数
            //=0:等于参数中所指定的写入字节数,即参数中所指定的写入字节数为0
            //<0:出错
        }
    }

    return;
}

//辅助函数
//获取较大的文件描述符
int get_max_fd(int fd1, int fd2)
{
    if (fd1 >= fd2)
    {
        return fd1;
    }
    else
    {
        return fd2;
    }
}