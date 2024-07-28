//头文件————————————————————
#include <string.h>   //strchr()
#include <fcntl.h>    //open()、O_RDONLY
#include <sys/stat.h> //open()
#include <unistd.h>   //dup2()、close()
#include <errno.h>    //errno

//宏————————————————————
#define FFLAG (O_WRONLY | O_CREAT | O_TRUNC)
#define FMODE (S_IRUSR | S_IWUSR)

//处理标准输入、输出的重定向————————————————————
//标准输入重定向'<'
int parseandredirectin(char *cmd)
{
    char *infile; //记录'<'第一次出现的位置
    int infd;     //打开文件的文件描述符
    int error;    //避免close()修改errno

    if ((infile = strchr(cmd, '<')) == NULL) //记录'<'第一次出现的位置
    {
        return 0;
    }
    *infile = '\0'; //分隔需要重定向的命令/文件名（目的）和命令/文件名（源）

    //截取（源）命令/文件名
    infile = strtok(infile + 1, " \t");
    if (infile == NULL)
    {
        return 0;
    }

    if ((infd = open(infile, O_RDONLY)) == -1) //以只读方式打开文件
    {
        return -1;
    }

    if (dup2(infd, STDIN_FILENO) == -1) //重定向
    {
        //避免close()修改errno
        error = errno;
        close(infd);
        errno = error;
        return -1;
    }

    return close(infd);
}

int parseandredirectout(char *cmd)
{
    char *outfile;
    int outfd;
    int error;

    if ((outfile = strchr(cmd, '>')) == NULL)
    {
        return 0;
    }
    *outfile = '\0';

    outfile = strtok(outfile + 1, " \t");
    if (outfile == NULL)
    {
        return 0;
    }

    if ((outfd = open(outfile, FFLAG, FMODE)) == -1) //创建可写的0长度文件
    {
        return -1;
    }

    if (dup2(outfd, STDOUT_FILENO) == -1)
    {
        error = errno;
        close(outfd);
        errno = error;
        return -1;
    }

    return close(outfd);
}