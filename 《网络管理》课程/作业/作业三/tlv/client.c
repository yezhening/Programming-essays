#include <sys/socket.h> //socket()、sendto()、recvfrom()
#include <netinet/in.h> //sockaddr_in、htonl()、htons()
#include <stdio.h>      //perror()
#include <stdlib.h>     //exit()
#include <string.h>     //memset()、bzero()、memcpy()、strlen()
#include <unistd.h>     //close()
#include <sys/stat.h>   //umask()
#include <fcntl.h>      //open()、O_WRONLY 、 O_CREAT 、 O_TRUNC

// #include <sys/types.h>
// #include <arpa/inet.h> //inet_addr()

//宏定义————————————————————
#define SERV_PORT 8888 //服务端端口
#define BUFF_SIZE 256  //发送接收缓冲区大小
#define MSG_SIZE 128   //消息大小
#define VALUE_SIZE 32  //值大小

//函数声明————————————————————
void handle(int sock_fd, struct sockaddr *serv_addr);
void create_request(char message[]);
void handle_response(char recv_buff[]);

//主函数————————————————————
int main(int argc, char *argv[])
{
    int sock_fd;                  //套接字文件描述符
    struct sockaddr_in serv_addr; //服务端网络信息结构体

    //创建套接字,获取套接字文件描述符
    if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("Failed to create the client's socket");
        exit(1);
    }

    //初始化sockaddr_in结构体
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //可用
    serv_addr.sin_port = htons(SERV_PORT);

    //操作
    handle(sock_fd, (struct sockaddr *)&serv_addr);
    //传服务端网络信息结构体 因为一个客户端只对应一个
    // sendto()、recvfrom()需要

    //关闭套接字文件描述符
    if ((close(sock_fd)) == -1)
    {
        perror("Failed to close the client's socket");
        exit(1);
    }

    return 0;
}

//函数定义————————————————————
//操作
void handle(int sock_fd, struct sockaddr *serv_addr)
{
    int addr_size; //网络信息结构体大小
    addr_size = sizeof(struct sockaddr);

    char send_buff[BUFF_SIZE]; //发送缓冲区
    char recv_buff[BUFF_SIZE]; //接收缓冲区

    bzero(&send_buff, sizeof(send_buff)); //清空
    bzero(&recv_buff, sizeof(recv_buff));

    //获取请求消息
    char message[MSG_SIZE];                //消息
    memset(message, ',', sizeof(message)); //置','作占位符、分隔符

    create_request(message);

    memcpy(send_buff, message, sizeof(message)); //数据拷入发送缓冲区

    //长度是int
    if ((sendto(sock_fd, &send_buff, BUFF_SIZE, 0, serv_addr, addr_size)) == -1) //发送消息
    {
        perror("Failed to send message");
        exit(1);
    }

    //长度是int *
    if ((recvfrom(sock_fd, recv_buff, BUFF_SIZE, 0, serv_addr, &addr_size)) == -1) //接收消息
    {
        perror("Failed to receive message");
        exit(1);
    }

    //处理响应
    handle_response(recv_buff);

    return;
}

//创建请求消息
void create_request(char message[])
{
    //内容
    char msg_type = '0';   //消息类型    0请求1响应
    char oid1[] = "1.1.1"; //对象标识符1
    char oid2[] = "1.2.1";

    //构造消息
    int msg_pos = 0; //赋值的位置

    message[0] = msg_type; //消息类型
    msg_pos = msg_pos + 2; //预留分隔符

    //对象标识符1
    for (int i = 0; i < strlen(oid1); i++)
    {
        message[msg_pos] = oid1[i];
        msg_pos++;
    }
    msg_pos = msg_pos + 1;

    //预留值1
    msg_pos = msg_pos + VALUE_SIZE;
    msg_pos = msg_pos + 1;

    // 2
    for (int i = 0; i < strlen(oid2); i++)
    {
        message[msg_pos] = oid2[i];
        msg_pos++;
    }
    msg_pos = msg_pos + 1;

    msg_pos = msg_pos + VALUE_SIZE;
    msg_pos = msg_pos + 1;

    return;
}

//处理响应
void handle_response(char recv_buff[])
{
    // printf("%s\n", recv_buff);

    //取值
    int delim_count = 0;     //定界符计数delimeter
    int get_value_flag = 0;  //取值标记 0无1第一个值
    char value1[VALUE_SIZE]; //值
    char value2[VALUE_SIZE];
    int value_count = 0; //值字符数组下标

    //拆
    for (int i = 0; i < strlen(recv_buff); i++)
    {
        if (recv_buff[i] == ',') //定界符判断取值
        {
            delim_count++;
            continue;
        }

        if (delim_count == 2)
        {
            get_value_flag = 1;
        }

        if (recv_buff[i] == '+') //预留值不需要 在赋值前判断
        {
            get_value_flag = 0; //恢复
            value_count = 0;    //清零
            continue;
        }

        if (get_value_flag == 1)
        {
            value1[value_count] = recv_buff[i];
            value_count = value_count + 1;
        }

        if (delim_count == 4)
        {
            get_value_flag = 2;
        }

        if (get_value_flag == 2)
        {
            value2[value_count] = recv_buff[i];
            value_count = value_count + 1;
        }

        if (delim_count == 5)
        {
            break;
        }
    }

    // printf("%s\n%s\n", value1, value2);

    //保存文件
    int fd;                            //文件描述符
    char file_path[] = "./result.txt"; //文件路径
    char newline_symbol = '\n';              //换行符以便写文件

    umask(0000);
    //临时设置0000，实际创建的文件权限即为mode值    否则创建文件仍只有用户可读写权限
    if ((fd = open(file_path, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO)) < 0) //打开文件
    //"download b.txt"
    //只写    若文件不存在新建文件，存在清空
    //文件用户、用户组和其他用户具有可读、可写及可执行权限  00777
    {
        perror("Failed to open thhe result file\n");
        return;
    }

    if (write(fd, value1, strlen(value1)) < 0) //写文件
    {
        close(fd);
        perror("Failed to write the result file\n");
        return;
    }

    if (write(fd, &newline_symbol, 1) < 0) //写文件
    {
        close(fd);
        perror("Failed to write the result file\n");
        return;
    }

    if (write(fd, value2, strlen(value2)) < 0) //写文件
    {
        close(fd);
        perror("Failed to write the result file\n");
        return;
    }

    //关闭文件描述符
    if ((close(fd)) == -1)
    {
        perror("Failed to close the result file\n");
    }
}