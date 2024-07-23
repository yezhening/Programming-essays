#include<stdio.h>
#include<stdlib.h>//srand()、rand()、exit()
#include<time.h>//time()

//进程控制块、链队列结构体
typedef struct Pcb//typedef重命名，struct Pcb名字长,定义变量不方便 struct Pcb和Pcb同；struct Pcb p和Pcb p
{
	int name;
	int state;
	int priority;
	double time;
	Pcb* next;//struct Pcb* next同，Pcb类型的指针，指向的是Pcb类型
}Pcb, * QueuePtr;//结构体变量 重命名，方便下面引用struct Pcb类型的指针:Queueptr front和struct Pcb *front同
typedef struct
{
	QueuePtr front;//指针指向地址,指向的是Pcb类型
	QueuePtr rear;
}LinkQueue;

//链队列操作
void InitQueue(LinkQueue& Q)//初始化链队列
{
	Q.front = Q.rear = new Pcb;//内容是结构体，头结点
	Q.front->next=NULL;
}
void EnQueue(LinkQueue& Q, QueuePtr p)//入队 传递链表指针/地址
{
	Q.rear->next = p;//有头节点
	Q.rear = p;
}
void PrintQueue(LinkQueue& Q)//打印链表内容
{
	if (Q.front == Q.rear)
	{
		printf("队列为空\n");
	}
	else
	{
		QueuePtr p;//移动指针
		p = Q.front->next;//头节点的下一个为首元节点,取地址
		printf("进程名   进程状态   优先级   需要在处理机上执行的时间\n");
		while (p != NULL)
		{
			printf("%d\t %d\t    %d\t     %.2f\n", p->name, p->state, p->priority, p->time);
			p = p->next;
		}
	}
}
void SortQueue(LinkQueue& Q)//基于优先数的调度算法，根据优先级排序链表	冒泡排序 不用修改指针指向，交换指针指向的内容即可！
{
	QueuePtr p,q;
	p = Q.front->next;
	while (p != NULL)
	{
		q = p->next;
		while (q != NULL)
		{
			if (p->priority < q->priority)//交换元素
			{
				int name = q->name;
				int state = q->state;
				int priority = q->priority;
				double time = q->time;
				q->name = p->name;
				q->state = p->state;
				q->priority = p->priority;
				q->time = p->time;
				p->name = name;
				p->state = state;
				p->priority = priority;
				p->time = time;
			}
			q = q->next;
		}
		p = p->next;
	}
}
QueuePtr GetHead(LinkQueue Q,int  executionTimes)//取队头元素 可有&	传指针变量不传实体变量,否则出错！！！
{
	if (Q.front == Q.rear)
	{
		printf("就绪队列为空，进程调度执行完毕\n");
		exit(0);
	}
	else
	{
		QueuePtr p;
		p = Q.front->next;
		printf("【第%d次执行】\n", executionTimes);
		printf("调该进程在CPU执行\n");
		printf("进程名   进程状态   优先级   需要在处理机上执行的时间\n");
		printf("%d\t %d\t    %d\t     %.2f\n", p->name, p->state, p->priority, p->time);
		return p;//注意
	}
}
void DeQueue(LinkQueue& Q)//出队
{
	//判断队空 易漏	直接在取头元素时判断，这里可不再判断
	QueuePtr p;
	p = Q.front->next;
	Q.front->next = p->next;
	if (Q.rear == p)
	{
		Q.rear = Q.front;//最后一个元素被删，队尾指针指向头结点 易漏
	}
	//delete p;//报错
	//free(p);//报错
	p = NULL;//本就为空×
}
void InsertQueue(LinkQueue& Q1, LinkQueue& Q2)//将等待队列中的第一个PCB加入就绪队列的队尾
{
	if (Q2.front != Q2.rear)//注意判断
	{
		QueuePtr p;
		p = Q2.front->next;
		p->state = 0;//修改进程状态
		Q2.front->next = p->next;
		if (Q2.rear == p)
		{
			Q2.rear = Q2.front;//易漏
		}
		Q1.rear->next = p;
		p->next = NULL;//本就为空×
		Q1.rear = p;//易漏
	}
	else
	{
		printf("等待队列已空\n");
	}
}
int EmptyQueue(LinkQueue& Q)//判断队空
{
	if (Q.front == Q.rear)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}

