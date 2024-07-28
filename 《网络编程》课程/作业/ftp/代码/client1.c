//头文件————————————————————
#include <sys/socket.h> //网络编程相关  socket()、AF_INET、 SOCK_STREAM、connect()、send()、recv()
#include <netinet/in.h> //sockaddr_in、htonl()、htons()
#include <stdio.h>
// fprintf()、printf()、fgets()、strlen()、stpncpy()、strncmp()、atoi()、fopen()、fseek()、ftell()、fclose()
#include <stdlib.h>    //exit()
#include <arpa/inet.h> //inet_addr()
#include <string.h>    //bzero()
#include <unistd.h>    //close()、write()、read()
#include <fcntl.h>     //open()、O_WRONLY 、 O_CREAT 、 O_TRUNC
#include <sys/types.h>
#include <sys/stat.h> //umask()

//宏————————————————————
#define COMMANDSIZE 256        //操作命令大小
#define BUFFERSIZE 256         //传递消息缓冲区大小    两者一致
#define CONTROLPORT 3321       //控制端口
#define DATASPECIFIEDPORT 3320 //数据指定端口
#define DATARANDOMPORT 3322    //数据随机端口

//函数声明————————————————————
void initialize(int sock_fd);
void handle_request(int sock_fd);

void login_username(int sock_fd, char msg_send[], char msg_recv[]);
int login_password(int sock_fd, char msg_send[], char msg_recv[]);
void show_help();
void quit(int sock_fd, char msg_send[], char msg_recv[]);
void list_file(int sock_fd, char msg_send[], char msg_recv[]);
int port_mode(int sock_fd, char msg_send[], char msg_recv[]);
int passive_mode(int sock_fd, char msg_send[], char msg_recv[]);

void upload_file(int sock_fd, char msg_send[], char msg_recv[], int work_mode_flag);
void download_file(int sock_fd, char msg_send[], char msg_recv[], int work_mode_flag);

void put_file(int sock_fd1, char msg_send[], char msg_recv[], int work_mode_flag);
void get_file(int sock_fd1, char msg_send[], char msg_recv[], int work_mode_flag);

//主函数————————————————————
int main()
{
    int sock_fd;                  //套接字文件描述符
    struct sockaddr_in serv_addr; //服务端网络信息结构体

    //创建套接字,获取套接字文件描述符
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) // TCP连接
    {
        fprintf(stderr, "Failed to create client's socket\n"); //向标准错误输出
        // perror("Failed to create client's socket");  //有的系统调用错误可能不设置errno，避免
        // return -1;   //退出函数
        exit(1); //退出程序
    }

    int optval = 1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
    {
        fprintf(stderr, "    Failed to set socket");
        exit(1);
    }
    //参数level代表欲设置的网络层, 一般设成SOL_SOCKET以存取socket层
    // SO_REUSEADDR允许在bind()过程中本地地址可重复使用
    //设置optval为1时可以将Time-WAIT状态下套接字的端口重新分配给新的套接字
    //参数optlen则为optval 的长度

    //初始化sockaddr结构体
    bzero(&serv_addr, sizeof(serv_addr)); //置0 对字节，包括'\0'
    serv_addr.sin_family = AF_INET;       // IPv4的地址族
    serv_addr.sin_addr.s_addr = inet_addr("192.168.137.2");
    //服务器的具体IP地址 将点分十进制IP地址转换成网络字节序的长整数型数 可用127.0.0.1
    //必须指定
    serv_addr.sin_port = htons(CONTROLPORT); //网络字节序短整型数   端口任意

    //与服务端建立连接
    if ((connect(sock_fd, (struct sockaddr *)(&serv_addr), sizeof(serv_addr))) == -1)
    {
        fprintf(stderr, "Failed to establish connection\n");
        exit(1);
    }

    initialize(sock_fd); //连接初始化

    handle_request(sock_fd); //服务端处理请求

    return 0;
}

