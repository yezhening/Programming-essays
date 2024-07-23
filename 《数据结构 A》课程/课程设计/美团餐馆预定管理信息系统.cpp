//-----头文件-----
#include<iostream>
#include<string>
#include<fstream>//读写文件
#include<sstream>//流
#include <cmath>//abs()函数
#include<stdlib.h>//system("cls");清屏
#include<vector>//容器用于KMP
#include<iomanip>//左对齐setw(20)
using namespace std;

//-----全局变量-----
#define MAXSIZE 10000
#define HASHSIZE 1000//散列表大小
#define NULLKEY 0//散列表单元为空的标记
#define MaxInt 0x7fffffff//图的邻接矩阵初始化为最大值
#define MVNum 10000//图的最大顶点数，开邻接矩阵

//-----用户信息，商家信息，距离信息，预定信息-----
//-----顺序表，图，链队列数据结构-----
typedef struct
{
	string useraccount;
	string password;
	string contact;
}Userinformation;
typedef struct
{
	Userinformation* elem;
	int length;
}SqList;

typedef struct
{
	string shoptype;
	string shopid;
	string shopname;
	string shoppassword;
	string avgscore;
	string aveprice;
	string address;
	string phone;
	string foodid[100];
	string foodname[100];
	string foodprice[100];
	string comment[100];
}Shopinformation;
typedef struct
{
	Shopinformation* elem;
	int length;
}SqList2;

typedef struct
{
	string shoptype;
	string shopname;
	string avgscore;
	string aveprice;
	string address;
	string phone;
	string foodname[100];
	string comment[100];
}RecommendationInformation;
typedef struct
{
	RecommendationInformation* elem;
	int length;
}SqList3;

typedef struct
{
	long int key;//long int而不是int
	string shopname;
	string shoptype;
	string shopid;
	string shoppassword;
	string avgscore;
	string aveprice;
	string address;
	string phone;
	string foodid[100];
	string foodname[100];
	string foodprice[100];
	string comment[100];
}ElemType;
typedef struct
{
	ElemType* elem;
	int count;// 当前数据元素个数 
	int size;//当前表的容量
}HashTable;

typedef struct QNode
{
	string useraccount;
	string mechantname;//shopname
	string ordertime;
	struct QNode* next;
}QNode, * QueuePtr;
typedef struct
{
	QueuePtr front;
	QueuePtr rear;
}LinkQueue;

typedef struct
{
	string address1;
	string address2;
	string distance;
}Distanceinformation;
typedef struct
{
	Distanceinformation* elem;
	int length;
}AmGraphSqList;

typedef struct
{
	int vexnum;
	double** arcs;//邻接矩阵 
}AMGraph;


//-----数据结构：顺序表，图，链队列操作-----
void InitList(SqList& L)
{
	L.elem = new Userinformation[MAXSIZE];
	if (!L.elem)
		exit(0);
	L.length = 0;
}

void CreatList(SqList& L)
{
	string s, s1, s2, s3;
	stringstream ss;//流对象
	Userinformation U;//结构体

	fstream fp;
	fp.open("C:\\Users\\DSHH\\Desktop\\美团餐馆预定管理信息系统\\user.txt", ios::in);//读方式打开文件
	if (!fp)
	{
		cout << "Can't open the user.txr file." << endl;
		exit(0);
	}

	getline(fp, s);//读一行
	while (getline(fp, s))
	{
		ss.str(s);//进流
		getline(ss, s1, ' ');//截取空格前字符串
		U.useraccount = s1;//赋值

		getline(ss, s2, ' ');
		U.password = s2;

		getline(ss, s3, ' ');
		U.contact = s3;

		L.elem[L.length] = U;//注意
		L.length++;

		ss.clear();//注意清空流
	}

	fp.close();//关闭文件
}

void PrintList(SqList& L)//
{
	int i;
	for (i = 0; i < L.length; i++)
	{
		cout << L.elem[i].useraccount << " " << L.elem[i].password << " " << L.elem[i].contact << endl;
	}
}


void InitList2(SqList2& L2)
{
	L2.elem = new Shopinformation[MAXSIZE];
	if (!L2.elem)
		exit(0);
	L2.length = 0;
}

void CreatList2(SqList2& L2)
{
	string s, s1, s2, s3, s4, s5, s6, s7, s8, s9;
	stringstream ss;
	Shopinformation S;
	int i, j, k;

	fstream fp;
	fp.open("C:\\Users\\DSHH\\Desktop\\美团餐馆预定管理信息系统\\shop.txt", ios::in);
	if (!fp)
	{
		cout << "Can't open the user.txr file." << endl;
		exit(0);
	}

	getline(fp, s);
	while (!fp.eof())
	{
		ss.str(s);
		getline(ss, s1, ' ');
		getline(ss, s2, ' ');
		S.shoptype = s2;
		ss.clear();

		getline(fp, s);
		ss.str(s);
		getline(ss, s1, ' ');
		getline(ss, s2, ' ');
		S.shopid = s2;
		ss.clear();

		getline(fp, s);
		ss.str(s);
		getline(ss, s1, ' ');
		getline(ss, s2, ' ');
		S.shopname = s2;
		ss.clear();

		getline(fp, s);
		ss.str(s);
		getline(ss, s1, ' ');
		getline(ss, s2, ' ');
		S.shoppassword = s2;
		ss.clear();

		getline(fp, s);
		ss.str(s);
		getline(ss, s1, ' ');
		getline(ss, s2, ' ');
		S.avgscore = s2;
		ss.clear();

		getline(fp, s);
		ss.str(s);
		getline(ss, s1, ' ');
		getline(ss, s2, ' ');
		S.aveprice = s2;
		ss.clear();

		getline(fp, s);
		ss.str(s);
		getline(ss, s1, ' ');
		getline(ss, s2, ' ');
		S.address = s2;
		ss.clear();

		getline(fp, s);
		ss.str(s);
		getline(ss, s1, ' ');
		getline(ss, s2, ' ');
		S.phone = s2;
		ss.clear();

		i = 0, j = 0;
		while (1)
		{
			getline(fp, s);
			if (s[0] == 's')
				break;
			ss.str(s);
			if (s[0] == 'f')
			{
				getline(ss, s1, ' ');
				getline(ss, s2, ',');
				S.foodid[i] = s2;

				getline(ss, s3, ' ');
				getline(ss, s4, ' ');
				getline(ss, s5, ',');//串串 香
				S.foodname[i] = s5;

				getline(ss, s6, ' ');
				getline(ss, s7, ' ');
				getline(ss, s8, ' ');//不是逗号
				S.foodprice[i] = s8;

				i++;

				ss.clear();
			}
			if (s[0] == 'C')
			{
				getline(ss, s1, ' ');
				getline(ss, s2);
				S.comment[j] = s2;

				j++;

				ss.clear();
			}
			if (fp.eof())
				break;
		}

		L2.elem[L2.length] = S;//注意

		for (k = i; k < 100; k++)
		{
			L2.elem[L2.length].foodid[k] = "null";
			L2.elem[L2.length].foodname[k] = "null";
			L2.elem[L2.length].foodprice[k] = "null";
		}
		for (k = j; k < 100; k++)
		{
			L2.elem[L2.length].comment[k] = "null";
		}

		L2.length++;
	}

	fp.close();
}

void PrintList2(SqList2& L2)
{
	int i, j, k;
	for (i = 0; i < L2.length; i++)
	{
		cout << L2.elem[i].shoptype << endl;
		cout << L2.elem[i].shopid << endl;
		cout << L2.elem[i].shopname << endl;
		cout << L2.elem[i].shoppassword << endl;
		cout << L2.elem[i].avgscore << endl;
		cout << L2.elem[i].aveprice << endl;
		cout << L2.elem[i].address << endl;
		cout << L2.elem[i].phone << endl;

		for (j = 0; j < 100; j++)
		{
			if (L2.elem[i].foodid[j] != "null")
			{
				cout << L2.elem[i].foodid[j] << endl;
				cout << L2.elem[i].foodname[j] << endl;
				cout << L2.elem[i].foodprice[j] << endl;
			}
		}

		for (k = 0; k < 100; k++)
		{
			if (L2.elem[i].comment[k] != "null")
				cout << L2.elem[i].comment[k] << endl;
		}
		cout << endl;
	}
}


void InitHashTable(HashTable& H)
{
	int i, j;

	H.elem = new ElemType[HASHSIZE];
	if (!H.elem)
		exit(0);
	H.count = 0;
	H.size = HASHSIZE;

	for (i = 0; i < H.size; i++)//元素初始化             
	{
		H.elem[i].key = -1;
		H.elem[i].shopname = "null";
		H.elem[i].shoptype = "null";
		H.elem[i].shopid = "null";
		H.elem[i].shoppassword = "null";
		H.elem[i].avgscore = "null";
		H.elem[i].aveprice = "null";
		H.elem[i].address = "null";
		H.elem[i].phone = "null";

		for (j = 0; j < 100; j++)
		{
			H.elem[i].foodid[j] = "null";
			H.elem[i].foodname[j] = "null";
			H.elem[i].foodprice[j] = "null";
			H.elem[i].comment[j] = "null";
		}
	}
}

void CreateHashTable(SqList2& L2, HashTable& H)
{
	unsigned HashNum(string shopname);
	int HashFunction(long hashnum);
	void InsertHashTable(SqList2 & L2, int i, HashTable & H, long key, int hashfunction);

	long hashnum;
	int i, hashfunction;

	for (i = 0; i < L2.length; i++)
	{
		hashnum = HashNum(L2.elem[i].shopname);
		hashfunction = -1;
		hashfunction = HashFunction(hashnum);
		InsertHashTable(L2, i, H, hashnum, hashfunction);
	}
}

