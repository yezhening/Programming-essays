#include<iostream>
#include<string>
using namespace std;

class Hotel
{
private:
	int number;
	string name;
public:
	static int total;
	string GetName()
	{
		return name;
	}
	void SetName(string name)
	{
		number = total;
		total++;
		this->name = name;
	}
	static int GetTotal()
	{
		return total;
	}
	void Print()
	{
		cout << "id: " << number << ", name: " << name << endl;
	}

};
int Hotel::total = 0;
int GetName(string& name)
{
	if (getline(cin, name))
		return true;
	else
		return false;
}

int main()
{
	int n;
	cin >> n;
	cin.get();
	Hotel* h = new Hotel[n + 1];
	string name;
	for (int i = 0; i < n; i++)
	{
		GetName(name);
		h[i].SetName(name);
	}
	h[n].SetName("YOU");
	while (GetName(name))
	{
		bool bFound = false;
		for (int i = 0; i < Hotel::GetTotal(); i++)
		{
			if (h[i].GetName() == name)
			{
				cout << name << " found! ";
				h[i].Print();
				bFound = true;
				break;
			}
		}
		if (!bFound)
			cout << name << " Not found!\n";
	}
	return 0;
}