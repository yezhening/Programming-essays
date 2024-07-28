//头文件————————————————————
#include <netinet/in.h> //sockaddr_in、htonl()、htons()
#include <sys/socket.h> //socket()、AF_INET、SOCK_STREAM、connect()、send()、recv()
#include <stdio.h>      //perror()、printf()、fileno()、fprintf()、fflush()
#include <stdlib.h>     //exit()、EXIT_FAILURE
#include <string.h>     //bzero()、strncat()、strlen()
#include <unistd.h>     //close()、fgets()
// #include <sys/select.h> //select()

//宏
#define CLIE_PORT 3221
#define BUFF_SIZE 512 //传递消息缓冲区大小

//函数声明
void handle(int sock_fd);               //处理
void assemble_req_msg(char msg_send[]); //组装请求报文
void handle_response(char msg_recv[]);  //处理响应

//主函数————————————————————
int main(int argc, char *argv[])
{
    int sock_fd;                  //套接字文件描述符
    struct sockaddr_in serv_addr; //服务端网络信息结构体

    //创建套接字并获取套接字文件描述符
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Failed to create client's socket: ");
        exit(EXIT_FAILURE);
    }

    //初始化sockaddr结构体
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(CLIE_PORT);

    //与服务端建立连接
    if ((connect(sock_fd, (struct sockaddr *)(&serv_addr), sizeof(serv_addr))) == -1)
    {
        perror("Failed to establish connection: ");
        exit(EXIT_FAILURE);
    }

    handle(sock_fd); //处理

    close(sock_fd); //关闭套接字文件描述符

    return 0;
}

//函数定义
//处理
void handle(int sock_fd)
{
    char msg_send[BUFF_SIZE]; //发送服务端消息缓冲区
    char msg_recv[BUFF_SIZE]; //接收服务端消息缓冲区

    // printf("Please enter your request content: ");   //标准输出会被selcet()阻塞，不设置提示信息了
    // fgets(msg_send, BUFF_SIZE, stdin); //标准输入
    //注意：当服务端进程终止，客户端进程阻塞于fgets()调用：输出“Please enter your request content: ”并
    //等待输入  输入后接收到的信息错误，客户端不知服务端进程已终止

    //使用select()监听标准输入或套接字  循环：长连接
    int max_fd;            //最大文件描述符值+1
    fd_set read_fd_set;    //监听的可读描述符集
    FD_ZERO(&read_fd_set); //置空

    int recv_bytes; //接收到的字节数

    while (1)
    {
        bzero(&msg_recv, sizeof(*msg_recv));
        bzero(&msg_send, sizeof(*msg_send));

        FD_SET(STDIN_FILENO, &read_fd_set); //或fileno(stdin)    监听标准输入：标准输入有数据
        FD_SET(sock_fd, &read_fd_set);      //或fileno(stdin)
        //监听套接字：1.对端发送数据    2.对端进程终止（FIN报文，recv()返回0）  3.对端主机崩溃并重新启动（RST报文，recv()返回-1）

        if (STDIN_FILENO >= sock_fd)
        {
            max_fd = STDIN_FILENO + 1;
        }
        else
        {
            max_fd = sock_fd + 1;
        }

        select(max_fd, &read_fd_set, NULL, NULL, NULL); //监听

        if (FD_ISSET(STDIN_FILENO, &read_fd_set)) //标准输入可读
        {
            fflush(stdin); //清标准输入缓冲区

            if (fgets(msg_send, BUFF_SIZE, stdin) == NULL)
            //标准输入    错误或遇到文件结尾(EOF)：Linux是在客户端标准输入Ctrl+D
            {
                printf("Ending an HTTP Connection\n");
                return; //函数返回后关闭套接字，正常结束进程   相当于关闭连接
            }

            assemble_req_msg(msg_send); //组装请求报文

            //发送请求
            if ((send(sock_fd, msg_send, BUFF_SIZE, 0)) == -1)
            {
                perror("Failed to send messages to the server: ");
                exit(EXIT_FAILURE);
            }
        }

        if (FD_ISSET(sock_fd, &read_fd_set)) //套接字可读
        {
            recv_bytes = recv(sock_fd, msg_recv, BUFF_SIZE, 0);

            if (recv_bytes > 0)
            {
                printf("[The server's response message]\n");
                printf("%s\n", msg_recv);

                handle_response(msg_recv); //处理响应
            }
            else if (recv_bytes == 0) //服务端进程提前终止   在服务端标准输入Ctrl+C中断进程
            {
                // printf("%d", recv_bytes);
                fprintf(stderr, "Server terminated prematurely\n");
                exit(EXIT_FAILURE);
            }
            else if (recv_bytes == -1) //其他错误
            {
                perror("Failed to receive messages from the server: ");
                exit(EXIT_FAILURE);
            }
        }
    }

    return;
}

