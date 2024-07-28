#include <fcntl.h>    //open
#include <sys/stat.h> //mkfifo()
#include <unistd.h>   //read(),close()
#include <stdio.h>    //printf()

int main()
{
    int fifo_fd;               //命名管道文件描述符
    char request_msg[100];     //请求消息
    int request_msg_len = 100; //读取消息长度，不太严谨

    mkfifo("./fifo", S_IRWXU);          // S_IRWXU = S_IRUSR | S_IWUSR | S_IXUSR 不太严谨
    fifo_fd = open("./fifo", O_RDONLY); //不太严谨

    read(fifo_fd, request_msg, request_msg_len); //读出命名管道
    printf("%s", request_msg);                   //打印

    close(fifo_fd);

    return 0;
}