//函数定义————————————————————
//连接初始化
void initialize(int sock_fd)
{
    char msg_recv[BUFFERSIZE]; //接收客户端消息缓冲区
    bzero(msg_recv, BUFFERSIZE);

    if ((recv(sock_fd, msg_recv, BUFFERSIZE, 0)) > 0) //接收消息
    {
        if ((strncmp(msg_recv, "200", 3)) == 0) //接收到欢迎消息    若接收到则必定是欢迎消息    可扩展
        {
            printf("%s\n", msg_recv);
            return;
        }
    }
    else
    {
        fprintf(stderr, "Failed to receive welcome message\n");
        close(sock_fd);
        exit(1); //客户端直接退出程序
    }
}

//服务端处理请求
void handle_request(int sock_fd)
{
    char command[COMMANDSIZE]; //操作命令
    char msg_recv[BUFFERSIZE]; //发送服务端消息缓冲区
    char msg_send[BUFFERSIZE]; //接收服务端消息缓冲区
    int login_flag = 0;        //记录是否登录 0无1有    注意有的不能每清空
    int work_mode_flag = -1;   //工作模式  0被动1主动

    //进入ftp操作界面   循环操作
    while (1)
    {
        printf("ftp>"); //提示符

        bzero(command, COMMANDSIZE); //每清空
        bzero(msg_recv, BUFFERSIZE);
        bzero(msg_send, BUFFERSIZE);

        if ((fgets(command, COMMANDSIZE, stdin)) == NULL) // 从输入流读取COMMANDSIZE-1个字符    不用get()
        {
            fprintf(stderr, "    Failed to get command\n");
            exit(1);
        }
        // printf("%ld",strlen(command));   //用ld    有换行符
        command[strlen(command) - 1] = '\0'; // fgets()读取的最后一个字符为换行符，替换为结束符

        //操作
        //将命令拷贝到发送服务端消息缓冲区
        stpncpy(msg_send, command, strlen(command)); //不用strcpy() 替换为结束符后strlen(command)-1了
        msg_send[strlen(command)] = '\0';            //手动加结束符

        if ((strncmp(command, "help", 4)) == 0) //不用strcmp()
        {
            show_help();
        }
        else if ((strncmp(command, "user", 4)) == 0)
        {
            login_username(sock_fd, msg_send, msg_recv);
        }
        else if ((strncmp(command, "pass", 4)) == 0)
        {
            login_flag = login_password(sock_fd, msg_send, msg_recv);
        }
        else if ((strncmp(command, "quit", 4)) == 0)
        {
            quit(sock_fd, msg_send, msg_recv);
        }
        else if ((strncmp(command, "list", 4)) == 0)
        {
            if (login_flag == 1)
            {
                list_file(sock_fd, msg_send, msg_recv);
            }
            else
            {
                printf("    The user not logged in\n");
            }
        }
        else if (strncmp(command, "port", 4) == 0)
        {
            if (login_flag == 1)
            {
                work_mode_flag = port_mode(sock_fd, msg_send, msg_recv);
            }
            else
            {
                printf("    The user not logged in\n");
            }
        }
        else if (strncmp(command, "pasv", 4) == 0)
        {
            if (login_flag == 1)
            {
                work_mode_flag = passive_mode(sock_fd, msg_send, msg_recv);
            }
            else
            {
                printf("    The user not logged in\n");
            }
        }
        else if (strncmp(command, "upload", 6) == 0)
        {
            if (login_flag == 1)
            {
                if (work_mode_flag == -1)
                {
                    printf("    Please select the working mode\n");
                    continue;
                }
                else
                {
                    upload_file(sock_fd, msg_send, msg_recv, work_mode_flag);
                }
            }
            else
            {
                printf("    The user not logged in\n");
            }
        }
        else if (strncmp(command, "download", 8) == 0)
        {
            if (login_flag == 1)
            {
                if (work_mode_flag == -1)
                {
                    printf("    Please select the working mode\n");
                    continue;
                }
                else
                {
                    download_file(sock_fd, msg_send, msg_recv, work_mode_flag);
                }
            }
            else
            {
                printf("    The user not logged in\n");
            }
        }
        else if (strncmp(command, "put", 3) == 0)
        {
            if (login_flag == 1)
            {
                if (work_mode_flag == -1)
                {
                    printf("    Please select the working mode\n");
                    continue;
                }
                else
                {
                    put_file(sock_fd, msg_send, msg_recv, work_mode_flag);
                }
            }
            else
            {
                printf("    The user not logged in\n");
            }
        }
        else if (strncmp(command, "get", 3) == 0)
        {
            if (login_flag == 1)
            {
                if (work_mode_flag == -1)
                {
                    printf("    Please select the working mode\n");
                    continue;
                }
                else
                {
                    get_file(sock_fd, msg_send, msg_recv, work_mode_flag);
                }
            }
            else
            {
                printf("    The user not logged in\n");
            }
        }
        else
        {
            printf("Command does not exist\n");
        }
    }
}