unsigned HashNum(string shopname)//汉字字符串转化为数字
{
	int i;
	long hashnum = 0;

	for (i = 0; i < shopname.length(); i++)
	{
		hashnum = hashnum + shopname[i] * i * i * i * i;
	}
	hashnum = abs(hashnum);//负数化为绝对值

	return hashnum;
}

int HashFunction(long hashnum)//哈希函数
{
	hashnum = hashnum % 997;
	return hashnum;
}

void InsertHashTable(SqList2& L2, int i, HashTable& H, long key, int hashfunction)//插入哈希表，发生冲突，用线性探测法解决
{
	int j;

	if (H.elem[hashfunction].key == key)
	{
		cout << "商家名 " << H.elem[hashfunction].shopname << " 已存在，插入失败！";
	}
	else if (H.elem[hashfunction].key == -1)
	{
		H.elem[hashfunction].key = key;
		H.elem[hashfunction].shopname = L2.elem[i].shopname;
		H.elem[hashfunction].shoptype = L2.elem[i].shoptype;
		H.elem[hashfunction].shopid = L2.elem[i].shopid;
		H.elem[hashfunction].shoppassword = L2.elem[i].shoppassword;
		H.elem[hashfunction].avgscore = L2.elem[i].avgscore;
		H.elem[hashfunction].aveprice = L2.elem[i].aveprice;
		H.elem[hashfunction].address = L2.elem[i].address;
		H.elem[hashfunction].phone = L2.elem[i].phone;

		for (j = 0; j < 100; j++)
		{
			H.elem[hashfunction].foodid[j] = L2.elem[i].foodid[j];
			H.elem[hashfunction].foodname[j] = L2.elem[i].foodname[j];
			H.elem[hashfunction].foodprice[j] = L2.elem[i].foodprice[j];
			H.elem[hashfunction].comment[j] = L2.elem[i].comment[j];
		}

		H.count++;
	}
	else
	{
		int hashfunction1;

		hashfunction1 = (hashfunction + 1) % H.size;
		while (H.elem[hashfunction1].key != key && H.elem[hashfunction1].key != -1 && hashfunction1 != hashfunction)
			hashfunction1 = (hashfunction1 + 1) % H.size;

		if (hashfunction1 == hashfunction)
			cout << "表已经满，不能插入商家： " << H.elem[hashfunction1].shopname << " 的信息" << endl;
		else if (H.elem[hashfunction1].key == -1)
		{
			H.elem[hashfunction1].key = key;
			H.elem[hashfunction1].shopname = L2.elem[i].shopname;
			H.elem[hashfunction1].shoptype = L2.elem[i].shoptype;
			H.elem[hashfunction1].shopid = L2.elem[i].shopid;
			H.elem[hashfunction1].shoppassword = L2.elem[i].shoppassword;
			H.elem[hashfunction1].avgscore = L2.elem[i].avgscore;
			H.elem[hashfunction1].aveprice = L2.elem[i].aveprice;
			H.elem[hashfunction1].address = L2.elem[i].address;
			H.elem[hashfunction1].phone = L2.elem[i].phone;

			for (j = 0; j < 100; j++)
			{
				H.elem[hashfunction1].foodid[j] = L2.elem[i].foodid[j];
				H.elem[hashfunction1].foodname[j] = L2.elem[i].foodid[j];
				H.elem[hashfunction1].foodprice[j] = L2.elem[i].foodid[j];
				H.elem[hashfunction1].comment[j] = L2.elem[i].comment[j];
			}

			H.count++;

		}
		else
			cout << "商家： " << H.elem[hashfunction1].shopname << " 的信息已存在于表中%d下标位置，插入失败！" << endl;
	}
}

void PrintHashTable(HashTable& H)
{
	int i, count = 0;

	for (i = 0; i < H.size; i++)
	{
		if (H.elem[i].key != -1)
		{
			cout << H.elem[i].key << endl;
			cout << H.elem[i].shopname << endl;
			cout << H.elem[i].phone << endl;
			count++;
		}
	}
	cout << count << endl;
	cout << H.count << endl;
}


void InitQueue(LinkQueue& Q)
{
	Q.front = Q.rear = new QNode;
	Q.front->next = NULL;
}

void CreatQueue(LinkQueue& Q)
{
	string s, s1, s2, s3;
	stringstream ss;

	fstream fp;
	fp.open("C:\\Users\\DSHH\\Desktop\\美团餐馆预定管理信息系统\\order.txt", ios::in);
	if (!fp)
	{
		cout << "Can't open the user.txr file." << endl;
		exit(0);
	}

	getline(fp, s);
	while (getline(fp, s))
	{
		QueuePtr p;
		ss.str(s);
		getline(ss, s1, ' ');
		p = new QNode;
		p->useraccount = s1;

		getline(ss, s2, ' ');
		p->mechantname = s2;

		getline(ss, s3, ' ');
		p->ordertime = s3;

		p->next = NULL;
		Q.rear->next = p;
		Q.rear = p;

		ss.clear();
	}

	fp.close();
}

void PrintQueue(LinkQueue& Q)
{
	QueuePtr p;
	p = Q.front;

	while (p != NULL)
	{
		cout << p->useraccount << " " << p->mechantname << " " << p->ordertime << endl;
		p = p->next;
	}
}


void InitAmGraphList(AmGraphSqList& A)
{
	A.elem = new Distanceinformation[300000];
	if (!A.elem)
		exit(0);
	A.length = 0;
}

void CreatAmGraphList(AmGraphSqList& L)
{
	string s, s1, s2, s3, s4, s5, s6, s7;
	double  result1, result2;
	stringstream ss, ss1, ss2;
	Distanceinformation A;

	fstream fp;
	fp.open("C:\\Users\\DSHH\\Desktop\\美团餐馆预定管理信息系统\\distance.txt", ios::in);
	if (!fp)
	{
		cout << "Can't open the user.txr file." << endl;
		exit(0);
	}

	getline(fp, s);
	while (getline(fp, s))
	{
		ss.str(s);
		getline(ss, s1, ' ');
		while (1)
		{
			getline(ss, s2, ' ');
			ss1.clear();
			ss1.str(s2);
			if (ss1 >> result1 && ss1.eof())
			{
				s3.pop_back();
				break;
			}//能否转为数字

			else
			{
				s3 += s2;
				s3.append(1, ' ');
			}

		}
		A.address1 = s3;
		s3.clear();


		while (1)
		{
			getline(ss, s4, ' ');
			ss2.clear();
			ss2.str(s4);
			if (ss2 >> result2 && ss2.eof())
			{
				A.distance = s4;
				s5.pop_back();
				break;
			}//能否转为数字

			else
			{
				s5 += s4;
				s5.append(1, ' ');
			}

		}
		A.address2 = s5;
		s5.clear();

		L.elem[L.length] = A;//注意
		L.length++;
		ss.clear();


	}
	fp.close();
}

void PrintAmGraphList(AmGraphSqList& A)
{
	int i;
	for (i = 0; i < 100; i++)
		cout << A.elem[i].address1 << " " << A.elem[i].address2 << " " << A.elem[i].distance << endl;
}

void InitAM(AMGraph& G)//初始化图
{
	G.vexnum = MVNum;
	int i;
	G.arcs = new double* [MVNum];
	for (i = 0; i < MVNum; i++)
		G.arcs[i] = new double[MVNum];
}
void CreateAm(SqList2& L2, AmGraphSqList& A, AMGraph& G)//创建图
{
	int i, j, row = 0, line = 0;
	for (i = 0; i < MVNum; i++)
		for (j = 0; j < MVNum; j++)
			G.arcs[i][j] = MaxInt;
	L2.elem[L2.length].shopname = "北林";
	L2.length = L2.length + 1;
	G.vexnum = L2.length;
	for (i = 0; i < A.length; i++)
	{
		for (j = 0; j < L2.length; j++)
		{
			if (A.elem[i].address1 == L2.elem[j].shopname)
			{
				row = j;
				break;
			}
		}
		for (j = 0; j < L2.length; j++)
		{
			if (A.elem[i].address2 == L2.elem[j].shopname)
			{
				line = j;
				break;
			}
		}
		G.arcs[row][line] = stod(A.elem[i].distance);
		G.arcs[line][row] = stod(A.elem[i].distance);
	}

}

