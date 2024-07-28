/*头文件*/
#include <fstream>	 //ifstream、ofstream
using namespace std; // ifstream、ofstream
#include <iostream>	 //cerr、close()、cout
#include <vector>	 //vector
#include <math.h>	 //log2()、ceil()
#include <iomanip>	 //left、setw()
#include <cstring>	 //strncpy()、strlen()
#include <algorithm> //find()

/*结构体*/
//字符-频率字典
struct Ch_fre_dict
{
	char ch;
	int fre;

	//查找字符c是否与ch相等
	bool operator==(const char &c) //重载"=="	为向量的find()查找
	{
		if (c == this->ch)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

//哈夫曼树结点
struct HTNode
{
	int weight; //频率权值
	int parent; //双亲、左、右结点在数组中的下标
	int lchild;
	int rchild;
	string ch_set; //字符集
};

//哈夫曼编码
struct Huffman_encode
{
	char ch;	   //字符
	string encode; //编码

	//查找字符c是否与ch相等
	bool operator==(const char &c) //重载"=="	为向量的find()查找
	{
		if (c == this->ch)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

/*函数声明*/
void read_file(vector<struct Ch_fre_dict> &ch_fre_table, string &text);										   //读取（文本）文件	传引用
void ch_fre_statistics(vector<struct Ch_fre_dict> &ch_fre_table, char ch);									   //字符-频率统计
void show_statistics(vector<struct Ch_fre_dict> ch_fre_table);												   //显示统计信息
void create_huffman_tree(HTNode *root, vector<struct Ch_fre_dict> ch_fre_table, int leaf_count);			   //构建哈夫曼树
void select_min(HTNode *root, int i, int &min1, int &min2);													   //查找权值最小的两个叶子节点的数组下标
void show_huffman_tree(HTNode *root, int tree_count);														   //显示哈夫曼树
void huffman_encode(HTNode *root, vector<struct Huffman_encode> &huffman_encode_table, int leaf_count);		   //哈夫曼编码
void show_huffman_encode(vector<struct Huffman_encode> huffman_encode_table);								   //显示哈夫曼编码表
void store_huffman_encode(vector<struct Huffman_encode> huffman_encode_table);								   //存储哈夫曼编码表
void text_encode(string text, vector<struct Huffman_encode> huffman_encode_table, string &encode_text);		   //文本编码
void text_decode(string encode_text, vector<struct Huffman_encode> huffman_encode_table, string &decode_text); //文本解码
void encode_statistics(string text, string encode_text, vector<struct Ch_fre_dict> ch_fre_table);			   //编码统计

/*主函数*/
int main()
{
	vector<struct Ch_fre_dict> ch_fre_table; //字符-频率表
	string text;							 //文本

	read_file(ch_fre_table, text); //读取（文本）文件	字符-频率统计
	show_statistics(ch_fre_table); //显示统计信息

	int leaf_count = ch_fre_table.size(); //字符数/叶子结点数
	int tree_count = 2 * leaf_count - 1;  //树结点数
	HTNode *root = new HTNode[tree_count];
	//动态创建数组	类似malloc()	n个叶子结点需要n+(n-1)=2n-1个树结点	逻辑形式是树，实际形式是数组

	create_huffman_tree(root, ch_fre_table, leaf_count); //构建哈夫曼树
	show_huffman_tree(root, tree_count);				 //显示哈夫曼树

	vector<struct Huffman_encode> huffman_encode_table; //哈夫曼编码表

	huffman_encode(root, huffman_encode_table, leaf_count); //哈夫曼编码
	show_huffman_encode(huffman_encode_table);				//显示哈夫曼编码表
	store_huffman_encode(huffman_encode_table);				//存储哈夫曼编码表

	string encode_text; //编码文本
	string decode_text; //解码文本

	text_encode(text, huffman_encode_table, encode_text);		 //文本编码
	text_decode(encode_text, huffman_encode_table, decode_text); //文本解码

	encode_statistics(text, encode_text, ch_fre_table); //编码统计

	delete[] root; //释放动态数组空间

	return 0;
}

/*函数定义*/
//读取（文本）文件
void read_file(vector<struct Ch_fre_dict> &ch_fre_table, string &text)
{
	//打开文件
	string file_name = "./content.txt"; //文件名

	ifstream in_file(file_name, ios::in); //文件输入流类	使用流类的构造函数打开文件	只读，文件不存在出错，文本模式
	if (!in_file)
	{
		cerr << "The 'content.txt' doesn't exist" << endl; //标准错误输出
		exit(1);
	}

	//读取文件
	char ch; //每一字符

	while (1)
	{
		in_file >> ch; // ifstream类内部对 >> 输出流运算符做重载	忽略换行符

		if (in_file.eof()) //文件末尾退出
		{
			break;
		}

		//字符-频率统计
		ch_fre_statistics(ch_fre_table, ch);

		//记录文本
		text = text + ch;
	}

	in_file.close(); //关闭文件

	return;
}

//字符-频率统计
void ch_fre_statistics(vector<struct Ch_fre_dict> &ch_fre_table, char ch)
{
	int new_ch_flag = 1; //是否是新字符标志 0否1是

	// vector<struct Ch_fre_dict>::iterator it;						//从向量中申请迭代器
	// for (it = ch_fre_table.begin(); it != ch_fre_table.end(); it++) //.end()返回的迭代器指向向量中最后一个元素的下一个元素（实际上不存在）
	// {
	// 	if (it->ch == ch) //迭代器类似指针	字符已存在，不是新字符，频率计数+1	*it.ch×(*it).ch√	find()扩展
	// 	{
	// 		new_ch_flag = 0; //==0	×
	// 		it->fre++;

	// 		return;
	// 	}
	// }

	//扩展
	vector<struct Ch_fre_dict>::iterator it;

	it = find(ch_fre_table.begin(), ch_fre_table.end(), ch); //不一定能找到
	if (it != ch_fre_table.end())							 //找到
	{
		new_ch_flag = 0; //==0	×
		it->fre++;
	}

	if (new_ch_flag == 1) //字符不存在
	{
		struct Ch_fre_dict ch_fre_dict; //字符—频率字典

		ch_fre_dict.ch = ch; //记录
		ch_fre_dict.fre = 1;

		ch_fre_table.push_back(ch_fre_dict); //加入字符—频率表
	}

	return;
}

//显示统计信息
void show_statistics(vector<struct Ch_fre_dict> ch_fre_table)
{
	cout << "【统计信息】" << endl;

	//统计字符总量、文本字结长度	可改进
	int ch_count = 0; //字符总量

	vector<struct Ch_fre_dict>::iterator it = ch_fre_table.begin();
	for (it; it != ch_fre_table.end(); it++)
	{
		ch_count = ch_count + it->fre;
	}

	cout << "字符总量：" << ch_count << endl;
	cout << "文本字节长度：" << ch_count << endl;

	//求概率、信息量
	cout << "字符	频率	概率" << endl;

	float prob = 0.0;
	float infor = 0.0; //信息量

	for (it = ch_fre_table.begin(); it != ch_fre_table.end(); it++)
	{
		prob = it->fre * 1.0 / ch_count; //概率 有小数而不是整数除法	避免舍入为0
		infor = infor + prob * log2(1 / prob);

		cout << left;  //设置左对齐输出
		cout << fixed; //设置浮点数有效位数->设置小数点位数

		cout << setw(8) << it->ch; //设置宽度
		cout << setw(8) << it->fre;
		cout << setprecision(2) << prob * 100 << '%' << endl;
	}

	cout << "信息量：" << setprecision(2) << infor << endl;
	cout << endl;

	return;
}

//构建哈夫曼树
void create_huffman_tree(HTNode *root, vector<struct Ch_fre_dict> ch_fre_table, int leaf_count)
{
	if (leaf_count <= 1) //只有一个叶子结点不可构建树
	{
		cerr << "There is only one leaf node and the tree cannot be built" << endl;
		exit(1);
	}

	//初始化
	for (int i = 0; i < leaf_count; i++) //构造叶子结点	叶子节点有权值	此时已将字符-频率表内容复制到树结构中
	{
		root[i].weight = ch_fre_table[i].fre; //频率作权值
		root[i].ch_set = ch_fre_table[i].ch;  //字符	赋值给字符串
	}

	for (int i = 0; i < 2 * leaf_count - 1; i++) //所有树结点均没有双亲和孩子节点
	{
		root[i].parent = -1; //数组下标为-1
		root[i].lchild = -1;
		root[i].rchild = -1;
	}

	// 叶子节点数-1次合并
	for (int i = leaf_count; i < 2 * leaf_count - 1; i++) //对除叶子节点的树节点操作
	{
		int min1 = -1; //权值最小的两个叶子节点的数组下标
		int min2;

		select_min(root, i, min1, min2); //查找权值最小的两个叶子节点的数组下标

		//合并，更新结点值	频率相加为新权值
		root[i].weight = root[min1].weight + root[min2].weight;
		root[min1].parent = i;
		root[min2].parent = i;
		root[i].lchild = min1;
		root[i].rchild = min2;
		root[i].ch_set = root[min1].ch_set + root[min2].ch_set; //字符拼接
	}

	return;
}

//查找权值最小的两个叶子节点的数组下标
void select_min(HTNode *root, int i, int &min1, int &min2)
{
	//查找数组下标最小的（从小到大查找），未进行合并的树节点（双亲节点值存的数组下标为-1）
	//初始化最小权重树节点的数组下标值
	for (int j = 0; j < i; j++) //对该树节点前面的所有节点	为叶子节点和权值比它小的节点
	{
		if (root[j].parent == -1)
		{
			min1 = j;
			break;
		}
	}

	//查找min1
	for (int j = 0; j < i; j++)
	{
		if ((root[j].parent == -1) && (root[j].weight < root[min1].weight)) //对未合并、权值更小的树节点	更新
		{
			min1 = j;
		}
	}

	//初始化min2
	for (int j = 0; j < i; j++)
	{
		if ((root[j].parent == -1) && (j != min1)) //最小且不予min1值同	注意
		{
			min2 = j;
			break;
		}
	}

	//查找min2
	for (int j = 0; j < i; j++)
	{
		if ((root[j].parent == -1) && (root[j].weight < root[min2].weight) && (j != min1))
		{
			min2 = j;
		}
	}

	return;
}

//显示哈夫曼树
void show_huffman_tree(HTNode *root, int tree_count)
{
	cout << "【哈夫曼树】" << endl;
	cout << "索引 权重/频率 双亲 左孩子 右孩子 字符集" << endl;

	cout << left; //设置左对齐输出
	for (int i = 0; i < tree_count; i++)
	{
		cout << setw(5) << i; //设置宽度
		cout << setw(9) << root[i].weight;
		cout << setw(4) << root[i].parent;
		cout << setw(7) << root[i].lchild;
		cout << setw(7) << root[i].rchild;
		cout << root[i].ch_set << endl;
	}

	cout << endl;

	return;
}

//哈夫曼编码
void huffman_encode(HTNode *root, vector<struct Huffman_encode> &huffman_encode_table, int leaf_count)
{
	//思路：从叶子到根结点逆向求每个字符的哈夫曼编码，存储在哈夫曼编码表中

	char *ch_encode = new char[leaf_count]; //每个字符的编码
	//树高-1=编码长度必<叶子结点数	因为每至少两个叶子结点成一树层
	//因为向上回溯、从后面填编码值，先分配定长空间，后再截取
	ch_encode[leaf_count - 1] = '\0'; //编码结束符	转换到数组下标需-1

	char *ch_encode1; //再分配的

	int encode_loc;
	int parent;
	int current;

	struct Huffman_encode huffman_encode; //哈夫曼编码

	for (int i = 0; i < leaf_count; i++) //逐字符/叶子结点求哈夫曼编码
	{
		encode_loc = leaf_count - 1; //每编码位置	开始时指向最后结束符位置
		parent = root[i].parent;	 // parent是当前树结点的双亲结点值，为双亲结点的数组下标，在回溯时会动态改变
		current = i;				 //记录当前树节点的数组下标，在回溯时会动态改变

		//从当前叶子结点向上回溯，直到根结点	只要不是根节点，树结点均有双亲结点值，双亲结点的数组下标不为-1
		while (parent != -1)
		{
			encode_loc--; //编码位置向前一个，判断后填编码值

			if (current == root[parent].lchild) //当前叶子结点是其双亲结点的左孩子
			{
				ch_encode[encode_loc] = '0'; //编码0
			}
			else
			{
				ch_encode[encode_loc] = '1';
			}

			//向上回溯更新当前树结点信息
			current = parent;
			parent = root[parent].parent;
		}

		//记录
		ch_encode1 = new char[leaf_count - encode_loc];									 //长度再分配
		strncpy(ch_encode1, &ch_encode[encode_loc], strlen(ch_encode + encode_loc - 1)); //存值	ch_encode是指针，参数需指针

		huffman_encode.ch = root[i].ch_set[0]; //字符	只针对叶子结点的字符而不是其他非叶子结点的字符集，string转char
		huffman_encode.encode = ch_encode1;	   //编码	char*转string

		huffman_encode_table.push_back(huffman_encode);

		delete[] ch_encode1; //释放空间
	}

	delete[] ch_encode; //释放空间

	return;
}

//显示哈夫曼编码表
void show_huffman_encode(vector<struct Huffman_encode> huffman_encode_table)
{
	cout << "【哈夫曼编码】" << endl;
	cout << "字符	编码" << endl;

	cout << left; //设置左对齐输出

	vector<struct Huffman_encode>::iterator it = huffman_encode_table.begin();
	for (it; it != huffman_encode_table.end(); it++)
	{
		cout << setw(8) << it->ch; //设置宽度
		cout << it->encode << endl;
	}

	cout << endl;

	return;
}

//存储哈夫曼编码表
void store_huffman_encode(vector<struct Huffman_encode> huffman_encode_table)
{
	//打开文件
	string file_name = "./huffman_encode.txt"; //文件名

	ofstream out_file(file_name, ios::out);
	//文件输出流类	使用流类的构造函数打开文件	只写，文件不存在新建，存在清除原内容，文本模式
	if (!out_file)
	{
		cerr << "Failed to open 'huffman_encode.txt' file" << endl; //标准错误输出
		exit(1);
	}

	//写文件
	out_file << "字符	编码" << endl;

	out_file << left; //设置左对齐输出

	vector<struct Huffman_encode>::iterator it = huffman_encode_table.begin();
	for (it; it != huffman_encode_table.end(); it++)
	{
		out_file << setw(8) << it->ch; //设置宽度
		out_file << it->encode << endl;
	}

	return;
}

//文本编码
void text_encode(string text, vector<struct Huffman_encode> huffman_encode_table, string &encode_text)
{
	cout << "【文本编解码】" << endl;
	cout << "文本：" << text << endl;
	cout << "文本编码：";

	for (int i = 0; i < text.size(); i++) //对文本每个字符
	{
		//查找编码
		vector<struct Huffman_encode>::iterator it;
		it = find(huffman_encode_table.begin(), huffman_encode_table.end(), text[i]); //必能找到

		cout << it->encode << ' ';

		encode_text = encode_text + it->encode;
	}
	cout << endl;

	cout << "文本编码：" << encode_text << endl;

	return;
}

//文本解码
void text_decode(string encode_text, vector<struct Huffman_encode> huffman_encode_table, string &decode_text)
{
	string encode;		//每一字符编码
	char ch;			//每一字符
	int match_flag = 0; //编码-字符匹配标志	0不匹配1匹配

	for (int i = 0; i < encode_text.size(); i++) //对编码文本每个码字
	{
		encode = encode + encode_text[i]; //记录码字

		//查找字符
		vector<struct Huffman_encode>::iterator it;
		for (it = huffman_encode_table.begin(); it != huffman_encode_table.end(); it++)
		{
			if (it->encode == encode) //匹配
			{
				ch = it->ch; //记录字符
				match_flag = 1;

				break;
			}
		}

		if ((match_flag == 1) && (it == huffman_encode_table.end())) //匹配后又不匹配
		{
			decode_text = decode_text + ch; //记录解码字符

			i = i - 1;		//编码码字退一位
			encode = "";	//记录码字重置
			match_flag = 0; //标志位重置
		}

		if ((match_flag == 1) && (i == encode_text.size() - 1)) //匹配后到码字末尾	'='×
		{
			decode_text = decode_text + ch; //记录解码字符
			break;
		}
	}

	cout << "文本解码：" << decode_text << endl;
	cout << endl;

	return;
}

//编码统计
void encode_statistics(string text, string encode_text, vector<struct Ch_fre_dict> ch_fre_table)
{
	cout << "【文本编码统计】" << endl;

	//统计文本编码中'0'和'1'的频率
	int zero_fre = 0; //文本编码中'0'和'1'的频率
	int one_fre = 0;

	for (int i = 0; i < encode_text.size(); i++)
	{
		if (encode_text[i] == '0')
		{
			zero_fre++;
		}
		else
		{
			one_fre++;
		}
	}

	cout << "文本编码中'0'的频率：" << zero_fre << endl;
	cout << "文本编码中'1'的频率：" << one_fre << endl;

	//统计文本编码后位长度
	int text_length = 0;   //文本位长度
	int encode_length = 0; //文本编码后位长度

	text_length = text.size() * ceil(log2((float)ch_fre_table.size())); //字符总量*log2(不同字符数+1)	转换为浮点数向上取整
	encode_length = encode_text.size();

	cout << "文本二进制位长度：" << text_length << endl;
	cout << "文本编码后二进制位长度：" << encode_length << endl;

	return;
}