int main()
{
	//配置参数
	srand((unsigned)time(NULL));//srand()给rand()提供种子seed,利用系统时间来改变系统的种子值,使得每次调用rand函数生成的伪随机数序列不同

	//初始化就绪队列、等待队列
	LinkQueue readyQueue;
	LinkQueue waitingQueue;
	InitQueue(readyQueue);
	InitQueue(waitingQueue);

	//初始化进程控制块
	Pcb pcb[10];//结构体变量构成的结构体数组
	int priority[10];//存放优先级数组
	for (int i = 0; i < 10; i++)
	{
		priority[i] = -1;//初始化数组
	}
	printf("*****************************************************\n");
	printf("初始进程控制块表\n");
	printf("进程名   进程状态   优先级   需要在处理机上执行的时间\n");
	for (int i = 0; i < 10; i++)
	{
		pcb[i].name = i;
		pcb[i].state = rand() % 2;
		int tempPriority = rand() % 10;//优先级数各不相同
		while(priority[tempPriority] == tempPriority)
		{
			tempPriority = (tempPriority + 1) % 10;//注意循环
		}
		priority[tempPriority] = tempPriority;//相应的优先级放在相应的数组下标
		pcb[i].priority = tempPriority;
		pcb[i].time = 1+1.0*rand()/RAND_MAX*100;
		pcb[i].next = NULL;
		printf("%d\t %d\t    %d\t     %.2f\n", pcb[i].name, pcb[i].state, pcb[i].priority, pcb[i].time);
		

		//入队
		if (pcb[i].state == 0)
		{
			EnQueue(readyQueue, &pcb[i]);//传递结构体变量地址
		}
		if (pcb[i].state == 1)
		{
			EnQueue(waitingQueue, &pcb[i]);
		}
	}
	printf("\n就绪队列\n");
	PrintQueue(readyQueue);
	printf("\n等待队列\n");
	PrintQueue(waitingQueue);
	printf("*****************************************************\n");
	
	//基于优先数的调度算法，根据优先级排序链表
	SortQueue(readyQueue);
	printf("排序后的就绪队列\n");
	PrintQueue(readyQueue);
	printf("\n等待队列\n");//等待队列不需要按优先级排序
	PrintQueue(waitingQueue);
	printf("*****************************************************\n");
	
	//执行进程
	int executionTimes = 0;
	while(1)
	{
		executionTimes++;
		//取就绪队列头元素进程在CPU执行
		QueuePtr executedPcb = GetHead(readyQueue, executionTimes);//传递指针变量
		DeQueue(readyQueue);
		printf("\n调一个进程执行后的就绪队列\n");
		PrintQueue(readyQueue);
		printf("\n调一个进程执行后的等待队列\n");
		PrintQueue(waitingQueue);
		printf("*****************************************************\n");

		//在进程执行过程中，产生随机数0或1，该随机数为1时，将等待队列中的第一个PCB加入就绪队列的队尾
		int random1 = rand() % 2;
		int emptyFlag = EmptyQueue(waitingQueue);
		printf("随机数为：%d\n", random1);
		if (random1 == 1 && emptyFlag == -1)
		{
			InsertQueue(readyQueue, waitingQueue);
			printf("将等待队列中的第一个PCB加入就绪队列的队尾后的就绪队列\n");
			PrintQueue(readyQueue);
			printf("\n将等待队列中的第一个PCB加入就绪队列的队尾后的等待队列\n");
			PrintQueue(waitingQueue);
			printf("*****************************************************\n");
			SortQueue(readyQueue);//加入后排序
			printf("排序后的就绪队列\n");
			PrintQueue(readyQueue);
			printf("\n等待队列\n");
			PrintQueue(waitingQueue);
			printf("*****************************************************\n");
		}
		else if (random1 == 1 && emptyFlag == 1)
		{
			printf("等待队列为空，不将等待队列中的第一个PCB加入就绪队列的队尾\n");
			printf("*****************************************************\n");
		}
		else
		{
			printf("不将等待队列中的第一个PCB加入就绪队列的队尾\n");
			printf("*****************************************************\n");
		}

		//在进程执行过程中，产生一个随机数，表示执行进程能在处理机上执行的时间，如果随机时间大于总需要的时间，则执行完成。如果小于，则从总时间中减去执行时间。
		double random2 = 1 + 1.0 * rand() / RAND_MAX * 100;
		printf("随机数为：%.2f 进程需要的处理机时间为：%.2f\n", random2, executedPcb->time);
		if (random2 > executedPcb->time)
		{
			printf("进程执行完成\n");
			printf("目前的就绪队列\n");
			PrintQueue(readyQueue);
			printf("\n目前的等待队列\n");
			PrintQueue(waitingQueue);
			printf("*****************************************************\n");
		}
		else
		{
			executedPcb->time = executedPcb->time - random2;
			//如果执行进程没有执行完成。则产生随机数0或1，当该随机数为0时，将执行进程加入就绪队列对尾；否则，将执行进程加入等待队列对尾
			executedPcb->next = NULL;//入队时传的是地址，手动修改结构体中指针指向，否则其一直指向下一位，输出时无限循环
			int random3 = rand() % 2;
			printf("进程未执行完成\n");
			printf("随机数为：%d\n", random3);
			if (random3 == 0)
			{
				printf("将执行进程加入就绪队列队尾\n");
				EnQueue(readyQueue, executedPcb);
				printf("进程没有执行完成将执行进程加入就绪队列队尾的就绪队列\n");
				PrintQueue(readyQueue);
				printf("\n等待队列\n");
				PrintQueue(waitingQueue);
				printf("*****************************************************\n");
			}
			else
			{
				printf("将执行进程加入等待队列队尾\n");
				executedPcb->state = 1;//状态改变
				EnQueue(waitingQueue, executedPcb);
				printf("就绪队列\n");
				PrintQueue(readyQueue);
				printf("\n进程没有执行完成将执行进程加入等待队列对尾的等待队列\n");
				PrintQueue(waitingQueue);
				printf("*****************************************************\n");
			}

			//由于优先级高，加入队尾的就绪队列仍然要放在队头,交换内容后队尾的指针设为空（本就为空）
			SortQueue(readyQueue);
			printf("排序后的就绪队列\n");
			PrintQueue(readyQueue);
			printf("\n等待队列\n");
			PrintQueue(waitingQueue);
			printf("*****************************************************\n");
		}
	}
	
	return 0;
}