//-----算法-----
//折半查找
int Search_Bin(SqList L, string key)
{
	int low = 1, high = L.length, mid;//置查找区间初值

	while (low <= high)
	{
		mid = (low + high) / 2;
		if (key == L.elem[mid].useraccount)//找到待查元素
			return mid;
		else if (key < L.elem[mid].useraccount)//继续在前一子表进行查找
			high = mid - 1;
		else//继续在后一子表进行查找
			low = mid + 1;
	}
	return 0;//表中不存在待查元素
}
//选择排序
void SelectSortList(SqList& L)
{
	int i, j;
	//int k;
	Userinformation temp;

	for (i = 0; i < L.length - 1; i++)
	{
		//k = i;
		for (j = i + 1; j <= L.length - 1; j++)
		{
			if ((L.elem[i]).useraccount > L.elem[j].useraccount)
			{
				//k = j;
				//if (k != i)
				{
					temp = L.elem[i];
					L.elem[i] = L.elem[j];
					L.elem[j] = temp;
				}
			}
		}
	}
}
//KMP
vector<int> GetNext(string t)
{
	int  i = -1, j = 0;
	vector<int> next(t.size(), -1);

	while (j < t.size() - 1)
	{
		if (i == -1 || t[j] == t[i])
		{
			++i;
			++j;
			if (t[j] != t[i])
				next[j] = i;
			else
				next[j] = next[i];
		}
		else
			i = next[i];
	}
	return next;
}
int KMP(string s, string t) //s主串，t为模式串 
{
	vector<int> getNext(string t);
	int m = s.size(), n = t.size(), i = 0, j = 0;//初始化

	vector<int> next = GetNext(t);
	while (i < m && j < n)//两个串均未比较到串尾；用mn可以，直接用函数不行
	{
		if (j == -1 || s[i] == t[j])//继续比较后继字符
		{
			++i;
			++j;
		}
		else//指针后退重新开始匹配
			j = next[j];
	}
	if (j == n)
		return i - j;
	else
		return -1;//匹配成功返回i-j,失败返回-1
}
//快速排序
int Partition(SqList3& L2, int low, int high)
{
	L2.elem[50] = L2.elem[low];//空闲数组空间
	string pivotkey;
	pivotkey = L2.elem[low].avgscore;
	while (low < high)
	{
		while (low < high && L2.elem[high].avgscore >= pivotkey)
			--high;
		L2.elem[low] = L2.elem[high];
		while (low < high && L2.elem[low].avgscore <= pivotkey)
			++low;
		L2.elem[high] = L2.elem[low];
	}
	L2.elem[low] = L2.elem[50];
	return low;
}
void QSort(SqList3& L2, int low, int high)
{
	int Partition(SqList3 & L2, int low, int high);
	int pivotloc;

	if (low < high)
	{

		pivotloc = Partition(L2, low, high);
		QSort(L2, low, pivotloc - 1);
		QSort(L2, pivotloc + 1, high);
	}
}
void QuickSort(SqList3& L2)
{
	QSort(L2, 0, L2.length - 1);
}
//散列查找
int SearchHashTable(HashTable H, string shopname)
{
	unsigned HashNum(string shopname);
	int HashFunction(long hashnum);

	long hashnum;
	int hashfunction, i, hashfunction1;

	hashnum = HashNum(shopname);
	hashfunction = HashFunction(hashnum);

	if (H.elem[hashfunction].key == NULLKEY)
		return -1;
	else if (H.elem[hashfunction].key == hashnum)
		return hashfunction;
	//return hashnum;
	else
	{
		for (i = 0; i < H.size; i++)
		{
			hashfunction1 = (hashfunction + 1) % H.size;
			if (H.elem[hashfunction1].key == NULLKEY)
				return -1;
			else if (H.elem[hashfunction1].key == hashnum)
				return hashfunction1;
			//return hashnum;
		}
		return -1;
	}
}
//迪杰斯特拉求最短路径
void ShortestPath_DIJ(SqList2& L2, AMGraph& G, string vex)
{//用Dijkstra算法求有向网G的v0顶点到其余顶点的最短路径 
	int S[MVNum];
	double D[MVNum];
	int Path[MVNum];

	int i, v00, v11, n, v, min, w;
	string v0, v1;

	v0 = "北林";
	v1 = vex;

	v00 = 845;
	for (i = 0; i < L2.length; i++)
	{
		if (v1 == L2.elem[i].shopname)
		{
			v11 = i;
			break;
		}
	}

	n = G.vexnum;

	for (v = 0; v < n; ++v)
	{
		S[v] = false;
		D[v] = G.arcs[v00][v];
		if (D[v] < MaxInt)
			Path[v] = v00;
		else
			Path[v] = -1;
	}
	S[v00] = true;
	D[v00] = 0;

	for (i = 1; i < n; ++i)
	{
		min = MaxInt;
		for (w = 0; w < n; ++w)
		{
			if (!S[w] && D[w] < min)
			{
				v = w;
				min = D[w];
			}
		}
		S[v] = true;
		for (w = 0; w < n; ++w)
		{
			if (!S[w] && (D[v] + G.arcs[v][w] < D[w]))
			{
				D[w] = D[v] + G.arcs[v][w];
				Path[w] = v;
			}
		}
	}

	//输出
	w = v11;
	cout << D[w] << endl;//输出第一行，v1所在的D[w]即为最终最短路径//输出错误

	cout << endl;
}


