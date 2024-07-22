#include <iostream>
#include <iomanip>
#include <cmath>
const double PI=acos(-1.0);
using namespace std;

class Shape
{
	public:
		Shape(){}
		virtual double area() = 0;
		virtual void input() = 0;
		virtual double volume() = 0;
		virtual ~Shape() {}
};
class Cuboid:public Shape
{
public:
	virtual double area();
	virtual void input();
	virtual double volume();
private:
	double length;
	double width;
	double height;
};
class Cylinder:public Shape
{
public:
	virtual double area();
	virtual void input();
	virtual double volume();
private:
	double r;
	double h;
};
class Ball:public Shape
{
public:
	virtual double area();
	virtual void input();
	virtual double volume();
private:
	double R;
};

double Cuboid::area()
{
	return 2*(length*width+length*height+width*height);
}
void Cuboid::input()
{
	cin>>length>>width>>height;
}
double Cuboid::volume()
{
	return length*width*height;
}
double Cylinder::area()
{
	return 2*PI*r*r+2*PI*r*h;
}
void Cylinder::input()
{
	cin>>r>>h;
}
double Cylinder::volume()
{
	return PI*r*r*h;
}
double Ball::area()
{
	return 4*PI*R*R;
}
void Ball::input()
{
	cin>>R;
}
double Ball::volume()
{
	return (4*PI*R*R*R)/3;
}
void work(Shape *s)
{
	s->input();
	cout << s->area() << " " << s->volume() << endl;
	delete s;
}

int main() 
{
	char c;
	while (cin >> c) 
	{
		switch (c) 
		{
			case 'y':
				work(new Cylinder());
				break;
			case 'c':
				work(new Cuboid());
				break;
			case 'q':
				work(new Ball());
				break;
			default:
				break;
		}
	}
	return 0;
}
