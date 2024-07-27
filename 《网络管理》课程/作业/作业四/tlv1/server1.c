//头文件————————————————————
#include <netinet/in.h> //sockaddr_in、htonl()、htons()
#include <sys/socket.h> //socket()、bind()、recvfrom()、sendto()
#include <stdio.h>      //perror()、snprintf()、popen()、fprintf()、fgets()、pclose()
#include <stdlib.h>     //exit()、atoi()
#include <string.h>     //memset()、bzero()、strncpy()、strlen()、memcpy()
#include <unistd.h>     //close()

//宏定义————————————————————
#define SERV_PORT 8888 //服务端端口
#define BUFF_SIZE 256  //发送接收缓冲区大小
#define VALUE_SIZE 128 //值大小<发送接收缓冲区大小

//结构体————————————————————
//对象标识符
struct Tlv
{
    unsigned char tag;      //标签    1B够用
    unsigned char length;   //长度    1B能表示128B值长度够用
    char value[VALUE_SIZE]; //值    128B确定格式，预先定义再填充
} Tlv;

//函数声明————————————————————
void handle(int sock_fd, struct sockaddr *clie_addr);
void hostname_response(int sock_fd, struct sockaddr *clie_addr);
void loadavg_response(int sock_fd, struct sockaddr *clie_addr);

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
    //强制类型转换为通用套接字结构体
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

    char recv_buff[BUFF_SIZE];            //接收缓冲区
    bzero(&recv_buff, sizeof(recv_buff)); //清空

    //接收请求消息
    //注意长度是int*
    // if ((recvfrom(sock_fd, recv_buff, BUFF_SIZE, 0, clie_addr, &addr_size)) == -1) //接收消息
    // {
    //     perror("Failed to receive message");
    //     exit(1);
    // }

    //阻塞循环接收
    while ((recvfrom(sock_fd, recv_buff, BUFF_SIZE, 0, clie_addr, &addr_size)) > 0) //接收消息
    {
        struct Tlv *oid;               //对象标识符
        oid = (struct Tlv *)recv_buff; //数据从接收缓冲区拷出  结构体

        //确定对象标识符
        //获取长度
        // char char_length[VALUE_SIZE]; //字符型长度
        // snprintf(char_length, VALUE_SIZE, "%x", oid->length);
        // int int_length; //整型长度
        // int_length = atoi(char_length);

        if (oid->tag == 0x06) //标签是对象标识符    可用十六进制数比较
        {
            if ((oid->value[0] == 0x29) && (oid->value[1] == 0x01)) //"1.1.1"请求主机命
            {
                hostname_response(sock_fd, clie_addr); //取主机名
            }
            else if ((oid->value[0] == 0x2a) && (oid->value[1] == 0x01)) //"1.2.1"请求平均负载
            {
                loadavg_response(sock_fd, clie_addr); //取主机名
            }
        }
    }

    return;
}

//取主机名
void hostname_response(int sock_fd, struct sockaddr *clie_addr)
{
    int addr_size; //网络信息结构体大小
    addr_size = sizeof(struct sockaddr);

    char result_buff[BUFF_SIZE];              //结果缓冲区
    char send_buff[BUFF_SIZE];                //发送缓冲区
    bzero(&result_buff, sizeof(result_buff)); //清空
    bzero(&send_buff, sizeof(send_buff));

    FILE *fp = NULL; //文件指针
    if ((fp = popen("hostname", "r")) == NULL)
    {
        fprintf(stderr, "Failed to execute 'hostname' command\n");
        return;
    }
    //创建一个管道，创建一个进程，执行shell命令，使用读取文件方式获得输出
    //避免创建临时文件，不受输出字符数的限制

    //取内容    注意包括换行符
    if ((fgets(result_buff, BUFF_SIZE, fp)) == NULL)
    {
        fprintf(stderr, "Failed to get hostname\n");
        return;
    }

    pclose(fp); //关闭

    //构造消息
    struct Tlv ia5_String;

    ia5_String.tag = 0x16;    //标签 IA5String，universal22  "00 0 10110"="16"
    ia5_String.length = 0x20; //长度 确定格式，预留32B    "0010 0000"="20"
    bzero(&(ia5_String.value), sizeof(ia5_String.value));
    strncpy(ia5_String.value, result_buff, strlen(result_buff));

    memcpy(send_buff, (unsigned char *)&ia5_String, sizeof(Tlv)); //数据拷入发送缓冲区  结构体

    //注意长度是int
    if ((sendto(sock_fd, &send_buff, BUFF_SIZE, 0, clie_addr, addr_size)) == -1) //发送消息
    {
        perror("Failed to send hostname");
        return;
    }
}

//取主机名
void loadavg_response(int sock_fd, struct sockaddr *clie_addr)
{
    int addr_size; //网络信息结构体大小
    addr_size = sizeof(struct sockaddr);

    char result_buff[BUFF_SIZE];              //结果缓冲区
    char send_buff[BUFF_SIZE];                //发送缓冲区
    bzero(&result_buff, sizeof(result_buff)); //清空
    bzero(&send_buff, sizeof(send_buff));

    FILE *fp = NULL; //文件指针
    if ((fp = popen("more /proc/loadavg", "r")) == NULL)
    {
        fprintf(stderr, "Failed to execute 'more /proc/loadavg' command\n");
        return;
    }
    //创建一个管道，创建一个进程，执行shell命令，使用读取文件方式获得输出
    //避免创建临时文件，不受输出字符数的限制

    //取内容    注意包括换行符
    if ((fgets(result_buff, BUFF_SIZE, fp)) == NULL)
    {
        fprintf(stderr, "Failed to get loadavg\n");
        return;
    }

    pclose(fp); //关闭

    //构造消息
    struct Tlv ia5_String;

    ia5_String.tag = 0x16;    //标签 IA5String，universal22  "00 0 10110"="16"
    ia5_String.length = 0x20; //长度 确定格式，预留32B    "0010 0000"="20"
    bzero(&(ia5_String.value), sizeof(ia5_String.value));
    strncpy(ia5_String.value, result_buff, strlen(result_buff));

    memcpy(send_buff, (unsigned char *)&ia5_String, sizeof(Tlv)); //数据拷入发送缓冲区  结构体

    //注意长度是int
    if ((sendto(sock_fd, &send_buff, BUFF_SIZE, 0, clie_addr, addr_size)) == -1) //发送消息
    {
        perror("Failed to send hostname");
        return;
    }
}