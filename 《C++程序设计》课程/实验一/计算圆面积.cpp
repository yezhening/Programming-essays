#include <iostream>
#include <iomanip>
#include <cmath>
const double PI=acos(-1.0);
using namespace std;
class Circle
{
public:
	void input();
	void calculate();
	void output();
private:
	double r;
	double area;
};
void Circle::input()
{
	cin>>r;
}
void Circle::calculate()
{
	area=PI*r*r;
}
void Circle::output()
{
	cout<<setiosflags(ios::fixed)<<setprecision(2);
	cout<<area<<endl;
}
int main()
{
	Circle p;
	p.input();
	p.calculate();
	p.output();
	return 0;
}