//显示帮助信息
void show_help()
{
    printf("    命令列表\n");
    printf("    |——————————————————————————\n");
    printf("    |help : 显示帮助信息\n");
    printf("    |user <name> : 登录用户名\n");
    printf("    |pass <password> : 登录密码\n");
    printf("    |quit : 断开连接退出\n");
    printf("    |list : 显示文件列表\n");
    printf("    |pasv : 使用被动模式\n");
    printf("    |upload <file> : 上传文件\n");
    printf("    |download <file> : 下载文件\n");
    printf("    |put <file>: 上传文件\n");
    printf("    |get <file>: 下载文件\n");
    printf("    |——————————————————————————\n");

    return;
}

//登录用户名
void login_username(int sock_fd, char msg_send[], char msg_recv[])
{
    if ((send(sock_fd, msg_send, BUFFERSIZE, 0)) == -1) //发送消息
    {
        fprintf(stderr, "    Failed to send user command\n");
        return;
    }

    if ((recv(sock_fd, msg_recv, BUFFERSIZE, 0)) > 0) //接收消息
    {
        if ((strncmp(msg_recv, "331", 3)) == 0) //接收到用户名确认消息    331：用户名正确，需要密码
        {
            printf("    %s\n", msg_recv);
            return;
        }
        else
        {
            fprintf(stderr, "    %s\n", msg_recv);
            return;
        }
    }
    else
    {
        fprintf(stderr, "    Failed to receive user name confirmation message\n");
        close(sock_fd);
        exit(1); //客户端直接退出程序
    }
}

//登录密码
int login_password(int sock_fd, char msg_send[], char msg_recv[])
{
    if ((send(sock_fd, msg_send, BUFFERSIZE, 0)) == -1) //发送消息
    {
        fprintf(stderr, "    Failed to send pass command\n");
        return 0;
    }

    if ((recv(sock_fd, msg_recv, BUFFERSIZE, 0)) > 0) //接收消息
    {
        if ((strncmp(msg_recv, "230", 3)) == 0) //接收到密码确认消息   230：用户已登录，继续进行
        {
            printf("    %s\n", msg_recv);
            return 1;
        }
        else if ((strncmp(msg_recv, "530", 3)) == 0) // 530：未登录
        {
            fprintf(stderr, "    %s\n", msg_recv);
            return 0;
        }
    }
    else
    {
        fprintf(stderr, "    Failed to receive password confirmation message\n");
        close(sock_fd);
        exit(1); //客户端直接退出程序
    }
}

//断开连接退出
void quit(int sock_fd, char msg_send[], char msg_recv[])
{
    if ((send(sock_fd, msg_send, BUFFERSIZE, 0)) == -1) //写命令    可用write()
    {
        fprintf(stderr, "    Failed to send 'quit' command\n");
        return;
    }

    if ((recv(sock_fd, msg_recv, BUFFERSIZE, 0)) > 0) //读一次   可用read()  读到服务端确认信息退出
    {
        printf("    %s\n", msg_recv);

        //关闭套接字
        if ((close(sock_fd)) == -1)
        {
            fprintf(stderr, "    Failed to close the socket\n");
            exit(1);
        }

        //提示信息
        printf("    The connection is down. Quit successfully\n");

        exit(0);
    }
    else
    {
        fprintf(stderr, "    Failed to receive server's disconnection confirmation information\n");
        exit(1);
    }
}

