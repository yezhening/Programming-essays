//头文件————————————————————
#include <stdio.h>      //printf()、scanf()、puts()、perror()、fprintf()、sprintf()、fflush()
#include <unistd.h>     //free()、close()
#include <string.h>     //memset()、strlen()、memcpy()、strtok()、strstr()
#include <stdlib.h>     //realloc()、malloc()
#include <sys/socket.h> //socket()、connect()、send()、recv()
#include <netinet/in.h> //sockaddr_in、htons()
#include <netdb.h>      //hostent、gethostbyname()、herror()
#include <arpa/inet.h>  //in_addr、inet_ntoa()、inet_addr()

//函数声明————————————————————
void get_whois(char *domain_ip, char **message);
int whois_query(char *server, char *query, char **response);
int hostname_to_ip(char *hostname, char *ip);

//主函数————————————————————
int main(int argc, char *argv[])
{
    char domain_ip[100] = ""; //所查询的域名或IP地址 数组大小：域名100IP20
    char *message = NULL;     //查询到的信息    指针同数组

    printf("Please enter domain name or ip address to WHOIS : "); //输入
    scanf("%s", domain_ip);

    get_whois(domain_ip, &message); //查询

    printf("\n\n"); //输出2.查询内容
    puts(message);

    free(message); //释放指针

    return 0;
}

//获取所查询域名对应的WHOIS查询服务端
//两次查询服务端，第一次是可指定的WHOIS查询服务端，第二次是所查询域名对应的WHOIS查询服务端
void get_whois(char *domain_ip, char **message)
{

    char *server = "whois.iana.org"; //所查询的服务端域名
    char *response = NULL;           //服务端响应信息
    char *temp_char = NULL;          //使用strtok()分解的中间字符
    char *whois_server = NULL;       //记录"whois"字段内容，域名对应的WHOIS查询服务端

    if ((whois_query(server, domain_ip, &response)) == -1)
    {
        fprintf(stderr, "Faied to query WHOIS\n");
    }

    // printf("\n这是服务端响应信息：\n%s\n", response); //根据响应信息处理需求，考虑不同服务端获取的不同信息格式

    //处理信息
    temp_char = strtok(response, "\n"); // 1.函数经典用法
    while (temp_char != NULL)           //循环截断，查找字段
    {
        whois_server = strstr(temp_char, "whois."); //取"whois"字段  strstr()：返回字符串中首次出现子串的地址
        if (whois_server != NULL)
        {
            break;
        }

        temp_char = strtok(NULL, "\n"); // 2.函数经典用法
    }

    if (whois_server != NULL)
    {
        printf("\nWHOIS server is : %s", whois_server);
        whois_query(whois_server, domain_ip, message); //在相应的二级服务端再次查询
    }
    else
    {
        *message = malloc(100); //动态分配内存空间
        strcpy(*message, "No whois message\n");
    }

    return;
}

// WHOIS查询
//使用网络套接字执行WHOIS查询，输出1.过程信息，获取响应内容
int whois_query(char *server, char *request, char **response)
{

    int socket_fd = 0;              //套接字文件描述符
    struct sockaddr_in serv_addr;   //服务端网络信息结构体
    char ip[32] = "";               // 32位IPv4地址
    char message[100] = "";         //请求所需查询的域名的特定格式
    int receive_size = 0;           //接收服务端响应信息字节数
    char receive_buffer[1500] = ""; //接收服务端响应缓冲区，按照IP一次最大接收1500字节
    int total_size = 0;             //记录响应信息字节数

    //创建套接字并获取套接字文件描述符
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) //第三个参数可为0
    {
        perror("Failed to create client's socket: ");
        return -1;
    }

    //初始化sockaddr服务端网络信息结构体
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    printf("\nResolving %s...", server); //所查询的服务端域名
    if ((hostname_to_ip(server, ip)) == -1)
    {
        fprintf(stderr, "Failed to get server's hostname to ip\n");
        return -1;
    }
    printf("%s", ip); //所查询的服务器域名对应的第一个IPv4地址

    serv_addr.sin_addr.s_addr = inet_addr(ip); //服务器的具体IP地址 将一个点分十进制的IP转换成一个长整数型数    必须指定
    serv_addr.sin_port = htons(43);            //端口号43为nicname	WHOIS目录服务

    //与服务端建立连接
    if (connect(socket_fd, (const struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) //可为(struct sockaddr *)
    {
        perror("Failed to establish connection: ");
        return -1;
    }

    //向服务端发送请求
    printf("\nQuerying for ... %s ...", request); //请求为所需查询的域名
    sprintf(message, "%s\r\n", request);          //特定格式
    if (send(socket_fd, message, strlen(message), 0) == -1)
    {
        perror("Faild to send request: ");
        return -1;
    }

    //接收服务端响应
    while (receive_size = recv(socket_fd, receive_buffer, sizeof(receive_buffer), 0)) //按照IP1500字节不断截断接收
    {
        *response = realloc(*response, receive_size + total_size); //重新分配响应内容所需的内存空间 原char **response=NULL;
        if (*response == NULL)
        {
            fprintf(stderr, "Failed to reallocate space\n");
        }
        memcpy(*response + total_size, receive_buffer, receive_size); //合并响应信息
        total_size += receive_size;                                   //记录响应信息字节数
    }
    printf("Done");

    if ((fflush(stdout)) == -1) //清空标准输出缓冲区
    {
        perror("Faied to flush stdout: ");
    }

    *response = realloc(*response, total_size + 1); //为响应信息添加结束符
    *(*response + total_size) = '\0';

    //关闭套接字
    close(socket_fd);

    return 0;
}

//将域名/主机名转换为IP地址
int hostname_to_ip(char *hostname, char *ip) //返回值类型为int用于检测错误
{
    //使用方式：http://c.biancheng.net/view/2357.html
    struct hostent *address;    //地址信息结构体
    struct in_addr **addr_list; // 32位IPv4地址结构   对应地址信息结构体中的"char **h_addr_list"成员
    int i;

    if ((address = gethostbyname(hostname)) == NULL)
    {
        herror("Faied to get host by name\n"); //使用gethostbyname()，不能用perror()打印错误信息（因为没有使用errno）
        return -1;                             //按照Unix/Linux系统函数定义方式：错误返回-1
    }

    addr_list = (struct in_addr **)address->h_addr_list; //强制类型转换  或 *(struct in_addr*)address

    for (i = 0; addr_list[i] != NULL; i++)
    {
        strcpy(ip, inet_ntoa(*addr_list[i])); //取一个域名可能对应的多个IP地址的第一个  将网络地址转换成“.”点隔的字符串格式
        return 0;
    }

    return 0;
}