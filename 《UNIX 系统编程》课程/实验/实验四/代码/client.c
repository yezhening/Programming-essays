//头文件————————————————————
#include <sys/socket.h> //网络编程相关
#include <stdio.h>      //perror()、gets()
#include <netinet/in.h> //sockaddr_in、htonl()、htons()
#include <unistd.h>     //close()
#include <string.h>     //memset()

//宏
#define BUFSIZE 1024 //传递消息缓冲区大小

//主函数————————————————————
int main()
{
    int sock_fd;                  //套接字文件描述符
    struct sockaddr_in serv_addr; //服务端网络信息结构体
    char msg_send[BUFSIZE];       //接收服务端消息缓冲区
    char msg_recv[BUFSIZE];       //发送服务端消息缓冲区

    //创建套接字并获取套接字文件描述符
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Failed to create client's socket");
        return -1;
    }

    //初始化sockaddr结构体
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("192.168.154.128"); 
    //服务器的具体IP地址 将一个点分十进制的IP转换成一个长整数型数
    //必须指定
    serv_addr.sin_port = htons((short)3333);

    //与服务端建立连接
    if ((connect(sock_fd, (struct sockaddr *)(&serv_addr), sizeof(serv_addr))) == -1)
    {
        perror("Failed to establish connection");
        return 1;
    }

    //向服务器发送请求
    while (1)
    {
        gets(msg_send);

        if ((write(sock_fd, msg_send, BUFSIZE)) == -1)
        {
            perror("Failed to send messages to the server");
            return 1;
        }
        if (strcmp(msg_send, "quit") == 0) //关闭客户端
        {
            break;
        }
        else
        {
            if ((read(sock_fd, msg_recv, BUFSIZE)) == -1)
            {
                perror("Failed to receive messages from the server");
                return 1;
            }

            printf("server's messages：%s\n", msg_recv);

            //清空缓冲区等待下一次输入
            memset(msg_send, 0, BUFSIZE);
            memset(msg_recv, 0, BUFSIZE);
        }
    }

    //关闭套接字
    close(sock_fd);

    return 0;
}