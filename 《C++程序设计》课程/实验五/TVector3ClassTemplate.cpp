#include <iostream>
//using namespace std;
template<typename T>
class Vector
{
public:
    Vector()
    {
        x = 0;
        y = 0;
        z = 0;
    }
    Vector(T a, T b, T c)
    {
        x = a;
        y = b;
        z = c;
    }
    Vector(const Vector& p)
    {
        x = p.x;
        y = p.y;
        z = p.z;
    }
    friend Vector operator +(const Vector& m, const Vector& n)
    {
        Vector t;
        t.x = m.x + n.x;
        t.y = m.y + n.y;
        t.z = m.z + n.z;
        return t;
    }
    friend Vector operator *(double m, const Vector& n)
    {
        Vector t;
        t.x = m * n.x;
        t.y = m * n.y;
        t.z = m * n.z;
        return t;
    }
    friend std::ostream& operator<<(std::ostream& os, const  Vector& p)
    {
        os << p.x << " " << p.y << " " << p.z;
        return os;
    }
    friend std::istream& operator>>(std::istream& is, Vector& p)
    {
        is >> p.x >> p.y >> p.z;
        return is;
    }
    bool operator==(const Vector& p)
    {
        double t;
        t = 0.00001;
        if (x - p.x > -t && x - p.x < t && y - p.y >  -t && y - p.y < t && z - p.z>-t && z - p.z < t)
            return true;
        return false;
    }
private:
    T x, y, z;
};
int main()
{
    double a, b, c;
    std::cin >> a >> b >> c;
    Vector<double> v1(a, b, c), v2(v1), v3, v4;
    double d;
    std::cin >> d;
    v4 = d * v1 + v2;

    std::cout << v4 << std::endl;

    Vector<double>  v;
    std::cin >> v;

    int flag = (v4 == v);
    std::cout << flag << std::endl;

    return 0;
}
