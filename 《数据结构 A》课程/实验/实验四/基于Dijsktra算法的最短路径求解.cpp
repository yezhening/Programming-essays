#include <iostream>
#include <cstring>//使用memset函数初始化顶点表
using namespace std;

#define MaxInt 999//表示极大值，即正无穷
#define MVNum 100//最大顶点数

typedef struct
{
	char vexs[MVNum];//顶点表 
	int arcs[MVNum][MVNum];//邻接矩阵 
	int vexnum, arcnum;//图的当前点数和边数 
}AMGraph;

int LocateVex(AMGraph G, char u)//查找顶点在顶点表中的位置
{//存在则返回顶点在顶点表中的下标;否则返回-1
	int i;
	for (i = 0; i < G.vexnum; ++i)
	{
		if (u == G.vexs[i])
			return i;
	}
	return -1;
}

void InitAM(AMGraph& G)//初始化图
{ 
	memset(G.vexs, 0, sizeof(G.vexs));//初始化顶点表
	int i, j;
	for (i = 0; i < MVNum; i++)//初始化邻接矩阵
	{
		for (int j = 0; j < MVNum; j++)
		{
			G.arcs[i][j] = MaxInt;
		}
	}
}

void CreateGraph(AMGraph& G)//创建图
{
	int i, j, k;
	for (i = 0; i < G.vexnum; i++)//创建顶点表
		cin >> G.vexs[i];
	for (k = 0; k < G.arcnum; k++)//创建邻接矩阵
	{
		char v1, v2;
		int w;
		cin >> v1 >> v2 >> w;
		i = LocateVex(G, v1);
		j = LocateVex(G, v2);
		G.arcs[i][j] = w;
		G.arcs[j][i] = w;
		G.arcs[k][k] = 0;//易漏
	}
}

void ShortestPath_DIJ(AMGraph G) 
{//用Dijkstra算法求有向网G的v0顶点到其余顶点的最短路径 
	int S[MVNum];
	int D[MVNum];
	int Path[MVNum];

	char v0, v1;
	cin >> v0 >> v1;
	int v00;//自己体会,邻接矩阵需要用到下标int而不是点char
	v00 = LocateVex(G, v0);

	int n;
	n = G.vexnum; 
	int v;                
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

	int i,min,w;               	
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
	w = LocateVex(G, v1);
	cout << D[w] << endl;//输出第一行，v1所在的D[w]即为最终最短路径
	
	char road[100];//G.vexnum用不了
	road[0] = G.vexs[w];
	int t = w; i = 0;
	while (1)//依据Path回溯寻找点
	{
		i++;//多加一次
		if (t == -1 || t == v00)
			break;
		road[i] = G.vexs[Path[t]];
		t = Path[t];
	}

	while (i-1>=0)//反向输出
	{
		cout << road[i-1];
		if (i-1!=0)
			cout << " ";//死亡空格
		i--;
	}
	cout << endl;
}

int main()
{
	while (1)
	{
		AMGraph G;
		InitAM(G);
		cin >> G.vexnum >> G.arcnum;
		if (G.vexnum == 0 && G.arcnum == 0)
			break;
		CreateGraph(G);
		ShortestPath_DIJ(G);
	}
}