#include<iostream>
#include<cmath>
using namespace std;

template<typename T>
class Point2
{
private:
    T x;
    T y;
public:
    Point2(T a, T b)
    {
        x = a;
        y = b;
    }
    T getx()
    {
        return x;
    }
    T gety()
    {
        return y;
    }
};
template<typename T>
class Line2
{
private:
    Point2<T> p1;
    Point2<T> p2;
public:
    Line2(class Point2 <T> a, class Point2 <T> b):p1(a),p2(b)
    {}
    T Length()
    {
        T length,cx,cy;
        if (p1.getx() >= p2.getx())
            cx = (p1.getx() - p2.getx()) * (p1.getx() - p1.getx());
        else
            cx= (p2.getx() - p1.getx()) * (p2.getx() - p1.getx());
        if (p1.gety() >= p2.gety())
            cy = (p1.gety() - p2.gety()) * (p1.gety() - p1.gety());
        else
            cy = (p2.gety() - p1.gety()) * (p2.gety() - p1.gety());
        length = fabs(sqrt(cx+cy));
        return length;
    }
};

int main()
{
    Point2<double> pt1(1.0, 1.0);
    Point2<double> pt2(3.0, 4.0);
    Line2<double> line(pt1, pt2);
    cout << line.Length() << endl;

    int x1, y1, x2, y2;
    cin >> x1 >> y1 >> x2 >> y2;
    Line2<int> nLine(Point2<int>(x1, y1), Point2<int>(x2, y2));
    cout << nLine.Length() << endl;

    return 0;
}




