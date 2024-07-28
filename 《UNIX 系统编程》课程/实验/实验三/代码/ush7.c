#define _GNU_SOURCE   //必需
#include <signal.h>   //处理信号
#include <setjmp.h>   //处理信号跳转
#include <stdio.h>    //perror()、fputs()、fgets()
#include <unistd.h>   //getpid()、setpgid()
#include <limits.h>   //MAX_CANON
#include <string.h>   //strlen()、strcmp()、strchr()
#include <sys/wait.h> //waitpid()
#include <sys/types.h>

//宏定义————————————————————
#define PROMPT_STRING "ush7>>" //输出命令行提示符
#define QUIT_STRING "q"        //退出程序符
//用作字符串而不是字符，否则strcmp()报错  passing argument 2 of ‘strcmp’ makes pointer from integer without a cast [enabled by default]
#define BACK_SYMBOL '&' //后台运行命令/进程组符

//全局变量————————————————————
static volatile sig_atomic_t okaytojump = 0; //信号跳转标志位  防止编译器对代码进行优化 要求变量信号原子操作
static sigjmp_buf jumptoprompt;              //跳转保存信号掩码的缓冲区

//相关函数声明————————————————————
int signalsetup(struct sigaction *def, sigset_t *mask, void (*handler)(int));
void executecmd(char *incmd);

//信号跳转
/* ARGSUSED */ //编译时抑制对未使用的变量signalnum的警告
static void jumphd(int signalnum)
{
    if (!okaytojump) //okaytojump为0，不需跳转
    {
        return;
    }

    okaytojump = 0;              //置位
    siglongjmp(jumptoprompt, 1); //跳转   在sigsetjmp()设定的点返回的值
}

//主函数————————————————————
//用waitpid来处理僵后台进程的命令解释程序
int main(void)
{
    struct sigaction defhandler; //信号默认处理结构
    sigset_t blockmask;          //信号掩码
    char inbuf[MAX_CANON];       //输入一行命令的缓冲区 终端规范输入队列的字节数
    int len;                     //记录命令字符长度/个数
    char *backp;                 //记录后台运行命令/进程组符在字符串中的位置
    int inbackground;            //判断命令/进程组后台运行标志位
    pid_t childpid;              //判断“父子”进程标志位

    if (signalsetup(&defhandler, &blockmask, jumphd) == -1) //建立信号结构
    {
        perror("Fail to set up shell signal handing");
        return 1;
    }

    for (;;) //程序不退出
    {
        //建立信号跳转点
        if ((sigsetjmp(jumptoprompt, 1)) && //非零将信号掩码当前状态保存在缓冲区
            (fputs("\n", stdout) == EOF))   //跳转后换行
        {
            continue;
        }

        okaytojump = 1;                                                        //置位，建立跳转点后可跳转
        printf("----------命令解释程序进程号：%d----------\n", (int)getpid()); //打印当前进程号

        if (fputs(PROMPT_STRING, stdout) == EOF) //输出命令行提升符
        {
            continue;
        }

        if (fgets(inbuf, MAX_CANON, stdin) == NULL) //接收一行命令
        {
            continue;
        }

        //处理命令
        len = strlen(inbuf);        //记录命令字符长度 / 个数
        if (inbuf[len - 1] == '\n') //最后缓冲的换行符改为结束符
        {
            inbuf[len - 1] = '\0';
        }

        if (strcmp(inbuf, QUIT_STRING) == 0) //程序退出，跳出循环
        {
            break;
        }

        //判断命令/进程组是否在后台运行
        if ((backp = strchr(inbuf, BACK_SYMBOL)) == NULL)
        {
            inbackground = 0;
        }
        else
        {
            inbackground = 1;
            *backp = 0; //置位
        }

        if (sigprocmask(SIG_BLOCK, &blockmask, NULL) == -1) //添加信号掩码  阻塞
        {
            perror("Failed to block signals");
        }

        //创建“子”进程处理命令
        if ((childpid = fork()) == -1)
        {
            perror("Failed to fork");
        }
        else if (childpid == 0) //子进程
        {
            printf("----------处理命令进程号：%d----------\n", (int)getpid()); //打印处理命令进程号

            if (inbackground && (setpgid(0, 0) == -1)) //如果是后台命令，创建后台进程组并作为组长  作为进程组ID，成为组长
            {
                return 1;
            }

            //信号处理
            if ((sigaction(SIGINT, &defhandler, NULL) == -1) || //安装默认结构
                (sigaction(SIGQUIT, &defhandler, NULL) == -1) ||
                (sigprocmask(SIG_UNBLOCK, &blockmask, NULL) == -1)) //解除阻塞
            {
                perror("Failed to set signal handling for command ");//漏' '
                return 1;
            }

            executecmd(inbuf); //执行命令
            return 1;
        }

        if (!inbackground) //显示地等待前台进程
        {
            waitpid(childpid, NULL, 0); //等待与调用者在同一进程组中的任意一个子进程   前台进程组只有一个
        }
        while (waitpid(-1, NULL, WNOHANG) > 0)
            ; //等待任意一个进程    后台
        // 即使“子”进程的状态不是立即可用的，WNOHANG也会是waitpid返回
    }
    return 0;
}