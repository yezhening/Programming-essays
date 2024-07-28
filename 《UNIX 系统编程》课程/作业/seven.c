#include<stdio.h>//printf()
#include <unistd.h> //getpid()、getpgrp()、getsid()

int main()
{
    printf("该会话进程的pid：%d\n", (int)getpid());
    printf("该会话进程的pgid：%d\n", (int)getpgrp()); //是r不是i
    printf("该会话进程的sid：%d\n", (int)getsid((int)getpid())); //有参数

    return 0;
}
