#include <iostream>

using namespace std;

template <typename T>
class MyClass
{
public:
    MyClass(T t) : t_(t) {}

    void myPrint()
    {
        cout << this->t_ << endl;
    }

private:
    T t_;
};

template <>
class MyClass<double>
{
public:
    MyClass(double t) : t_(t) {}

    void myPrint()
    {
        cout << this->t_ << " double" << endl;
    }

private:
    double t_;
};

int main()
{
    MyClass<int> a(1);
    a.myPrint();

    MyClass<double> b(2.0);
    b.myPrint();

    return 0;
}

/*
输出：
1
2 double
*/