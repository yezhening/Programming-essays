// #include <initializer_list> // initializer_list<>
#include <iostream>

using std::cout;
using std::endl;
using std::initializer_list;

void print(initializer_list<int> li) // 使用 initializer_list<> 对象接收可变参数
{
    for (const int l : li)
    {
        cout << l << " ";
    }
    cout << endl;

    return;
}

int main()
{
    // 临时匿名 initializer_list<> 对象
    print({0, 1});
    print(initializer_list<int>{0, 1, 2});

    // 具名 initializer_list<> 对象
    initializer_list<int> init_list{0, 1, 2, 3};
    print(init_list);

    return 0;
}
/*
输出：
0 1
0 1 2
0 1 2 3
*/