//显示文件列表
void list_file(int sock_fd, char msg_send[], char msg_recv[])
{
    if ((send(sock_fd, msg_send, BUFFERSIZE, 0)) == -1) //写命令    可用write()
    {
        fprintf(stderr, "    Failed to send 'list' command\n");
        return;
    }

    printf("    ");
    while ((recv(sock_fd, msg_recv, BUFFERSIZE, 0)) > 0) //循环读   可用read()
    //未读到出循环返回×  会阻塞!!!一直等待数据
    {
        if ((strncmp(msg_recv, "end", 3)) == 0) //读到终止符退出循环
        {
            break;
        }

        printf("%s ", msg_recv);
    }
    printf("\n");

    return;
}

//使用主动模式
int port_mode(int sock_fd, char msg_send[], char msg_recv[])
{
    if ((send(sock_fd, msg_send, BUFFERSIZE, 0)) == -1) //发送消息
    {
        fprintf(stderr, "    Failed to send port command\n");
        return -1;
    }

    if ((recv(sock_fd, msg_recv, BUFFERSIZE, 0)) > 0) //接收消息
    {
        if ((strncmp(msg_recv, "150", 3)) == 0) // 150：表示服务器准备在端口20打开新连接，发送数据
        {
            printf("    %s\n", msg_recv);
            return 1;
        }
        else
        {
            fprintf(stderr, "    %s\n", msg_recv);
            return -1;
        }
    }
    else
    {
        fprintf(stderr, "    Failed to receive port mode confirmation message\n");
        close(sock_fd);
        exit(1); //客户端直接退出程序
    }
}

//使用被动模式
int passive_mode(int sock_fd, char msg_send[], char msg_recv[])
{
    if ((send(sock_fd, msg_send, BUFFERSIZE, 0)) == -1) //发送消息
    {
        fprintf(stderr, "    Failed to send pasv command\n");
        return -1;
    }

    if ((recv(sock_fd, msg_recv, BUFFERSIZE, 0)) > 0) //接收消息
    {
        if ((strncmp(msg_recv, "227", 3)) == 0) // 227：进入被动模式 (h1,h2,h3,h4,p1,p2)
        {
            printf("    %s\n", msg_recv);
            return 0;
        }
        else
        {
            fprintf(stderr, "    %s\n", msg_recv);
            return -1;
        }
    }
    else
    {
        fprintf(stderr, "    Failed to receive passive mode confirmation message\n");
        close(sock_fd);
        exit(1); //客户端直接退出程序
    }
}

