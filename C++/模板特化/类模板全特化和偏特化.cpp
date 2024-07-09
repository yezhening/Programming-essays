#include <iostream>

using namespace std;

template <typename T, typename U>
class MyClass
{
public:
    MyClass(T t, U u) : t_(t), u_(u) {}

    void myPrint()
    {
        cout << this->t_ << " 泛化 " << this->u_ << endl;
    }

private:
    T t_;
    U u_;
};

// 全特化
template <>
class MyClass<double, int>
{
public:
    MyClass(double t, int u) : t_(t), u_(u) {}

    void myPrint()
    {
        cout << this->t_ << " 全特化 " << this->u_ << endl;
    }

private:
    double t_;
    int u_;
};

// 偏特化
template <typename T>
class MyClass<T, int>
{
public:
    MyClass(T t, int u) : t_(t), u_(u) {}

    void myPrint()
    {
        cout << this->t_ << " 偏特化 " << this->u_ << endl;
    }

private:
    T t_;
    int u_;
};

int main()
{
    MyClass<double, string> a(1.0, "2");
    a.myPrint();

    MyClass<double, int> b(1.0, 3);
    b.myPrint();

    MyClass<int, int> c(1, 4);
    c.myPrint();

    return 0;
}

/*
输出：
1 泛化 2
1 全特化 3
1 偏特化 4
*/