//头文件————————————————————
#include <netinet/in.h> //sockaddr_in、htonl()、htons()
#include <sys/socket.h> //socket()、sendto()、recvfrom()
#include <stdio.h>      //perror()、fprintf()、putchar()
#include <stdlib.h>     //exit()、atoi()
#include <string.h>     //memset()、bzero()、strlen()、memcpy()
#include <libconfig.h>
// config_t、config_init()、config_read_file()、config_destroy()、config_lookup_string()、config_setting_t、
// config_lookup()、config_setting_length()、config_setting_get_elem()、config_setting_lookup_string()
#include <sys/stat.h> //umask()
#include <fcntl.h>    //open()、O_WRONLY、O_CREAT、O_APPEND
#include <unistd.h>   //write()、close()

//宏定义————————————————————
#define SERV_PORT 3333 //服务端端口
#define NUM_SIZE 4     //对象标识符各数字位数   一般不超过128共3位，包括标识符共4位
#define BUFF_SIZE 256  //发送接收缓冲区大小
#define VALUE_SIZE 128 //值大小<发送接收缓冲区大小

//结构体————————————————————
//TLV表示的BER编码
struct Tlv
{
    unsigned char tag;      //标签    1B够用
    unsigned char length;   //长度    1B能表示128B值长度够用
    char value[VALUE_SIZE]; //值    128B确定短格式，预先定义再填充
} Tlv;

//函数声明————————————————————
void handle(int sock_fd, struct sockaddr *serv_addr);
void loadavg_request(int sock_fd, struct sockaddr *serv_addr, const char *object_id);
void object_id_encode(const char *object_id, struct Tlv *oid);
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

    //操作  读取配置文件
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
//操作  读取配置文件
void handle(int sock_fd, struct sockaddr *serv_addr)
{
    //获取需请求信息，即对象标识符
    config_t cfg;              //全局配置结构体
    const char *str;           //值字符串
    config_setting_t *setting; //一个“列表、群组”配置
    int group_count = 0;       //一个列表的群组数量

    config_init(&cfg); //初始化

    if (!config_read_file(&cfg, "configuration.cfg")) //读文件
    {
        fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
                config_error_line(&cfg), config_error_text(&cfg));
        //(null):0 - file I/O error
        // configuration.cfg:4 - syntax error
        config_destroy(&cfg);
        return;
    }

    if (config_lookup_string(&cfg, "name", &str)) //读第一行配置
    {
        printf("File name: %s\n\n", str);
    }
    else
    {
        fprintf(stderr, "No 'name' setting in configuration file\n"); //不是必须内容，不退出程序
    }

    //对负载
    setting = config_lookup(&cfg, "content.loadavg"); //读列表配置
    if (setting != NULL)
    {
        printf("%s\n", "loadavg");
        printf("%-25s %-10s %-15s %s\n", "name", "object_id", "type", "description");

        group_count = config_setting_length(setting); //一个列表的群组数量

        for (int i = 0; i < group_count; ++i) //对每一个群组
        {
            config_setting_t *loadavg;                     //每一个群组配置
            loadavg = config_setting_get_elem(setting, i); //必存在 不设出错处理

            //一个群组的各键值对
            const char *name = NULL;        //名称
            const char *object_id = NULL;   //对象标识符
            const char *type = NULL;        //类型
            const char *description = NULL; //描述

            if (!(config_setting_lookup_string(loadavg, "name", &name) &&
                  config_setting_lookup_string(loadavg, "object_id", &object_id) &&
                  config_setting_lookup_string(loadavg, "type", &type) &&
                  config_setting_lookup_string(loadavg, "description", &description)))
            {
                continue; //无键值对则跳过
            }

            printf("%-25s %-10s %-15s %s\n", name, object_id, type, description);

            loadavg_request(sock_fd, serv_addr, object_id); //请求平均负载
        }
        putchar('\n');
    }
    else
    {
        fprintf(stderr, "No setting in configuration file\n");
    }

    config_destroy(&cfg);

    return;
}

// 请求平均负载
void loadavg_request(int sock_fd, struct sockaddr *serv_addr, const char *object_id)
{
    //创建请求消息
    struct Tlv oid; //对象标识符

    object_id_encode(object_id, &oid); //构造对象标识符 传地址

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

    // 处理响应
    handle_response(recv_buff);

    return;
}

//对象标识符编码
void object_id_encode(const char *object_id, struct Tlv *oid)
{
    oid->tag = 0x06; //标签 OBJECT IDENTIFIER，Universal6  "00 0 00110"="06"

    bzero(&(oid->value), sizeof(oid->value));

    //以下可扩展
    oid->length = 0x02; //长度

    //取对象标识符各数字
    int num_count = 1;        //存取数字计数
    char num1[NUM_SIZE] = ""; //赋值以让其有结束符
    char num2[NUM_SIZE] = "";
    char num3[NUM_SIZE] = "";
    int num_i = 0; //存取下标

    for (int i = 0; i < strlen(object_id); i++)
    {
        if (object_id[i] == '.')
        {
            num_count++;
            num_i = 0;
            continue;
        }

        if (num_count == 1)
        {
            num1[num_i] = object_id[i];
            num_i++;
        }
        else if (num_count == 2)
        {
            num2[num_i] = object_id[i];
            num_i++;
        }
        else if (num_count == 3)
        {
            num3[num_i] = object_id[i];
            num_i++;
        }
    }

    //转换为int数据类型
    int num1_int;
    int num2_int;
    int num3_int;

    num1_int = atoi(num1);
    num2_int = atoi(num2);
    num3_int = atoi(num3);

    //计算编码十进制数
    int value0 = 40 * num1_int + num2_int;
    int value1 = num3_int % 128;

    //转换编码十六进制数    值
    if (value0 == 41)
    {
        oid->value[0] = 0x29;
    }

    switch (value1)
    {
    case 1:
        oid->value[1] = 0x01;
        break;
    case 2:
        oid->value[1] = 0x02;
        break;
    case 3:
        oid->value[1] = 0x03;
        break;
    default:
        break;
    }

    return;
}

//处理响应
void handle_response(char recv_buff[])
{
    struct Tlv *ia5_String;               // IA5String
    ia5_String = (struct Tlv *)recv_buff; //数据从接收缓冲区拷出  结构体

    //保存文件
    int fd;                             //文件描述符
    char file_path[] = "./result2.txt"; //文件路径

    umask(0000);
    //临时设置0000，实际创建的文件权限即为mode值    否则创建文件仍只有用户可读写权限
    if ((fd = open(file_path, O_WRONLY | O_CREAT | O_APPEND, S_IRWXU | S_IRWXG | S_IRWXO)) < 0) //打开文件
    //只写    若文件不存在新建文件，存在附加
    //文件用户、用户组和其他用户具有可读、可写及可执行权限  00777
    {
        perror("Failed to open the result2 file\n");
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

