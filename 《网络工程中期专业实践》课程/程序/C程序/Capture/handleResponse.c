//头文件————————————————————
#include <pcap.h> //pcap库相关
#include <stdio.h>
#include <string.h> //strcat()、strcpy()、strlen()、strcmp()
#include <stdlib.h> //EXIT_FAILURE
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <fcntl.h>       //O_WRONLY | O_CREAT
#include <unistd.h>      //write()、close()
#include <mysql/mysql.h> //在/usr/include/mysql下的mysql.h  mysqlclient库相关

//宏定义————————————————————
/* 802.11无线网的Radiotap帧头等封装大小 */
#define HEADER_80211_FRAME_SIZE 21                                   //存在Ridiotap header大小为21字节
#define DATA_80211_FRAME_SIZE 26 /* header for 802.11 data packet */ //不是24，最后多2字节的"Qos Control"字段，无"address4"字段
#define LLC_HEADER_SIZE 8                                            /* LLC frame for encapsulation */

#define RESPONSEFILE "/home/ricardo/桌面/Packets/res-100-01.pcap" //响应数据包文件路径  绝对路径
#define PAYLOAD_FIFE_PATH "/home/ricardo/桌面/ResponsePackets/"   //保存载荷文件路径

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
void print_payload(const u_char *payload, int len);
void print_hex_ascii_line(const u_char *payload, int len, int offset);

void save_file(int count, const u_char *payload, int size_payload);
void get_payload(const char *payload, int size_payload, char *payload_content);
void get_content_type(char *payload_content, char *content_type);

void save_database(char *sql_statement);

//函数定义————————————————————
/* * print data in rows of 16 bytes: offset   hex   ascii
 * 00000   47 45 54 20 2f 20 48 54  54 50 2f 31 2e 31 0d 0a   GET / HTTP/1.1.. */
void print_hex_ascii_line(const u_char *payload, int len, int offset)
{
    int i;
    int gap;
    const u_char *ch;

    /* offset */
    printf("%05d   ", offset);

    /* hex */
    ch = payload;
    for (i = 0; i < len; i++)
    {
        printf("%02x ", *ch);
        ch++;
        /* print extra space after 8th byte for visual aid */
        if (i == 7)
            printf(" ");
    }
    /* print space to handle line less than 8 bytes */
    if (len < 8)
        printf(" ");

    /* fill hex gap with spaces if not full line */
    if (len < 16)
    {
        gap = 16 - len;
        for (i = 0; i < gap; i++)
        {
            printf("   ");
        }
    }
    printf("   ");

    /* ascii (if printable) */
    ch = payload;
    for (i = 0; i < len; i++)
    {
        if (isprint(*ch))
            printf("%c", *ch);
        else
            printf(".");
        ch++;
    }

    printf("\n");

    return;
}

