#include<iostream>
#include<string>
using namespace std;

class PersonInfo
{
public:
	PersonInfo(string n, int a)
	{
		name = n;
		age = a;
	}
	string Name()
	{
		return name;
	}
	int Age()
	{
		return age;
	}
private:
	string name;
	int age;
};

int main()
{
	string    name;
	int    year;
	cin >> name >> year;
	PersonInfo info(name, year);
	cout << "I am " << info.Name() << ", " << info.Age() << " years old.\n";
	return 0;
}

