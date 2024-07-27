#include <sys/socket.h> //socket()、bind()、sendto()
#include <netinet/in.h> //sockaddr_in、htonl()、htons()
#include <stdio.h>      //perror()
#include <stdlib.h>     //exit()、system()
#include <string.h>     //memset()、bzero()、memcpy()
#include <unistd.h>     //close()
#include <fcntl.h>      //open()、O_WRONLY

// #include <sys/types.h>
// #include <arpa/inet.h> //inet_addr()

//宏定义————————————————————
#define SERV_PORT 8888 //服务端端口
#define BUFF_SIZE 256  //接收发送缓冲区大小
#define MSG_SIZE 128   //消息大小
#define VALUE_SIZE 32  //值大小

//函数声明————————————————————
void handle(int sock_fd, struct sockaddr *clie_addr);
void create_response(char message[]);

//主函数————————————————————
int main(int argc, char *argv[])
{
    int sock_fd;                  //套接字文件描述符
    struct sockaddr_in serv_addr; //服务端网络信息结构体
    struct sockaddr_in clie_addr; //客户端网络信息结构体

    //创建套接字,获取套接字文件描述符
    if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("Failed to create the server's socket");
        exit(1);
    }

    //初始化sockaddr_in结构体
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERV_PORT);

    //绑定套接字与网络信息
    if ((bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) == -1)
    {
        perror("Failed to bind the socket to port");
        exit(1);
    }

    //操作
    handle(sock_fd, (struct sockaddr *)&clie_addr);
    //传客户端网络信息结构体    因为一个服务端可对应多个
    // revcvfrom()、sendto()需要

    //关闭套接字文件描述符
    if ((close(sock_fd)) == -1)
    {
        perror("Failed to close the server's socket");
        exit(1);
    }

    return 0;
}

//函数定义————————————————————
void handle(int sock_fd, struct sockaddr *clie_addr)
{
    int addr_size; //网络信息结构体大小
    addr_size = sizeof(struct sockaddr);

    char recv_buff[BUFF_SIZE]; //接收缓冲区
    char send_buff[BUFF_SIZE]; //发送缓冲区

    bzero(&recv_buff, sizeof(recv_buff)); //清空
    bzero(&send_buff, sizeof(send_buff));

    //长度是int*
    if ((recvfrom(sock_fd, recv_buff, BUFF_SIZE, 0, clie_addr, &addr_size)) == -1) //接收消息
    {
        perror("Failed to receive message");
        exit(1);
    }

    //获取响应消息
    char message[MSG_SIZE];                //消息
    memset(message, ',', sizeof(message)); //置','作占位符、分隔符

    memcpy(message, recv_buff, sizeof(recv_buff)); //接收缓冲区数据拷贝为消息作进一步处理
    bzero(&recv_buff, sizeof(recv_buff));          //清空

    create_response(message);

    memcpy(send_buff, message, sizeof(message)); //数据拷入发送缓冲区

    //长度是int
    if ((sendto(sock_fd, &send_buff, BUFF_SIZE, 0, clie_addr, addr_size)) == -1) //发送消息
    {
        perror("Failed to send message");
        exit(1);
    }

    return;
}

//创建响应消息
void create_response(char message[])
{
    //内容
    char msg_type = '1';    //消息类型    0请求1响应
    char value[VALUE_SIZE]; //值
    bzero(&value, sizeof(value));

    int fd;             //文件描述符
    int byte_count = 0; //一次read()读取的字节数

    //构造消息
    int msg_pos = 0; //赋值的位置

    message[0] = msg_type; //消息类型
    msg_pos = msg_pos + 2; //跳过分隔符

    //跳键
    for (msg_pos; msg_pos < strlen(message); msg_pos++)
    {
        if (message[msg_pos] == ',')
        {
            break;
        }
    }
    msg_pos = msg_pos + 1; //跳过分隔符

    //取值
    if ((system("hostname > temp.txt")) == -1) //将标准输出重定向到临时文件再获取   注意有换行符
    {
        perror("Failed to execute 'system' function'");
        return;
    }

    if ((fd = open("./temp.txt", O_RDONLY)) == -1) //只写
    {
        perror("Failed to open file\n");
        return;
    }

    if ((byte_count = read(fd, value, VALUE_SIZE)) < 0) //读文件
    {
        close(fd);
        perror("Failed to read file\n");
        return;
    }

    for (int i = 0; i < strlen(value) - 1; i++) //不用换行符
    {
        message[msg_pos] = value[i];
        msg_pos++;
    }

    //将预留值替换为'+',和分隔符作区分
    for (int i = 0; i < (VALUE_SIZE - (strlen(value) - 1)); i++)
    {
        message[msg_pos] = '+';
        msg_pos++;
    }
    msg_pos = msg_pos + 1; //跳过分隔符

    //跳键
    for (msg_pos; msg_pos < strlen(message); msg_pos++)
    {
        if (message[msg_pos] == ',')
        {
            break;
        }
    }
    msg_pos = msg_pos + 1; //跳过分隔符

    //将标准输出重定向到临时文件再获取
    //以绝对路径执行命令
    if ((system("more /proc/loadavg > temp.txt")) == -1)
    {
        perror("Failed to execute 'system' function'");
    }

    if ((fd = open("./temp.txt", O_RDONLY)) == -1) //只写
    {
        perror("Failed to open file\n");
        return;
    }

    if ((byte_count = read(fd, value, VALUE_SIZE)) < 0) //读文件
    {
        close(fd);
        perror("Failed to read file\n");
        return;
    }

    for (int i = 0; i < strlen(value) - 1; i++)
    {
        message[msg_pos] = value[i];
        msg_pos++;
    }

    return;
}
