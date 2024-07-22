#include<iostream>
#include<string>
#include<sstream>
#include<map>
#include<iterator>
#include<algorithm>
using namespace std;

int termFrequency(string p, map<string, unsigned> &m)
{
	stringstream strm(p);
	string temp;
	int num;
	int count = 0, frequency = 1;
	while (strm >> temp)
	{
		temp.erase(remove(temp.begin(), temp.end(), '.'), temp.end());
		temp.erase(remove(temp.begin(), temp.end(), ','), temp.end());
		temp.erase(remove(temp.begin(), temp.end(), '"'), temp.end());
		transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
		stringstream t(temp);
		if (t >> num)
			continue;
		map<string, unsigned>::iterator it = m.find(temp);
		if (it == m.end())
		{
			m[temp] = frequency;
			count++;
		}
		else
			m[temp]++;
	}
	return count;
}
void alphabetSortedFrequency(map<string, unsigned> &m)
{
	map<string, unsigned>::iterator it;
	for (it = m.begin(); it != m.end(); it++)
		cout << it->first << ":" << it->second << endl;
}

int main() {

	// 从标准输入获取文本串
	std::string content;
	std::getline(std::cin, content, '\n');

	map<string, unsigned> msu;

	// 要求termFrequency实现分词，去掉标点
	// 获取单词存放在map中，记录词频（出现次数）
	// 最后返回不重复的单词数量    
	unsigned nWords = termFrequency(content, msu);

	// 按首字母A-Z排序一行一词输出词频
	alphabetSortedFrequency(msu);

	return 0;
}

