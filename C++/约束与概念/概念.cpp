#include <iostream>

// 概念: Integral 概念要求 T 是 int 类型
template <typename T>
concept Integral = std::is_integral_v<T>;

// T 是 int 类型，模板有效
// 否则 模板无效
template <Integral T>
void print(T arg)
{
    std::cout << arg << std::endl;
}

int main()
{
    print(1);
    // 正确：1 是 int 类型，模板有效
    // 输出：1

    // print("1");
    // 错误："1" 不是 int 类型，无法匹配函数模板（SFINAE）

    return 0;
}