/* * print packet payload data (avoid printing binary data) */
void print_payload(const u_char *payload, int len)
{

    int len_rem = len;
    int line_width = 16; /* number of bytes per line */
    int line_len;
    int offset = 0; /* zero-based offset counter */
    const u_char *ch = payload;

    if (len <= 0)
        return;

    /* data fits on one line */
    if (len <= line_width)
    {
        print_hex_ascii_line(ch, len, offset);
        return;
    }

    /* data spans multiple lines */
    for (;;)
    {
        /* compute current line length */
        line_len = line_width % len_rem;
        /* print line */
        print_hex_ascii_line(ch, line_len, offset);
        /* compute total remaining */
        len_rem = len_rem - line_len;
        /* shift pointer to remaining bytes to print */
        ch = ch + line_len;
        /* add offset */
        offset = offset + line_width;
        /* check if we have line width chars or less */
        if (len_rem <= line_width)
        {
            /* print last line and get out */
            print_hex_ascii_line(ch, len_rem, offset);
            break;
        }
    }

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

/* 取Contnet-Type */
void get_content_type(char *payload_content, char *content_type)
{
    int i = 0;
    int j = 0;
    int content_type_flag = 0;

    for (i = 0; i < strlen(payload_content); i++) //取content_type
    {
        if (payload_content[i] == 'C' && payload_content[i + 8] == 'T' && payload_content[i + 9] == 'y' && payload_content[i + 10] == 'p') //注意偶然性
        {
            content_type_flag = 1;
            i = i + 14; //"Content-Type: "
        }

        if (payload_content[i] == '.' && payload_content[i + 1] == '.') //即\r\n
        {
            content_type_flag = 0;
        }

        if (content_type_flag == 1)
        {
            content_type[j] = payload_content[i];
            j++;
        }
    }
}

/* 保存文件 */
void save_file(int count, const u_char *payload, int size_payload)
{
    char file_path[100] = ""; //文件路径    错误：已放弃 (核心已转储)：初始化数组大小不够
    char char_count[3] = "";  //字符串型计数，作文件名

    sprintf(char_count, "%d", count - 1);

    strcat(file_path, PAYLOAD_FIFE_PATH);
    strcat(file_path, char_count);

    char payload_content[2000] = ""; //载荷
    get_payload(payload, size_payload, payload_content);

    if (strcmp(payload_content, "") == 0) //存在载荷为空情况，载荷为空不存文件
    {
        return;
    }

    // printf("载荷内容:%s\n", payload_content);

    char content_type[100] = ""; // Content-Type
    get_content_type(payload_content, content_type);

    // printf("content-type内容:%s\n", content_type);

    //依据"Content-Type"内容完善不同格式的文件路径
    int binary_flag = 0; //是否为二进制文件类型

    if (strcmp(content_type, "text/html") == 0)
    {
        strcat(file_path, ".html");
    }
    else if (strcmp(content_type, "text/plain") == 0)
    {
        strcat(file_path, ".txt");
    }
    else if (strcmp(content_type, "image/jpeg") == 0)
    {
        strcat(file_path, ".jpg");
        binary_flag = 1;
    }
    else if (strcmp(content_type, "image/gif") == 0)
    {
        strcat(file_path, ".gif");
        binary_flag = 1;
    }
    else if (strcmp(content_type, "video/quicktime") == 0)
    {
        strcat(file_path, ".mov"); //或".qt"文件
        binary_flag = 1;
    }
    else if (strcmp(content_type, "application/vnd.ms-powserpoint") == 0)
    {
        strcat(file_path, ".ppt");
    }
    else //未知的存为".txt"文件
    {
        strcat(file_path, ".txt");
    }

    //保存  存在保存为二进制文件，使用fopen()
    FILE *fp;

    if (binary_flag == 0)
    {
        if ((fp = fopen(file_path, "w+")) == NULL) //打开可读/写文件，若文件存在则文件长度清为零，即该文件内容会消失。若文件不存在则建立该文件
        {
            fprintf(stderr, "Failed to open the file\n");
        }
    }
    else
    {
        if ((fp = fopen(file_path, "wb+")) == NULL) //以读/写方式打开或建立一个二进制文件，允许读和写
        {
            fprintf(stderr, "Failed to open the file\n");
        }
    }

    fwrite(payload_content, sizeof(payload_content), 1, fp); //不管返回值，不判断错误
    fclose(fp);

    printf("\nSaving the file succeeded\n");
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
        exit(EXIT_FAILURE);
    }
    printf("Updated %lu rows\n", (unsigned long)mysql_affected_rows(&connect_fd));

    mysql_close(&connect_fd); //关闭连接 无返回值

    printf("Database saved successfully\n");
}

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

    /* print source and destination IP addresses */
    printf("       From: %s\n", inet_ntoa(ip->ip_src));
    printf("         To: %s\n", inet_ntoa(ip->ip_dst));

    /* determine protocol */
    printf("   Protocol: TCP\n");

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

    printf("   Src port: %d\n", ntohs(tcp->th_sport));
    printf("   Dst port: %d\n", ntohs(tcp->th_dport));

    /* define/compute tcp payload (segment) offset */
    payload = (u_char *)(packet + HEADER_80211_FRAME_SIZE + DATA_80211_FRAME_SIZE + LLC_HEADER_SIZE + size_ip + size_tcp);

    /* compute tcp payload (segment) size */
    size_payload = ntohs(ip->ip_len) - (size_ip + size_tcp);

    /*	 * Print payload data; it might be binary, so don't just	 * treat it as a string. */
    if (size_payload > 0)
    {
        printf("   Payload (%d bytes):\n", size_payload);
        // print_payload(payload, size_payload);
    }

    //保存文件
    save_file(count, payload, size_payload);

    //处理数据
    char source_ip[20] = "";
    strcpy(source_ip, inet_ntoa(ip->ip_src));
    char destination_ip[20] = "";
    strcpy(destination_ip, inet_ntoa(ip->ip_dst));

    int source_port = ntohs(tcp->th_sport);
    char char_source_port[10] = ""; //字符串型,用于拼接
    sprintf(char_source_port, "%d", source_port);
    int destination_port = ntohs(tcp->th_dport);
    char char_destination_port[10] = ""; //字符串型
    sprintf(char_destination_port, "%d", destination_port);

    char payload_content[2000] = ""; //载荷
    get_payload(payload, size_payload, payload_content);

    if (strcmp(payload_content, "") == 0) //存在载荷为空情况，载荷为空不存数据库
    {
        return;
    }

    // printf("载荷内容:%s\n", payload_content);

    char content_type[100] = ""; // Content-Type
    get_content_type(payload_content, content_type);

    int frame_size = 0;
    frame_size = HEADER_80211_FRAME_SIZE + DATA_80211_FRAME_SIZE + LLC_HEADER_SIZE + size_ip + size_tcp + size_payload;
    char char_frame_size[10] = ""; //字符串型
    sprintf(char_frame_size, "%d", frame_size);

    char file_path[100] = ""; //文件路径    错误：已放弃 (核心已转储)：初始化数组大小不够
    char char_count[3] = "";  //字符串型计数，作文件名

    sprintf(char_count, "%d", count - 1); //编号

    strcat(file_path, PAYLOAD_FIFE_PATH);
    strcat(file_path, char_count);

    //依据"Content-Type"内容完善不同格式的文件路径
    if (strcmp(content_type, "text/html") == 0)
    {
        strcat(file_path, ".html");
    }
    else if (strcmp(content_type, "text/plain") == 0)
    {
        strcat(file_path, ".txt");
    }
    else if (strcmp(content_type, "image/jpeg") == 0)
    {
        strcat(file_path, ".jpg");
    }
    else if (strcmp(content_type, "image/gif") == 0)
    {
        strcat(file_path, ".gif");
    }
    else if (strcmp(content_type, "video/quicktime") == 0)
    {
        strcat(file_path, ".mov"); //或".qt"文件
    }
    else if (strcmp(content_type, "application/vnd.ms-powserpoint") == 0)
    {
        strcat(file_path, ".ppt");
    }
    else //未知的存为".txt"文件
    {
        strcat(file_path, ".txt");
    }

    // printf("%d,%s,%s,%d,%d,%s,%d,%s\n", count-1, source_ip, destination_ip, source_port, destination_port, content_type, frame_size, file_path);

    // SQL语句
    char sql_statement[2000] = "insert into response(number,source_ip,destination_ip,source_port,destination_port,\
content_type,frame_size,file_path) values(";

    strcat(sql_statement, char_count);
    strcat(sql_statement, ",'");
    strcat(sql_statement, source_ip);
    strcat(sql_statement, "','");
    strcat(sql_statement, destination_ip);
    strcat(sql_statement, "',");
    strcat(sql_statement, char_source_port);
    strcat(sql_statement, ",");
    strcat(sql_statement, char_destination_port);
    strcat(sql_statement, ",'");
    strcat(sql_statement, content_type);
    strcat(sql_statement, "',");
    strcat(sql_statement, char_frame_size);
    strcat(sql_statement, ",'");
    strcat(sql_statement, file_path);
    strcat(sql_statement, "')");

    // printf("%s\n", sql_statement);

    //保存数据库
    save_database(sql_statement);

    return;
}

//主函数————————————————————
int main(int argc, char **argv)
{
    char errbuf[PCAP_ERRBUF_SIZE]; /* error buffer */

    pcap_t *handle; /* packet capture handle */ // pcap_open_live()
    // pcap_compile()
    char filter_exp[] = "(tcp[((tcp[12:1] & 0xf0) >> 2):4] = 0x48545450)"; /* filter expression [3] */ //过滤"HTTP"
    struct bpf_program fp;                                                                             /* compiled filter program (expression) */

    int num_packets = 100; /* number of packets to capture */ // pcap_loop()

    /* open capture device */
    if ((handle = pcap_open_offline(RESPONSEFILE, errbuf)) == NULL)
    {
        fprintf(stderr, "Couldn't open file: %s: %s\n", RESPONSEFILE, errbuf);
        exit(EXIT_FAILURE);
    }

    /* make sure we're capturing on an BSD wireless with Radio tap header */
    if (pcap_datalink(handle) != DLT_IEEE802_11_RADIO)
    {
        fprintf(stderr, "%s is not an BSD wireless with Radio tap header\n", RESPONSEFILE);
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

    printf("Handle complete\n");

    return 0;
}