//-----一级菜单************************************************************************************
//1美团餐馆预定管理信息系统
void SystemMenu(SqList& L, SqList2& L2, HashTable& H, AMGraph& G, LinkQueue& Q)
{
	void UserLoginSystem(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void ShopLoginSystem(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void BackStageSystem(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);

	int select;

	cout << "/*************美团餐馆预定管理信息系统*************/" << endl;
	cout << "1. 用户系统" << endl;
	cout << "2. 商家系统" << endl;
	cout << "3. 后台管理系统" << endl;
	cout << "4. 退出系统" << endl;
	cout << endl;
	cout << "请输出相应数字键进行选择：";

	cin >> select;
	switch (select)
	{
	case 1:
		system("cls");
		UserLoginSystem(L, L2, H, G, Q);
		break;
	case 2:
		system("cls");
		ShopLoginSystem(L, L2, H, G, Q);
		break;
	case 3:
		system("cls");
		BackStageSystem(L, L2, H, G, Q);
		break;
	case 4:
		exit(0);
	}
}

//-----二级菜单************************************************************************************
//1.1用户系统
void UserLoginSystem(SqList& L, SqList2& L2, HashTable& H, AMGraph& G, LinkQueue& Q)
{
	void UserSystem(SqList & L, string useraccount, string password, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	int Search_Bin(SqList L, string key);
	void UserRegister(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void SystemMenu(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);

	int select, result, i;
	string useraccount, password, contact;

	cout << "/*************美团餐馆预定管理信息系统*************/" << endl;
	cout << "--------------------用户登录系统--------------------" << endl;
	cout << "1. 登录" << endl;
	cout << "2. 注册" << endl;
	cout << "3. 返回上一级：主菜单" << endl;
	cout << endl;
	cout << "请输出相应数字键进行选择：";

	cin >> select;
	switch (select)
	{
	case 1:
		system("cls");
		cout << "请输入您的账号和密码：" << endl;
		cin >> useraccount >> password;

		i = Search_Bin(L, useraccount);
		if (L.elem[i].password == password)
			result = 1;
		else
			result = 0;

		if (result == 1)
		{
			cout << "登录成功！" << endl;
			cout << endl;
			UserSystem(L, useraccount, password, L2, H, G, Q);
		}
		else
		{
			cout << "登陆失败！" << endl;
			cout << "请重新输入相应数字键进行选择！" << endl;
			cout << endl;
			UserLoginSystem(L, L2, H, G, Q);
		}
		break;
	case 2:
		system("cls");
		UserRegister(L, L2, H, G, Q);
		break;
	case 3:
		system("cls");
		SystemMenu(L, L2, H, G, Q);
		break;
	}
}

void UserSystem(SqList& L, string useraccount, string password, SqList2& L2, HashTable& H, AMGraph& G, LinkQueue& Q)
{
	void PersonalInformationManagement(SqList & L, string useraccount, string password, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void FindShop(SqList & L, string useraccount, string password, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void OrderFunction(SqList & L, string useraccount, string password, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void RecommendationFunction(SqList & L, string useraccount, string password, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void UserLoginSystem(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void SystemMenu(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);

	int select;

	cout << "/*************美团餐馆预定管理信息系统*************/" << endl;
	cout << "----------------------用户系统----------------------" << endl;
	cout << "1. 个人信息管理" << endl;
	cout << "2. 查询商家" << endl;
	cout << "3. 预定功能" << endl;
	cout << "4. 推荐功能" << endl;
	cout << "5. 返回上一级" << endl;
	cout << "6. 返回主菜单" << endl;
	cout << endl;
	cout << "请输出相应数字键进行选择：";

	cin >> select;
	switch (select)
	{
	case 1:
		system("cls");
		PersonalInformationManagement(L, useraccount, password, L2, H, G, Q);
		break;
	case 2:
		system("cls");
		FindShop(L, useraccount, password, L2, H, G, Q);
		break;
	case 3:
		system("cls");
		OrderFunction(L, useraccount, password, L2, H, G, Q);
		break;
	case 4:
		system("cls");
		RecommendationFunction(L, useraccount, password, L2, H, G, Q);
		break;
	case 5:
		system("cls");
		UserLoginSystem(L, L2, H, G, Q);
		break;
	case 6:
		system("cls");
		SystemMenu(L, L2, H, G, Q);
		break;
	}
}

void UserRegister(SqList& L, SqList2& L2, HashTable& H, AMGraph& G, LinkQueue& Q)
{
	int Search_Bin(SqList L, string key);
	void UserLoginSystem(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void SystemMenu(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void SelectSortList(SqList & L);

	int result = -1, select;
	string useraccount, password, contact;

	cout << "请输入要注册的用户账号、密码和联系方式：" << endl;
	cin >> useraccount >> password >> contact;
	result = Search_Bin(L, useraccount);
	if (result == -1)
	{
		cout << "该用户账号已存在！" << endl;
		cout << endl;
		cout << "1. 重新注册" << endl;
		cout << "2. 返回上一级" << endl;
		cout << "3. 返回主菜单" << endl;
		cout << "重新输入相应数字键进行选择：";
		cin >> select;

		switch (select)
		{
		case 1:
			system("cls");
			UserRegister(L, L2, H, G, Q);
			break;
		case 2:
			system("cls");
			UserLoginSystem(L, L2, H, G, Q);
			break;
		case 3:
			system("cls");
			SystemMenu(L, L2, H, G, Q);
			break;
		}
	}
	else
	{
		L.elem[L.length].useraccount = useraccount;
		L.elem[L.length].password = password;
		L.elem[L.length].contact = contact;
		L.length++;
		SelectSortList(L);

		fstream fp;
		int i;

		fp.open("C:\\Users\\DSHH\\Desktop\\美团餐馆预定管理信息系统\\user.txt", ios::out);
		if (!fp)
		{
			cout << "Can't open the user.txr file." << endl;
			exit(0);
		}

		fp << "用户账号" << " " << "密码" << " " << "联系方式" << endl;
		for (i = 0; i < L.length; i++)
		{
			fp << L.elem[i].useraccount << " " << L.elem[i].password << " " << L.elem[i].contact << endl;
		}

		fp.close();

		cout << "注册成功！" << endl;
		cout << endl;
		UserLoginSystem(L, L2, H, G, Q);
	}



}

//1.2商家系统
void ShopLoginSystem(SqList& L, SqList2& L2, HashTable& H, AMGraph& G, LinkQueue& Q)
{
	int SearchHashTable(HashTable H, string shopname);
	void ShopSystem(SqList2 & L2, HashTable & H, string shopname, int result, LinkQueue & Q);

	string shopname, shoppassword;
	int result = -1;

	cout << "/*************美团餐馆预定管理信息系统*************/" << endl;
	cout << "--------------------商家登录系统--------------------" << endl;
	cout << "请输入商家名称和密码：" << endl;

	cin >> shopname >> shoppassword;

	result = SearchHashTable(H, shopname);
	if (result != -1)
	{
		if (H.elem[result].shoppassword == shoppassword)
		{

			cout << "登录成功！" << endl;
			ShopSystem(L2, H, shopname, result, Q);

		}
		//cout << "登录失败！" << endl;
		//cout << "请重新输入商家名称和密码：" << endl;
	}
	//cout << "登录失败！" << endl;
}

void ShopSystem(SqList2& L2, HashTable& H, string shopname, int result, LinkQueue& Q)
{
	void ShopInformationManagement(SqList2 & L2, HashTable & H, string shopname, int result);
	void OrderFunction1(string shopname, LinkQueue & Q2);

	system("cls");
	cout << "/*************美团餐馆预定管理信息系统*************/" << endl;
	cout << "----------------------商家系统----------------------" << endl;
	cout << "1.商家信息管理" << endl;
	cout << "2.预定功能" << endl;
	cout << "3.返回上一级" << endl;
	cout << endl;
	cout << "请输出相应数字键进行选择：";


	LinkQueue Q2;
	Q2.front = Q2.rear = new QNode;
	Q2.front->next = NULL;

	QueuePtr p, q;
	p = Q.front;//Q第一行是空行

	while (p != NULL)
	{
		if (p->mechantname == shopname)
		{
			q = new QNode;
			q->useraccount = p->useraccount;
			q->mechantname = p->mechantname;
			q->ordertime = p->ordertime;
			q->next = NULL;
			Q2.rear->next = q;
			Q2.rear = q;
		}
		p = p->next;
	}

	int select;
	cin >> select;
	switch (select)
	{
	case 1:
		system("cls");
		ShopInformationManagement(L2, H, shopname, result);
		break;
	case 2:
		system("cls");
		OrderFunction1(shopname, Q2);
		break;
		//case 3:


	}
}

//1.3后台管理系统
void BackStageSystem(SqList& L, SqList2& L2, HashTable& H, AMGraph& G, LinkQueue& Q)
{
	void UserManagement(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void ShopManagement(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void SystemMenu(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);

	int select;

	cout << "/*************美团餐馆预定管理信息系统*************/" << endl;
	cout << "1. 用户管理" << endl;
	cout << "2. 商家管理" << endl;
	cout << "3. 返回上一级：主菜单" << endl;
	cout << endl;
	cout << "请输出相应数字键进行选择：";

	cin >> select;
	switch (select)
	{
	case 1:
		system("cls");
		UserManagement(L, L2, H, G, Q);
		break;
	case 2:
		system("cls");
		ShopManagement(L, L2, H, G, Q);
		break;
	case 3:
		system("cls");
		SystemMenu(L, L2, H, G, Q);
		break;
	}
}


//-----三级菜单************************************************************************************
//1.1.1个人信息管理
void PersonalInformationManagement(SqList& L, string useraccount, string password, SqList2& L2, HashTable& H, AMGraph& G, LinkQueue& Q)
{
	void LookPersonalInformation(SqList & L, string useraccount, string password, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void AmendPersonalInformation(SqList & L, string useraccount, string password, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void DeletePersonalInformation(SqList & L, string useraccount, string password, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void UserSystem(SqList & L, string useraccount, string password, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void SystemMenu(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);

	int select;

	cout << "/*************美团餐馆预定管理信息系统*************/" << endl;
	cout << "--------------- 用户系统:个人信息管理---------------" << endl;
	cout << "（1） 查看个人信息" << endl;
	cout << "（2） 修改个人信息" << endl;
	cout << "（3） 注销个人信息" << endl;
	cout << "（4） 返回上一级" << endl;
	cout << "（5） 返回主菜单" << endl;
	cout << endl;
	cout << "请输出相应数字键进行选择：";

	cin >> select;
	switch (select)
	{
	case 1:
		system("cls");
		LookPersonalInformation(L, useraccount, password, L2, H, G, Q);
		break;
	case 2:
		system("cls");
		AmendPersonalInformation(L, useraccount, password, L2, H, G, Q);
		break;
	case 3:
		DeletePersonalInformation(L, useraccount, password, L2, H, G, Q);
	case 4:
		system("cls");
		UserSystem(L, useraccount, password, L2, H, G, Q);
		break;
	case 5:
		system("cls");
		SystemMenu(L, L2, H, G, Q);
		break;
	}
}
//1.1.2查询商家
void FindShop(SqList& L, string useraccount, string password, SqList2& L2, HashTable& H, AMGraph& G, LinkQueue& Q)
{
	int SearchHashTable(HashTable H, string shopname);
	void ShortestPath_DIJ(SqList2 & L2, AMGraph & G, string vex);
	void UserSystem(SqList & L, string useraccount, string password, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void SystemMenu(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);

	string shopname;
	int result = -1, i, select;

	cout << "请输入商家名称：" << endl;
	cin >> shopname;
	result = SearchHashTable(H, shopname);//地址

	if (result != -1)
	{
		cout << endl;
		cout << "商家信息如下：" << endl;
		cout << "商家类型：" << H.elem[result].shopname << endl;
		cout << "商家类型：" << H.elem[result].shoptype << endl;
		cout << "评分：" << H.elem[result].avgscore << endl;
		cout << "平均消费：" << H.elem[result].aveprice << endl;
		cout << "地址：" << H.elem[result].address << endl;
		cout << "联系电话：" << H.elem[result].phone << endl;
		cout << "特色菜品：";
		for (i = 0; i < 100; i++)
		{
			if (H.elem[result].foodname[i] != "null")
				cout << H.elem[result].foodname[i] << " ";
		}
		cout << endl;

		cout << "从北林到该商家的距离为：";
		ShortestPath_DIJ(L2, G, shopname);

		cout << endl;
		cout << "1.返回上一级" << endl;
		cout << "2.返回主菜单" << endl;
		cout << "请输出相应数字键进行选择：";
		cin >> select;
		switch (select)
		{
		case 1:
			system("cls");
			UserSystem(L, useraccount, password, L2, H, G, Q);
			break;
		case 2:
			system("cls");
			SystemMenu(L, L2, H, G, Q);
			break;
		}
	}
}
//1.1.3预定功能
void OrderFunction(SqList& L, string useraccount, string password, SqList2& L2, HashTable& H, AMGraph& G, LinkQueue& Q)
{
	void LookAllOrder(SqList & L, string useraccount, string password, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void FindOrder(SqList & L, string useraccount, string password, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void AddOrder(SqList & L, string useraccount, string password, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void UserSystem(SqList & L, string useraccount, string password, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void SystemMenu(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);

	int select;

	cout << "/*************美团餐馆预定管理信息系统*************/" << endl;
	cout << "----------------------用户系统----------------------" << endl;
	cout << "（1）查看所有预定" << endl;
	cout << "（2）查询某个预定" << endl;
	cout << "（3）添加预定" << endl;
	cout << "（4）返回上一级" << endl;
	cout << "（5）返回主菜单" << endl;
	cout << endl;
	cout << "请输出相应数字键进行选择：";

	cin >> select;
	switch (select)
	{
	case 1:
		system("cls");
		LookAllOrder(L, useraccount, password, L2, H, G, Q);
		break;
	case 2:
		system("cls");
		FindOrder(L, useraccount, password, L2, H, G, Q);
		break;
	case 3:
		system("cls");
		AddOrder(L, useraccount, password, L2, H, G, Q);
		break;
	case 4:
		system("cls");
		UserSystem(L, useraccount, password, L2, H, G, Q);
		break;
	case 5:
		system("cls");
		SystemMenu(L, L2, H, G, Q);
		break;
	}

}
//1.1.4推荐功能
void RecommendationFunction(SqList& L, string useraccount, string password, SqList2& L2, HashTable& H, AMGraph& G, LinkQueue& Q)
{
	string shoptype, specialfood, shopcharacteristics;
	int KMP(string s, string t);
	void QuickSort(SqList3 & L3);
	void UserSystem(SqList & L, string useraccount, string password, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void SystemMenu(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);

	int result1 = -1, result2 = -1, result3 = -1, i, j, k, a[1000], count = 0, select;

	cout << "/*************美团餐馆预定管理信息系统*************/" << endl;
	cout << "------------------用户系统:推荐功能-----------------" << endl;
	cout << "请输入商家的类型：";
	getchar();
	getline(cin, shoptype);//吸收回车
	cout << "请输入需要的特色菜：";
	getline(cin, specialfood);
	cout << "请输入商家特点：";
	getline(cin, shopcharacteristics);

	for (i = 0; i < L2.length; i++)
	{
		if (shoptype == "")
			result1 = 0;
		else
			result1 = KMP(L2.elem[i].shoptype, shoptype);
		if (specialfood == "")
			result2 = 0;
		else
		{
			for (j = 0; j < 10; j++)
			{
				result2 = KMP(L2.elem[i].foodname[j], specialfood);
				if (result2 != -1)
					break;
			}
		}
		if (shopcharacteristics == "")
			result3 = 0;
		else
		{
			for (k = 0; k < 10; k++)
			{
				result3 = KMP(L2.elem[i].comment[k], shopcharacteristics);
				if (result3 != -1)
					break;
			}
		}
		if (result1 != -1 && result2 != -1 && result3 != -1)
		{
			a[i] = i;//三个都满足，记录地址
			count++;
		}
		if (result1 == 0 && result2 == 0 && result3 == 0)
			count = 0;
	}

	cout << endl;
	cout << "为您推荐以下商家：" << endl;
	//cout << "符合要求的餐馆有：" << count << "个！" << endl;

	if (count == 0)
	{
		cout << endl;
		cout << "1.请重新输入要求" << endl;
		cout << "2.返回上一级" << endl;
		cout << "3.返回主菜单" << endl;
		cout << "请输出相应数字键进行选择：";
		cin >> select;
		switch (select)
		{
		case 1:
			system("cls");
			RecommendationFunction(L, useraccount, password, L2, H, G, Q);
			break;
		case 2:
			system("cls");
			UserSystem(L, useraccount, password, L2, H, G, Q);
			break;
		case 3:
			system("cls");
			SystemMenu(L, L2, H, G, Q);
			break;
		}
	}
	else
	{
		cout << left << setw(40) << "名称";
		cout << left << setw(20) << "类型";
		cout << left << setw(5) << "评分";
		cout << left << setw(10) << "人均消费";
		cout << left << setw(70) << "地址";
		cout << left << setw(20) << "联系方式" << endl;
		cout << endl;

		SqList3 L3;//存放符合要求的餐馆
		L3.elem = new RecommendationInformation[MAXSIZE];
		if (!L3.elem)
			exit(0);
		L3.length = 0;

		j = 0;
		for (i = 0; i < L2.length - 1; i++)//最后一个放“北林”
		{
			if (a[i] == i)
			{
				L3.elem[j].shoptype = L2.elem[i].shoptype;
				L3.elem[j].shopname = L2.elem[i].shopname;
				L3.elem[j].avgscore = L2.elem[i].avgscore;
				L3.elem[j].aveprice = L2.elem[i].aveprice;
				L3.elem[j].address = L2.elem[i].address;
				L3.elem[j].phone = L2.elem[i].phone;
				j++;
			}
		}
		L3.length = j;

		QuickSort(L3);
		for (i = L3.length - 1; i >= 0; i--)//正向排序，逆向输出
		{
			cout << left << setw(40) << L3.elem[i].shopname;
			cout << left << setw(20) << L3.elem[i].shoptype;
			cout << left << setw(5) << L3.elem[i].avgscore;
			cout << left << setw(10) << L3.elem[i].aveprice;
			cout << left << setw(70) << L3.elem[i].address;
			cout << left << setw(20) << L3.elem[i].phone << endl;
			cout << endl;
		}

		cout << "1.返回上一级" << endl;
		cout << "2.返回主菜单" << endl;
		cout << "请输出相应数字键进行选择：";
		cin >> select;
		switch (select)
		{
		case 1:
			system("cls");
			UserSystem(L, useraccount, password, L2, H, G, Q);
			break;
		case 2:
			system("cls");
			SystemMenu(L, L2, H, G, Q);
			break;
		}
	}
}



//1.2.1商家信息管理
void ShopInformationManagement(SqList2& L2, HashTable& H, string shopname, int result)
{
	void FindShopInformation(SqList2 & L2, HashTable & H, int result);
	void AmendShopInformation(SqList2 & L2, HashTable & H, string shopname, int result);
	void DeleteShopInformation(SqList2 & L2, HashTable & H, string shopname, int result);

	system("cls");
	cout << "/*************美团餐馆预定管理信息系统*************/" << endl;
	cout << "---------------商家系统：商家信息管理---------------" << endl;
	cout << "（1）查看商家信息" << endl;
	cout << "（2）修改商家信息" << endl;
	cout << "（3）注销商家信息" << endl;

	int select;
	cin >> select;
	switch (select)
	{
	case 1:
		system("cls");
		FindShopInformation(L2, H, result);
		break;
	case 2:
		system("cls");
		AmendShopInformation(L2, H, shopname, result);
		break;
	case 3:
		system("cls");
		DeleteShopInformation(L2, H, shopname, result);
		break;
	}
}
//1.2.2预定功能
void OrderFunction1(string shopname, LinkQueue& Q2)
{
	void ShowALLOrder(string shopname, LinkQueue & Q2);
	void FindUserOrder(LinkQueue & Q2);
	void DeleteOrder(LinkQueue & Q2);

	cout << "/*************美团餐馆预定管理信息系统*************/" << endl;
	cout << "------------------商家系统：预订系统----------------" << endl;
	cout << "（1） 显示所有预定" << endl;
	cout << "（2） 查询某用户预定" << endl;
	cout << "（3） 删除预定" << endl;
	cout << "返回上一级" << endl;
	cout << endl;
	cout << "请输入相应数字键进行选择：";

	int select;
	cin >> select;
	switch (select)
	{
	case 1:
		system("cls");
		ShowALLOrder(shopname, Q2);
		break;
	case 2:
		system("cls");
		FindUserOrder(Q2);
		break;
	case 3:
		system("cls");
		DeleteOrder(Q2);
		break;
	}
}


//1.3.1用户管理
void UserManagement(SqList& L, SqList2& L2, HashTable& H, AMGraph& G, LinkQueue& Q)
{
	void LookAllUser(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void FindUser(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void DeleteUser(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void BackStageSystem(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void SystemMenu(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);

	int select;

	cout << "/*************美团餐馆预定管理信息系统*************/" << endl;
	cout << "---------------后台管理系统：用户管理---------------" << endl;
	cout << "（1） 查看所有用户" << endl;
	cout << "（2） 查询某个用户" << endl;
	cout << "（3） 删除某个用户" << endl;
	cout << "（4） 返回上一级" << endl;
	cout << "（5） 返回主菜单" << endl;
	cout << endl;
	cout << "请输入相应数字键进行选择：";

	cin >> select;
	switch (select)
	{
	case 1:
		system("cls");
		LookAllUser(L, L2, H, G, Q);
		break;
	case 2:
		system("cls");
		FindUser(L, L2, H, G, Q);
		break;
	case 3:
		system("cls");
		DeleteUser(L, L2, H, G, Q);
		break;
	case 4:
		system("cls");
		BackStageSystem(L, L2, H, G, Q);
		break;
	case 5:
		system("cls");
		SystemMenu(L, L2, H, G, Q);
		break;
	}

}
//1.3.2商家管理
void ShopManagement(SqList& L, SqList2& L2, HashTable& H, AMGraph& G, LinkQueue& Q)
{
	void BackStageSystem(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void SystemMenu(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void LookAllShop(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void FindShop(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void DeleteShop(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);

	int select;

	cout << "/*************美团餐馆预定管理信息系统*************/" << endl;
	cout << "---------------后台管理系统：商家管理---------------" << endl;
	cout << "（1） 查看所有商家" << endl;
	cout << "（2） 查询某个商家" << endl;
	cout << "（3） 删除某个商家" << endl;
	cout << "（4） 返回上一级" << endl;
	cout << "（5） 返回主菜单" << endl;

	cout << endl;
	cout << "请输入相应数字键进行选择：";

	cin >> select;
	switch (select)
	{
	case 1:
		system("cls");
		LookAllShop(L, L2, H, G, Q);
		break;
	case 2:
		system("cls");
		FindShop(L, L2, H, G, Q);
		break;
	case 3:
		system("cls");
		DeleteShop(L, L2, H, G, Q);
		break;
	case 4:
		system("cls");
		BackStageSystem(L, L2, H, G, Q);
		break;
	case 5:
		system("cls");
		SystemMenu(L, L2, H, G, Q);
		break;
	}
}

//-----四级菜单************************************************************************************
//1.1.1.1查看个人信息
void LookPersonalInformation(SqList& L, string useraccount, string password, SqList2& L2, HashTable& H, AMGraph& G, LinkQueue& Q)
{
	void PersonalInformationManagement(SqList & L, string useraccount, string password, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void SystemMenu(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);

	int i, select;
	string contact;

	i = Search_Bin(L, useraccount);
	contact = L.elem[i].contact;


	cout << "该用户的信息：" << endl;
	cout << "账号：" << useraccount << endl;
	cout << "联系方式：" << contact << endl;
	cout << endl;

	cout << "1.返回上一级" << endl;
	cout << "2.返回主菜单" << endl;
	cout << "请输出相应数字键进行选择：";
	cin >> select;
	switch (select)
	{
	case 1:
		system("cls");
		PersonalInformationManagement(L, useraccount, password, L2, H, G, Q);
		break;
	case 2:
		system("cls");
		SystemMenu(L, L2, H, G, Q);
		break;
	}
}
//1.1.1.2修改个人信息
void AmendPersonalInformation(SqList& L, string useraccount, string password, SqList2& L2, HashTable& H, AMGraph& G, LinkQueue& Q)//修改个人信息
{
	int Search_Bin(SqList L, string key);
	void PersonalInformationManagement(SqList & L, string useraccount, string password, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void SystemMenu(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);

	string newpassword, newcontact;
	int i, j, select;

	cout << "请重新输入该用户的密码和联系方式：" << endl;
	cout << "密码:";
	cin >> newpassword;
	cout << endl;
	cout << "联系方式：";
	cin >> newcontact;
	cout << endl;

	i = Search_Bin(L, useraccount);
	L.elem[i].password = newpassword;
	L.elem[i].contact = newcontact;

	fstream fp;
	fp.open("C:\\Users\\DSHH\\Desktop\\美团餐馆预定管理信息系统\\user.txt", ios::out);
	if (!fp)
	{
		cout << "Can't open the user.txr file." << endl;
		exit(0);
	}

	fp << "用户账号" << " " << "密码" << " " << "联系方式" << endl;
	for (j = 0; j < L.length; j++)
	{
		fp << L.elem[j].useraccount << " " << L.elem[j].password << " " << L.elem[j].contact << endl;
	}

	fp.close();

	cout << "修改成功！" << endl;
	cout << "1.返回上一级" << endl;
	cout << "2.返回主菜单" << endl;
	cout << "请输出相应数字键进行选择：";
	cin >> select;
	switch (select)
	{
	case 1:
		system("cls");
		PersonalInformationManagement(L, useraccount, password, L2, H, G, Q);
		break;
	case 2:
		system("cls");
		SystemMenu(L, L2, H, G, Q);
		break;
	}
}
//1.1.1.3删除个人信息
void DeletePersonalInformation(SqList& L, string useraccount, string password, SqList2& L2, HashTable& H, AMGraph& G, LinkQueue& Q)
{
	int Search_Bin(SqList L, string key);
	void PersonalInformationManagement(SqList & L, string useraccount, string password, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void SystemMenu(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);

	int i, j, k, select;

	i = Search_Bin(L, useraccount);
	for (j = i; j <= L.length - 1; j++)
	{
		L.elem[j] = L.elem[j + 1];
	}
	--L.length;

	fstream fp;
	fp.open("C:\\Users\\DSHH\\Desktop\\美团餐馆预定管理信息系统\\user.txt", ios::out);
	if (!fp)
	{
		cout << "Can't open the user.txr file." << endl;
		exit(0);
	}

	fp << "用户账号" << " " << "密码" << " " << "联系方式" << endl;
	for (k = 0; k < L.length; k++)
	{
		fp << L.elem[k].useraccount << " " << L.elem[k].password << " " << L.elem[k].contact << endl;
	}

	fp.close();

	cout << "删除成功！" << endl;
	cout << "1.返回上一级" << endl;
	cout << "2.返回主菜单" << endl;
	cout << "请输出相应数字键进行选择：";
	cin >> select;
	switch (select)
	{
	case 1:
		system("cls");
		PersonalInformationManagement(L, useraccount, password, L2, H, G, Q);
		break;
	case 2:
		system("cls");
		SystemMenu(L, L2, H, G, Q);
		break;
	}
}
//1.1.3.1查看所有预定
void LookAllOrder(SqList& L, string useraccount, string password, SqList2& L2, HashTable& H, AMGraph& G, LinkQueue& Q)
{
	void OrderFunction(SqList & L, string useraccount, string password, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void SystemMenu(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);

	QueuePtr p;
	p = Q.front;
	int select;

	cout << "您预定的信息如下：" << endl;
	cout << left << setw(40) << "餐馆名称" << left << setw(20) << "预定时间" << endl;
	cout << endl;

	while (p != NULL)
	{
		if (p->useraccount == useraccount)
		{
			cout << left << setw(40) << p->mechantname << left << setw(20) << p->ordertime << endl;
			cout << endl;
		}
		p = p->next;
	}

	cout << endl;
	cout << "1.返回上一级" << endl;
	cout << "2.返回主菜单" << endl;
	cout << "请输出相应数字键进行选择：";
	cin >> select;
	switch (select)
	{
	case 1:
		system("cls");
		OrderFunction(L, useraccount, password, L2, H, G, Q);
		break;
	case 2:
		system("cls");
		SystemMenu(L, L2, H, G, Q);
		break;
	}
}
//1.1.3.2查看某个预定
void FindOrder(SqList& L, string useraccount, string password, SqList2& L2, HashTable& H, AMGraph& G, LinkQueue& Q)
{
	void OrderFunction(SqList & L, string useraccount, string password, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void SystemMenu(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);

	string mechantname;
	QueuePtr p;
	p = Q.front;
	int flag = 0, select;

	cout << "请输入商家名称：" << endl;
	cin >> mechantname;
	cout << endl;
	cout << "该用户在此商家的预定信息如下：" << endl;
	cout << left << setw(20) << "预定时间" << endl;
	cout << endl;

	while (p != NULL)
	{
		if (p->useraccount == useraccount)
		{
			if (p->mechantname == mechantname)
			{
				flag = 1;
				cout << left << setw(40) << p->ordertime << endl;
				cout << endl;
			}
		}
		p = p->next;
	}
	if (flag == 1)
		cout << "查询成功！" << endl;
	else
		cout << "查询失败!" << endl;

	cout << endl;
	cout << "1.返回上一级" << endl;
	cout << "2.返回主菜单" << endl;
	cout << "请输出相应数字键进行选择：";
	cin >> select;
	switch (select)
	{
	case 1:
		system("cls");
		OrderFunction(L, useraccount, password, L2, H, G, Q);
		break;
	case 2:
		system("cls");
		SystemMenu(L, L2, H, G, Q);
		break;
	}
}
//1.1.3.3添加预定
void AddOrder(SqList& L, string useraccount, string password, SqList2& L2, HashTable& H, AMGraph& G, LinkQueue& Q)
{
	void OrderFunction(SqList & L, string useraccount, string password, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void SystemMenu(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);

	string mechantname, newordertime;
	int flag = 0, select;
	QueuePtr p, q;

	cout << "请输入商家名称和预定时间：" << endl;
	cin >> mechantname >> newordertime;
	cout << endl;

	p = Q.front;
	while (p != NULL)
	{
		if (p->mechantname == mechantname)
		{
			flag = 1;
			q = new QNode;
			q->useraccount = useraccount;
			q->mechantname = mechantname;
			q->ordertime = newordertime;
			Q.rear->next = q;
			q->next = NULL;
			Q.rear = q;

			fstream fp;
			fp.open("C:\\Users\\DSHH\\Desktop\\美团餐馆预定管理信息系统\\order.txt", ios::out);
			if (!fp)
			{
				cout << "Can't open the order.txr file." << endl;
				exit(0);
			}

			p = Q.front;
			fp << "用户账号" << " " << "商家名称" << " " << "预定时间" << endl;
			p = p->next;
			while (p != NULL)
			{
				fp << p->useraccount << " " << p->mechantname << " " << p->ordertime << endl;
				p = p->next;
			}
			fp.close();

			cout << "预定成功！" << endl;
			break;
		}
		p = p->next;
	}
	if (flag == 0)
		cout << "预定失败！" << endl;

	cout << endl;
	cout << "1.返回上一级" << endl;
	cout << "2.返回主菜单" << endl;
	cout << "请输出相应数字键进行选择：";
	cin >> select;
	switch (select)
	{
	case 1:
		system("cls");
		OrderFunction(L, useraccount, password, L2, H, G, Q);
		break;
	case 2:
		system("cls");
		SystemMenu(L, L2, H, G, Q);
		break;
	}
}


//1.2.1.1查看商家信息
void FindShopInformation(SqList2& L2, HashTable& H, int result)
{
	int i;
	cout << "商家信息如下：" << endl;
	cout << "商家类型：" << H.elem[result].shoptype << endl;
	cout << "评分：" << H.elem[result].avgscore << endl;
	cout << "平均消费：" << H.elem[result].aveprice << endl;
	cout << "地址：" << H.elem[result].address << endl;
	cout << "联系电话：" << H.elem[result].phone << endl;
	cout << "特色菜品：";
	for (i = 0; i < 100; i++)
	{
		if (H.elem[result].foodname[i] != "null")
			cout << H.elem[result].foodname[i] << " ";
	}
	cout << endl;
	cout << "评价：";
	int j;
	for (j = 0; j < 100; j++)
	{
		if (H.elem[result].comment[j] != "null")
			cout << H.elem[result].comment[j] << " ";
	}
	cout << endl;
}
//1.2.1.2修改商家信息
void AmendShopInformation(SqList2& L2, HashTable& H, string shopname, int result)
{

	cout << "请输入商家需要修改的信息：" << endl;
	string shoptype, shopid, newshopname, shoppassword, aveprice, address, phone, foodid[100], foodname[100], foodprice[100];
	cout << "商家类型：";
	cin >> shoptype;
	cout << "商家ID：";
	cin >> shopid;
	cout << "商家名字：";
	cin >> newshopname;
	cout << "商家密码：";
	cin >> shoppassword;
	cout << "平均消费：";
	cin >> aveprice;
	cout << "地址：";
	cin >> address;
	cout << "联系电话：";
	cin >> phone;
	int foodnum;
	cout << "请输入需要修改的特色菜个数：" << endl;
	cin >> foodnum;
	int i;
	for (i = 0; i < foodnum; i++)
	{
		cout << "特色菜ID：";
		cin >> foodid[i];
		cout << "特色菜名字：";
		cin >> foodname[i];
		cout << "特色菜价格：";
		cin >> foodprice[i];
	}


	int j, k;
	for (j = 0; j < L2.length; j++)
	{
		if (L2.elem[j].shopname == shopname)
		{
			L2.elem[j].shoptype = shoptype;
			L2.elem[j].shopid = shopid;
			L2.elem[j].shopname = newshopname;
			L2.elem[j].shoppassword = shoppassword;
			L2.elem[j].aveprice = aveprice;
			L2.elem[j].address = address;
			L2.elem[j].phone = phone;
			for (k = 0; k < foodnum; k++)
			{
				L2.elem[j].foodid[k] = foodid[k];
				L2.elem[j].foodname[k] = foodname[k];
				L2.elem[j].foodprice[k] = foodprice[k];
			}
		}
	}


	fstream fp;
	fp.open("C:\\Users\\DSHH\\Desktop\\美团餐馆预定管理信息系统\\shop.txt", ios::out);
	if (!fp)
	{
		cout << "Can't open the user.txr file." << endl;
		exit(0);
	}
	for (j = 0; j < L2.length; j++)
	{
		fp << "shoptype: " << L2.elem[j].shoptype << endl;
		fp << "shopId: " << L2.elem[j].shopid << endl;
		fp << "shopName: " << L2.elem[j].shopname << endl;
		fp << "shopPassword: " << L2.elem[j].shoppassword << endl;
		fp << "avgScore: " << L2.elem[j].avgscore << endl;
		fp << "avePrice: " << L2.elem[j].aveprice << endl;
		fp << "address: " << L2.elem[j].address << endl;
		fp << "phone: " << L2.elem[j].phone << endl;
		if (L2.elem[j].shopname == shopname)
		{
			for (k = 0; k < foodnum; k++)
				fp << "food_id: " << L2.elem[j].foodid[k] << ", " << "food_name: " << L2.elem[j].foodname[k] << ", " << "food_price: " << L2.elem[j].foodprice[k] << endl;
		}
		else
		{
			for (k = 0; k < 100; k++)
				fp << "food_id: " << L2.elem[j].foodid[k] << ", " << "food_name: " << L2.elem[j].foodname[k] << ", " << "food_price: " << L2.elem[j].foodprice[k] << endl;
		}
		for (k = 0; k < 100; k++)
		{
			if (L2.elem[j].comment[k] != "null")
				fp << "Comment" << k << ": " << L2.elem[j].comment[k] << endl;
		}
	}

	fp.close();
	void InitList2(SqList2 & L2);
	void CreateList2(SqList2 & L2);
	InitList2(L2);
	CreatList2(L2);
	void InitHashTable(HashTable & H);
	void CreateHashTable(SqList2 & L2, HashTable & H);
	InitHashTable(H);
	CreateHashTable(L2, H);
}
//1.2.1.3删除商家信息
void DeleteShopInformation(SqList2& L2, HashTable& H, string shopname, int result)
{
	fstream fp;
	fp.open("C:\\Users\\DSHH\\Desktop\\美团餐馆预定管理信息系统\\shop.txt", ios::out);
	if (!fp)
	{
		cout << "Can't open the user.txr file." << endl;
		exit(0);
	}
	int j, k;
	for (j = 0; j < L2.length; j++)
	{
		if (L2.elem[j].shopname != shopname)
		{
			fp << "shoptype: " << L2.elem[j].shoptype << endl;
			fp << "shopId: " << L2.elem[j].shopid << endl;
			fp << "shopName: " << L2.elem[j].shopname << endl;
			fp << "shopPassword: " << L2.elem[j].shoppassword << endl;
			fp << "avgScore: " << L2.elem[j].avgscore << endl;
			fp << "avePrice: " << L2.elem[j].aveprice << endl;
			fp << "address: " << L2.elem[j].address << endl;
			fp << "phone: " << L2.elem[j].phone << endl;
			for (k = 0; k < 100; k++)
				fp << "food_id: " << L2.elem[j].foodid[k] << ", " << "food_name: " << L2.elem[j].foodname[k] << ", " << "food_price: " << L2.elem[j].foodprice[k] << endl;
			for (k = 0; k < 100; k++)
			{
				if (L2.elem[j].comment[k] != "null")
					fp << "Comment" << k << ": " << L2.elem[j].comment[k] << endl;
			}
		}

	}
	fp.close();
	cout << "注销成功！" << endl;
	void InitList2(SqList2 & L2);
	void CreateList2(SqList2 & L2);
	InitList2(L2);
	CreatList2(L2);
	void InitHashTable(HashTable & H);
	void CreateHashTable(SqList2 & L2, HashTable & H);
	InitHashTable(H);
	CreateHashTable(L2, H);
}


//1.2.2.1查看所有预定
void ShowALLOrder(string shopname, LinkQueue& Q2)
{
	cout << "用户账号 商家名称 预定时间" << endl;
	QueuePtr p;
	p = Q2.front->next;
	while (p != NULL)
	{
		if (p->mechantname == shopname)
		{
			cout << p->useraccount << " " << p->mechantname << " " << p->ordertime << endl;
		}
		p = p->next;
	}
}
//1.2.2.2
void FindUserOrder(LinkQueue& Q2)
{
	cout << "请输入用户账号：";
	string useraccount;
	cin >> useraccount;
	cout << "用户账号 商家名称 预定时间" << endl;
	QueuePtr p;
	p = Q2.front->next;
	while (p != NULL)
	{
		if (p->useraccount == useraccount)
		{
			cout << p->useraccount << " " << p->mechantname << " " << p->ordertime << endl;
		}
		p = p->next;
	}
}
//1.2.2.3
void DeleteOrder(LinkQueue& Q2)
{
	//排序

	cout << "请输入需要处理的预定个数：";
	int disposenum;
	cin >> disposenum;
	cout << "用户账号 预定时间" << endl;

	QueuePtr p;
	p = Q2.front->next;
	int count = 0;
	while (p != NULL)
	{
		cout << p->useraccount << " " << p->ordertime << endl;
		Q2.front->next = p->next;
		QueuePtr t;
		t = p;
		p = p->next;//队尾指针被删没写
		delete t;
		count++;
		if (count == disposenum)
			break;
	}
	cout << "处理成功！" << endl;
}


//1.3.1.1查看所有用户
void LookAllUser(SqList& L, SqList2& L2, HashTable& H, AMGraph& G, LinkQueue& Q)
{
	void SelectSortList(SqList & L);
	void UserManagement(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void SystemMenu(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);

	int i, select;

	SelectSortList(L);
	cout << left << setw(10) << "用户账号";
	cout << left << setw(15) << "联系方式" << endl;
	for (i = 0; i < L.length; i++)
	{
		cout << left << setw(10) << L.elem[i].useraccount;
		cout << left << setw(15) << L.elem[i].contact << endl;
		cout << endl;
	}

	cout << endl;
	cout << "1.返回上一级" << endl;
	cout << "2.返回主菜单" << endl;
	cout << "请输出相应数字键进行选择：";

	cin >> select;
	switch (select)
	{
	case 1:
		system("cls");
		UserManagement(L, L2, H, G, Q);
		break;
	case 2:
		system("cls");
		SystemMenu(L, L2, H, G, Q);
		break;
	}
}
//1.3.1.2查询某个用户
void FindUser(SqList& L, SqList2& L2, HashTable& H, AMGraph& G, LinkQueue& Q)
{
	int Search_Bin(SqList L, string key);
	void UserManagement(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void SystemMenu(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);

	string useraccount;
	int i, select;

	cout << "请输入要查询用户的账号:" << endl;
	cin >> useraccount;

	i = Search_Bin(L, useraccount);
	if (L.elem[i].useraccount == useraccount)
	{
		cout << "该用户的信息是：" << endl;
		cout << left << setw(10) << "账号";
		cout << left << setw(15) << "联系方式" << endl;
		cout << left << setw(10) << L.elem[i].useraccount;
		cout << left << setw(15) << L.elem[i].contact << endl;
		cout << endl;
		cout << "1.返回上一级" << endl;
		cout << "2.返回主菜单" << endl;
		cout << "请输出相应数字键进行选择：";

		cin >> select;
		switch (select)
		{
		case 1:
			system("cls");
			UserManagement(L, L2, H, G, Q);
			break;
		case 2:
			system("cls");
			SystemMenu(L, L2, H, G, Q);
			break;
		}
	}
	else
	{
		cout << "该用户账号不存在！" << endl;
		cout << endl;
		cout << "1.重新查询" << endl;
		cout << "2.返回上一级" << endl;
		cout << "3.返回主菜单" << endl;
		cout << "请输出相应数字键进行选择：";

		cin >> select;
		switch (select)
		{
		case 1:
			system("cls");
			FindUser(L, L2, H, G, Q);
			break;
		case 2:
			system("cls");
			UserManagement(L, L2, H, G, Q);
			break;
		case 3:
			system("cls");
			SystemMenu(L, L2, H, G, Q);
			break;
		}
	}
}
//1.3.1.3删除某个用户
void DeleteUser(SqList& L, SqList2& L2, HashTable& H, AMGraph& G, LinkQueue& Q)
{
	int Search_Bin(SqList L, string key);
	void UserManagement(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void SystemMenu(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);

	string useraccount;
	int i, j, k, select;

	cout << "请输入要删除用户的账号:" << endl;
	cin >> useraccount;
	i = Search_Bin(L, useraccount);

	if (L.elem[i].useraccount != useraccount)
	{
		cout << "该用户账号不存在！" << endl;
		cout << endl;
		cout << "1.重新删除" << endl;
		cout << "2.返回上一级" << endl;
		cout << "3.返回主菜单" << endl;
		cout << "请输出相应数字键进行选择：";

		cin >> select;
		switch (select)
		{
		case 1:
			system("cls");
			DeleteUser(L, L2, H, G, Q);
			break;
		case 2:
			system("cls");
			UserManagement(L, L2, H, G, Q);
			break;
		case 3:
			system("cls");
			SystemMenu(L, L2, H, G, Q);
			break;
		}
	}
	else
	{
		for (j = i; j <= L.length - 1; j++)
		{
			L.elem[j] = L.elem[j + 1];
		}
		--L.length;

		fstream fp;
		fp.open("C:\\Users\\DSHH\\Desktop\\美团餐馆预定管理信息系统\\user.txt", ios::out);
		if (!fp)
		{
			cout << "Can't open the user.txr file." << endl;
			exit(0);
		}

		fp << "用户账号" << " " << "密码" << " " << "联系方式" << endl;
		for (k = 0; k < L.length; k++)
		{
			fp << L.elem[k].useraccount << " " << L.elem[k].password << " " << L.elem[k].contact << endl;
		}

		fp.close();

		cout << "删除成功！" << endl;
		cout << endl;
		cout << "1.返回上一级" << endl;
		cout << "2.返回主菜单" << endl;
		cout << "请输出相应数字键进行选择：";
		cin >> select;
		switch (select)
		{
		case 1:
			system("cls");
			UserManagement(L, L2, H, G, Q);
			break;
		case 2:
			system("cls");
			SystemMenu(L, L2, H, G, Q);
			break;
		}
	}
}
//1.3.2.1查看所有商家
void LookAllShop(SqList& L, SqList2& L2, HashTable& H, AMGraph& G, LinkQueue& Q)
{
	void ShopManagement(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void SystemMenu(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	//排序商家名
	int i, j, select;

	cout << "商家信息如下：" << endl << endl;
	for (i = 0; i < H.count; i++)
	{
		cout << "*********************************************" << endl;
		cout << "商家名称：" << H.elem[i].shopname << endl;
		cout << "商家类型：" << H.elem[i].shoptype << endl;
		cout << "评分：" << H.elem[i].avgscore << endl;
		cout << "平均消费：" << H.elem[i].aveprice << endl;
		cout << "地址：" << H.elem[i].address << endl;
		cout << "联系电话：" << H.elem[i].phone << endl;
		cout << "特色菜品：";
		for (j = 0; j < 100; j++)
		{
			if (H.elem[i].foodname[j] != "null")
				cout << H.elem[i].foodname[j] << " ";
		}
		cout << endl;
		cout << "评价：";
		int j;
		for (j = 0; j < 100; j++)
		{
			if (H.elem[i].comment[j] != "null")
				cout << H.elem[i].comment[j] << " ";
		}
		cout << endl;
		cout << "*********************************************" << endl;
		cout << endl;
	}

	cout << endl;
	cout << "1.返回上一级" << endl;
	cout << "2.返回主菜单" << endl;
	cout << "请输出相应数字键进行选择：";

	cin >> select;
	switch (select)
	{
	case 1:
		system("cls");
		ShopManagement(L, L2, H, G, Q);
		break;
	case 2:
		system("cls");
		SystemMenu(L, L2, H, G, Q);
		break;
	}
}
//1.3.2.2查询某个商家
void FindShop(SqList& L, SqList2& L2, HashTable& H, AMGraph& G, LinkQueue& Q)
{
	int SearchHashTable(HashTable H, string shopname);
	void ShopManagement(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void SystemMenu(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);

	string shopname;
	int result, i, j, select;

	cout << "请输入商家的名称：" << endl;
	cin >> shopname;

	result = SearchHashTable(H, shopname);
	cout << "商家信息如下：" << endl;
	cout << "商家名称：" << H.elem[result].shopname << endl;
	cout << "商家类型：" << H.elem[result].shoptype << endl;
	cout << "评分：" << H.elem[result].avgscore << endl;
	cout << "平均消费：" << H.elem[result].aveprice << endl;
	cout << "地址：" << H.elem[result].address << endl;
	cout << "联系电话：" << H.elem[result].phone << endl;
	cout << "特色菜品：";
	for (i = 0; i < 100; i++)
	{
		if (H.elem[result].foodname[i] != "null")
			cout << H.elem[result].foodname[i] << " ";
	}
	cout << endl;
	cout << "评价：";
	for (j = 0; j < 100; j++)
	{
		if (H.elem[result].comment[j] != "null")
			cout << H.elem[result].comment[j] << " ";
	}
	cout << endl;

	cout << endl;
	cout << "1.返回上一级" << endl;
	cout << "2.返回主菜单" << endl;
	cout << "请输出相应数字键进行选择：";

	cin >> select;
	switch (select)
	{
	case 1:
		system("cls");
		ShopManagement(L, L2, H, G, Q);
		break;
	case 2:
		system("cls");
		SystemMenu(L, L2, H, G, Q);
		break;
	}
}
//1.3.2.3删除某个商家
void DeleteShop(SqList& L, SqList2& L2, HashTable& H, AMGraph& G, LinkQueue& Q)
{
	unsigned HashNum(string shopname);
	int HashFunction(long hashnum);
	void ShopManagement(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);
	void SystemMenu(SqList & L, SqList2 & L2, HashTable & H, AMGraph & G, LinkQueue & Q);

	string shopname;
	long hashnum, hashfunction;
	int j, k, select;

	cout << "请输入要删除商家的名称：" << endl;
	cin >> shopname;

	hashnum = HashNum(shopname);
	hashfunction = HashFunction(hashnum);

	fstream fp;
	fp.open("C:\\Users\\DSHH\\Desktop\\美团餐馆预定管理信息系统\\shop.txt", ios::out);
	if (!fp)
	{
		cout << "Can't open the user.txr file." << endl;
		exit(0);
	}

	for (j = 0; j < H.count; j++)
	{
		if (H.elem[j].shopname != H.elem[hashfunction].shopname)
		{
			fp << "shoptype: " << H.elem[j].shoptype << endl;
			fp << "shopId: " << H.elem[j].shopid << endl;
			fp << "shopName: " << H.elem[j].shopname << endl;
			fp << "shopPassword: " << H.elem[j].shoppassword << endl;
			fp << "avgScore: " << H.elem[j].avgscore << endl;
			fp << "avePrice: " << H.elem[j].aveprice << endl;
			fp << "address: " << H.elem[j].address << endl;
			fp << "phone: " << H.elem[j].phone << endl;
			for (k = 0; k < 100; k++)
			{
				if (H.elem[j].comment[k] != "null")
					fp << "food_id: " << H.elem[j].foodid[k] << ", " << "food_name: " << H.elem[j].foodname[k] << ", " << "food_price: " << H.elem[j].foodprice[k] << endl;
			}
			for (k = 0; k < 100; k++)
			{
				if (H.elem[j].comment[k] != "null")
					fp << "Comment" << k << ": " << H.elem[j].comment[k] << endl;
			}
		}

	}
	fp.close();
	cout << "删除成功！" << endl;
	void InitList2(SqList2 & L2);
	void CreateList2(SqList2 & L2);
	InitList2(L2);
	CreatList2(L2);
	void InitHashTable(HashTable & H);
	void CreateHashTable(SqList2 & L2, HashTable & H);
	InitHashTable(H);
	CreateHashTable(L2, H);

	cout << endl;
	cout << "1.返回上一级" << endl;
	cout << "2.返回主菜单" << endl;
	cout << "请输出相应数字键进行选择：";

	cin >> select;
	switch (select)
	{
	case 1:
		system("cls");
		ShopManagement(L, L2, H, G, Q);
		break;
	case 2:
		system("cls");
		SystemMenu(L, L2, H, G, Q);
		break;
	}
}


//主函数
int main()
{
	SqList L;
	InitList(L);
	CreatList(L);
	//PrintList(L);

	SqList2 L2;
	InitList2(L2);
	CreatList2(L2);
	//PrintList2(L2);

	HashTable H;
	InitHashTable(H);
	CreateHashTable(L2, H);
	//PrintHashTable(H);

	LinkQueue Q;
	InitQueue(Q);
	CreatQueue(Q);
	//PrintQueue(Q);

	AmGraphSqList A;
	InitAmGraphList(A);
	//CreatAmGraphList(A);
	//PrintAmGraphList(A);

	AMGraph G;
	InitAM(G);
	CreateAm(L2, A, G);

	SystemMenu(L, L2, H, G, Q);
	return 0;
}


//多余
/*void FindMerchant(LinkQueue& Q)
{
	void MerchantInformation(LinkQueue & Q, string mechantname);

	cout << "请输入商家名称：" << endl;
	string mechantname;
	cin >> mechantname;
	cout << "商家信信息如下：" << endl;
	MerchantInformation(Q, mechantname);

}

void MerchantInformation(LinkQueue& Q, string mechantname)
{

}*/