//上传文件
void upload_file(int sock_fd1, char msg_send[], char msg_recv[], int work_mode_flag)
{
    if ((send(sock_fd1, msg_send, BUFFERSIZE, 0)) == -1) //先写命令
    {
        fprintf(stderr, "    Failed to send 'upload' command\n");
        return;
    }

    sleep(1); //单位s    先让服务端接收命令并监听，否则因为时序可能连接失败    我真是太聪明了！！！

    //创建数据连接;
    int sock_fd;
    struct sockaddr_in serv_addr;

    //创建套接字,获取套接字文件描述符
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "Failed to create client's data socket\n");
        return;
    }

    //初始化sockaddr结构体
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //得使用不冲突的
    if (work_mode_flag == 1)                            //主动模式
    {
        serv_addr.sin_port = htons(DATASPECIFIEDPORT);
    }
    else //被动模式
    {
        serv_addr.sin_port = htons(DATARANDOMPORT);
    }

    //与服务端建立连接
    if ((connect(sock_fd, (struct sockaddr *)(&serv_addr), sizeof(serv_addr))) == -1)
    {
        close(sock_fd);
        fprintf(stderr, "    Failed to establish data connection\n");
        return;
    }

    int fd;             //文件描述符
    int byte_count = 0; //一次read()读取的字节数

    if ((fd = open(msg_send + 7, O_RDONLY)) == -1) //打开文件  "upload a.txt"文件名  只读
    {
        close(sock_fd);
        fprintf(stderr, "    Failed to open upload file\n");
        return;
    }

    bzero(msg_send, BUFFERSIZE); //清空   以便第二次写

    while ((byte_count = read(fd, msg_send, BUFFERSIZE)) > 0) //读文件
    {
        if (send(sock_fd, msg_send, byte_count, 0) == -1) //继续写缓冲区 注意此时是按读取的字节数写 否则会有多余空值
        {
            close(fd);
            close(sock_fd);
            fprintf(stderr, "    Failed to send file content\n");
            return;
        }
    }

    int optval = 1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
    {
        fprintf(stderr, "    Failed to set upload socket");
        return;
    }
    //参数level代表欲设置的网络层, 一般设成SOL_SOCKET以存取socket层
    // SO_REUSEADDR允许在bind()过程中本地地址可重复使用
    //设置optval为1时可以将Time-WAIT状态下套接字的端口重新分配给新的套接字
    //参数optlen则为optval 的长度

    //关闭文件描述符
    close(fd);
    close(sock_fd);

    return;
}

//下载文件
void download_file(int sock_fd1, char msg_send[], char msg_recv[], int work_mode_flag)
{
    if ((send(sock_fd1, msg_send, BUFFERSIZE, 0)) == -1) //先写命令
    {
        fprintf(stderr, "    Failed to send 'download' command\n");
        return;
    }

    sleep(1); //单位s    先让服务端接收命令并监听，否则因为时序可能连接失败    我真是太聪明了！！！

    //创建数据连接;
    int sock_fd;
    struct sockaddr_in serv_addr;

    //创建套接字,获取套接字文件描述符
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "    Failed to create client's data socket\n");
        return;
    }

    //初始化sockaddr结构体
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //得使用不冲突的
    if (work_mode_flag == 1)                            //主动模式
    {
        serv_addr.sin_port = htons(DATASPECIFIEDPORT);
    }
    else //被动模式
    {
        serv_addr.sin_port = htons(DATARANDOMPORT);
    }

    //与服务端建立连接
    if ((connect(sock_fd, (struct sockaddr *)(&serv_addr), sizeof(serv_addr))) == -1)
    {
        close(sock_fd);
        fprintf(stderr, "    Failed to establish data connection\n");
        return;
    }

    int fd;             //文件描述符
    int byte_count = 0; //一次read()读取的字节数

    umask(0000);
    //临时设置0000，实际创建的文件权限即为mode值    否则创建文件仍只有用户可读写权限
    if ((fd = open(msg_send + 9, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO)) < 0) //打开文件
    //"download b.txt"
    //只写    若文件不存在新建文件，存在清空
    //文件用户、用户组和其他用户具有可读、可写及可执行权限  00777
    {
        close(sock_fd);
        fprintf(stderr, "    Failed to open download file\n");
        return;
    }

    if ((byte_count = recv(sock_fd, msg_recv, BUFFERSIZE, 0)) > 0) //读缓冲区   有数据
    {
        if (write(fd, msg_recv, byte_count) < 0) //写文件   无数据出错  注意此时是按读取的字节数写  否则会有多余空值
        {
            close(fd);
            close(sock_fd);
            fprintf(stderr, "    Failed to write file\n");
            return;
        }
    }

    int optval = 1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
    {
        fprintf(stderr, "    Failed to set download socket");
        return;
    }
    //参数level代表欲设置的网络层, 一般设成SOL_SOCKET以存取socket层
    // SO_REUSEADDR允许在bind()过程中本地地址可重复使用
    //设置optval为1时可以将Time-WAIT状态下套接字的端口重新分配给新的套接字
    //参数optlen则为optval 的长度

    //关闭文件描述符
    close(fd);
    close(sock_fd);

    return;
}

