#include <iostream>

class A
{
public:
    int num = 1;
};

std::ostream &operator<<(std::ostream &os, const A &a)
{
    os << a.num;
    return os;
}

int main()
{
    A a{};

    std::cout << a << std::endl;
    // 输出: 1

    return 0;
}