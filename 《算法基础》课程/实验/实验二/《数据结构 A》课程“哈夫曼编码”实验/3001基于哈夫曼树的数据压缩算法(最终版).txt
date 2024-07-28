#include <iostream>
#include <cstring>//使用memset、strcpy函数
#include <cmath>//必须用
using namespace std;

int fre[26];//记录字符出现频率数组
char namelist[26];
int num = 0;//输入一串字符数目
int k = 0;//遍历字符/叶子结点数变量
int kn = 0;//字符/叶子结点数knotnum

typedef struct//哈夫曼树的存储表示
{
	int weight;
	int parent, lchild, rchild;
}HTNode, * HuffmanTree;

typedef char** HuffmanCode;

typedef struct //哈夫曼编码的存储表示。定义模糊
{
	int Freque;
	HuffmanCode HCC;
	char ch;
}Letter;

void InitData(int fre[])//初始化数据
{
	int i;
	for (i = 0; i < 26; i++)
		fre[i] = 0;//初始化字符出现频率

	memset(namelist, 0, sizeof(namelist));
	/*memset：作用是在一段内存块中填充某个给定的值，它是对较大的结构体或数组进行清零操作的一种
	最快方法。这条语句是把namelist中长度为sizeof(namelist)即所有的字节换做字符“0”，常用来对
	指针或字符串的初始化。*/
	num = 0;
	k = 0;
	kn = 0;
	return;//勿漏
}

void RecordAndCalculate(int n[], string s)//记录字符及计算频率
{
	int i = 0;

	//字符转化为数字
	while (s[i] != '\0')//字符串有结尾符
	{
		int e;//ACALL码转换变量
		e = (int)s[i];//字母转化为ASCALL码
		if (e >= 97)//如果是小写字母97-122
		{
			e = e - 97;//转化为数字0-25
			n[e]++;//桶排序，插旗子。该数字频率+1
		}
		i++;
	}
	num = i;//共有的字符数

	//记录字符/叶子结点数
	for (i = 0; i < 26; i++)
	{
		if (n[i])
			kn++;
	}

	//数字转化为字符并输出
	for (i = 0; i < 26; i++)
	{
		if (n[i])
		{
			char e;
			e = (char)(i + 97);
			cout << e << ":" << n[i];//输出第1行
			if (k != kn-1)//最后的错误！若是kn最后有空格，kn-1则没有
				cout << " ";//n个数据中n-1个空格，换成"-"测试最后有无空格
			fre[k] = n[i];//记录频率
			namelist[k] = e;//记录字符
			k++;
		}
	}
	cout << endl;
}

void Select(HuffmanTree HT, int len, int& s1, int& s2)
{
	int min1 = 0x3f3f3f3f, min2 = 0x3f3f3f3f;//赋予整形最大值，专业些
	int i;

	for (i = 1; i <= len; i++)//选择第一个数据
	{
		if (HT[i].weight < min1 && HT[i].parent == 0)
		{
			min1 = HT[i].weight;//min1不断更新比较，从而选出最小值
			s1 = i;
		}
	}

	int temp;
	temp = HT[s1].weight;
	HT[s1].weight = 0x3f3f3f3f;//将原值存放起来，赋予最大值，防止选择s2时s1被重复选择

	for (i = 1; i <= len; i++)//选择第二个数据
	{
		if (HT[i].weight < min2 && HT[i].parent == 0)
		{
			min2 = HT[i].weight;
			s2 = i;
		}
	}
	HT[s1].weight = temp;//恢复s1的值
	//不需要return,s1、s2的值已经改变
}

void CreateHuffmanTree(HuffmanTree& HT, int n)//构造哈夫曼树HT
{
	//初始化
	if (n <= 1)
		return;
	int m;
	m = 2 * n - 1;
	HT = new HTNode[m + 1];
	int i;
	for (i = 1; i <= m; ++i)
		/*根据for循环的语法定义 ++i 和 i++的结果是一样的，都要等代码块执行完毕才能执行语句3，
		但是性能是不同的。在大量数据的时候++i的性能要比i++的性能好原因：i++由于是在使用当前值
		之后再 + 1，所以需要一个临时的变量来转存。而++i则是在直接 + 1，省去了对内存的操作的环节，
		相对而言能够提高性能*/
	{
		HT[i].parent = 0;
		HT[i].lchild = 0;
		HT[i].rchild = 0;
	}
	for (i = 1; i <= n; ++i)
	{
		//cin >> HT[i].weight;不是手动输入权值，需要函数计算频率
		HT[i].weight = fre[i - 1];//频率数组是n[0-25],此处循环从1开始需要-1
	}

	//构造
	int s1, s2;
	for (i = n + 1; i <= m; ++i)
	{
		Select(HT, i - 1, s1, s2);
		HT[s1].parent = i;
		HT[s2].parent = i;
		HT[i].lchild = s1;
		HT[i].rchild = s2;
		HT[i].weight = HT[s1].weight + HT[s2].weight;
	}
}

void CreatHuffmanCode(HuffmanTree HT, HuffmanCode& HC, int n)
//从叶子到根逆向求每个字符的赫夫曼编码，存储在编码表HC中
{
	int i, start, c, f;
	HC = new char* [n + 1];
	char* cd;
	cd = new char[n];
	cd[n - 1] = '\0';

	for (i = 1; i <= n; ++i)
	{
		start = n - 1;
		c = i;
		f = HT[i].parent;
		while (f != 0)
		{
			--start;
			if (HT[f].lchild == c)
				cd[start] = '0';
			else
				cd[start] = '1';
			c = f;
			f = HT[f].parent;
		}

		HC[i] = new char[n - start];
		strcpy(HC[i], &cd[start]);
	}

	delete cd;
}

void Print(HuffmanTree HT, HuffmanCode HC)
{
	//输出第2至2n行
	int i;
	for (i = 1; i <= 2 * k - 1; i++)
		cout << i << " " << HT[i].weight << " " << HT[i].parent << " " << HT[i].lchild << " " << HT[i].rchild << endl;

	//输出第2n+1行
	//int i;//易错,重置i×重定义
	for (i = 1; i <= k; i++)
	{
		cout << namelist[i - 1] << ":" << HC[i];//字符数组是namelist[0-25],此处循环从1开始需要-1
		if (i != k)
			cout << " ";
	}
	cout << endl;
}

void EncodeAndCode(HuffmanCode HC, string s)//编码与解码
{
	//输出第2n + 2行
	int i = 0;
	while (s[i])//遍历字符
	{
		int j = 0;
		while (j < 26)
		{
			if (s[i] == namelist[j])//如果字符和字符数组中字符一样
				cout << HC[j + 1];//字符数组namelist[0-25],编码数组标号+1 
			j++;
		}
		i++;
	}
	cout << endl;

	//输出第2n + 3行
	i = 0;
	while (s[i])
	{
		cout << s[i];
		i++;
	}
	cout << endl;
}

int main()
{
	while (1)
	{
		string s;
		cin >> s;
		if (s == "0")
			break;
		if (s.length() == 1)//结点为一个的特殊情况，投机取巧
		{
			cout << s << ":" << 1 << endl;
			cout << "1 1 0 0 0" << endl;
			cout << s << ":" << 0 << endl;
			cout << 0 << endl;
			cout << s << endl;
		}

		if (s.length() != 1)
		{
			InitData(fre);
			RecordAndCalculate(fre, s);
			HuffmanTree HT;
			CreateHuffmanTree(HT, k);
			HuffmanCode HC;
			CreatHuffmanCode(HT, HC, k);
			Print(HT, HC);
			EncodeAndCode(HC, s);
		}
	}
	return 0;
}