#include <iostream>  
#include <stack>  
using namespace std;

const int number=1000;  
int train[number];  
stack<int> station;

int main()
{	
	int count;	
	while(cin>>count)	
	{		
		int i;		
		for(i=1;i<=count;i++) 		
		{	
			cin >> train[i];
		}		
		int AC=1,CB=1;		
		int flag=1;		
		while(CB<=count)		
		{			
			if(AC==train[CB])
			{
				AC++;
				CB++;
			}			//判断进栈车厢的编号和出栈车厢的编号是不是相同 						
			else if(!station.empty()&& station.top()== train[CB])
			{ 
				station.pop();
				CB++;
			}			//如果不同判断栈顶与出栈车厢的编号是否相同			//出栈，下一个车厢进入 			
			else if(AC<=count)//否则判断车辆是否全部进站 			
			{
				station.push(AC);
				AC++;
			}//进站			
			else
			{				
				flag=0;//车辆全部进站后仍不符合				
				break; 			
			} 		
		}
		if (flag == 1)
			cout << "Yes" << endl;
		else
			cout << "No" << endl; 
	} 	 
} 

