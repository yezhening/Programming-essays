#include <iostream>

using namespace std;

template <typename T>
void myPrint(T t)
{
    cout << t << endl;
}

template <>
void myPrint<double>(double t)
{
    cout << t <<" double"<< endl;
}

int main()
{
    myPrint(1);
    myPrint(2.0);

    return 0;
}

/*
输出：
1
2 double
*/