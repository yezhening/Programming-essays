//头文件————————————————————
#include <sys/socket.h> //socket()、setsockopt()、bind()、listen()
#include <stdio.h>      //perror()
#include <stdlib.h>     //exit()、malloc()、free()、memset()、memcpy()
#include <netinet/in.h> //sockaddr_in、htons()
#include <string.h>     //bzero()、strstr()、strcat()
#include <unistd.h>     //close()
// io_uring 是一套系统调用接口，虽然只有3个系统调用，但实际使用复杂
// liburing是封装io_uring的便于用户使用的库
#include <liburing.h>
// io_uring、 io_uring_queue_init()、io_uring_sqe、io_uring_get_sqe()、 io_uring_prep_accept()
// io_uring_sqe_set_data()、io_uring_submit()、io_uring_wait_cqe()、io_uring_cqe_get_data()
// io_uring_prep_readv()、io_uring_prep_writev()
#include <sys/uio.h> //iovec

//宏————————————————————
#define SERV_PORT 3333     //服务端端口
#define LISTEN_MAX_COUNT 5 //所监听的最大连接数 两个客户端间通信
#define RING_QUEUE_SIZE 32 //环队列大小
#define BUFF_SIZE 32       //缓冲区大小

//枚举————————————————————
//请求类型
enum Request_type
{
    Accept, // accept()
    Readv,  // readv()
    Writev, // writev()
};

//结构体————————————————————
//请求数据  用于判别操作类型和保存操作数据
struct Request_data
{
    enum Request_type request_type; //请求类型
    //请求内容  保存在读写数据时所需要的信息
    int connect_fd;           //连接套接字文件描述符
    struct iovec io_vector[]; //消息    可不指定大小
    //设计得很精妙：因为需要确定在同一个连接套接字文件描述符上进行读写
};

//客户端数据
struct Client_data
{
    int connect_fd;     //与客户端建立连接的连接套接字文件描述符
    char nick_name[10]; //客户端昵称
    //必须指定大小，否则在接受下一个客户端连接后前一个客户端昵称丢失
};

// 全局变量————————————————————
struct io_uring ring_queue; //环缓冲区/（循）环队列  是io_uring的核心结构
// struct io_uring
// {
//     struct io_uring_sq sq; // submission queue  提交队列
//     struct io_uring_cq cq; // completion queue  完成队列
//     unsigned flags;        //标志位集合
//     int ring_fd;           //操作环队列的环文件描述符
// };

struct Client_data client_data[LISTEN_MAX_COUNT]; //客户端数据  有所监听的最大连接数个

//函数声明————————————————————
void handle(int listen_fd);                                                             //处理
void submit_accept(int listen_fd, struct sockaddr_in *clie_addr, socklen_t *addr_size); //提交accept()请求
void submit_readv(int connect_fd);                                                      //提交readv()请求
void submit_writev(int connect_fd, struct Request_data *req_data);                      //提交writev()请求

