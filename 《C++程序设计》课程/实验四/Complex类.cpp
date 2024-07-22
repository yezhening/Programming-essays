#include<iostream>
using namespace std;

class Complex
{
private:
	double x;
	double y;
public:
	Complex(double x = 0.0, double y = 0.0);
	Complex& operator+=(const Complex&);
	Complex& operator-=(const Complex&);
	Complex& operator*=(const Complex&);
	Complex& operator/=(const Complex&);
	Complex operator++();//前置++，实部和虚部分别加1
	Complex operator++(int);//后置++，实部和虚部分别加1
	Complex operator--();//前置--，实部和虚部分别减1
	Complex operator--(int);//后置--，实部和虚部分别减1
	Complex operator-();//实部和虚部分别求反

	friend bool operator>(const Complex& c1, const Complex& c2);//c1的模大于c2的模，返回true，否则返回false
	friend bool operator<(const Complex& c1, const Complex& c2);//c1的模小于c2的模，返回true，否则返回false
	friend Complex operator+(const Complex&, const Complex&);
	friend Complex operator-(const Complex&, const Complex&);
	friend Complex operator*(const Complex&, const Complex&);
	friend Complex operator/(const Complex&, const Complex&);
	friend bool operator==(const Complex&, const Complex&);
	friend bool operator!=(const Complex&, const Complex&);
	friend ostream& operator<<(ostream&, const Complex&);
	friend istream& operator>>(istream&, Complex&);
};

Complex::Complex(double x, double y)
{
	this->x = x;
	this->y = y;
}
Complex& Complex::operator+=(const Complex& p)
{
	x = x + p.x;
	y = y + p.y;
	return *this;
}
Complex& Complex::operator-=(const Complex& p)
{
	x = x - p.x;
	y = y - p.y;
	return *this;
}
Complex& Complex::operator*=(const Complex& p)
{
	Complex t;
	t.x = x;
	t.y = y;
	x = t.x * p.x - t.y * p.y;
	y = t.x * p.y + t.y * p.x;
	return *this;
}
Complex& Complex::operator/=(const Complex& p)
{
	Complex t;
	t.x = x;
	t.y = y;
	x = ((t.x * p.x + t.y * p.y) / (p.x * p.x + p.y * p.y));
	y = ((t.y * p.x - t.x * p.y) / (p.x * p.x + p.y * p.y));
	return *this;
}
Complex Complex::operator++()
{
	x++;
	y++;
	return *this;
}
Complex Complex::operator++(int)
{
	Complex t;
	t.x = x;
	t.y = y;
	x++;
	y++;
	return t;
}
Complex Complex::operator--()
{
	x--;
	y--;
	return *this;
}
Complex Complex::operator--(int)
{
	Complex t;
	t.x = x;
	t.y = y;
	x--;
	y--;
	return t;
}
Complex Complex::operator-()
{
	Complex t;
	t.x = -x;
	t.y = -y;
	return t;
}
bool operator>(const Complex& c1, const Complex& c2)
{
	if ((c1.x * c1.x + c1.y * c1.y) > (c2.x * c2.x + c2.y * c2.y))
		return true;
	return false;
}
bool operator<(const Complex& c1, const Complex& c2)
{
	if ((c1.x * c1.x + c1.y * c1.y) < (c2.x * c2.x + c2.y * c2.y))
		return true;
	return false;
}
Complex operator+(const Complex& i, const Complex& j)
{
	Complex t;
	t.x = i.x + j.x;
	t.y = i.y + j.y;
	return t;
}
Complex operator-(const Complex& i, const Complex& j)
{
	Complex t;
	t.x = i.x - j.x;
	t.y = i.y - j.y;
	return t;
}
Complex operator*(const Complex& i, const Complex& j)
{
	Complex t;
	t.x = i.x * j.x - i.y * j.y;
	t.y = i.x * j.y + i.y * j.x;
	return t;
}
Complex operator/(const Complex& i, const Complex& j)
{
	Complex t;
	t.x = ((i.x * j.x + i.y * j.y) / (j.x * j.x + j.y * j.y));
	t.y = ((i.y * j.x - i.x * j.y) / (j.x * j.x + j.y * j.y));
	return t;
}
bool operator==(const Complex& i, const Complex& j)
{
	if (i.x == j.x && i.y == j.y)
		return true;
	return false;
}
bool operator!=(const Complex& i, const Complex& j)
{
	if (i.x != j.x || i.y != j.y)
		return true;
	return false;
}
ostream& operator<<(ostream& i, const Complex& j)
{
	i << j.x << " " << "+" << " " << j.y << "i";
	return i;
}
istream& operator>>(istream& i, Complex& j)
{
	i >> j.x >> j.y;
	return i;
}

int main()
{
	Complex c1, c2;
	cin >> c1 >> c2;
	cout << "c1 = " << c1 << "\n" << "c2 = " << c2 << endl;
	cout << "c1+c2 = " << c1 + c2 << endl;
	cout << "c1-c2 = " << c1 - c2 << endl;
	cout << "c1*c2 = " << c1 * c2 << endl;
	cout << "c1/c2 = " << c1 / c2 << endl;
	cout << (c1 += c2) << endl;
	cout << (c1 -= c2) << endl;
	cout << (c1 *= c2) << endl;
	cout << (c1 /= c2) << endl;
	cout << (c1 == c2) << " " << (c1 != c2) << endl;
	Complex c3;
	c3 = -c1;
	cout << c3 << endl;
	c3 = c1++;
	cout << c3 << endl;
	c3 = ++c1;
	cout << c3 << endl;
	c3 = c1--;
	cout << c3 << endl;
	c3 = --c1;
	cout << c3 << endl;
	cout << (c3 > c1) << endl;
	cout << (c2 < c1) << endl;
	return 0;
}