//上传文件————实现断点续传
void put_file(int sock_fd1, char msg_send[], char msg_recv[], int work_mode_flag)
{
    //查看本地/客户端文件大小
    FILE *fp;               //文件指针
    int clie_file_size = 0; //客户端文件大小计数    单位：字节

    //打开文件
    if ((fp = fopen(msg_send + 4, "r")) == NULL) //"put c.txt"  只读
    {
        fprintf(stderr, "    Failed to open file\n");
        return;
    }

    //定位至文件末尾
    if ((fseek(fp, 0, SEEK_END)) == -1) //与文件末尾的偏移
    {
        fprintf(stderr, "    Failed to seek file's end\n");
        return;
    }

    clie_file_size = ftell(fp);                                    //获取客户端文件大小
    printf("    The client's file size : %d B\n", clie_file_size); //注意包括换行符

    if ((send(sock_fd1, msg_send, BUFFERSIZE, 0)) == -1) //写命令 "put c.txt"
    {
        fprintf(stderr, "    Failed to send 'put' command\n");
        return;
    }

    if ((recv(sock_fd1, msg_recv, BUFFERSIZE, 0)) < 0) // 读服务端已接收文件大小 10
    {
        fprintf(stderr, "    Failed to receive file size\n");
        return;
    }

    int serv_recv_count = 0;          //服务端已接收的文件字节/字符数
    serv_recv_count = atoi(msg_recv); //将字符型转换为整型

    printf("    The server has received the file size : %d B\n", serv_recv_count); //注意包括换行符

    //定位至服务端已接收的文件字节/字符数
    if ((fseek(fp, serv_recv_count, SEEK_SET)) == -1) //与文件开头的偏移
    {
        fprintf(stderr, "    Failed to seek file\n");
        return;
    }

    char buffer[BUFFERSIZE];    //读文件时的临时缓冲区
    char msg_send1[BUFFERSIZE]; //新定义发送服务端缓冲区    避免冲突

    bzero(&buffer, sizeof(buffer));
    bzero(&msg_send1, sizeof(msg_send1));

    //读取文件内容拷贝至发送服务端缓冲区
    while ((fgets(buffer, BUFFERSIZE, fp)) != NULL) //每次读取一行
    {
        strncat(msg_send1, buffer, strlen(buffer));
    }

    //关闭文件指针
    fclose(fp);

    //创建数据连接;
    int sock_fd;
    struct sockaddr_in serv_addr;

    //创建套接字,获取套接字文件描述符
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "Failed to create client's data socket\n");
        return;
    }

    //初始化sockaddr结构体
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //得使用不冲突的
    if (work_mode_flag == 1)                            //主动模式
    {
        serv_addr.sin_port = htons(DATASPECIFIEDPORT);
    }
    else //被动模式
    {
        serv_addr.sin_port = htons(DATARANDOMPORT);
    }

    //与服务端建立连接
    if ((connect(sock_fd, (struct sockaddr *)(&serv_addr), sizeof(serv_addr))) == -1)
    {
        close(sock_fd);
        fprintf(stderr, "    Failed to establish data connection\n");
        return;
    }

    //发送  使用strlen(msg_send1)，避免服务端接收'NULL'字符
    if ((send(sock_fd, msg_send1, strlen(msg_send1), 0)) == -1)
    {
        close(sock_fd);
        fprintf(stderr, "    Failed to send file content\n");
        return;
    }

    int optval = 1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
    {
        fprintf(stderr, "    Failed to set upload socket");
        return;
    }
    //参数level代表欲设置的网络层, 一般设成SOL_SOCKET以存取socket层
    // SO_REUSEADDR允许在bind()过程中本地地址可重复使用
    //设置optval为1时可以将Time-WAIT状态下套接字的端口重新分配给新的套接字
    //参数optlen则为optval 的长度

    //关闭文件描述符
    close(sock_fd);

    return;
}