//主函数————————————————————
int main(int argc, char *argv[])
{
    int listen_fd; //监听套接字文件描述符

    //创建套接字并获取套接字文件描述符
    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Failed to create the server's socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serv_addr; //服务端网络信息结构体
    bzero(&serv_addr, sizeof(serv_addr));

    //初始化服务端网络信息结构体
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(SERV_PORT);

    //设置套接字选项为可重用本地地址
    int reuse = 1;

    if ((setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse))) == -1)
    {
        close(listen_fd);

        perror("Failed to set the socket's options");
        exit(EXIT_FAILURE);
    }

    //绑定套接字与网络信息
    if ((bind(listen_fd, (struct sockaddr *)(&serv_addr), sizeof(serv_addr))) == -1)
    {
        close(listen_fd);

        perror("Failed to bind the socket");
        exit(EXIT_FAILURE);
    }

    //套接字设置被动监听状态
    if ((listen(listen_fd, LISTEN_MAX_COUNT)) == -1)
    {
        close(listen_fd);

        perror("Failed to configure the socket's listening status");
        exit(EXIT_FAILURE);
    }

    //初始化环队列
    if ((io_uring_queue_init(RING_QUEUE_SIZE, &ring_queue, 0)) < 0)
    {
        close(listen_fd);

        perror("Failed to initialize the ring queue");
        exit(EXIT_FAILURE);
    }
    // int io_uring_queue_init(unsigned int entries, struct io_uring *ring, unsigned int flags);
    // entries：环队列大小
    // ring：环缓冲区/（循）环队列
    // flags：标志位集合  无特殊使用0

    //考虑到客户端连接建立、断开，使用初始化、查找位置的方式填写客户端数据
    //初始化客户端数据
    for (int i = 0; i < LISTEN_MAX_COUNT; i++)
    {
        client_data[i].connect_fd = -1;
        //与客户端建立连接的连接套接字文件描述符为-1
        memcpy(client_data[i].nick_name, "anonymous\0", sizeof("anonymous\0"));
        //客户端昵称为匿名  刚好十个字符为定义的数组大小
    }

    //处理
    handle(listen_fd);

    //释放环队列
    io_uring_queue_exit(&ring_queue); //函数原型说明无失败设置

    return 0;
}

//函数定义————————————————————
//处理
void handle(int listen_fd)
{
    struct sockaddr_in clie_addr; //客户端网络信息结构体
    socklen_t addr_size;          //网络信息结构体大小

    bzero(&clie_addr, sizeof(clie_addr));
    addr_size = sizeof(struct sockaddr);

    submit_accept(listen_fd, &clie_addr, &addr_size); //提交accept()请求

    //获取完成的提交条目，根据类型作不同处理
    struct io_uring_cqe *cqe; // completion queue entry 完成队列条目
    // struct io_uring_cqe
    // {
    //     __u64 user_data; /* sqe->data submission passed back */
    //     //用户自定义数据。在用户态（内核态外）的sqe填写的，内核不改变其中数据，所以可用于判别操作类型和保存操作数据
    //     __s32 res; /* result code for this event */
    //     // sqe的执行结果/事件的返回码/系统调用的返回值，依据操作不同而不同
    //     __u32 flags; //标志位集合
    // };

    struct Request_data *req_data; //请求数据

    while (1)
    {
        if ((io_uring_wait_cqe(&ring_queue, &cqe)) < 0) //获取完成的提交条目
        {
            close(listen_fd);

            perror("Failed to wait the completion queue entry");
            break;
        }
        // io_uring_wait_cqe(): 如果没有已完成的提交条目，会阻塞线程，等待提交条目完成
        //类比I/O多路复用：能监听多个文件描述符，但阻塞在该系统调用
        // io_uring_peek_cqe()：如果没有已完成的提交条目，会立即返回，cqe_ptr 参数被置空

        req_data = io_uring_cqe_get_data(cqe); //获取请求数据
        // req_data = (struct Request_data *)cqe->user_data;//获取请求数据

        //根据类型作不同处理
        switch (req_data->request_type)
        {
        case Accept:
            //一个accept()请求已完成，再次提交accept()请求以等待接收更多客户端的连接请求
            bzero(&clie_addr, sizeof(clie_addr));
            submit_accept(listen_fd, &clie_addr, &addr_size);

            //连接建立后，记录客户端数据：1.连接文件描述符
            for (int i = 0; i < LISTEN_MAX_COUNT; i++)
            {
                if (client_data[i].connect_fd == -1) //查找位置
                {
                    client_data[i].connect_fd = cqe->res;

                    break;
                }
            }

            //连接建立后，套接字可接收
            //结构cqe的成员res是sqe的执行结果/事件的返回码/系统调用的返回值，依据操作不同而不同
            //对于accept()、io_uring_prep_accept()操作，返回值是套接字连接文件描述符
            //当前liburing.h中无io_uring_prep_recv()，有io_uring_prep_readv()
            submit_readv(cqe->res); //提交readv()请求

            //释放请求数据
            //因为在其他提交请求操作中都会设置请求数据以判别操作类型和保存操作数据，这里的请求数据已无用
            // malloc()和free()配套使用
            free(req_data);

            break;
        case Readv:
            //一个readv()请求已完成，对所有已连接客户端提交writev()请求以准备发送数据给客户端
            for (int i = 0; i < LISTEN_MAX_COUNT; i++)
            {
                if (client_data[i].connect_fd != -1)
                {
                    submit_writev(client_data[i].connect_fd, req_data);
                }
            }

            free(req_data->io_vector[0].iov_base);
            free(req_data);
            //注意：两次释放的内容并不相同，必须释放两次

            break;
        case Writev:
            // close(req_data->connect_fd);
            //若是echo回射程序，通信一次，通信完关闭连接描述符

            //一个writev()请求已完成，重新提交该连接套接字文件描述符的readv()请求以准备从客户端接收数据
            submit_readv(req_data->connect_fd);

            free(req_data->io_vector[0].iov_base);
            free(req_data);

            break;
        }

        //默认情况下I/O完成事件不会从完成队列中清除，io_uring_wait_cqe()会重复获取到相同的事件
        //标记完成事件已经被消费（处理）
        io_uring_cqe_seen(&ring_queue, cqe); //函数原型说明无失败设置
    }

    return;
}

