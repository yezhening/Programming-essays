//头文件————————————————————
#include <netinet/in.h> //sockaddr_in、htonl()、htons()
#include <sys/socket.h> // socket()、setsockopt()、bind()、listen()、accept()、recv()、send()
#include <stdio.h>      //perror()
#include <stdlib.h>     //exit()、EXIT_FAILURE
#include <string.h>     //bzero()、memcpy()、strcat()
#include <unistd.h>     //close()、ftruncate()、write()
#include <math.h>       //pow()
#include <fcntl.h>      //opon()

//宏————————————————————
#define SERV_PORT 3221   //服务端端口号
#define BUFF_SIZE 150000 //传递消息缓冲区大小
// char str_code_length[10]; //字符型编码长度
// char bit_width_code[4] = "";          // 1.编码位宽编码    由取值1-8，最多3位 预留结束符
// char instr_length_code[3] = ""; //2.指示长度编码   最多2位
// char length_code[17] = "";                 //3.长度编码
// char trip_code[text_length + 1]; //行程编码   记录异或增量编码是否是离群值   20000
// char xor_delta_code[120000] = "";                //5.异或增量编码
// char code_sum[150000] = ""; //编码和 无
// const char data_file_path[] = "./server_data.txt"; //数据文本文件的文件路径
// char data[300000] = ""; //数据

//函数声明————————————————————
void xor_delta_decoding(); //异或增量解码

//主函数————————————————————
int main(int arg, char *argv[])
{
    xor_delta_decoding();

    return 0;
}

