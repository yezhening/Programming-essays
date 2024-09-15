#include <iostream>
#include <memory>

class MyClass
{
public:
    MyClass()
    {
        std::cout << "MyClass created" << std::endl;
    }

    ~MyClass()
    {
        std::cout << "MyClass destroyed" << std::endl;
    }
};

int main()
{
    // 创建
    std::shared_ptr<MyClass> sp = std::make_shared<MyClass>();
    std::weak_ptr<MyClass> wp = sp;
    // std::weak_ptr<> 基于 std::shared_ptr<>
    // 目的是提供一个不拥有所有权的智能指针，用于观察 std::shared_ptr 管理的对象，以避免循环引用或延迟获取对象

    // 1. 避免循环引用：不增加引用计数
    std::cout << "sp use_count: " << sp.use_count() << std::endl;
    std::cout << "wp use_count: " << wp.use_count() << std::endl;

    // 2. 延迟获取对象
    std::shared_ptr<MyClass> sp_2{nullptr};
    if (!wp.expired())
    {
        sp_2 = wp.lock();
        std::cout << "sp_2 use_count: " << sp_2.use_count() << std::endl;
    }

    // 释放了 std::shared_ptr<>（引用计数为0），std::weak_ptr<> 也释放
    sp_2.reset();
    if (wp.expired())
    {
        std::cout << "sp_2 destroyed" << std::endl;
    }

    sp.reset();
    if (wp.expired())
    {
        std::cout << "sp destroyed" << std::endl;
    }

    return 0;
}
/*
输出:
MyClass created
sp use_count: 1
wp use_count: 1
sp_2 use_count: 2
MyClass destroyed
sp destroyed
*/