#include <iostream>
#include <string>
using namespace std;

class Person
{
public:
	Person(string a,string b);
	Person(const Person &);
	string getName() const;
    string getAddress() const;
private:
	string name;
	string address;
};

Person::Person(string a,string b):name(a),address(b)
{}
Person::Person(const Person &p)
{
	name=p.name;
	address=p.address;
}
string Person::getName()const
{
	return name;	
}
string Person::getAddress()const
{
	return address;
}

int main()
{
   const Person Amy("Amy","Beijing Forestry Univeristy");   //姓名和地址
   const Person copy_Amy(Amy);
   cout<<"Name: "<<Amy.getName()<<"\nAddress: "<<Amy.getAddress()<<endl; 
   cout<<"\nThis is a copy of Amy:\n";
   cout<<"Name: "<<copy_Amy.getName()<<"\nAddress: "<<copy_Amy.getAddress()<<endl; 
   return 0;
}