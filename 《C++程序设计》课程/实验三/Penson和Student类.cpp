#include <iostream>
#include <string>
using namespace std;

class Person
{
protected:
	string name;
public:
   virtual void input();
   virtual void display();
};
class Student:public Person
{
private:
	string number;
public:
	virtual void input();
	virtual void display();	
};

void Person::input()
{
	cin>>name;
}
void Person::display()
{
	cout<<name<<endl;
}
void Student::input()
{
	cin>>number>>name;
}
void Student::display()
{
	cout<<number<<" "<<name<<endl;
}

int main()
{
	Person * p;
	p = new Person;
	p->input();
	p->display();
	delete p;
	p = new Student;
	p->input();
	p->display();
	delete p;
	return 0;
}