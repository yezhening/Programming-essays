#include<iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

void processPoints()
{
	fstream file1, file2;
	string s1, s2;
	double line[6], sum[6] = { 0 };
	int count = 0;

	file1.open("points.csv", ios::in);
	file2.open("points_offset.csv", ios::out);

	file2 << fixed << setprecision(3);
	getline(file1, s1);
	file2 << s1 << endl;
	while (getline(file1, s1))
	{
		stringstream ss1(s1);
		int i = 0;
		while (getline(ss1, s2, ','))
		{
			stringstream ss2(s2);
			ss2 >> line[i];
			sum[i] += line[i];
			i++;
		}
		count++;
		file2 << line[0] + 100 << "," << line[1] - 50;
		for (int j = 2; j < 6; j++)
		{
			file2 << "," << line[j];
		}
		file2 << endl;
	}
	file1.close();
	file2.close();

	fstream file3;
	file3.open("points.csv", ios::app | ios::out);

	file3 << fixed << setprecision(3);
	file3 << (sum[0] / count) << "," << (sum[1] / count) << "," << (sum[2] / count) << endl;
	file3.close();
}

int main() {
	std::cout << "Point cloud in processing..." << endl;
	processPoints();
	return 0;
}

