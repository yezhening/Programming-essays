//头文件————————————————————
#include <pcap.h>   //pcap库相关
#include <stdlib.h> //EXIT_FAILURE
#include <unistd.h> //daemon()
#include <syslog.h> //syslog()
#include <fcntl.h>  //fcntl()、open()

//宏定义————————————————————
/* default snap length (maximum bytes per packet to capture) */
#define SNAP_LEN 1518
#define LOCKFILE "/var/run/snifferRequest.pid"                 //锁文件路径
#define LOCKMODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)       //打开锁文件模式
#define REQUESTFILE "/home/ricardo/桌面/Capture/request.pcap" //请求数据包文件路径  绝对路径

//函数声明————————————————————
void lockFile(int lockFd);                                                           //上锁
void saveFile(u_char *args, const struct pcap_pkthdr *header, const u_char *packet); //保存文件

//函数定义————————————————————
//上锁
void lockFile(int lockFd)
{
    //锁结构
    static struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_start = 0;
    lock.l_whence = SEEK_SET;
    lock.l_len = 0;
    lock.l_pid = getpid();

    if ((fcntl(lockFd, F_SETLK, &lock)) == -1)
    {
        syslog(LOG_ERR, "Failed to lock file");
        exit(1);
    }
}
//保存文件
void saveFile(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
    pcap_dump(args, header, packet);
}

//主函数————————————————————
int main(int argc, char **argv)
{
    int lockFd; //锁文件描述符

    //守护进程
    if ((daemon(0, 1)) == -1)
    {
        syslog(LOG_ERR, "Failed to creat daemon");
        exit(1);
    }

    //后上锁	若先上锁，可能由于守护进程机制而脱离锁控制
    if ((lockFd = open(LOCKFILE, O_RDWR | O_CREAT, LOCKMODE)) == -1)
    {
        syslog(LOG_ERR, "Failed to open lock file");
        exit(1);
    }
    lockFile(lockFd);

    char errbuf[PCAP_ERRBUF_SIZE]; /* error buffer */

    // pcap_lookupnet()
    bpf_u_int32 mask; /* subnet mask */
    bpf_u_int32 net;  /* ip */

    char *dev = NULL; /* capture device name */ // pcap_lookupdev
    pcap_t *handle; /* packet capture handle */ // pcap_open_live()
    // pcap_compile()
    char filter_exp[] = "ip and (tcp src port 80) and ((tcp[32:4] = 0x47455420) or (tcp[48:4] = 0x504f5354))"; /* filter expression [3] */
    struct bpf_program fp;                                                                                     /* compiled filter program (expression) */

    int num_packets = 300; /* number of packets to capture */ // pcap_loop

    pcap_dumper_t *dumpFd; //转储文件描述符

    /* check for capture device name on command-line */
    if (argc == 2)
    {
        dev = argv[1];
    }
    else if (argc > 2)
    {
        syslog(LOG_ERR, "Error: unrecognized command-line options");
        exit(EXIT_FAILURE);
    }
    else
    {
        /* find a capture device if not specified on command-line */
        if ((dev = pcap_lookupdev(errbuf)) == NULL) // deprecated弃用    pcap_findalldevs()
        {
            syslog(LOG_ERR, "Couldn't find default device: %s", errbuf);
            exit(EXIT_FAILURE);
        }
    }

    /* get network number and mask associated with capture device */
    if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1)
    {
        syslog(LOG_ERR, "Couldn't get netmask for device %s: %s", dev, errbuf);
        net = 0;
        mask = 0;
    }

    /* print capture info */
    syslog(LOG_INFO, "Device: %s", dev);
    syslog(LOG_INFO, "Number of packets: %d", num_packets);
    syslog(LOG_INFO, "Filter expression: %s", filter_exp);

    /* open capture device */
    if ((handle = pcap_open_live(dev, SNAP_LEN, 1, 1000, errbuf)) == NULL)
    {
        syslog(LOG_ERR, "Couldn't open device %s: %s", dev, errbuf);
        exit(EXIT_FAILURE);
    }

    /* make sure we're capturing on an Ethernet device [2] */
    if (pcap_datalink(handle) != DLT_EN10MB)
    {
        syslog(LOG_ERR, "%s is not an Ethernet", dev);
        exit(EXIT_FAILURE);
    }

    /* compile the filter expression */
    if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1)
    {
        syslog(LOG_ERR, "Couldn't parse filter %s: %s", filter_exp, pcap_geterr(handle));
        exit(EXIT_FAILURE);
    }

    /* apply the compiled filter */
    if (pcap_setfilter(handle, &fp) == -1)
    {
        syslog(LOG_ERR, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
        exit(EXIT_FAILURE);
    }

    //保存文件
    if ((dumpFd = pcap_dump_open(handle, REQUESTFILE)) == NULL)
    {
        syslog(LOG_ERR, "Failed to open dump");
        exit(1);
    }
    pcap_loop(handle, num_packets, saveFile, (u_char *)dumpFd);

    pcap_dump_flush(dumpFd);
    pcap_dump_close(dumpFd);

    /* cleanup */
    pcap_freecode(&fp);
    pcap_close(handle);

    syslog(LOG_INFO, "Capture complete");

    return 0;
}