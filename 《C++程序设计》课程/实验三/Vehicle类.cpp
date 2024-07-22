#include <iostream>
#include <string>
using namespace std;

class Vehicle
{
public:
	virtual void display()=0;
protected:
	string name;
	string color;
};
class Car: public Vehicle
{
public:
	Car(string n,string c,int p);
	virtual void display();
private:
	int passage;
};
class Truck: public Vehicle
{
public:
	Truck(string n,string c,double p);
	virtual void display();
private:
	double capacity;
};

Car::Car(string n,string c,int p)
{
	name=n;
	color=c;
	passage=p;
}
void Car::display()
{
	cout<<"Car name:"<<name<<" "<<"Car color:"<<color<<" "<<"Car passager:"<<passage<<endl;
}
Truck::Truck(string n,string c,double p)
{
	name=n;
	color=c;
	capacity=p;
}
void Truck::display()
{
	cout<<"Truck name:"<<name<<" "<<"Truck color:"<<color<<" "<<"Truck capacity:"<<capacity<<endl;
}

int main()
{
	Vehicle *p;
	char type;
	char name[110],color[110];
	int pas;
	double cap;
	while(cin>>type)
	{
		cin>>name>>color;
		if(type == 'C')
		{
			cin>>pas;
			Car car(name,color,pas);
			p = &car;
			p->display();
		}
		else if(type == 'T')
		{
			cin>>cap;
			Truck truck(name,color,cap);
			p = &truck;
			p->display();
		}
	}
	return 0;
}