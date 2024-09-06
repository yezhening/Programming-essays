#include <cstdarg> // #va_list、#va_start()、#va_arg()、#va_end()
#include <iostream>

using std::cout;
using std::endl;

// 形参的一般形式：
// num：参数数量
// ...：参数列表
void print(int num, ...)
{
    // 1. 定义 va_list
    va_list para_list; // 类型宏；参数列表

    // 2. 初始化 va_list
    va_start(para_list, num); // 函数宏；va_list 指向参数列表的第一个参数

    // 3. 遍历 va_list
    for (int i = 0; i < num; ++i)
    {
        cout << va_arg(para_list, int) << " "; // 函数宏；依据类型，va_list 指向参数列表的下一个参数
    }
    cout << endl;

    // 4. 清理 va_list
    va_end(para_list); // 函数宏；清理 va_list

    return;
}

int main()
{
    print(2, 0, 1);
    // 实参的一般形式：
    // 2：参数数量
    // 0 1：参数列表

    print(3, 0, 1, 2);

    return 0;
}
/*
输出：
0 1
0 1 2
*/