//提交accept()请求
void submit_accept(int listen_fd, struct sockaddr_in *clie_addr, socklen_t *addr_size)
{
    //获取提交条目
    struct io_uring_sqe *sqe; // submission queue entry 提交条目
    // struct io_uring_sqe
    // {
    //     __u8 opcode; /* type of operation for this sqe */ //操作码
    //     __u8 flags; /* IOSQE_ flags */                    //标志位集合
    //     __u16 ioprio; /* ioprio for the request */        //操作的优先级
    //     __s32 fd; /* file descriptor to do IO on */       //与操作相关的文件描述符
    //     union
    //     {
    //         __u64 off; /* offset into file */ //操作在与操作相关的文件描述符的偏移量
    //         __u64 addr2;
    //     };
    //     union
    //     {
    //         __u64 addr; /* pointer to buffer or iovecs */
    //         //操作执行的地址
    //         //如果操作码opcode描述了一个传输数据的操作，这个操作是基于向量的，addr就指向struct iovec的数组首地址
    //         //如果不是基于向量的，那么addr必须直接包含一个地址
    //         __u64 splice_off_in;
    //     };
    //     __u32 len; /* buffer size or number of iovecs */ // 非向量场景表示缓冲区的长度，而向量场景就表示iovec的数量
    //     union                                            //针对特定操作码opcode的标志位集合
    //     {
    //         __kernel_rwf_t rw_flags;
    //         __u32 fsync_flags;
    //         __u16 poll_events;   /* compatibility */
    //         __u32 poll32_events; /* word-reversed for BE */
    //         __u32 sync_range_flags;
    //         __u32 msg_flags;
    //         __u32 timeout_flags;
    //         __u32 accept_flags;
    //         __u32 cancel_flags;
    //         __u32 open_flags;
    //         __u32 statx_flags;
    //         __u32 fadvise_advice;
    //         __u32 splice_flags;
    //     };
    //     __u64 user_data; /* data to be passed back at completion time */ //用户自定义数据
    //     union
    //     {
    //         struct
    //         {
    //             /* pack this to avoid bogus arm OABI complaints */
    //             union
    //             {
    //                 /* index into fixed buffers, if used */
    //                 __u16 buf_index;
    //                 /* for grouped buffer selection */
    //                 __u16 buf_group;
    //             } __attribute__((packed));
    //             /* personality to use, if used */
    //             __u16 personality;
    //             __s32 splice_fd_in;
    //         };
    //         __u64 __pad2[3]; //内存对齐到64字节
    //     };
    // };

    if ((sqe = io_uring_get_sqe(&ring_queue)) == NULL)
    {
        close(listen_fd);

        perror("Failed to get the 'accept' submission queue entry"); //函数原型未说明失败会设置errno
        exit(EXIT_FAILURE);
    }

    //设置提交条目的操作为accept()
    io_uring_prep_accept(sqe, listen_fd, (struct sockaddr *)clie_addr, addr_size, 0); //函数原型说明出错设置CQE

    //设置请求数据
    struct Request_data *req_data; //请求数据 用于判别操作类型和保存操作数据

    req_data = malloc(sizeof(struct Request_data));
    req_data->request_type = Accept;

    io_uring_sqe_set_data(sqe, req_data); //函数原型说明无失败设置

    //提交提交条目到提交队列
    if ((io_uring_submit(&ring_queue)) < 0) //成功返回提交到提交队列的提交条目数
    {
        close(listen_fd);

        perror("Failed to submit the 'accept' submission queue entry to the submission queue");
        exit(EXIT_FAILURE);
    }

    return;
}

