#include<iostream>
#include<string>
using namespace std;

class CPU
{
public:
	CPU(string c, double f)
	{
		cputype = c;
		frequency = f;
	}
	string getcputype()
	{
		return cputype;
	}
	double getfrequency()
	{
		return frequency;
	}
private:
	string cputype;
	double frequency;
};
class Disk
{
public:
	Disk(string d, double a)
	{
		disktype = d;
		capacity = a;
	}
	string getdisktype()
	{
		return disktype;
	}
	double getcapacity()
	{
		return capacity;
	}
private:
	string disktype;
	double capacity;
};
class Computer
{
public:
	Computer(class CPU c, class Disk d) :p1(c),p2(d)
	{}
	void Print()
	{
		cout << "The computer has a cpu and a disk." << endl;
		cout << "CPU type: " << p1.getcputype() << ", CPU frequency: " << p1.getfrequency() << " GHz" << endl;
		cout << "disk type: " << p2.getdisktype() << ", disk capacity: " << p2.getcapacity() <<" T" << endl;
	}
private:
	CPU p1;
	Disk p2;
};

int main()
{
	string cpuType, diskType;
	double frequency, capacity;
	cin >> cpuType >> frequency >> diskType >> capacity;
	CPU cpu(cpuType, frequency);
	Disk disk(diskType, capacity);
	Computer computer(cpu, disk);

	computer.Print();
	return 0;
}

