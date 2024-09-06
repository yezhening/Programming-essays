#include <iostream>

using std::cout;
using std::endl;

// 一元左折
template <typename... T>
int add_one_left(T... args)
{
    return (... + args);
}

// 一元右折
template <typename... T>
int sub_one_right(T... args)
{
    return (args - ...);
}

// 二元左折
template <typename... T>
int add_two_left(T... args)
{
    return (100 + ... + args);
}

// 二元右折
template <typename... T>
int sub_two_right(T... args)
{
    return (args - ... - 100);
}

int main()
{
    cout << add_one_left(1, 2, 3) << endl;
    // 过程：((1 + 2) + 3)
    // 输出: 6

    cout << sub_one_right(1, 2, 3) << endl;
    // 过程：(1 - (2 - 3))
    // 输出: 2

    cout << add_two_left(1, 2, 3) << endl;
    // 过程：(((100 + 1) + 2) + 3)
    // 输出: 106

    cout << sub_two_right(1, 2, 3) << endl;
    // 过程：(1 - (2 - (3 - 100))
    // 输出: -98

    return 0;
}