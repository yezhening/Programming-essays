#include <iostream>

using namespace std;

template <typename T>
void myPrint(T t)
{
    cout << "泛化 " << t << endl;
}

template <>
void myPrint<double>(double t)
{
    cout << "特化 " << t << endl;
}

int main()
{
    myPrint(1);
    myPrint(2.0);

    return 0;
}

/*
输出：
泛化 1
特化 2
*/