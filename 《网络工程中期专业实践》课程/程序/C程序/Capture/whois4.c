//头文件————————————————————
#include <pcap.h>  //pcap库相关
#include <ctype.h> //isprint()

#include <stdio.h>      //printf()、scanf()、puts()、perror()、fprintf()、sprintf()、fflush()
#include <unistd.h>     //free()、close()
#include <string.h>     //memset()、strlen()、memcpy()、strtok()、strstr()、strcpy()
#include <stdlib.h>     //realloc()、malloc()、EXIT_FAILURE
#include <sys/socket.h> //socket()、connect()、send()、recv()
#include <netinet/in.h> //sockaddr_in、htons()
#include <netdb.h>      //hostent、gethostbyname()、herror()
#include <arpa/inet.h>  //in_addr、inet_ntoa()、inet_addr()

#include <mysql/mysql.h> //在/usr/include/mysql下的mysql.h  mysqlclient库相关

//宏定义————————————————————
/* 802.11无线网的Radiotap帧头等封装大小 */
#define HEADER_80211_FRAME_SIZE 18                                   //存在Ridiotap header大小为21字节，显示信息会出错，不管
#define DATA_80211_FRAME_SIZE 26 /* header for 802.11 data packet */ //不是24，最后多2字节的"Qos Control"字段，无"address4"字段
#define LLC_HEADER_SIZE 8                                            /* LLC frame for encapsulation */

#define REQUESTFILE "/home/ricardo/桌面/Packets/req-300-01.pcap" //请求数据包文件路径  绝对路径

//结构体————————————————————
/* IP header */
struct sniff_ip
{
    u_char ip_vhl;                 /* version << 4 | header length >> 2 */
    u_char ip_tos;                 /* type of service */
    u_short ip_len;                /* total length */
    u_short ip_id;                 /* identification */
    u_short ip_off;                /* fragment offset field */
#define IP_RF 0x8000               /* reserved fragment flag */
#define IP_DF 0x4000               /* dont fragment flag */
#define IP_MF 0x2000               /* more fragments flag */
#define IP_OFFMASK 0x1fff          /* mask for fragmenting bits */
    u_char ip_ttl;                 /* time to live */
    u_char ip_p;                   /* protocol */
    u_short ip_sum;                /* checksum */
    struct in_addr ip_src, ip_dst; /* source and dest address */
};
#define IP_HL(ip) (((ip)->ip_vhl) & 0x0f)
#define IP_V(ip) (((ip)->ip_vhl) >> 4)

/* TCP header */
typedef u_int tcp_seq;
struct sniff_tcp
{
    u_short th_sport; /* source port */
    u_short th_dport; /* destination port */
    tcp_seq th_seq;   /* sequence number */
    tcp_seq th_ack;   /* acknowledgement number */
    u_char th_offx2;  /* data offset, rsvd */
#define TH_OFF(th) (((th)->th_offx2 & 0xf0) >> 4)
    u_char th_flags;
#define TH_FIN 0x01
#define TH_SYN 0x02
#define TH_RST 0x04
#define TH_PUSH 0x08
#define TH_ACK 0x10
#define TH_URG 0x20
#define TH_ECE 0x40
#define TH_CWR 0x80
#define TH_FLAGS (TH_FIN | TH_SYN | TH_RST | TH_ACK | TH_URG | TH_ECE | TH_CWR)
    u_short th_win; /* window */
    u_short th_sum; /* checksum */
    u_short th_urp; /* urgent pointer */
};

//函数声明————————————————————
void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);
void get_payload(const char *payload, int size_payload, char *payload_content);
void get_host(char *payload_content, char *host);

int whois_query(char *server, char *query, char **response);
int hostname_to_ip(char *hostname, char *ip);

void get_data_save(char *domain_ip, char *message);
void save_database(char *sql_statement);

//主函数————————————————————
int main(int argc, char **argv)
{
    char errbuf[PCAP_ERRBUF_SIZE]; /* error buffer */

    pcap_t *handle; /* packet capture handle */ // pcap_open_live()
    // pcap_compile()
    char filter_exp[] = "((tcp[((tcp[12:1] & 0xf0) >> 2):4] = 0x47455420) or (tcp[((tcp[12:1] & 0xf0) >> 2):4] = 0x504f5354) or (tcp[((tcp[12:1] & 0xf0) >> 2):4] = 0x48454144))"; /* filter expression [3] */ //过滤Get、Post和Head请求方法
    struct bpf_program fp;                                                                                                                                                                                     /* compiled filter program (expression) */

    int num_packets = 300; /* number of packets to capture */ // pcap_loop()

    /* open capture device */
    if ((handle = pcap_open_offline(REQUESTFILE, errbuf)) == NULL)
    {
        fprintf(stderr, "Couldn't open file: %s: %s\n", REQUESTFILE, errbuf);
        exit(EXIT_FAILURE);
    }

    /* make sure we're capturing on an BSD wireless with Radio tap header */
    if (pcap_datalink(handle) != DLT_IEEE802_11_RADIO)
    {
        fprintf(stderr, "%s is not an BSD wireless with Radio tap header\n", REQUESTFILE);
        exit(EXIT_FAILURE);
    }

    /* compile the filter expression */
    if (pcap_compile(handle, &fp, filter_exp, 0, PCAP_NETMASK_UNKNOWN) == -1)
    {
        fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
        exit(EXIT_FAILURE);
    }

    /* apply the compiled filter */
    if (pcap_setfilter(handle, &fp) == -1)
    {
        fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
        exit(EXIT_FAILURE);
    }

    /* now we can set our callback function */
    pcap_loop(handle, num_packets, got_packet, NULL);

    /* cleanup */
    pcap_freecode(&fp);
    pcap_close(handle);

    printf("\nQuery WHOIS complete\n");

    return 0;
}