//下载文件————实现断点续传
void get_file(int sock_fd1, char msg_send[], char msg_recv[], int work_mode_flag)
{
    if ((send(sock_fd1, msg_send, BUFFERSIZE, 0)) == -1) //写命令 "get d.txt"
    {
        fprintf(stderr, "    Failed to send 'get' command\n");
        return;
    }

    if ((recv(sock_fd1, msg_recv, BUFFERSIZE, 0)) < 0) // 读服务端文件大小 100
    {
        fprintf(stderr, "    Failed to receive the server's file size\n");
        return;
    }

    int serv_file_size = 0;                                        //服务端文件大小计数    单位：字节
    serv_file_size = atoi(msg_recv);                               //将字符型转换为整型
    printf("    The server's file size : %d B\n", serv_file_size); //注意包括换行符

    //查看本地/客户端文件大小
    int fd;                  //文件描述符
    int byte_count;          //一次read()读取的字节数
    char buffer[BUFFERSIZE]; //读文件时的临时缓冲区

    umask(0000);
    //临时设置0000，实际创建的文件权限即为mode值    否则创建文件仍只有用户可读写权限
    if ((fd = open(msg_send + 4, O_RDWR | O_CREAT | O_APPEND, S_IRWXU | S_IRWXG | S_IRWXO)) < 0) //打开文件
    //可读写    若文件不存在新建文件  存在不清空，附加内容
    //文件用户、用户组和其他用户具有可读、可写及可执行权限  00777
    {
        close(fd);
        fprintf(stderr, "    Failed to open the get file\n");
        return;
    }

    if ((byte_count = read(fd, buffer, BUFFERSIZE)) >= 0) //读文件大小
    {
        sprintf(msg_send, "%d", byte_count); //将整型转换为字符型   重写缓冲区

        if ((send(sock_fd1, msg_send, BUFFERSIZE, 0)) == -1) //发送客户端已接收文件大小
        {
            close(fd);
            fprintf(stderr, "    Failed to send the file size\n");
            return;
        }
    }
    else
    {
        fprintf(stderr, "    Failed to read the file size\n");
        return;
    }

    printf("    The client's has received the file size : %d B\n", byte_count); //注意包括换行符

    sleep(1); //等服务端先监听套接字

    //创建数据连接;
    int sock_fd;
    struct sockaddr_in serv_addr;

    //创建套接字,获取套接字文件描述符
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "Failed to create client's data socket\n");
        return;
    }

    //初始化sockaddr结构体
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //得使用不冲突的
    if (work_mode_flag == 1)                            //主动模式
    {
        serv_addr.sin_port = htons(DATASPECIFIEDPORT);
    }
    else //被动模式
    {
        serv_addr.sin_port = htons(DATARANDOMPORT);
    }

    //与服务端建立连接
    if ((connect(sock_fd, (struct sockaddr *)(&serv_addr), sizeof(serv_addr))) == -1)
    {
        close(sock_fd);
        fprintf(stderr, "    Failed to establish data connection\n");
        return;
    }

    //写文件
    if ((byte_count = recv(sock_fd, msg_recv, BUFFERSIZE, 0)) > 0) //读缓冲区    有数据
    {
        if (write(fd, msg_recv, byte_count) < 0) //写文件   无数据出错  注意此时是按读取的字节数写  否则会有多余空值
        {
            close(fd);
            close(sock_fd);
            fprintf(stderr, "    Failed to write file\n");
            return;
        }
    }

    int optval = 1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
    {
        fprintf(stderr, "    Failed to set upload socket");
        return;
    }
    //参数level代表欲设置的网络层, 一般设成SOL_SOCKET以存取socket层
    // SO_REUSEADDR允许在bind()过程中本地地址可重复使用
    //设置optval为1时可以将Time-WAIT状态下套接字的端口重新分配给新的套接字
    //参数optlen则为optval 的长度

    //关闭文件描述符
    close(fd);
    close(sock_fd);

    return;
}