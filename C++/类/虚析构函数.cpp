// 参考：https://zhuanlan.zhihu.com/p/602287423

#include <iostream>

class A
{
public:
    A()
    {
        std::cout << "create A" << std::endl;
    }

    // ~A()
    // {
    // 	std::cout << "delete A" << std::endl;
    // }
    virtual ~A()
    {
        std::cout << "delete A" << std::endl;
    }
};

class B : public A
{
public:
    B()
    {
        std::cout << "create B" << std::endl;
    }

    ~B()
    {
        std::cout << "delete B" << std::endl;
    }
};

int main()
{
    A *a = new B();
    delete a;

    return 0;
}
/*
输出：
create A
create B
delete B
delete A
*/