//函数定义————————————————————
//异或增量解码
void xor_delta_decoding()
{
    //网络连接————————————————————
    int listen_fd; //监听套接字文件描述符

    //创建套接字,获取套接字文件描述符
    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Failed to create the server's socket");
        exit(EXIT_FAILURE);
    }

    //设置套接字选项为可重用本地地址
    int reuse = 1;

    if ((setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse))) == -1)
    {
        close(listen_fd);

        perror("Failed to set the socket's options");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serv_addr; //服务端网络信息结构体

    //初始化服务端网络信息结构体
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERV_PORT);

    //绑定套接字与网络信息
    if ((bind(listen_fd, (struct sockaddr *)(&serv_addr), sizeof(serv_addr))) == -1)
    {
        close(listen_fd);

        perror("Failed to bind the socket");
        exit(EXIT_FAILURE);
    }

    //套接字设置被动监听状态
    if ((listen(listen_fd, 1)) == -1)
    {
        close(listen_fd);

        perror("Failed to configure the socket's listening status");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in clie_addr; //客户端网络信息结构体
    int addr_size;                //网络信息结构体大小
    int connect_fd;               //连接套接字文件描述符

    bzero(&clie_addr, sizeof(clie_addr));
    addr_size = sizeof(struct sockaddr);

    //与客户端建立连接
    if ((connect_fd = accept(listen_fd, (struct sockaddr *)(&clie_addr), &addr_size)) == -1)
    {
        close(connect_fd);
        close(listen_fd);

        perror("Failed to accept connection");
        exit(EXIT_FAILURE);
    }

    //数据传输————————————————————
    char msg_recv_temp[BUFF_SIZE]; //接收客户端消息临时缓冲区
    int recv_bytes;                //一次接收的字节数
    char msg_recv[BUFF_SIZE];      //接收客户端消息缓冲区
    int total_bytes = 0;           //接收的总字节数

    bzero(&msg_recv_temp, sizeof(*msg_recv_temp));
    bzero(&msg_recv, sizeof(*msg_recv));

    while (1) //循环接收数据
    {
        recv_bytes = recv(connect_fd, msg_recv_temp, BUFF_SIZE, 0); //接收

        if (recv_bytes >= 0) //有数据
        {
            strncat(msg_recv, msg_recv_temp, recv_bytes); //拷入接收客户端消息缓冲区
        }
        else //出错
        {
            close(connect_fd);
            close(listen_fd);

            perror("Failed to receive messages from the client");
            exit(EXIT_FAILURE);
        }

        total_bytes = total_bytes + recv_bytes; //更新接收的总字节数
        if (total_bytes == BUFF_SIZE)           //接收完数据    接收到编码长度个字节停止接收
        {
            break;
        }
    }
    strcat(msg_recv, "\0");
    //若设置大容量缓冲区，用if一次接收，大文件传输一次接收不完全。必须循环接收
    //在接收时，数据字符串不一定有结束符，不能用strlen(msg_recv)测接收长度
    //所以返回值recv_bytes和strlen(msg_recv)会不同

    // printf("接收的消息：%s\n\n", msg_recv); //接收的消息

    //获取编码信息————————————————————
    // 1.获取编码位宽
    char bit_width_code[4] = ""; // 1.编码位宽编码    由取值1-8，最多3位 预留结束符

    memcpy(bit_width_code, msg_recv, 3); // 1.编码位宽编码
    bit_width_code[3] = '\0';

    //二进制转十进制
    int int_binary_num; //整型二进制数
    int bit_width = 0;  //编码位宽

    for (int i = 0; i < 3; i++)
    {
        int_binary_num = bit_width_code[i] - '0';
        bit_width = bit_width + int_binary_num * pow(2, i); //编码位宽
    }

    // 2.获取指示长度编码
    char instr_length_code[3] = ""; //指示长度编码   最多2位

    memcpy(instr_length_code, msg_recv + 3, 2); //指示长度编码
    instr_length_code[2] = '\0';

    //二进制转十进制
    int instr_length = 0; //指示长度

    for (int i = 0; i < 2; i++)
    {
        int_binary_num = instr_length_code[i] - '0';
        instr_length = instr_length + int_binary_num * pow(2, i); //指示长度
    }

    // 2.获取长度编码
    char length_code[17] = ""; //长度编码

    if (instr_length == 1)
    {
        memcpy(length_code, msg_recv + 5, 8); //长度编码
        length_code[8] = '\0';
    }
    else if (instr_length == 2)
    {
        memcpy(length_code, msg_recv + 5, 16);
        length_code[16] = '\0';
    }

    //二进制转十进制
    int text_length = 0; //文本/行程编码长度

    for (int i = 0; i < strlen(length_code); i++)
    {
        int_binary_num = length_code[i] - '0';
        text_length = text_length + int_binary_num * pow(2, i); //文本/行程编码长度
    }

    // 4.获取行程编码
    char trip_code[text_length + 1]; //行程编码   记录异或增量编码是否是离群值

    if (instr_length == 1)
    {
        memcpy(trip_code, msg_recv + 13, text_length); //行程编码
        trip_code[text_length] = '\0';
    }
    else if (instr_length == 2)
    {
        memcpy(trip_code, msg_recv + 21, text_length);
        trip_code[text_length] = '\0';
    }

    // 5.获取异或增量编码
    char xor_delta_code[120000] = ""; //异或增量编码

    if (instr_length == 1)
    {
        memcpy(xor_delta_code, msg_recv + 13 + text_length, strlen(msg_recv) - 13 - text_length); //异或增量编码
        xor_delta_code[strlen(xor_delta_code)] = '\0';
    }
    else if (instr_length == 2)
    {
        memcpy(xor_delta_code, msg_recv + 21 + text_length, strlen(msg_recv) - 21 - text_length);
        xor_delta_code[strlen(xor_delta_code)] = '\0';
    }

    //解码————————————————————
    //获取第一个字符
    char binary_num[9] = ""; //字符的二进制数
    char last_code[9] = "";  //前一个字符的二进制编码

    memcpy(binary_num, xor_delta_code, 8); //第一个字符的二进制编码
    binary_num[8] = '\0';

    memcpy(last_code, xor_delta_code, 8); //记录前一个字符的二进制编码
    last_code[8] = '\0';

    //二进制转十进制
    int ascii = 0; //字符的ASCII码

    for (int i = 0; i < strlen(binary_num); i++)
    {
        int_binary_num = binary_num[i] - '0';
        ascii = ascii + int_binary_num * pow(2, i); //字符的ASCII码
    }

    // ASCII码转字符
    const int text_size = 32768;      //文本大小
    char text_content[text_size + 1]; //文本内容
    int text_content_pos = 0;         //遍历的文本内容位置

    text_content[text_content_pos] = (char)ascii;
    text_content_pos = text_content_pos + 1;

    //获取其他字符
    char xor_value[9] = "";     //异或值
    int xor_delta_code_pos = 8; //遍历的异或增量编码位置

    for (int i = 1; i < strlen(trip_code); i++) //行程码字符数即其他字符数
    {
        //由行程码判断取字符的几位二进制数
        if (trip_code[i] == '0') //不是离群值
        {
            memcpy(xor_value, xor_delta_code + xor_delta_code_pos, bit_width); //取编码位宽位二进制数
            xor_value[bit_width] = '\0';

            for (int j = bit_width; j < 8; j++) //不够8位补0
            {
                xor_value[j] = '0';
            }

            xor_delta_code_pos = xor_delta_code_pos + bit_width; //更新遍历的异或增量编码位置
        }
        else if (trip_code[i] == '1') //是离群值
        {
            memcpy(xor_value, xor_delta_code + xor_delta_code_pos, 8); //取8位二进制数
            xor_value[8] = '\0';

            xor_delta_code_pos = xor_delta_code_pos + 8; //更新遍历的异或增量编码位置
        }

        //获取字符二进制编码
        for (int j = 0; j < 8; j++)
        {
            if (xor_value[j] == last_code[j])
            {
                binary_num[j] = '0';
            }
            else
            {
                binary_num[j] = '1';
            }
        }
        binary_num[8] = '\0';

        //二进制转十进制
        ascii = 0;

        for (int j = 0; j < strlen(binary_num); j++)
        {
            int_binary_num = binary_num[j] - '0';
            ascii = ascii + int_binary_num * pow(2, j); //字符的ASCII码
        }

        memcpy(last_code, binary_num, 8); //记录前一个字符的二进制编码
        last_code[8] = '\0';

        // ASCII码转字符
        text_content[text_content_pos] = (char)ascii;
        text_content_pos = text_content_pos + 1;
    }

    text_content[strlen(text_content)] = '\0';

    //数据传输
    char msg_send[BUFF_SIZE]; //发送客户端消息缓冲区

    bzero(&msg_send, sizeof(*msg_send));

    memcpy(msg_send, text_content, BUFF_SIZE);

    if ((send(connect_fd, msg_send, BUFF_SIZE, 0)) == -1) //发送数据
    {
        perror("Failed to send messages to the client");
        exit(EXIT_FAILURE);
    }

    //显示————————————————————
    printf("位宽：%d\n", bit_width);
    printf("指示长度：%d\n", instr_length);
    printf("文本/行程编码长度：%d\n", text_length);
    // printf("行程编码：%s\n\n", trip_code);
    // printf("异或增量编码：%s\n\n", xor_delta_code);
    // printf("文本内容：%s\n", text_content);

    //数据存入文本文件————————————————————
    //拼接数据
    char data[300000] = ""; //数据

    strcat(data, "receive messages:————————————————————\n");
    strcat(data, msg_recv);
    strcat(data, "\n\n");

    strcat(data, "text content:————————————————————\n");
    strcat(data, text_content);

    data[strlen(data)] = '\0';

    //数据存入文本文件
    const char data_file_path[] = "./server_data.txt"; //数据文本文件的文件路径
    int text_file_fd;                                  //数据文本文件的文件描述符

    if ((text_file_fd = open(data_file_path, O_WRONLY)) == -1) //打开文本文件   只写
    {
        perror("Failed to open the data text file");
        exit(EXIT_FAILURE);
    }

    ftruncate(text_file_fd, 0); //清空文件

    if ((write(text_file_fd, data, strlen(data))) <= 0) //写入文本文件
    {
        perror("Failed to write the data text file");
        exit(EXIT_FAILURE);
    }

    close(text_file_fd); //关闭文本文件的文件描述符

    //关闭套接字文件描述符
    close(connect_fd);
    close(listen_fd);

    return;
}