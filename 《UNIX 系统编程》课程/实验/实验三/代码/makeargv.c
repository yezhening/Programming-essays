//头文件————————————————————
#include <errno.h>  //errno
#include <stdlib.h> //NULL:stddef.h、malloc()、sizeof()
#include <string.h> //strspn()、strlen()、strcpy()、strtok()

//创建参数数组————————————————————
int makeargv(const char *s, const char *delimiters, char ***argvp)
{
    const char *snew; //记录去前分隔符的真正字符串的开始
    char *t;          //记录分隔的每一个字符串
    //参数计数
    int numtokens;
    int i;
    int error; //记录错误码

    if ((s == NULL) || (delimiters == NULL) || (argvp == NULL)) //判空
    {
        errno = EINVAL;
        return -1;
    }
    *argvp = NULL; //置空

    //记录去前分隔符的真正字符串的开始
    //字符指针偏移
    //返回的是字符串中第一个不在指定字符串中出现的字符下标
    snew = s + strspn(s, delimiters);

    //初始化t变量
    if ((t = malloc(strlen(snew) + 1)) == NULL) //分配空间  +1给结束符
    {
        return -1;
    }
    strcpy(t, snew); //初始化

    //参数计数
    numtokens = 0;
    if (strtok(t, delimiters) != NULL) //第一次截取
    {
        for (numtokens = 1; strtok(NULL, delimiters) != NULL; numtokens++); //已有一次   剩余的截取
    }

    //分配参数数组空间
    //二级指针，参数数组，指向各参数
    //+1给参数数组结束符NULL
    //char*对指针
    if ((*argvp = malloc((numtokens + 1) * sizeof(char *))) == NULL)
    {
        error = errno;
        free(t);
        errno = error;
        return -1;
    }

    //初始化参数数组
    if (numtokens == 0) //无释放
    {
        free(t);
    }
    else //有赋值
    {
        //取值
        strcpy(t, snew);
        **argvp = strtok(t, delimiters); //一级指针，指向参数   取第一个字符串

        //赋值  每参数/字符串
        for (i = 1; i < numtokens; i++)
        {
            //二级指针数组中每参数
            //一级指针赋值
            *((*argvp) + i) = strtok(NULL, delimiters);
        }
        *((*argvp) + numtokens) = NULL; //数组最后为NULL

        return numtokens; //返回参数个数
    }
}