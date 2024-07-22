#include <iostream>
#include <string>
using namespace std;

class Hotel
{
public:
	void add(string name);
	static int total;
	static int getTotal();
	string getName();
	void print();
private:
	string name;
	int n;
};

void Hotel::add(string name)
{
	this->name=name;
	total++;
	n=total;
}
int Hotel::total=0;
int Hotel::getTotal()
{
	return total;
}
string Hotel::getName()
{
	return name;
}
void Hotel::print()
{
	cout<<n<<" "<<name<<" "<<total<<endl;
}

int main()
{
    Hotel h[100];
    h[0].add("Susan");
    h[1].add("Peter");
    h[2].add("John");
    h[3].add("Mary");
    h[4].add("Alice");
    string name;
    cin>>name;
    for(int i=0;i<Hotel::getTotal();i++)
    {
       if(h[i].getName()==name)
       {
          h[i].print();
          break;
       }
    }
    return 0;
}