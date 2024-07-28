#include <fcntl.h> //open
#include <sys/stat.h>
#include <unistd.h> //getpid,write(),close()
#include <time.h>   //time()、ctime()
#include <stdio.h>  //sprintf()
#include <string.h> //strlen

int main()
{
    int fifo_fd;           //命名管道文件描述符
    pid_t client_pid;      //客户端进程ID
    time_t request_time;   //请求时间
    char request_msg[100]; //请求消息
    int request_msg_len;   //请求消息长度

    fifo_fd = open("./fifo", O_WRONLY);
    client_pid = (int)getpid();
    request_time = time(NULL);
    sprintf(request_msg, "客户端进程ID：%d，请求时间：%s", client_pid, ctime(&request_time)); // time()返回time_t，ctime()返回char*  用time_t会发生段错误
    request_msg_len = strlen(request_msg);                                                    //不是100，而是实际字符长度

    write(fifo_fd, request_msg, request_msg_len); //写入命名管道

    close(fifo_fd);

    return 0;
}