//函数定义————————————————————
/* dissect/print packet */
void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
    static int count = 1; /* packet counter */

    /* declare pointers to packet headers */
    const struct sniff_ip *ip;   /* The IP header */
    const struct sniff_tcp *tcp; /* The TCP header */
    const char *payload;         /* Packet payload */

    int size_ip;
    int size_tcp;
    int size_payload;

    printf("\nPacket number %d:\n", count);
    count++;

    /* define/compute ip header offset */
    ip = (struct sniff_ip *)(packet + HEADER_80211_FRAME_SIZE + DATA_80211_FRAME_SIZE + LLC_HEADER_SIZE);
    size_ip = IP_HL(ip) * 4;
    if (size_ip < 20)
    {
        if (size_ip < 20)
        {
            printf("   * Invalid IP header length: %u bytes\n", size_ip);
            return;
        }
    }

    /*	 *  OK, this packet is TCP.	 */
    /* define/compute tcp header offset */
    tcp = (struct sniff_tcp *)(packet + HEADER_80211_FRAME_SIZE + DATA_80211_FRAME_SIZE + LLC_HEADER_SIZE + size_ip);
    size_tcp = TH_OFF(tcp) * 4;
    if (size_tcp < 20)
    {
        if (size_tcp < 20)
        {
            printf("   * Invalid TCP header length: %u bytes\n", size_tcp);
            return;
        }
    }

    /* define/compute tcp payload (segment) offset */
    payload = (u_char *)(packet + HEADER_80211_FRAME_SIZE + DATA_80211_FRAME_SIZE + LLC_HEADER_SIZE + size_ip + size_tcp);

    /* compute tcp payload (segment) size */
    size_payload = ntohs(ip->ip_len) - (size_ip + size_tcp);

    char payload_content[2000] = ""; //载荷
    get_payload(payload, size_payload, payload_content);

    // printf("载荷内容:%s\n", payload_content);

    char host[100] = "";
    get_host(payload_content, host);

    // printf("这是Host内容：%s\n", host);

    //查询WHOIS
    char *server = "whois.krnic.net"; //所查询的服务端域名
    char domain_ip[100] = "";         //所查询的域名或IP地址 数组大小：域名100IP20
    char *message = NULL;             //查询到的信息    指针同数组

    // 处理域名
    if ((host[0] == 'w') && (host[1] == 'w') && (host[2] == 'w'))
    {
        strcpy(domain_ip, host + 4); //去"www." 指针直接加即可
    }
    else //存在不以"www."开头的内容，如"shuc-pc-hunt.ksord.com"
    {
        strcpy(domain_ip, host);
    }

    if ((whois_query(server, domain_ip, &message)) == -1) //查询
    {
        fprintf(stderr, "Faied to query WHOIS\n");
    }

    printf("\n\n"); //输出2.查询内容
    puts(message);

    //取域名所注册的单位、联系人及联系电话，保存数据库
    get_data_save(domain_ip, message);

    free(message); //释放指针

    return;
}

/* 取载荷   不能返回一个局部变量的地址->将字符串指针作为函数参数传入，并返回该指针->传指针，返回值为空 */
void get_payload(const char *payload, int size_payload, char *payload_content)
{
    /* ascii (if printable) */
    int i = 0;
    int len = size_payload;
    const u_char *ch = payload;

    for (i = 0; i < len; i++) //取载荷
    {
        if (isprint(*ch))
        {
            payload_content[i] = *ch;
        }
        else
        {
            payload_content[i] = '.';
        }
        ch++;
    }
    payload_content[i] = '\0';
}

/* 取Host */
void get_host(char *payload_content, char *host)
{
    int i = 0;
    int j = 0;
    int host_flag = 0;

    for (i = 0; i < strlen(payload_content); i++) //取host
    {
        if (payload_content[i] == 'H' && payload_content[i + 1] == 'o' && payload_content[i + 2] == 's' && payload_content[i + 3] == 't')
        {
            host_flag = 1;
            i = i + 6; //"Host: "
        }

        if (payload_content[i] == '.' && payload_content[i + 1] == '.') //即\r\n
        {
            host_flag = 0;
        }

        if (host_flag == 1)
        {
            host[j] = payload_content[i];
            j++;
        }
    }
}

/* WHOIS网络连接查询
//使用网络套接字执行WHOIS查询，输出1.过程信息，获取响应内容 */
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

/* 将域名/主机名转换为IP地址 */
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