//提交readv()请求
void submit_readv(int connect_fd)
{
    //获取提交条目
    struct io_uring_sqe *sqe; // submission queue entry 提交条目

    if ((sqe = io_uring_get_sqe(&ring_queue)) == NULL)
    {
        close(connect_fd);

        perror("Failed to get the 'readv' submission queue entry"); //函数原型未说明失败会设置errno
        exit(EXIT_FAILURE);                                         //暂不考虑完善的服务端程序，操作失败一律退出
    }

    //设置请求数据
    struct Request_data *req_data; //请求数据 用于判别操作类型和保存操作数据

    req_data = malloc(sizeof(struct Request_data));
    //注意：Request_data结构体的大小包括iovec结构体数组第一个结构体的大小
    //若是有多个结构体，需申请sizeof(struct Request_data) + sizeof(struct iovec) × 结构体数量的空间
    //默认iovec结构体数组只有一个结构体
    req_data->request_type = Readv;
    req_data->connect_fd = connect_fd;
    req_data->io_vector[0].iov_base = malloc(BUFF_SIZE);
    req_data->io_vector[0].iov_len = BUFF_SIZE;

    memset(req_data->io_vector[0].iov_base, 0, BUFF_SIZE);

    // 设置提交条目的操作为readv()
    io_uring_prep_readv(sqe, connect_fd, &req_data->io_vector[0], 1, 0); //函数原型说明出错设置CQE

    io_uring_sqe_set_data(sqe, req_data); //函数原型说明无失败设置

    //提交提交条目到提交队列
    if ((io_uring_submit(&ring_queue)) < 0) //成功返回提交到提交队列的提交条目数
    {
        close(connect_fd);

        perror("Failed to submit the 'readv' submission queue entry to the submission queue");
        exit(EXIT_FAILURE);
    }

    return;
}

