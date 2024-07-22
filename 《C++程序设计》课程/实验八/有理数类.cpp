#include<iostream>
#include <cmath>
using namespace std;

class Rational
{
private:
    int z;    //分子
    int m;    //分母
public:
    Rational(int a = 0, int b = 1);  //构造有理数分数，分子默认为0，分母默认为1
    Rational& yuefen(); //约分函数对分数化简
    friend Rational operator+(const Rational& r1, const Rational& r2);
    friend Rational operator-(const Rational& r1, const Rational& r2);
    friend Rational operator*(const Rational& r1, const Rational& r2);
    friend Rational operator/(const Rational& r1, const Rational& r2);
    Rational& operator+=(const Rational& r);
    Rational& operator-=(const Rational& r);
    Rational& operator*=(const Rational& r);
    Rational& operator/=(const Rational& r);
    friend bool operator==(const Rational&, const Rational&);//判断两个有理数是否相等
    friend bool operator!=(const Rational&, const Rational&);//判断两个有理数是否不等
    friend ostream& operator<<(ostream&, const Rational&);
    friend istream& operator>>(istream&, Rational&);
};

int gcd(int a, int b) 
{
    if (b == 0)
        return a;
    return gcd(b, a % b);
}
Rational::Rational(int a, int b)
{
    z = a;
    m = b;
}
Rational& Rational::yuefen()
{
    if (m < 0)
    {
        z = -z;
        m = -m;
    }
    int t;
    t = fabs(gcd(z, m));
    z = z / t;
    m = m / t;
    return *this;
}
Rational operator+(const Rational& r1, const Rational& r2) 
{ 
    Rational t;			
    t.z = r1.z * r2.m + r2.z * r1.m;			
    t.m = r1.m * r2.m;			
    t.yuefen();			
    return t; 
}
Rational operator-(const Rational& r1, const Rational& r2) 
{ 
    Rational t;			
    t.z = r1.z * r2.m - r2.z * r1.m;			
    t.m = r1.m * r2.m;			
    t.yuefen();			
    return t; 
}
Rational operator*(const Rational& r1, const Rational& r2) 
{ 
    Rational t;			
    t.z = r1.z * r2.z;			
    t.m = r1.m * r2.m;			
    t.yuefen();			
    return t; 
}   
Rational operator/(const Rational& r1, const Rational& r2) 
{
    Rational t;			
    t.z = r1.z * r2.m;			
    t.m = r1.m * r2.z;		
    t.yuefen();			
    return t;		
}
Rational& Rational::operator+=(const Rational& r)
{
    *this = *this + r;
    this->yuefen();
    return *this;
}
Rational& Rational::operator-=(const Rational& r) 
{ 
    *this = *this - r;	
    this->yuefen();
    return *this; 
}
Rational& Rational::operator*=(const Rational& r) 
{ 
    *this = *this * r;
    this->yuefen();
    return *this; 
}	
Rational& Rational::operator/=(const Rational& r) 
{ 
    *this = *this / r;
    this->yuefen();
    return *this; 
}
bool operator==(const Rational& r1, const Rational& r2) 
{ 
    if (r1.z == r2.z && r1.m == r2.m)				
        return true;			
    else				
        return false; 
}
bool operator!=(const Rational& r1, const Rational& r2)
{
    if (r1.z != r2.z || r1.m != r2.m)
        return true;
    else
        return false;
}
ostream& operator<<(ostream& os, const Rational& r) 
{ 
    Rational t;
    if (r.m < 0)
    {
        t.z = -r.z;
        t.m = -r.m;
    }
    else
    {
        t.z = r.z;
        t.m = r.m;
    }
    t.yuefen();
    os << t.z << "/" << t.m;
    return os;
}
istream& operator>>(istream& is, Rational& r) 
{ 
    is >> r.z >> r.m;			
    return is; 
}

int main()
{
    Rational r1, r2, r3;

    while (cin >> r1 >> r2)
    {
        cout << "r1 = " << r1 << "\n" << "r2 = " << r2 << endl;
        r3 = r1 + r2;
        cout << "r1+r2 = " << r3 << endl;
        r3 = r1 - r2;
        cout << "r1-r2 = " << r3 << endl;
        r3 = r1 * r2;
        cout << "r1*r2 = " << r3 << endl;
        r3 = r1 / r2;

        cout << "r1/r2 = " << r3 << endl;
        cout << (r1 == r2) << " " << (r1 != r2) << endl;
        cout << (r1 += r2) << endl;
        cout << (r1 -= r2) << endl;
        cout << (r1 *= r2) << endl;
        cout << (r1 /= r2) << endl;
    }

    return 0;
}


