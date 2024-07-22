#include<iostream>
#include<string>
#include<sstream>
using namespace std;

void readPapers(string p)
{
	stringstream strm(p);
	string temp;
	int count1 = 0, count2 = 0;
	while (strm >> temp)
	{
		count1++;
		int i;
		for (i = 0; i < temp.size(); i++)
		{
			if (temp[i] == '.' || temp[i] == ',' || temp[i] == '"')
				count2++;
		}
	}
	cout << count1 <<","<< count2 <<endl;
}

int main() {
    std::string content;
    std::getline(std::cin, content, '\n');
    readPapers(content);
    return 0;
}
