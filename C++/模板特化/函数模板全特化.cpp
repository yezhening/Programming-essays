// 函数模板只支持全特化，不支持偏特化
#include <iostream>

using namespace std;

template <typename T, typename U>
void myPrint(T t, U u)
{
    cout << t << " 泛化 " << u << endl;
}

// 全特化
template <>
void myPrint<double, int>(double t, int u)
{
    cout << t << " 全特化 " << u << endl;
}

int main()
{
    myPrint(1, "a");
    myPrint(1.0, 3);

    return 0;
}

/*
输出：
1 泛化 a
1 全特化 3
*/