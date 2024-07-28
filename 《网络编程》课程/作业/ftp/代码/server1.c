//头文件————————————————————
#include <sys/socket.h>
//网络编程相关  socket()、AF_INET、SOCK_STREAM、bind()、listen()、accept()、recv()、send()、setsockopt()
#include <netinet/in.h> //sockaddr_in、htonl()、htons()
#include <stdio.h>      //fprintf()、printf()、sprintf()
#include <stdlib.h>     //exit()
#include <arpa/inet.h>  //inet_addr()
#include <string.h>     //bzero()、strlen()、strncpy()、strncat()
#include <unistd.h>     //read()、write()、close()、fork()
#include <dirent.h>     //DIR、opendir()、readdir()、closedir()
#include <fcntl.h>      //open()、O_WRONLY 、O_CREAT 、O_TRUNC、O_RDWR、O_APPEND
#include <sys/types.h>
#include <sys/stat.h> //umask()

//宏————————————————————
#define COMMANDSIZE 256        //操作命令大小
#define BUFFERSIZE 256         //传递消息缓冲区大小
#define CONTROLPORT 3321       //控制端口
#define LISTENMAXCOUNT 5       //所监听的最大连接数
#define FILEPATHSIZE 50        //文件路径大小
#define USERDATESIZE 50        //用户数据大小
#define USERCOUNT 2            //用户数量
#define DATASPECIFIEDPORT 3320 //数据指定端口
#define DATARANDOMPORT 3322    //数据随机端口

//结构体
struct User_database //用户数据库
{
    char name[USERDATESIZE];
    char password[USERDATESIZE];
} user_database[USERCOUNT] = {
    {"ano", "null"}, // anonymous
    {"con", "123"}}; // consumer

//函数声明————————————————————
void initialize(int conn_fd);
void handle_request(int conn_fd);

int login_username(int conn_fd, char *username, char msg_send[]);
int login_password(int conn_fd, char *password, char msg_send[], int login_user_number);
void quit(int conn_fd, char msg_send[]);
void list_file(int conn_fd, char msg_send[]);
int port_mode(int conn_fd, char msg_send[]);
int passive_mode(int conn_fd, char msg_send[]);

void upload_file(char *file_name, char msg_recv[], char msg_send[], int work_mode_flag);
void download_file(char *file_name, char msg_recv[], char msg_send[], int work_mode_flag);

void put_file(int conn_fd, char *file_name, char msg_send[], char msg_recv[], int work_mode_flag);
void get_file(int conn_fd, char *file_name, char msg_send[], char msg_recv[], int work_mode_flag);

