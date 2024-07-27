//头文件————————————————————
#include <netinet/in.h> //sockaddr_in、htonl()、htons()
#include <sys/socket.h> //socket()
#include <stdio.h>      //perror()
#include <stdlib.h>     //exit()
#include <string.h>     //memset()、bzero()、memcpy()
#include <unistd.h>     //close()、write()
#include <sys/stat.h>   //umask()
#include <fcntl.h>      //open()、O_WRONLY、O_CREAT、O_APPEND

//宏定义————————————————————
#define SERV_PORT 8888 //服务端端口
#define BUFF_SIZE 256  //发送接收缓冲区大小
#define VALUE_SIZE 128 //值大小<发送接收缓冲区大小

//结构体————————————————————
// TLV表示的BER编码
struct Tlv
{
    unsigned char tag;      //标签    1B够用
    unsigned char length;   //长度    1B能表示128B值长度够用
    char value[VALUE_SIZE]; //值    128B确定格式，预先定义再填充
} Tlv;

//函数声明————————————————————
void handle(int sock_fd, struct sockaddr *serv_addr);

void hostname_request(int sock_fd, struct sockaddr *serv_addr);
void loadavg_request(int sock_fd, struct sockaddr *serv_addr);

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
    //传服务端网络信息结构体，因为一个客户端只对应一个
    //强制类型转换为通用套接字结构体
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
    hostname_request(sock_fd, serv_addr); //请求主机名
    loadavg_request(sock_fd, serv_addr);  //请求平均负载

    return;
}

// 请求主机名
void hostname_request(int sock_fd, struct sockaddr *serv_addr)
{
    //创建请求消息
    struct Tlv oid; //对象标识符

    // //构造对象标识符
    oid.tag = 0x06;    //标签 OBJECT IDENTIFIER，Universal6  "00 0 00110"="06"
    oid.length = 0x02; //长度 确定格式，2B    "0000 0010"="02"
    bzero(&(oid.value), sizeof(oid.value));
    oid.value[0] = 0x29;
    oid.value[1] = 0x01;
    //值   "1.1.1"
    // 1B    40*1+1=41="0010 1001"="29"
    // 1B    0*128+1="0000 0001"="01"

    int addr_size; //网络信息结构体大小
    addr_size = sizeof(struct sockaddr);

    char send_buff[BUFF_SIZE]; //发送缓冲区
    char recv_buff[BUFF_SIZE]; //接收缓冲区

    bzero(&send_buff, sizeof(send_buff)); //清空
    bzero(&recv_buff, sizeof(recv_buff));

    memcpy(send_buff, (unsigned char *)&oid, sizeof(Tlv)); //数据拷入发送缓冲区  结构体

    //注意长度是int
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

// 请求平均负载
void loadavg_request(int sock_fd, struct sockaddr *serv_addr)
{
    //创建请求消息
    struct Tlv oid; //对象标识符

    // //构造对象标识符
    oid.tag = 0x06;    //标签 OBJECT IDENTIFIER，Universal6  "00 0 00110"="06"
    oid.length = 0x02; //长度 确定格式，2B    "0000 0010"="02"
    bzero(&(oid.value), sizeof(oid.value));
    oid.value[0] = 0x2a;
    oid.value[1] = 0x01;
    //值   "1.2.1"
    // 1B    40*1+2=42="0010 1010"="2a"
    // 1B    0*128+1="0000 0001"="01"

    int addr_size; //网络信息结构体大小
    addr_size = sizeof(struct sockaddr);

    char send_buff[BUFF_SIZE]; //发送缓冲区
    char recv_buff[BUFF_SIZE]; //接收缓冲区

    bzero(&send_buff, sizeof(send_buff)); //清空
    bzero(&recv_buff, sizeof(recv_buff));

    memcpy(send_buff, (unsigned char *)&oid, sizeof(Tlv)); //数据拷入发送缓冲区  结构体

    //注意长度是int
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

//处理响应
void handle_response(char recv_buff[])
{
    struct Tlv *ia5_String;               // IA5String
    ia5_String = (struct Tlv *)recv_buff; //数据从接收缓冲区拷出  结构体

    //保存文件
    int fd;                             //文件描述符
    char file_path[] = "./result1.txt"; //文件路径

    umask(0000);
    //临时设置0000，实际创建的文件权限即为mode值    否则创建文件仍只有用户可读写权限
    if ((fd = open(file_path, O_WRONLY | O_CREAT | O_APPEND, S_IRWXU | S_IRWXG | S_IRWXO)) < 0) //打开文件
    //只写    若文件不存在新建文件，存在附加
    //文件用户、用户组和其他用户具有可读、可写及可执行权限  00777
    {
        perror("Failed to open the result1 file\n");
        return;
    }

    if (write(fd, ia5_String->value, strlen(ia5_String->value)) < 0) //写文件
    {
        close(fd);
        perror("Failed to write the result1 file\n");
        return;
    }

    return;
}