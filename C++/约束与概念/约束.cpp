#include <iostream>

// 约束: T 是 int 类型，函数模板才有效
template <typename T, typename = std::enable_if_t<std::is_integral_v<T>, void>>
// T 是 int 类型，则 std::is_integral_v<T> == true
// std::enable_if_t<true, void> == void
// template <typename T, typename = void>，模板有效
// 否则 template <typename T, typename = 未定义>，模板无效
void print(T arg)
{
    std::cout << arg << std::endl;
}

int main()
{
    print(1);
    // 正确：1 是 int 类型
    // 输出：1

    // print("1"); 
    // 错误："1" 不是 int 类型，无法匹配函数模板（SFINAE）

    return 0;
}