//组装请求报文  可扩展请求方法
void assemble_req_msg(char msg_send[])
{
    //准备内容  除url都写死
    char request_method[] = "GET"; //请求方法   1

    // printf("%ld", strlen(msg_send));//注意：包括换行符
    char url[strlen(msg_send)]; //统一资源定位系统 2
    strncpy(url, msg_send, strlen(msg_send));
    url[strlen(msg_send) - 1] = '\0'; //将换行符替换为结束符

    bzero(&msg_send, sizeof(*msg_send)); //置空，以重新组装 需解引用

    char protocol_version[] = "HTTP/1.1"; //协议版本 3

    //头部字段键值
    char host[] = "www.server.com";   //主机名   4
    char accept[] = "txt/plain";      //客户端希望接收的数据类型   5
    char accept_language[] = "en-us"; //客户端支持的语言   6
    char accept_encodeing[] = "gzip"; //客户端支持的压缩格式   7
    char user_agent[] = "Mozila/4.0"; //客户端/浏览器版本/信息   8
    char connection[] = "keep-alive"; //连接类型   长连接   9

    char request_body[] = ""; //请求体  GET方法为空 10

    //组装
    strncat(msg_send, request_method, strlen(request_method)); // 1
    strncat(msg_send, " ", 2);                                 //不拼接最后一个字节  不能等于源字符串长度    需给其+1
    strncat(msg_send, url, strlen(url));                       // 2
    strncat(msg_send, " ", 2);
    strncat(msg_send, protocol_version, strlen(protocol_version)); // 3
    strncat(msg_send, "\n", 2);

    strncat(msg_send, "Host: ", 7); // 4
    strncat(msg_send, host, strlen(host));
    strncat(msg_send, "\n", 2);

    strncat(msg_send, "Accept: ", 9); // 5
    strncat(msg_send, accept, strlen(accept));
    strncat(msg_send, "\n", 2);

    strncat(msg_send, "Accept-Language: ", 18); // 6
    strncat(msg_send, accept_language, strlen(accept_language));
    strncat(msg_send, "\n", 2);

    strncat(msg_send, "Accept-Encodeing: ", 19); // 7
    strncat(msg_send, accept_encodeing, strlen(accept_encodeing));
    strncat(msg_send, "\n", 2);

    strncat(msg_send, "User-Agent: ", 13); // 8
    strncat(msg_send, user_agent, strlen(user_agent));
    strncat(msg_send, "\n", 2);

    strncat(msg_send, "Connection: ", 13); // 9
    strncat(msg_send, connection, strlen(connection));
    strncat(msg_send, "\n", 2);

    strncat(msg_send, "\n", 2);

    strncat(msg_send, request_body, strlen(request_body));
    strncat(msg_send, "\0", 2); //必须

    printf("[Request message]\n");
    printf("%s", msg_send);

    return;
}

//处理响应
void handle_response(char msg_recv[])
{
    char content[BUFF_SIZE]; //响应内容
    int con_i = 0;    // 响应内容数组下标计数

    char newline_flag = 0; //换行符标记 0无1有两个

    for (int i = 0; i < strlen(msg_recv); i++)
    {
        if ((msg_recv[i] == '\n') && (msg_recv[i - 1] == '\n')) //判定标记位    换行符的ASCII码
        {
            newline_flag = 1;
            continue;
        }

        if (newline_flag == 1) //取
        {
            content[con_i] = msg_recv[i];
            con_i++; //忘记了脑热
        }
    }
    content[strlen(content) - 1] = '\0'; //需-1

    printf("\n[The server's response content]\n");
    printf("%s\n\n", content);

    return;
}