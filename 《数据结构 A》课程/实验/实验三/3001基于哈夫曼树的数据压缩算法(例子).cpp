#include <iostream>
#include <cstring>
#include <cmath>
using namespace std;int d;int fre[26];int kn=0;int k=0;char namelist[26];
 
typedef struct{
	int vtag;
	int weight;
	int parent,lchild,rchild;
}HTNode,*HuffmanTree;
 
typedef char** HuffmanCode;
 
typedef struct{
	int Freque; 
	HuffmanCode HCC;
	char ch;
}Letter;
 
void Select(HuffmanTree HT,int len,int &s1,int &s2)
{
	int i,min1=0x3f3f3f3f,min2=0x3f3f3f3f;//先赋予最大值
	for(i=1;i<=len;i++)
	{
		if(HT[i].weight<min1&&HT[i].parent==0)
		{
			min1=HT[i].weight;
			s1=i;
		}	
	}
	int temp=HT[s1].weight;//将原值存放起来，然后先赋予最大值，防止s1被重复选择
	HT[s1].weight=0x3f3f3f3f;
	for(i=1;i<=len;i++)
	{
		if(HT[i].weight<min2&&HT[i].parent==0)
		{
			min2=HT[i].weight;
			s2=i;
		}
	}
	HT[s1].weight=temp;//恢复原来的值
}
 
void CreateHuffmanTree(HuffmanTree &HT,int n)
{
	int i;
	if(n<=1) return;
	int m=2*n-1;
	HT=new HTNode[m+1];
	for(i=1;i<=m;i++)
	{
		HT[i].parent=0;
		HT[i].lchild=0;
		HT[i].rchild=0;
	 } 
	for(i=1;i<=n;i++)
		HT[i].weight=fre[i-1];
	int s1,s2;
	for(i=n+1;i<=m;i++)
	{
		Select(HT,i-1,s1,s2);
		HT[s1].parent=i;
		HT[s2].parent=i;
		HT[i].lchild=s1;
		HT[i].rchild=s2;
		HT[i].weight=HT[s1].weight+HT[s2].weight;
	}
}
 
void FrequeStats(int n[],string s)
{
	int i=0;
	while(s[i]!='\0')
	{
		int e=(int) s[i];
		if(e>=97)
			n[e-97]++;
		i++;
	}
	d=i;
//	cout<<"一共输入了"<<d<<"个字符"<<endl;
	for(i=0;i<26;i++)
		if(n[i]) 
		{
				kn++;
		}
	for(i=0;i<26;i++)
		if(n[i]) 
		{
				char e=(char)(i+97);
				cout<<e<<":"<<n[i];
				if(k!=kn-1)cout<<" ";//多输出了一个 
				fre[k]=n[i];
				namelist[k]=e;
				k++;
		}
	//cout<<"一共有"<<k<<"个叶子结点"<<endl;
	cout<<endl;
}
 
void CreatHuffmanCode(HuffmanTree HT,HuffmanCode &HC,int n)
{
	//从叶子到根逆向求每个字符的赫夫曼编码，存储在编码表HC中
	int i,start,c,f;
	HC=new char*[n+1];         						//分配n个字符编码的头指针矢量
	char *cd=new char[n];							//分配临时存放编码的动态数组空间
	cd[n-1]='\0';                            		//编码结束符
	for(i=1;i<=n;++i)
	{                      							//逐个字符求赫夫曼编码
		start=n-1;                          		//start开始时指向最后，即编码结束符位置
		c=i; 
		f=HT[i].parent;                 			//f指向结点c的双亲结点
		while(f!=0)
		{                          					//从叶子结点开始向上回溯，直到根结点
			--start;                          		//回溯一次start向前指一个位置
			if(HT[f].lchild==c)  
				cd[start]='0';						//结点c是f的左孩子，则生成代码0
			else 
				cd[start]='1';                 		//结点c是f的右孩子，则生成代码1
			c=f; 
			f=HT[f].parent;             			//继续向上回溯
		}                                  			//求出第i个字符的编码      
		HC[i]=new char[n-start];         			// 为第i 个字符编码分配空间
		strcpy(HC[i], &cd[start]);        			//将求得的编码从临时空间cd复制到HC的当前行中
	}
	delete cd;                            			//释放临时空间
}													// CreatHuffanCode
void Show(HuffmanTree HT,HuffmanCode HC)
{
	for(int i=1;i<=2*k-1;i++)
		cout<<i<<" "<<HT[i].weight<<" "<<HT[i].parent<<" "<<HT[i].lchild<<" "<<HT[i].rchild<<endl; 
 
	for(int i=1;i<=k;i++)
	{
		cout<<namelist[i-1]<<":"<<HC[i];
		if(i!=k)cout<<" ";//多输出了一个 
	}
	cout<<endl; 
}
 
void Tranfer(HuffmanCode HC,string s)
{
	int i=0;
	while(s[i])
	{
		int j=0;
		while(j<26)
		{
			if(s[i]==namelist[j])//如果该字符与namelist中某个字符相等
			cout<<HC[j+1];//输出其对应编码 
			j++;
		}
		i++;
	}
	cout<<endl;
	
	i=0;
	while(s[i])
	{
		char e;
		e=s[i];
		cout<<e;
		i++; 
	}
	cout<<endl; 
}
 
void InitData(int fre[])
{
		
		for(int i=0;i<26;i++)
			fre[i]=0;
		//memset(fre,0,sizeof(fre));//不能用这个，为什么？ 
		memset(namelist,0,sizeof(namelist));
		d=0;k=0;kn=0;
 
		return;
}
 
int main()
{
	while(1)
	{
		string s;
		cin>>s;
		if("0"==s) break;
		
		InitData(fre);
		FrequeStats(fre,s);
		HuffmanTree HT;
		HuffmanCode HC;
		CreateHuffmanTree(HT,k);
		CreatHuffmanCode(HT,HC,k);
		Show(HT,HC);
		Tranfer(HC,s);
	}
	return 0;
}