//取域名所注册的单位、联系人及联系电话  可扩展记录字段
void get_data_save(char *domain_ip, char *message)
{
    char *temp_char = NULL;               //使用strtok()分解的中间字符
    char *domain_name = NULL;             //记录"Domain Name"字段内容
    char *registrant_organization = NULL; //记录"Registrant Organization"
    char *registrar = NULL;               //记录"Registrar"
    char *registrar_phone = NULL;         //记录"Registrar Abuse Contact Phone"

    //只使用strstr()会将所查询内容改，判断是否调用  有的字段可能没有，每字段一个标志
    int domain_name_flag = 0;
    int registrant_organization_flag = 0;
    int registrar_flag = 0;
    int registrar_phone_flag = 0;

    //处理信息
    temp_char = strtok(message, "\n"); // 1.函数经典用法
    while (temp_char != NULL)          //循环截断，查找字段
    {
        // printf("这是%s\n",temp_char);
        if (domain_name_flag == 0)
        {
            domain_name = strstr(temp_char, "Domain Name: "); //取字段  strstr()：返回字符串中首次出现子串的地址
            if (domain_name != NULL)
            {
                domain_name_flag = 1;
            }
        }
        if (registrant_organization_flag == 0)
        {
            registrant_organization = strstr(temp_char, "Registrant Organization: ");
            if (registrant_organization != NULL)
            {
                registrant_organization_flag = 1;
            }
        }
        if (registrar_flag == 0)
        {
            registrar = strstr(temp_char, "Registrar: ");
            if (registrar != NULL)
            {
                registrar_flag = 1;
            }
        }
        if (registrar_phone_flag == 0)
        {
            registrar_phone = strstr(temp_char, "Registrar Abuse Contact Phone: ");
            if (registrar_phone != NULL)
            {
                registrar_phone_flag = 1;
            }
        }

        temp_char = strtok(NULL, "\n"); // 2.函数经典用法
    }

    // printf("以下是保存到数据库的信息：\n%s\n%s\n%s\n%s\n", domain_name, registrant_organization, registrar, registrar_phone);

    //去前缀键、末尾句号取值
    if (domain_name != NULL)
    {
        domain_name = domain_name + 13; //"Domain Name: "
    }
    else
    {
        domain_name = domain_ip; //显示声明，方便SQL语句拼接   无说明未查询到信息，使用查询的关键字存数据库
    }
    if (registrant_organization != NULL)
    {
        registrant_organization = registrant_organization + 25; //"Registrant Organization: "
    }
    else
    {
        registrant_organization = "none";
    }
    if (registrar != NULL)
    {
        registrar = registrar + 11; //"Registrar: "
    }
    else
    {
        registrar = "none";
    }
    if (registrar_phone != NULL)
    {
        registrar_phone = registrar_phone + 31; //"Registrar Abuse Contact Phone: "
    }
    else
    {
        registrar_phone = "none";
    }

    // printf("以下是保存到数据库的信息：\n%s\n%s\n%s\n%s\n", domain_name, registrant_organization, registrar, registrar_phone);

    // SQL语句  段错误可能是数组开小了
    char sql_statement[2000] = "insert into whois(domain_name,registrant_organization,registrar,registrar_phone) values('";

    strcat(sql_statement, domain_name);
    strcat(sql_statement, "','");
    strcat(sql_statement, registrant_organization);
    strcat(sql_statement, "','");
    strcat(sql_statement, registrar);
    strcat(sql_statement, "','");
    strcat(sql_statement, registrar_phone);
    strcat(sql_statement, "')");

    // printf("%s\n", sql_statement);

    //保存数据库
    save_database(sql_statement);

    return;
}

/* 保存数据库 */
void save_database(char *sql_statement)
{
    MYSQL connect_fd; //数据库连接文件描述符

    if ((mysql_init(&connect_fd)) == NULL) //初始化数据库连接文件描述符
    {
        fprintf(stderr, "Failed to init the connect file descriptor\n");
        exit(EXIT_FAILURE);
    }

    if ((mysql_real_connect(&connect_fd, "localhost", "root", "123456", "packets", 0, NULL, 0)) == NULL) //连接
    {
        fprintf(stderr, "Failed to connect the database: %d: %s\n", mysql_errno(&connect_fd),
                mysql_error(&connect_fd));
        exit(EXIT_FAILURE);
    }

    if (mysql_set_character_set(&connect_fd, "utf8") != 0) //设置字符集
    {
        fprintf(stderr, "Failed to set character: %s/n", mysql_error(&connect_fd));
    }

    //执行
    if (mysql_query(&connect_fd, sql_statement) != 0)
    {
        fprintf(stderr, "Failed to execute the sql statement: %d: %s\n", mysql_errno(&connect_fd),
                mysql_error(&connect_fd));
        // exit(EXIT_FAILURE);
        return; //存在相同域名的重复查询存储，使用域名作数据表主键，跳过而不是退出程序
    }
    printf("Updated %lu rows\n", (unsigned long)mysql_affected_rows(&connect_fd));

    mysql_close(&connect_fd); //关闭连接 无返回值

    printf("Database saved successfully\n");
}