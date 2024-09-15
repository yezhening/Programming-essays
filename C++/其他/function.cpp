#include <functional>
#include <iostream>

void print(int)
{
    std::cout << "function" << std::endl;
}

int main()
{
    std::function<void(int)> func = print; // 用于包装任何“可调用对象”。进阶可用于实现“回调”
    func(1);                               // 输出: function

    return 0;
}