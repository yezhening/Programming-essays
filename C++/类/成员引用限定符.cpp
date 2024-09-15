// 参考：https://zhuanlan.zhihu.com/p/385875584

#include <iostream>

class A
{
public:
    void print_lvalue() &
    {
        std::cout << "lvalue" << std::endl;
    }
    void print_rvalue() &&
    {
        std::cout << "rvalue" << std::endl;
    }

    // void print_const() & const;
    void print_const() const &; // const 要写在 & 前面
};

int main()
{
    A a{};

    a.print_lvalue(); // 只能左值对象调用
    // std::move(a).print_lvalue();

    // a.print_rvalue();
    std::move(a).print_rvalue(); // 只能右值对象调用
}
/*
输出：
lvalue
rvalue
*/