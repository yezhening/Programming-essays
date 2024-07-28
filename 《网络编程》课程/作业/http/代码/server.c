//头文件————————————————————
#include <netinet/in.h> //sockaddr_in、htonl()、htons()
#include <sys/socket.h>
// socket()、AF_INET、SOCK_STREAM、bind()、listen()、accept()、recv()、send()
#include <stdio.h>  //perror()
#include <stdlib.h> //exit()、EXIT_FAILURE、EXIT_SUCCESS
#include <string.h> //bzero()、strncpy()、strlen()、strncat()
#include <unistd.h> //close()、fork()、getpid()
#include <errno.h>  //errno

//宏————————————————————
#define SERV_PORT 3221
#define LISTENMAXCOUNT 5 //所监听的最大连接数
#define BUFF_SIZE 512    //传递消息缓冲区大小

//函数声明————————————————————
void handle_request(int conn_fd);                        //处理请求
void assemble_res_msg(char msg_recv[], char msg_send[]); //组装响应报文

//主函数————————————————————
int main(int arg, char *argv[])
{
    int sock_fd;                  //套接字文件描述符
    struct sockaddr_in serv_addr; //服务端网络信息结构体
    struct sockaddr_in cli_addr;  //客户端网络信息结构体
    int addr_size;                //网络信息结构体大小
    int conn_fd;                  //已建立连接的套接字文件描述符
    pid_t pid;                    //进程号

    //创建套接字,获取套接字文件描述符
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Failed to create server's socket: ");
        exit(EXIT_FAILURE);
    }

    //初始化sockaddr结构体  对服务端
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERV_PORT);

    bzero(&cli_addr, sizeof(cli_addr)); //对客户端

    //绑定套接字与网络信息
    if ((bind(sock_fd, (struct sockaddr *)(&serv_addr), sizeof(serv_addr))) == -1)
    {
        close(sock_fd);
        perror("Failed to bind the socket: ");
        exit(EXIT_FAILURE);
    }

    //套接字设置被动监听状态
    if ((listen(sock_fd, LISTENMAXCOUNT)) == -1)
    {
        close(sock_fd);
        perror("Failed to configure listening: ");
        exit(EXIT_FAILURE);
    }

    addr_size = sizeof(struct sockaddr);

    //循环监听客户端请求
    while (1)
    {
        //与客户端建立连接
        if ((conn_fd = accept(sock_fd, (struct sockaddr *)(&cli_addr), &addr_size)) == -1)
        {
            perror("Failed to accept connection: ");
            continue; //继续监听
        }

        //创建子进程处理请求
        if ((pid = fork()) == 0) //子进程
        {
            close(sock_fd);

            handle_request(conn_fd);

            exit(EXIT_SUCCESS);
        }
    }

    //关闭套接字文件描述符
    close(sock_fd);

    return 0;
}

//函数定义————————————————————
//处理请求
void handle_request(int conn_fd)
{
    char msg_recv[BUFF_SIZE]; //接收客户端消息缓冲区
    char msg_send[BUFF_SIZE]; //发送客户端消息缓冲区

    int recv_bytes; //接收的消息字节数

    while (1) //循环接收数据————长连接
    {
        bzero(&msg_recv, sizeof(*msg_recv));
        bzero(&msg_send, sizeof(*msg_send));

        recv_bytes = recv(conn_fd, msg_recv, BUFF_SIZE, 0);

        // printf("%d\n", recv_bytes); //返回值始终是BUFF_SIZE
        if (recv_bytes > 0) //有数据
        {
            assemble_res_msg(msg_recv, msg_send); //组装响应报文

            if ((send(conn_fd, msg_send, BUFF_SIZE, 0)) == -1)
            {
                perror("Failed to send messages to the client: ");
                return; //函数返回后正常结束子进程
            }
        }
        else if (recv_bytes == 0) //文件末尾EOF，在客户端标准输入Ctrl+D
        {
            printf("The process %d received the end of the file\n", getpid());
            return;
        }
        else if ((recv_bytes == -1) && (errno == EINTR)) //信号或网络中断recv()
        {
            continue; //继续接收数据
        }
        else if (recv_bytes == -1) //其他错误
        {
            perror("Failed to receive messages from the client: ");
            return;
        }
    }

    return;
}

//组装响应报文
void assemble_res_msg(char msg_recv[], char msg_send[])
{
    //取请求报文url 可扩展：根据url请求内容
    int blank_flag = 0; //空格符标志 0无1有
    char url[50];       //统一资源定位系统 大小写死
    int url_i = 0;      // url数组下标计数

    for (int i = 0; i < strlen(msg_recv); i++)
    {
        if ((msg_recv[i] == ' ') && (blank_flag == 0)) //判定标志位
        {
            blank_flag = 1;
            continue;
        }
        else if ((msg_recv[i] == ' ') && (blank_flag == 1))
        {
            url[url_i] = '\0'; //必须
            blank_flag = 0;
            break; //退出循环
        }

        if (blank_flag == 1) //取
        {
            url[url_i] = msg_recv[i];
            url_i++;
        }
    }

    // strncpy(msg_send, url, strlen(url));
    // msg_send[strlen(url)] = '\0'; //必须

    //准备内容  都写死
    char status_line[] = "HTTP/1.1 200 OK"; //状态行   1

    //头部字段键值
    char date[] = "Date: Mon, 27 Jul 2009 12:28:53 GMT"; //日期  2
    char content_type[] = "Content-Type: text/plain";    //内容类型 3
    char content_length[] = "Content-Length: 51";        //内容长度 4

    char response_body[]="This is contents"; //响应体  相当于回射  5

    //组装
    strncat(msg_send, status_line, strlen(status_line)); // 1
    strncat(msg_send, "\n", 2);

    strncat(msg_send, date, strlen(date)); // 2
    strncat(msg_send, "\n", 2);
    strncat(msg_send, content_type, strlen(content_type)); // 3
    strncat(msg_send, "\n", 2);
    strncat(msg_send, content_length, strlen(content_length)); // 4
    strncat(msg_send, "\n", 2);

    strncat(msg_send, "\n", 2);

    strncat(msg_send, response_body, strlen(response_body)); // 5
    strncat(msg_send, "\0", 2);                             //必须

    return;
}