//主函数————————————————————
int main(int arg, char **argv)
{
    int sock_fd;                  //套接字文件描述符
    struct sockaddr_in serv_addr; //服务端网络信息结构体
    struct sockaddr_in cli_addr;  //客户端网络信息结构体
    int addr_size;                //网络信息结构体大小
    int conn_fd;                  //已建立连接的套接字文件描述符
    int pid;                      //进程号

    //创建套接字,获取套接字文件描述符
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) // TCP连接
    {
        fprintf(stderr, "Failed to create server's socket\n"); //向标准错误输出
        // perror("Failed to create server's socket");  //有的系统调用错误可能不设置errno，避免
        // return -1;   //退出函数
        exit(1); //退出程序
    }

    int optval = 1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
    {
        fprintf(stderr, "    Failed to set download socket");
        exit(1);
    }
    //参数level代表欲设置的网络层, 一般设成SOL_SOCKET以存取socket层
    // SO_REUSEADDR允许在bind()过程中本地地址可重复使用
    //设置optval为1时可以将Time-WAIT状态下套接字的端口重新分配给新的套接字
    //参数optlen则为optval的长度

    //初始化sockaddr结构体  对服务端
    bzero(&serv_addr, sizeof(serv_addr));          //置0 对字节，包括'\0'
    serv_addr.sin_family = AF_INET;                // IPv4的地址族
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //本地任意ip地址
    serv_addr.sin_port = htons(CONTROLPORT);       //网络字节序短整型数   端口任意

    bzero(&cli_addr, sizeof(cli_addr)); //对客户端

    //绑定套接字与网络信息
    if ((bind(sock_fd, (struct sockaddr *)(&serv_addr), sizeof(serv_addr))) == -1)
    {
        close(sock_fd);
        fprintf(stderr, "Failed to bind the socket\n");
        exit(1);
    }

    //套接字设置被动监听状态
    if ((listen(sock_fd, LISTENMAXCOUNT)) == -1)
    {
        close(sock_fd);
        fprintf(stderr, "Failed to configure listening");
        exit(1);
    }

    addr_size = sizeof(struct sockaddr);

    //循环监听客户端请求
    while (1)
    {
        //与客户端建立连接
        if ((conn_fd = accept(sock_fd, (struct sockaddr *)(&cli_addr), &addr_size)) == 0)
        {
            fprintf(stderr, "Failed to accept connection");
            exit(1);
        }

        if (setsockopt(conn_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
        {
            fprintf(stderr, "    Failed to set download socket");
            exit(1);
        }
        //参数level代表欲设置的网络层, 一般设成SOL_SOCKET以存取socket层
        // SO_REUSEADDR允许在bind()过程中本地地址可重复使用
        //设置optval为1时可以将Time-WAIT状态下套接字的端口重新分配给新的套接字
        //参数optlen则为optval的长度

        // 使用子进程处理
        if ((pid = fork()) == 0)
        {
            close(sock_fd); //关闭不需要的

            initialize(conn_fd); //连接初始化

            handle_request(conn_fd); //处理请求
        }
    }

    ///关闭套接字文件描述符
    close(conn_fd); //可能重复关闭
    close(sock_fd);
    return 0;
}

//函数实现————————————————————
//子进程连接初始化
void initialize(int conn_fd)
{
    char msg_send[BUFFERSIZE]; //发送客户端消息缓冲区
    bzero(msg_send, BUFFERSIZE);

    char welcome_msg[30] = "200 Welcome to the FTP server"; //欢迎信息  220:服务就绪，可以执行新用户请求
    strncpy(msg_send, welcome_msg, strlen(welcome_msg));
    msg_send[strlen(welcome_msg)] = '\0'; //必须!!!

    if ((send(conn_fd, msg_send, BUFFERSIZE, 0)) == -1) //发送消息
    {
        fprintf(stderr, "Failed to send welcome message\n");
        exit(1); //退出子进程
    }
}

//子进程处理请求
void handle_request(int conn_fd)
{
    char command[COMMANDSIZE]; //操作命令
    char msg_recv[BUFFERSIZE]; //接收客户端消息缓冲区
    char msg_send[BUFFERSIZE]; //发送客户端消息缓冲区
    int login_user_number;     //记录登录用户序号
    int login_flag = 0;        //用户登录标记 0无1有
    int work_mode_flag = -1;   //工作模式  0被动1主动

    while (1)
    {
        bzero(msg_recv, BUFFERSIZE);
        bzero(msg_send, BUFFERSIZE);

        if (recv(conn_fd, msg_recv, BUFFERSIZE, 0) < 0) // 读命令   可用read()
        {
            fprintf(stderr, "Failed to receive command\n");
            exit(1);
        }

        printf("ftp>"); //提示符    需要换行符刷新标准输出缓冲区才显示

        //将接收客户端消息缓冲区拷贝到命令
        bzero(command, COMMANDSIZE);
        stpncpy(command, msg_recv, strlen(msg_recv)); //不用strcpy()
        // printf("%ld", strlen(msg_recv));
        command[strlen(msg_recv)] = '\0'; //手动将换行符替换为结束符

        printf("command : [%s]\n", command); //提示所接收命令 必须有换行符才显示

        //操作
        if ((strncmp(command, "user", 4)) == 0)
        {
            login_user_number = login_username(conn_fd, command + 5, msg_send); //"user consumer"   记录登录用户序号
        }
        else if ((strncmp(command, "pass", 4)) == 0)
        {
            login_flag = login_password(conn_fd, command + 5, msg_send, login_user_number); //"pass 123456"
        }
        else if ((strncmp(command, "quit", 4)) == 0) //断开连接
        {
            quit(conn_fd, msg_send);
        }
        else if ((strncmp(command, "list", 4)) == 0)
        {
            list_file(conn_fd, msg_send);
        }
        else if ((strncmp(command, "port", 4)) == 0)
        {
            work_mode_flag = port_mode(conn_fd, msg_send);
        }
        else if ((strncmp(command, "pasv", 4)) == 0)
        {
            work_mode_flag = passive_mode(conn_fd, msg_send);
        }
        else if ((strncmp(command, "upload", 6)) == 0) //客户端上传文件
        {
            upload_file(command + 7, msg_recv, msg_send, work_mode_flag); //"upload a.txt"
        }
        else if ((strncmp(command, "download", 8)) == 0) //客户端下载文件
        {
            download_file(command + 9, msg_recv, msg_send, work_mode_flag); //"download b.txt"
        }
        else if ((strncmp(command, "put", 3)) == 0)
        {
            put_file(conn_fd, command + 4, msg_send, msg_recv, work_mode_flag); //"put c.txt"
        }
        else if ((strncmp(command, "get", 3)) == 0)
        {
            get_file(conn_fd, command + 4, msg_send, msg_recv, work_mode_flag); //"get d.txt"
        }
    }
}

//客户端登录用户名
int login_username(int conn_fd, char *user_name, char msg_send[])
{
    int i; //循环变量

    for (i = 0; i < USERCOUNT; i++)
    {
        if ((strncmp(user_name, user_database[i].name, strlen(user_database[i].name))) == 0)
        {
            strncpy(msg_send, "331 The user name is correct", 29);
            break;
        }
    }

    if (i == USERCOUNT) //不存在用户名
    {
        strncpy(msg_send, "The user name does not exist", 29);
    }

    if ((send(conn_fd, msg_send, BUFFERSIZE, 0)) == -1) //发送消息
    {
        fprintf(stderr, "    Failed to send user name confirmation message\n");
        return 0;
    }

    return i; //返回登录用户序号
}

//客户端登录密码
int login_password(int conn_fd, char *password, char msg_send[], int login_user_number)
{
    int login_flag = 0; //记录是否登录 0无1有

    if ((strncmp(password, user_database[login_user_number].password, strlen(user_database[login_user_number].password))) == 0) //可扩展
    {
        strncpy(msg_send, "230 Access granted", 19);
        login_flag = 1;
    }
    else
    {
        strncpy(msg_send, "530 Password error", 19);
        login_flag = 0;
    }

    if ((send(conn_fd, msg_send, BUFFERSIZE, 0)) == -1) //发送消息
    {
        fprintf(stderr, "    Failed to send password confirmation message\n");
        return 0;
    }

    return login_flag;
}

//断开连接
void quit(int conn_fd, char msg_send[])
{
    char *message = "The server receives a disconnection request";
    strncpy(msg_send, message, strlen(message));

    if ((send(conn_fd, msg_send, BUFFERSIZE, 0)) == -1) //写命令    可用write()
    {
        fprintf(stderr, "    Failed to send 'quit' command\n");
        return;
    }

    //关闭套接字文件描述符
    close(conn_fd);
    exit(0); //退出子进程
}

//显示文件列表
void list_file(int conn_fd, char msg_send[])
{
    DIR *directory = NULL;      //目录（文件）结构体指针
    struct dirent *item = NULL; //目录（项）结构体指针

    if ((directory = opendir("./server_file")) == NULL) //打开服务器端文件目录   返回DIR
    {
        fprintf(stderr, "    Failed to open directory\n");
        return; //函数执行失败返回，不退出
    }

    while ((item = readdir(directory)) != NULL) //读取目录,返回dirent
    {
        if ((sprintf(msg_send, item->d_name, BUFFERSIZE)) < 0) //取文件名
        {
            fprintf(stderr, "    Failed to get file name\n");
            return;
        }

        if ((send(conn_fd, msg_send, BUFFERSIZE, 0)) == -1) //写 可用write()
        {
            fprintf(stderr, "    Failed to send file name\n");
            return;
        }
    }

    //数据发送完    发送终止符
    bzero(msg_send, BUFFERSIZE);
    strncpy(msg_send, "end", 3);
    if ((send(conn_fd, msg_send, BUFFERSIZE, 0)) == -1) //写 可用write()
    {
        fprintf(stderr, "    Failed to send 'end' character\n");
        return;
    }

    //关闭套接字目录流
    closedir(directory);

    return;
}

//使用主动模式
int port_mode(int conn_fd, char msg_send[])
{
    strncpy(msg_send, "150 Entering port mode", 23); // 3320端口

    if ((send(conn_fd, msg_send, BUFFERSIZE, 0)) == -1) //发送消息
    {
        fprintf(stderr, "    Failed to send port mode confirmation message\n");
        return -1;
    }

    return 1;
}

//使用被动模式
int passive_mode(int conn_fd, char msg_send[])
{
    strncpy(msg_send, "227 Entering passive mode (127,0,0,1,12,250)", 49); // 12*256+250=3322端口

    if ((send(conn_fd, msg_send, BUFFERSIZE, 0)) == -1) //发送消息
    {
        fprintf(stderr, "    Failed to send passive mode confirmation message\n");
        return -1;
    }

    return 0;
}

//客户端上传文件
void upload_file(char *file_name, char msg_recv[], char msg_send[], int work_mode_flag)
{
    int sock_fd;                  //套接字文件描述符
    struct sockaddr_in serv_addr; //服务端网络信息结构体
    struct sockaddr_in cli_addr;  //客户端网络信息结构体
    int addr_size;                //网络信息结构体大小
    int conn_fd;                  //已建立连接的套接字文件描述符

    //创建套接字,获取套接字文件描述符
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "    Failed to create server's data socket\n");
        return;
    }

    //初始化sockaddr结构体  对服务端
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (work_mode_flag == 1) //主动模式
    {
        serv_addr.sin_port = htons(DATASPECIFIEDPORT);
    }
    else //被动模式
    {
        serv_addr.sin_port = htons(DATARANDOMPORT);
    }

    bzero(&cli_addr, sizeof(cli_addr)); //对客户端

    //绑定套接字与网络信息
    if ((bind(sock_fd, (struct sockaddr *)(&serv_addr), sizeof(serv_addr))) == -1)
    {
        close(sock_fd);
        fprintf(stderr, "    Failed to bind the data socket\n");
        return;
    }

    //套接字设置被动监听状态
    if ((listen(sock_fd, LISTENMAXCOUNT)) == -1)
    {
        close(sock_fd);
        fprintf(stderr, "    Failed to configure data listening");
        return;
    }

    addr_size = sizeof(struct sockaddr);

    //与客户端建立连接  一次
    if ((conn_fd = accept(sock_fd, (struct sockaddr *)(&cli_addr), &addr_size)) == 0)
    {
        close(sock_fd);
        fprintf(stderr, "    Failed to accept data connection");
        return;
    }

    int fd;             //文件描述符
    int byte_count = 0; //一次read()读取的字节数

    char file_path[FILEPATHSIZE] = "./server_file/"; //服务端文件路径  "./server_file/a.txt"
    //不用指针用数组    否则报错“段错误 (核心已转储)”
    strncat(file_path, file_name, strlen(file_name)); //不用strcat()

    printf("    file path : [%s]\n", file_path); // ftp>    对齐

    umask(0000);
    //临时设置0000，实际创建的文件权限即为mode值    否则创建文件仍只有用户可读写权限
    if ((fd = open(file_path, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO)) < 0) //打开文件
    //只写    若文件不存在新建文件，存在清空
    //文件用户、用户组和其他用户具有可读、可写及可执行权限  00777
    {
        close(sock_fd);
        fprintf(stderr, "    Failed to open upload file\n");
        return;
    }

    if ((byte_count = recv(conn_fd, msg_recv, BUFFERSIZE, 0)) > 0) //读缓冲区    有数据
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

    if (setsockopt(conn_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
    {
        fprintf(stderr, "    Failed to set upload socket");
        return;
    }
    //参数level代表欲设置的网络层, 一般设成SOL_SOCKET以存取socket层
    // SO_REUSEADDR允许在bind()过程中本地地址可重复使用
    //设置optval为1时可以将Time-WAIT状态下套接字的端口重新分配给新的套接字
    //参数optlen则为optval的长度

    //关闭文件描述符
    close(fd);
    close(conn_fd);
    close(sock_fd);

    return;
}

//客户端下载文件
void download_file(char *file_name, char msg_recv[], char msg_send[], int work_mode_flag)
{
    int sock_fd;                  //套接字文件描述符
    struct sockaddr_in serv_addr; //服务端网络信息结构体
    struct sockaddr_in cli_addr;  //客户端网络信息结构体
    int addr_size;                //网络信息结构体大小
    int conn_fd;                  //已建立连接的套接字文件描述符

    //创建套接字,获取套接字文件描述符
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "    Failed to create server's data socket\n");
        return;
    }

    //初始化sockaddr结构体  对服务端
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (work_mode_flag == 1) //主动模式
    {
        serv_addr.sin_port = htons(DATASPECIFIEDPORT);
    }
    else //被动模式
    {
        serv_addr.sin_port = htons(DATARANDOMPORT);
    }

    bzero(&cli_addr, sizeof(cli_addr)); //对客户端

    //绑定套接字与网络信息
    if ((bind(sock_fd, (struct sockaddr *)(&serv_addr), sizeof(serv_addr))) == -1)
    {
        close(sock_fd);
        fprintf(stderr, "    Failed to bind the data socket\n");
        return;
    }

    //套接字设置被动监听状态
    if ((listen(sock_fd, LISTENMAXCOUNT)) == -1)
    {
        close(sock_fd);
        fprintf(stderr, "    Failed to configure data listening");
        return;
    }

    addr_size = sizeof(struct sockaddr);

    //与客户端建立连接  一次
    if ((conn_fd = accept(sock_fd, (struct sockaddr *)(&cli_addr), &addr_size)) == 0)
    {
        close(sock_fd);
        fprintf(stderr, "    Failed to accept data connection");
        return;
    }

    int fd;             //文件描述符
    int byte_count = 0; //一次read()读取的字节数

    char file_path[FILEPATHSIZE] = "./server_file/"; //服务端文件路径  "./server_file/a.txt"
    //不用指针用数组    否则报错“段错误 (核心已转储)”
    strncat(file_path, file_name, strlen(file_name)); //不用strcat()

    printf("    file path : [%s]\n", file_path); // ftp>

    if ((fd = open(file_path, O_RDONLY)) == -1) //只读
    {
        close(sock_fd);
        fprintf(stderr, "    Failed to open download file\n");
        return;
    }

    if ((byte_count = read(fd, msg_send, BUFFERSIZE)) > 0) //读文件
    {
        if ((send(conn_fd, msg_send, byte_count, 0)) == -1) //继续写缓冲区 注意此时是按读取的字节数写 否则会有多余空值
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
        fprintf(stderr, "    Failed to set download socket");
        return;
    }

    if (setsockopt(conn_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
    {
        fprintf(stderr, "    Failed to set download socket");
        return;
    }
    //参数level代表欲设置的网络层, 一般设成SOL_SOCKET以存取socket层
    // SO_REUSEADDR允许在bind()过程中本地地址可重复使用
    //设置optval为1时可以将Time-WAIT状态下套接字的端口重新分配给新的套接字
    //参数optlen则为optval的长度

    //关闭文件描述符
    close(fd);
    close(conn_fd);
    close(sock_fd);

    return;
}

//客户端上传文件————实现断点续传
void put_file(int conn_fd, char *file_name, char msg_send[], char msg_recv[], int work_mode_flag)
{
    int fd;                  //文件描述符
    int byte_count;          //一次read()读取的字节数
    char buffer[BUFFERSIZE]; //读文件时的临时缓冲区

    //获取文件路径
    char file_path[FILEPATHSIZE] = "./server_file/"; //服务端文件路径  "./server_file/c.txt"
    //不用指针用数组    否则报错“段错误 (核心已转储)”
    strncat(file_path, file_name, strlen(file_name)); //不用strcat()

    printf("    file path : [%s]\n", file_path); // ftp>

    //获取已接收文件大小
    umask(0000);
    //临时设置0000，实际创建的文件权限即为mode值    否则创建文件仍只有用户可读写权限
    if ((fd = open(file_path, O_RDWR | O_CREAT | O_APPEND, S_IRWXU | S_IRWXG | S_IRWXO)) < 0) //打开文件
    //可读写    若文件不存在新建文件  存在不清空，附加内容
    //文件用户、用户组和其他用户具有可读、可写及可执行权限  00777
    {
        close(fd);
        fprintf(stderr, "    Failed to open the put file\n");
        return;
    }

    if ((byte_count = read(fd, buffer, BUFFERSIZE)) >= 0) //读文件大小
    {
        sprintf(msg_send, "%d", byte_count); //将整型转换为字符型

        if ((send(conn_fd, msg_send, BUFFERSIZE, 0)) == -1) //发送服务端已接收文件大小
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

    //建立数据连接
    int sock_fd;                  //套接字文件描述符
    struct sockaddr_in serv_addr; //服务端网络信息结构体
    struct sockaddr_in cli_addr;  //客户端网络信息结构体
    int addr_size;                //网络信息结构体大小
    int conn_fd1;                 //已建立连接的套接字文件描述符   另一个

    //创建套接字,获取套接字文件描述符
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "    Failed to create server's data socket\n");
        return;
    }

    //初始化sockaddr结构体  对服务端
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (work_mode_flag == 1) //主动模式
    {
        serv_addr.sin_port = htons(DATASPECIFIEDPORT);
    }
    else //被动模式
    {
        serv_addr.sin_port = htons(DATARANDOMPORT);
    }

    bzero(&cli_addr, sizeof(cli_addr)); //对客户端

    //绑定套接字与网络信息
    if ((bind(sock_fd, (struct sockaddr *)(&serv_addr), sizeof(serv_addr))) == -1)
    {
        close(sock_fd);
        fprintf(stderr, "    Failed to bind the data socket\n");
        return;
    }

    //套接字设置被动监听状态
    if ((listen(sock_fd, LISTENMAXCOUNT)) == -1)
    {
        close(sock_fd);
        fprintf(stderr, "    Failed to configure data listening");
        return;
    }

    addr_size = sizeof(struct sockaddr);

    //与客户端建立连接  一次
    if ((conn_fd1 = accept(sock_fd, (struct sockaddr *)(&cli_addr), &addr_size)) == 0)
    {
        close(sock_fd);
        fprintf(stderr, "    Failed to accept data connection");
        return;
    }

    //写文件
    if ((byte_count = recv(conn_fd1, msg_recv, BUFFERSIZE, 0)) > 0) //读缓冲区    有数据
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
        fprintf(stderr, "    Failed to set put socket");
        return;
    }

    if (setsockopt(conn_fd1, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
    {
        fprintf(stderr, "    Failed to set put socket");
        return;
    }
    //参数level代表欲设置的网络层, 一般设成SOL_SOCKET以存取socket层
    // SO_REUSEADDR允许在bind()过程中本地地址可重复使用
    //设置optval为1时可以将Time-WAIT状态下套接字的端口重新分配给新的套接字
    //参数optlen则为optval的长度

    //关闭文件描述符
    close(fd);
    close(conn_fd1);
    close(sock_fd);

    return;
}

//客户端下载文件————实现断点续传
void get_file(int conn_fd, char *file_name, char msg_send[], char msg_recv[], int work_mode_flag)
{
    //获取文件路径
    char file_path[FILEPATHSIZE] = "./server_file/"; //服务端文件路径  "./server_file/c.txt"
    //不用指针用数组    否则报错“段错误 (核心已转储)”
    strncat(file_path, file_name, strlen(file_name)); //不用strcat()

    printf("    file path : [%s]\n", file_path); // ftp>

    //查看本地/服务端文件大小
    FILE *fp;               //文件指针
    int serv_file_size = 0; //服务端文件大小计数    单位：字节

    //打开文件
    if ((fp = fopen(file_path, "r")) == NULL) //只读
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

    serv_file_size = ftell(fp);              //获取服务端文件大小
    sprintf(msg_send, "%d", serv_file_size); //将整型转换为字符型   重写缓冲区

    if ((send(conn_fd, msg_send, BUFFERSIZE, 0)) == -1) //写大小
    {
        fprintf(stderr, "    Failed to send the file size\n");
        return;
    }

    //获取客户端接收文件大小
    if ((recv(conn_fd, msg_recv, BUFFERSIZE, 0)) < 0) // 读客户端已接收文件大小 10
    {
        fprintf(stderr, "    Failed to receive the file size\n");
        return;
    }

    int clie_recv_count = 0;          //客户端已接收的文件字节/字符数
    clie_recv_count = atoi(msg_recv); //将字符型转换为整型

    //取文件内容
    //定位至客户端已接收的文件字节/字符数
    if ((fseek(fp, clie_recv_count, SEEK_SET)) == -1) //与文件开头的偏移
    {
        fprintf(stderr, "    Failed to seek file\n");
        return;
    }

    char buffer[BUFFERSIZE];    //读文件时的临时缓冲区
    char msg_send1[BUFFERSIZE]; //新定义发送客户端缓冲区    避免冲突

    bzero(&buffer, sizeof(buffer));
    bzero(&msg_send1, sizeof(msg_send1));

    //读取文件内容拷贝至发送客户端缓冲区
    while ((fgets(buffer, BUFFERSIZE, fp)) != NULL) //每次读取一行
    {
        strncat(msg_send1, buffer, strlen(buffer));
    }

    //关闭文件指针
    fclose(fp);

    //建立数据连接
    int sock_fd;                  //套接字文件描述符
    struct sockaddr_in serv_addr; //服务端网络信息结构体
    struct sockaddr_in cli_addr;  //客户端网络信息结构体
    int addr_size;                //网络信息结构体大小
    int conn_fd1;                 //已建立连接的套接字文件描述符   另一个

    //创建套接字,获取套接字文件描述符
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "    Failed to create server's data socket\n");
        return;
    }

    //初始化sockaddr结构体  对服务端
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (work_mode_flag == 1) //主动模式
    {
        serv_addr.sin_port = htons(DATASPECIFIEDPORT);
    }
    else //被动模式
    {
        serv_addr.sin_port = htons(DATARANDOMPORT);
    }

    bzero(&cli_addr, sizeof(cli_addr)); //对客户端

    //绑定套接字与网络信息
    if ((bind(sock_fd, (struct sockaddr *)(&serv_addr), sizeof(serv_addr))) == -1)
    {
        close(sock_fd);
        fprintf(stderr, "    Failed to bind the data socket\n");
        return;
    }

    //套接字设置被动监听状态
    if ((listen(sock_fd, LISTENMAXCOUNT)) == -1)
    {
        close(sock_fd);
        fprintf(stderr, "    Failed to configure data listening");
        return;
    }

    addr_size = sizeof(struct sockaddr);

    //与客户端建立连接  一次
    if ((conn_fd1 = accept(sock_fd, (struct sockaddr *)(&cli_addr), &addr_size)) == 0)
    {
        close(sock_fd);
        fprintf(stderr, "    Failed to accept data connection");
        return;
    }

    //发送  使用strlen(msg_send1)，避免客户端接收'NULL'字符
    if ((send(conn_fd1, msg_send1, strlen(msg_send1), 0)) == -1)
    {
        close(conn_fd1);
        close(sock_fd);
        fprintf(stderr, "    Failed to send file content\n");
        return;
    }

    int optval = 1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
    {
        fprintf(stderr, "    Failed to set put socket");
        return;
    }

    if (setsockopt(conn_fd1, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
    {
        fprintf(stderr, "    Failed to set put socket");
        return;
    }
    //参数level代表欲设置的网络层, 一般设成SOL_SOCKET以存取socket层
    // SO_REUSEADDR允许在bind()过程中本地地址可重复使用
    //设置optval为1时可以将Time-WAIT状态下套接字的端口重新分配给新的套接字
    //参数optlen则为optval的长度

    //关闭文件描述符
    close(conn_fd1);
    close(sock_fd);

    return;
}