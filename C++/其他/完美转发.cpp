#include <iostream>

template <typename T>
void print_type(T &arg) // 只接收左值
{
    std::cout << "lvalue" << std::endl;
}

template <typename T>
void print_type(T &&arg) // 只接收右值
{
    std::cout << "rvalue" << std::endl;
}

template <typename T>
void print_type_2(T &&arg) // 1. 万能引用：可接收左值或右值
{
    // 2. 引用折叠：只会出现 0-2 个 & 符号
    // 若 T = int，则 int &&
    // 若 T = int&， 则 int& && = int&
    // 若 T = int&&，则 int&& && = int&&
    print_type(std::forward<T>(arg)); // 3. 完美转发：保留参数的左值或右值属性
}

int main()
{
    int arg = 1;
    print_type_2(arg); // 左值
    print_type_2(1);   // 右值

    return 0;
}
/*
输出：
lvalue
rvalue
*/