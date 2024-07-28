#include <stdio.h> //printf()

int main()
{
    char *str = "#include <stdio.h> //printf()%c%cint main()%c{%c    char *str = %c%s%c;    //代码的引用框架%c    printf( str, 10, 10, 10, 10, 34, str, 34, 10, 10, 10, 10 );   //ASCII中，10为换行，34为引号%c%c    return 0;%c}"; //代码的引用框架
    printf(str, 10, 10, 10, 10, 34, str, 34, 10, 10, 10, 10);   //ASCII中，10为换行，34为引号

    return 0;
}