//提交writev()请求
void submit_writev(int connect_fd, struct Request_data *req_data)
{
    //获取提交条目
    struct io_uring_sqe *sqe; // submission queue entry 提交条目

    if ((sqe = io_uring_get_sqe(&ring_queue)) == NULL)
    {
        perror("Failed to get the 'writev' submission queue entry"); //函数原型未说明失败会设置errno
        exit(EXIT_FAILURE);                                          //暂不考虑完善的服务端程序，操作失败一律退出
    }

    //设置请求数据
    struct Request_data *copy_req_data; //拷贝请求数据 用于判别操作类型和保存操作数据
    // req_data请求数据是io_uring_prep_readv()的，在处理函数中会被释放
    // copy_req_data拷贝请求数据是io_uring_prep_writev()的，消息是req_data请求数据中的消息，所以命名为拷贝

    copy_req_data = malloc(sizeof(struct Request_data));
    //注意：Request_data结构体的大小包括iovec结构体数组第一个结构体的大小
    //若是有多个结构体，需申请sizeof(struct Request_data) + sizeof(struct iovec) × 结构体数量的空间
    //默认iovec结构体数组只有一个结构体
    copy_req_data->request_type = Writev;
    // copy_req_data->connect_fd = req_data->connect_fd;
    copy_req_data->connect_fd = connect_fd; // 1.准备发送数据的连接套接字文件描述符
    copy_req_data->io_vector[0].iov_base = malloc(BUFF_SIZE);
    copy_req_data->io_vector[0].iov_len = BUFF_SIZE;

    char temp_data[BUFF_SIZE]; //中间数据    用于根据不同接收数据构造相应的发送数据
    bzero(&temp_data, sizeof(temp_data));

    //判断是否是命令
    if (strstr(req_data->io_vector[0].iov_base, "NICK:") != NULL) //是NICK命令
    {
        for (int i = 0; i < LISTEN_MAX_COUNT; i++)
        {
            if (client_data[i].connect_fd == req_data->connect_fd) // 2.找到接收数据的连接套接字文件描述符  必找得到
            {
                //记录客户端数据：2.昵称
                //对每个连接套接字文件描述符都会重复记录一次
                // memcpy(client_data[i].nick_name, req_data->io_vector[0].iov_base + 5, sizeof(req_data->io_vector[0].iov_base) - 5);
                memcpy(client_data[i].nick_name, req_data->io_vector[0].iov_base + 5, sizeof("anonymous\0"));
                // strcpy(client_data[i].nick_name, req_data->io_vector[0].iov_base + 5);
                //注意：字符串操作
                //函数原型要求的参数是size_t类型，使用sizeof()。sizeof()包括'\0'，strlen()不包括'\0'
                //参数需要-5    排错排了一晚（应该是这个错吧，不是...）

                //构造发送数据
                strcat(temp_data, client_data[i].nick_name);
                strcat(temp_data, " has connect\0");

                break;
            }
        }
    }
    else if (strstr(req_data->io_vector[0].iov_base, "QUIT:") != NULL) //是QUIT命令
    {
        for (int i = 0; i < LISTEN_MAX_COUNT; i++)
        {
            if (client_data[i].connect_fd == req_data->connect_fd) // 2.找到接收数据的连接套接字文件描述符  必找得到
            {
                //构造发送数据
                strcat(temp_data, client_data[i].nick_name);
                strcat(temp_data, " has quit\0");

                // //关闭接收数据的连接套接字
                // close(client_data[i].connect_fd);

                // //清空客户端数据
                // client_data[i].connect_fd = -1;
                // memcpy(client_data[i].nick_name, "anonymous\0", sizeof("anonymous\0")); 

                break;
            }
        }
    }
    else //不是命令
    {
        for (int i = 0; i < LISTEN_MAX_COUNT; i++)
        {
            if (client_data[i].connect_fd == req_data->connect_fd) // 2.找到接收数据的连接套接字文件描述符  必找得到
            {
                // 构造发送数据
                strcat(temp_data, client_data[i].nick_name);
                strcat(temp_data, ":");
                strcat(temp_data, req_data->io_vector[0].iov_base);
                strcat(temp_data, "\0");

                break;
            }
        }
    }

    memcpy(copy_req_data->io_vector[0].iov_base, temp_data, copy_req_data->io_vector[0].iov_len); //记录发送数据

    // 设置提交条目的操作为writev()
    io_uring_prep_writev(sqe, copy_req_data->connect_fd, copy_req_data->io_vector, 1, 0);

    io_uring_sqe_set_data(sqe, copy_req_data); //函数原型说明无失败设置

    //提交提交条目到提交队列
    if ((io_uring_submit(&ring_queue)) < 0) //成功返回提交到提交队列的提交条目数
    {
        perror("Failed to submit the 'writev' submission queue entry to the submission queue");
        exit(EXIT_FAILURE);
    }

    return;
}

//未实现内容————————————————————
// io_uring_prep_*()函数原型说明出错设置CQE    操作可能成功或失败，需判决
//冗余————————————————————
//记录客户端数据：2.昵称，对每个连接套接字文件描述符都会重复记录一次