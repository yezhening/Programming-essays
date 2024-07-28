#include<stdio.h>
#include <unistd.h> //getuid()、getgid()、geteuid()、getegid()

int main()
{
    printf("当前进程的用户ID：%d\n", getuid());
    printf("当前进程的组ID：%d\n",getgid());
    printf("当前进程的有效用户ID：%d\n",geteuid());
    printf("当前进程的有效组ID：%d\n", getegid());

    return 0;
}