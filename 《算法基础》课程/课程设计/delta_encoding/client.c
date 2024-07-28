//头文件————————————————————
#include <netinet/in.h> //sockaddr_in、htonl()、htons()
#include <sys/socket.h> //socket()、connect()、send()、recv()
#include <stdio.h>      //perror()
#include <stdlib.h>     //exit()、EXIT_FAILURE
#include <string.h>     //bzero()、memcpy()、strlen()、strncat()、strcat()
#include <unistd.h>     //close()、read()、ftruncate()、write()
#include <fcntl.h>      //opon()

//宏————————————————————
#define SERV_PORT 3221   //服务器端口
#define BUFF_SIZE 150000 //传递消息缓冲区大小
// const char file_path[] = "./text.txt"; //文本文件的文件路径
// const int text_size = 20000;           //文本大小
// const int bit_width = 5;         //编码位宽   从1开始，取值1-8
// char bit_width_code[4] = "";          // 1.编码位宽编码    由取值1-8，最多3位 预留结束符 3
// char instr_length_code[3] = ""; //2.指示长度编码   最多2位   2
// char length_code[17] = "";                 //3.长度编码  16
// char trip_code[strlen(text_content) + 1]; //4.行程编码   记录异或增量编码是否是离群值    20000
// char xor_delta_code[120000] = "";                //5.异或增量编码
// char code_sum[150000] = ""; //编码和
// char str_code_length[10]; //字符型编码长度
// const char data_file_path[] = "./client_data.txt"; //数据文本文件的文件路径
// char data[300000] = ""; //数据

//函数声明————————————————————
void xor_delta_coding(); //异或增量编码

//主函数————————————————————
int main(int argc, char *argv[])
{
    xor_delta_coding(); //异或增量编码

    return 0;
}

