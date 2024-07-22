#include<iostream>
#include <iomanip>
#include <cmath>
using namespace std;
const double PI = acos(-1.0);

class Point
{
private:
    double x, y;
public:
    Point(double a = 0, double b = 0) :x(a), y(b) {}
    ~Point() {}
    double getX() const { return x; }
    double getY() const { return y; }
    void setX(double a) { x = a; }
    void setY(double b) { y = b; }
};

class Ellipse
{
public:
    Point pAxis;
    Ellipse(double x = 0, double y = 0) :pAxis(x, y) { }
    ~Ellipse() { }
    Ellipse(const Ellipse&);
    Ellipse transposition();    // 转置（交换长短半轴）
    double area();            // 面积
    double eccentricity();        // 离心率
    string position(const Point);    // 位置关系
};

Ellipse::Ellipse(const Ellipse& p)
{
    this->pAxis=p.pAxis;
}
Ellipse Ellipse:: transposition()
{
    Ellipse t;
    t.pAxis.setX ( pAxis.getY());
    t.pAxis.setY(pAxis.getX());
    return t;
}
double Ellipse::area()
{
    double area;
    area = (PI * pAxis.getX() * pAxis.getY());
    return area;
}
double Ellipse::eccentricity()
{
    double  e;
    if(pAxis.getX()> pAxis.getY())
        e = sqrt(1-((pAxis.getY() * pAxis.getY())/(pAxis.getX() * pAxis.getX())));
    else
        e = sqrt(1 - ((pAxis.getX() * pAxis.getX()) / (pAxis.getY() * pAxis.getY())));
    return e;
}
string Ellipse::position(const Point p)
{
    double position;
    position = ((p.getX() * p.getX()) / (pAxis.getX() * pAxis.getX())) + ((p.getY() * p.getY()) / (pAxis.getY() * pAxis.getY()));
    if (position < 1)
        return  "inside";
    else if (position == 1)
        return  "at";
    else
        return "outside";
}


int main()
{
    double a, b;
    while (cin >> a >> b)
    {
        Ellipse d1(a, b), d2(d1), d3 = d2.transposition();
        cout << d1.area() << endl;
        cout << d2.eccentricity() << endl;
        cin >> a >> b;
        Point p(a, b);
        cout << d3.position(p) << endl;
    }
    return 0;
}


