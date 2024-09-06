#include <iostream>

// 概念: Integral 概念要求 T 类型的两个变量能够相加且结果是 int 类型
template <typename T>
concept Integral = requires(T a, T b) 
{
    { a + b } -> std::same_as<int>;
};

// T 类型的两个变量能够相加且结果是 int 类型，模板有效
// 否则 模板无效
void print(Integral auto arg)
{
    std::cout << arg << std::endl;
}

int main()
{
    print(1);
    // 正确：1 是 int 类型，该类型的两个变量能够相加且结果是 int 类型，模板有效
    // 输出：1

    // print("1");
    // 错误："1" 是 const char * 类型，该类型的两个变量不能够相加且结果不是 int 类型，无法匹配函数模板（SFINAE）

    return 0;
}