//函数定义————————————————————
//异或增量编码
void xor_delta_coding()
{
    //网络连接————————————————————
    int sock_fd; //套接字文件描述符

    //创建套接字并获取套接字文件描述符
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Failed to create the client's socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serv_addr; //服务端网络信息结构体

    //初始化服务端网络信息结构体
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERV_PORT);

    //与服务端建立连接
    if ((connect(sock_fd, (struct sockaddr *)(&serv_addr), sizeof(serv_addr))) == -1)
    {
        close(sock_fd);

        perror("Failed to establish connection");
        exit(EXIT_FAILURE);
    }

    //获取文本文件内容————————————————————
    const char file_path[] = "./text.txt"; //文本文件的文件路径
    int text_file_fd;                      //文本文件的文件描述符
    const int text_size = 20000;           //文本大小
    char text_content[text_size + 1];      //文本内容
    int read_bytes;                        //读取文本文件的字节数

    if ((text_file_fd = open(file_path, O_RDONLY)) == -1) //打开文本文件
    {
        perror("Failed to open the text file");
        exit(EXIT_FAILURE);
    }

    if ((read_bytes = read(text_file_fd, text_content, text_size)) <= 0) //读取文本文件
    {
        perror("Failed to read the text file");
        exit(EXIT_FAILURE);
    }
    text_content[read_bytes] = '\0'; //添加结束符

    close(text_file_fd); //关闭文本文件的文件描述符

    //编码————————————————————
    // 1.编码位宽编码
    const int bit_width = 5;         //编码位宽   从1开始，取值1-8
    int bit_width_value = bit_width; //用于转换成二进制的编码位宽值 会动态改变
    char bit_width_code[4] = "";     // 1.编码位宽编码    由取值1-8，最多3位 预留结束符

    //转换成二进制数
    int remainder;        //余数
    char ch_remainder;    //字符型余数
    int binary_num_i = 0; //存放二进制数的位置  从左到右为低到高位

    while (bit_width_value != 0)
    {
        remainder = bit_width_value % 2; //取余数

        ch_remainder = remainder + '0';              //转换成字符型余数
        bit_width_code[binary_num_i] = ch_remainder; //存二进制数
        binary_num_i++;

        bit_width_value = bit_width_value / 2; //更新值
    }

    for (int i = binary_num_i; i < 3; i++) //不足3位补0
    {
        bit_width_code[i] = '0';
    }
    bit_width_code[3] = '\0'; //添加结束符

    // 2.指示长度编码和3.长度编码
    int text_length = strlen(text_content); //文本长度、行程码长度
    char length_code[17] = "";              //长度编码

    //转换成二进制数
    binary_num_i = 0; //存放二进制数的位置  从左到右为低到高位

    while (text_length != 0)
    {
        remainder = text_length % 2; //取余数

        ch_remainder = remainder + '0';           //转换成字符型余数
        length_code[binary_num_i] = ch_remainder; //存二进制数
        binary_num_i++;

        text_length = text_length / 2; //更新值
    }

    text_length = strlen(text_content); //计算完后恢复值

    if (binary_num_i <= 8) //若够8位，则binary_num_i++;后binary_num_i值为8
    {
        for (int i = binary_num_i; i < 8; i++) //不足8位补0
        {
            length_code[i] = '0';
        }
        length_code[8] = '\0';
    }
    else if (8 < binary_num_i <= 16) //若超8位，则binary_num_i++;后binary_num_i值为9
    {
        for (int i = binary_num_i; i < 16; i++) //不足16位补0
        {
            length_code[i] = '0';
        }
        length_code[16] = '\0';
    }

    int instr_length;               //指示长度   指示后面有几个字节是长度编码
    char instr_length_code[3] = ""; //指示长度编码   最多2位

    // if (0 <= text_length <= 255)//不能这么写！傻逼了
    if (0 <= text_length && text_length <= 255)
    {
        instr_length = 1;
    }
    else if (256 <= text_length && text_length <= 65535)
    {
        instr_length = 2;
    }

    //转换成二进制数
    binary_num_i = 0; //存放二进制数的位置  从左到右为低到高位

    while (instr_length != 0)
    {
        remainder = instr_length % 2; //取余数

        ch_remainder = remainder + '0';                 //转换成字符型余数
        instr_length_code[binary_num_i] = ch_remainder; //存二进制数
        binary_num_i++;

        instr_length = instr_length / 2; //更新值
    }

    for (int i = binary_num_i; i < 2; i++) //不足2位补0
    {
        instr_length_code[i] = '0';
    }
    instr_length_code[2] = '\0';

    // 4.行程编码和5.异或增量编码
    char trip_code[strlen(text_content) + 1]; //行程编码   记录异或增量编码是否是离群值
    char xor_delta_code[120000] = "";         //异或增量编码
    char last_code[9] = "";                   //前一个编码  是异或值

    for (int i = 0; i < strlen(text_content); i++) //对每一字符
    {
        int ascii; //字符的ASCII码

        ascii = (int)text_content[i]; //转换成ASCII码

        //转换成二进制数
        char binary_num[9]; //二进制数 ASCII码最多8位
        binary_num_i = 0;   //存放二进制数的位置  从左到右为低到高位  注意要每循环置位

        while (ascii != 0)
        {
            remainder = ascii % 2; //取余数

            ch_remainder = remainder + '0';          //转换成字符型余数
            binary_num[binary_num_i] = ch_remainder; //存二进制数
            binary_num_i++;

            ascii = ascii / 2; //更新值
        }

        for (int j = binary_num_i; j < 8; j++) //不足8位补0
        {
            binary_num[j] = '0';
        }
        binary_num[8] = '\0'; //添加结束符

        if (i == 0) //第一个数直接编码
        {
            trip_code[i] = '0';                    //行程编码
            memcpy(xor_delta_code, binary_num, 8); //异或增量编码

            memcpy(last_code, binary_num, 8); //记录前一个编码
            last_code[8] = '\0';

            continue;
        }

        //其他数异或增量编码
        char xor_value[9] = ""; //异或值

        //取异或值
        for (int j = 0; j < 8; j++)
        {
            if (binary_num[j] == last_code[j])
            {
                xor_value[j] = '0';
            }
            else
            {
                xor_value[j] = '1';
            }
        }
        xor_value[8] = '\0'; //添加结束符

        //判断是否是离群值：异或值的大于编码位宽位置是否存在二进制编码1 0否1是
        int outlier_flag = 0; //离群值标记 0否1是

        for (int j = bit_width; j < 8; j++)
        {
            if (xor_value[j] == '1') //是
            {
                outlier_flag = 1;

                break;
            }
        }

        if (outlier_flag == 0) //不是离群值
        {
            trip_code[i] = '0'; //行程编码为0

            strncat(xor_delta_code, xor_value, bit_width); //截取编码位宽数加入异或增量编码

            memcpy(last_code, binary_num, 8); //记录前一个编码
            last_code[8] = '\0';
        }
        else //是离群值
        {
            trip_code[i] = '1'; //行程编码为1

            strncat(xor_delta_code, xor_value, 8); //直接加入异或增量编码

            memcpy(last_code, binary_num, 8); //记录前一个编码
            last_code[8] = '\0';
        }
    }

    trip_code[strlen(text_content)] = '\0'; //添加结束符
    xor_delta_code[strlen(xor_delta_code)] = '\0';

    //获取编码
    char code_sum[150000] = ""; //编码和

    strncat(code_sum, bit_width_code, strlen(bit_width_code));
    strncat(code_sum, instr_length_code, strlen(instr_length_code));
    strncat(code_sum, length_code, strlen(length_code));
    strncat(code_sum, trip_code, strlen(trip_code));
    strncat(code_sum, xor_delta_code, strlen(xor_delta_code));
    code_sum[strlen(code_sum)] = '\0';

    //获取压缩率
    double compre_ratio = 0.0;

    compre_ratio = 1.0 * strlen(code_sum) / (strlen(text_content) * 8) * 100;
    //转换为浮点数，不让计算结果四舍五入成整数

    //数据传输————————————————————
    //发送数据
    char msg_send[BUFF_SIZE]; //发送服务端消息缓冲区

    bzero(&msg_send, sizeof(*msg_send));

    memcpy(msg_send, code_sum, BUFF_SIZE);

    if ((send(sock_fd, msg_send, BUFF_SIZE, 0)) == -1) //发送数据
    {
        close(sock_fd);

        perror("Failed to send messages to the server");
        exit(EXIT_FAILURE);
    }
    // send()一次总会传BUFF_SIZE个字节

    //接收数据
    char msg_recv_temp[BUFF_SIZE]; //接收服务端消息缓冲区
    int recv_bytes;                //一次接收的字节数
    char msg_recv[BUFF_SIZE];      //接收服务端消息缓冲区
    int total_bytes = 0;           //接收的总字节数

    bzero(&msg_recv_temp, sizeof(*msg_recv_temp));
    bzero(&msg_recv, sizeof(*msg_recv));

    while (1) //循环接收数据
    {
        recv_bytes = read(sock_fd, msg_recv_temp, BUFF_SIZE); //接收

        if (recv_bytes >= 0) //有数据
        {
            strncat(msg_recv, msg_recv_temp, recv_bytes); //拷入接收服务端消息缓冲区
        }
        else //出错
        {
            close(sock_fd);

            perror("Failed to receive messages from the server");
            exit(EXIT_FAILURE);
        }

        total_bytes = total_bytes + recv_bytes; //更新接收的总字节数
        if (total_bytes == BUFF_SIZE)           //接收完数据    recv()总会接收BUFF_SIZE个字节
        {
            break;
        }
    }
    strcat(msg_recv, "\0");
    //若设置大容量缓冲区，用if一次接收，大文件传输一次接收不完全。必须循环接收
    //在接收时，数据字符串不一定有结束符，不能用strlen(msg_recv)测接收长度
    //所以返回值recv_bytes和strlen(msg_recv)会不同

    // printf("接收的消息：%s\n", msg_recv); //接收的消息

    //显示————————————————————
    // printf("文本内容：%s\n\n", text_content);
    printf("文本长度：%ld\n", strlen(text_content));
    // printf("位宽编码：%s\n\n", bit_width_code);
    // printf("指示长度编码：%s\n\n", instr_length_code);
    // printf("长度编码：%s\n\n", length_code);
    // printf("行程编码：%s\n\n", trip_code);
    // printf("异或增量编码：%s\n\n", xor_delta_code);
    // printf("编码和：%s\n\n", code_sum);
    printf("基本编码长度：%ld\n", strlen(text_content) * 8);
    printf("异或增量编码长度：%ld\n", strlen(code_sum));
    printf("压缩率：%f%%\n", compre_ratio);

    //数据存入文本文件————————————————————
    //拼接数据
    char data[380000] = ""; //数据

    strcat(data, "text content:————————————————————\n");
    strcat(data, text_content);
    strcat(data, "\n\n");

    strcat(data, "bit width code:————————————————————\n");
    strcat(data, bit_width_code);
    strcat(data, "\n\n");

    strcat(data, "instruction length code:————————————————————\n");
    strcat(data, instr_length_code);
    strcat(data, "\n\n");

    strcat(data, "length code:————————————————————\n");
    strcat(data, length_code);
    strcat(data, "\n\n");

    strcat(data, "trip code:————————————————————\n");
    strcat(data, trip_code);
    strcat(data, "\n\n");

    strcat(data, "xor delta code:————————————————————\n");
    strcat(data, xor_delta_code);
    strcat(data, "\n\n");

    strcat(data, "code sum:————————————————————\n");
    strcat(data, code_sum);
    strcat(data, "\n\n");

    strcat(data, "receive messages:————————————————————\n");
    strcat(data, msg_recv);

    data[strlen(data)] = '\0';

    //数据存入文本文件
    const char data_file_path[] = "./client_data.txt"; //数据文本文件的文件路径

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
    close(sock_fd);

    return;
}