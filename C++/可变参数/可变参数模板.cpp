#include <iostream>

using std::cout;
using std::endl;

// 参数数量 == 0 时调用
int sum()
{
    return 0;
}

// 参数数量 >= 1
template <typename T, typename... Args>
int sum(T first, Args... args)
{
    return first + sum(args...); // 递归调用
}

int main()
{
    cout << sum(1, 2, 3) << endl;
    // 过程:
    // 1 + sum(2, 3)
    // 1 + 2 + sum(3)
    // 1 + 2 + 3 + sum()
    // 1 + 2 + 3 + 0
    // 输出: 6

    return 0;
}