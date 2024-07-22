#include <iostream>
using namespace std;
class Rectangle
{
public:
	void number();
	void Area();
	void Perimeter();
	void caculate();
	void output();
private:
	double length;
	double width;
	double area1;
	double area2;
	double area3;
	double perimeter;
	double money1;
	double money2;
};
void Rectangle::number()
{
	cin>>length>>width;
}
void Rectangle::Area()
{
	area1=length*width;
	area2=(length+3)*(width+3);
	area3=area2-area1;
}
void Rectangle::Perimeter()
{
	perimeter=2*(length+width+6);
}
void Rectangle::caculate()
{
	money1=50*perimeter;
	money2=240*area3;
}
void Rectangle::output()
{
	cout<<money1<<endl;
	cout<<money2<<endl;
}
int main()
{
	Rectangle rect;
	rect.number();
	rect.Area();
	rect.Perimeter();
	rect.caculate();
	rect.output();
	return 0;
}