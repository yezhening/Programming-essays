//宏————————————————————
// #define _XOPEN_SOURCE
#define _GNU_SOURCE
// sigaction编译错误：incomplete type is not allowed、无sigset_t、sigaction.h文件报错
//部分头文件依赖于features让用户配置编译环境的头文件
//_GUN_SOURCE宏可以让用户打开所有feature，在头文件定义前

//头文件————————————————————
#include <stdio.h>
#include <signal.h> //信号处理

//建立信号结构————————————————————
int signalsetup(struct sigaction *def, sigset_t *mask, void (*handler)(int)) // sigaction无提示  旧的信号处理结构，信号掩码，新的信号处理行为
{
    struct sigaction catch; //新的信号处理结构

    catch.sa_handler = handler; //提示为__sigaction_handler 置新信号行为
    def->sa_handler = SIG_DFL;  //恢复旧信号默认行为
    catch.sa_flags = 0;         //无特殊标识符和选项
    def->sa_flags = 0;

    //掩码
    if ((sigemptyset(&(def->sa_mask)) == -1) || //置空
        (sigemptyset(&(catch.sa_mask)) == -1) ||
        (sigaddset(&(catch.sa_mask), SIGINT) == -1) || //添加需阻塞的额外信号    阻塞
        (sigaddset(&(catch.sa_mask), SIGQUIT) == -1) ||
        (sigaction(SIGINT, &catch, NULL) == -1) || //需阻塞的信号绑定相关有新信号行为的信号结构
        (sigaction(SIGQUIT, &catch, NULL) == -1) ||
        (sigemptyset(mask) == -1) ||
        (sigaddset(mask, SIGINT) == -1) ||
        (sigaddset(mask, SIGQUIT) == -1))//漏"==-1"
    {
        return -1;
    }
    return 0;
}
