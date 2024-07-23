#include<stdio.h>
#include<stdlib.h>//srand()、rand()、exit();
#include<time.h>//time()

//进程控制块、链队列、链表结构体
typedef struct Pcb
{
	int id;
	int type;//0表示生产者，1表示消费者
	int state;//0表示就绪，1表示等待
	char product;//生产者的是其id号，消费者为' '
	Pcb* next;
}Pcb,*QueuePtr;
typedef struct
{
	QueuePtr front;
	QueuePtr rear;
}LinkQueue;
typedef struct
{
	QueuePtr front;
	QueuePtr rear;
}LinkList;

//链队列操作
void InitQueue(LinkQueue& Q)//初始化链队列
{
	Q.front = Q.rear = new Pcb;
	Q.front->next = NULL;
}
void EnQueue(LinkQueue& Q, QueuePtr p)//入队
{
	Q.rear->next = p;
	Q.rear = p;
}
void PrintQueue(LinkQueue& Q)//打印队列
{
	if (Q.front == Q.rear)
	{
		printf("队列为空\n");
	}
	else
	{
		QueuePtr p;
		p = Q.front->next;
		printf("进程名   进程类型   进程状态   进程产品\n");
		printf("———————————————————\n");
		while (p != NULL)
		{
			printf("%d\t %d\t    %d\t       %c\n", p->id, p->type, p->state, p->product);
			p = p->next;
		}
	}
}
QueuePtr GetHead(LinkQueue Q,int rwflag)//取队头元素
{
	void process();

	if (rwflag == 'r')
	{
		if (Q.front == Q.rear)
		{
			printf("就绪队列为空，进程调度执行完毕\n");

			//程序询问是否要继续？如果重新开始执行，否则退出程序
			char choice=NULL;//初始化
			printf("\n请问是否继续执行程序？\n");
			printf("是请输入'y',否请输入'n':");
			getchar();//一定要吸收回车
			scanf_s("%c",&choice,sizeof(choice));//scanf_s的安全边界检查
			//scanf("%c", &choice);

			if (choice == 'y')
			{
				printf("\n");
				process();
			}
			else
			{
				exit(0);
			}
		}
		else
		{
			QueuePtr p;
			p = Q.front->next;
			return p;
		}
	}
	else
	{
		if (Q.front == Q.rear)
		{
			printf("等待队列为空，不唤醒进程进入就绪队列\n");
		}
		else
		{
			QueuePtr p;
			p = Q.front->next;
			return p;
		}
	}
}
void DeQueue(LinkQueue& Q)//出队
{
	QueuePtr p;
	p = Q.front->next;
	Q.front->next = p->next;
	if (Q.rear == p)
	{
		Q.rear = Q.front;
	}
	p = NULL;
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

//链表操作
void InitList(LinkList& L)//初始化链表 传递地址、指针
{
	L.front = L.rear = new Pcb;
	L.front->next = NULL;
}
void InsertList(LinkList& L,QueuePtr p)//插入链表
{
	L.rear->next = p;
	L.rear = p;
}
void PrintList(LinkList &L)//打印链表
{
	if (L.front == L.rear)
	{
		printf("链表为空\n");
	}
	else
	{
		QueuePtr p;
		p = L.front->next;
		printf("进程名   进程类型   进程状态   进程产品\n");
		printf("———————————————————\n");
		while (p != NULL)
		{
			printf("%d\t %d\t    %d\t       %c\n", p->id, p->type, p->state, p->product);
			p = p->next;
		}
	}
}

void process()//一整个执行流程
{
	//配置参数
	srand((unsigned)time(NULL));

	//定义缓冲区,使用数组，存放产品
	char* buffer;
	int bufferSize;

	printf("请输入初始化缓冲区大小，输入为整数类型的数字：");
	scanf_s("%d", &bufferSize);
	//scanf("%d", &bufferSize);
	buffer = (char*)malloc(bufferSize * sizeof(char));
	for (int i = 0; i < bufferSize; i++)//初始化
	{
		buffer[i] = ' ';
	}
	int emptyPointer = -1;//指向空缓冲区指针
	int fullPointer = -1;//指向满缓冲区指针

	//初始化就绪队列、生产者等待队列、消费者等待队列
	LinkQueue readyQueue;
	LinkQueue producerWaitingQueue;
	LinkQueue consumerWaitingQueue;
	InitQueue(readyQueue);
	InitQueue(producerWaitingQueue);
	InitQueue(consumerWaitingQueue);
	char rwflag = 'r';//判断是就绪队列/等待队列调用取队头元素函数,默认为就绪队列

	//初始化结束进程over链表
	LinkList over;//使用指针定义
	InitList(over);//传递指针

	//由用户指定要产生的进程及其类别(随机)，存入进入就绪队列。
	//初始化进程控制块
	int pcbNum;//动态数组
	Pcb* pcb;

	printf("请输入初始化进程个数：");
	scanf_s("%d", &pcbNum);
	pcb = (Pcb*)malloc(pcbNum * sizeof(Pcb));

	printf("*******************************************\n");
	printf("【初始进程控制块表】\n");
	printf("进程名   进程类型   进程状态   进程产品\n");
	printf("———————————————————\n");
	//Pcb pcb[10];//默认生成十个进程
	for (int i = 0; i < pcbNum; i++)
	{
		pcb[i].id = i;
		pcb[i].type = rand() % 2;
		pcb[i].state = 0;
		if (pcb[i].type == 0)
		{
			pcb[i].product = (char)i + '0';//int向char类型转换会被截取低字节，char是按ASCLL型存储的，加上
		}
		else
		{
			pcb[i].product = ' ';
		}
		pcb[i].next = NULL;
		printf("%d\t %d\t    %d\t       %c\n", pcb[i].id, pcb[i].type, pcb[i].state, pcb[i].product);

		EnQueue(readyQueue, &pcb[i]);
	}

	printf("\n【缓冲区】\n");
	printf("缓冲区为空\n");
	printf("\n【就绪队列】\n");
	PrintQueue(readyQueue);
	printf("\n【生产者等待队列】\n");
	PrintQueue(producerWaitingQueue);
	printf("\n【消费者等待队列】\n");
	PrintQueue(consumerWaitingQueue);
	printf("\n【结束进程链表】\n");
	PrintList(over);
	printf("*******************************************\n");

	//执行进程
	int executionTimes = 0;//执行次数
	while (1)
	{
		executionTimes++;

		//调度程序从就绪队列中提取一个就绪进程运行
		rwflag = 'r';
		QueuePtr executedPcb = GetHead(readyQueue,rwflag);
		printf("【第%d次执行】\n", executionTimes);
		printf("调该进程在CPU执行\n");
		printf("进程名   进程类型   进程状态   进程产品\n");
		printf("———————————————————\n");
		printf("%d\t %d\t    %d\t       %c\n", executedPcb->id, executedPcb->type, executedPcb->state, executedPcb->product);
		DeQueue(readyQueue);

		if (executedPcb->type == 0)//如果进程类型是生产者
		{
			//如果申请的资源被阻塞则进入相应的等待队列
			emptyPointer = (emptyPointer + 1) % bufferSize;//落后一个指针 循环
			if (buffer[emptyPointer] != ' ')
			{
				emptyPointer = (emptyPointer - 1) % bufferSize;//注意指针回退！！！
				if (emptyPointer == 0)//注意特殊情况
				{
					emptyPointer = bufferSize - 1;
				}

				printf("\n【缓冲区】\n");
				printf("[");
				for (int i = 0; i < bufferSize; i++)
				{
					if (i == bufferSize - 1)
					{
						printf("%c]\n", buffer[i]);
					}
					else
					{
						printf("%c,", buffer[i]);
					}
				}
				printf("缓冲区已满，无法执行该进程，将其挂入生产者等待队列\n");

				executedPcb->state = 1;
				executedPcb->next = NULL;//入队时传的是地址，手动修改结构体中指针指向，否则其一直指向下一位，输出时无限循环
				EnQueue(producerWaitingQueue, executedPcb);

				printf("\n【就绪队列】\n");
				PrintQueue(readyQueue);
				printf("\n【生产者等待队列】\n");
				PrintQueue(producerWaitingQueue);
				printf("\n【消费者等待队列】\n");
				PrintQueue(consumerWaitingQueue);
				printf("\n【结束进程链表】\n");
				PrintList(over);
				printf("*******************************************\n");
			}
			else
			{
				buffer[emptyPointer] = executedPcb->product;
				executedPcb->next = NULL;
				//运行结束的进程进入over链表
				InsertList(over, executedPcb);

				//进程运行结束时，会检查对应的等待队列，激活队列中的进程进入就绪队列。
				if (EmptyQueue(producerWaitingQueue) == -1)
				{
					rwflag = 'w';
					QueuePtr awakenPcb = GetHead(producerWaitingQueue,rwflag);
					printf("\n进程执行完成，唤醒该生产者进程进入就绪队列\n");
					printf("进程名   进程类型   进程状态   进程产品\n");
					printf("———————————————————\n");
					printf("%d\t %d\t    %d\t       %c\n", awakenPcb->id, awakenPcb->type, awakenPcb->state, awakenPcb->product);
					awakenPcb->state = 0;
					DeQueue(producerWaitingQueue);
					EnQueue(readyQueue, awakenPcb);
				}
				else
				{
					printf("\n进程执行完成，生产者等待队列为空，无需唤醒进程\n");
				}

				printf("\n【缓冲区】\n");
				printf("[");
				for (int i = 0; i < bufferSize; i++)
				{
					if (i == bufferSize - 1)
					{
						printf("%c]\n", buffer[i]);
					}
					else
					{
						printf("%c,", buffer[i]);
					}
				}
				printf("\n【就绪队列】\n");
				PrintQueue(readyQueue);
				printf("\n【生产者等待队列】\n");
				PrintQueue(producerWaitingQueue);
				printf("\n【消费者等待队列】\n");
				PrintQueue(consumerWaitingQueue);
				printf("\n【结束进程链表】\n");
				PrintList(over);
				printf("*******************************************\n");
			}
		}
		else//如果进程类型是消费者
		{
			fullPointer = (fullPointer + 1) % bufferSize;
			if (buffer[fullPointer] == ' ')
			{
				fullPointer = (fullPointer - 1) % bufferSize;
				if (fullPointer == 0)
				{
					fullPointer = bufferSize - 1;
				}
				printf("\n【缓冲区】\n");
				printf("缓冲区为空,无法执行该进程，将其挂入等待队列\n");

				executedPcb->state = 1;
				executedPcb->next = NULL;
				EnQueue(consumerWaitingQueue, executedPcb);

				printf("\n【就绪队列】\n");
				PrintQueue(readyQueue);
				printf("\n【生产者等待队列】\n");
				PrintQueue(producerWaitingQueue);
				printf("\n【消费者等待队列】\n");
				PrintQueue(consumerWaitingQueue);
				printf("\n【结束进程链表】\n");
				PrintList(over);
				printf("*******************************************\n");
			}
			else
			{
				buffer[fullPointer] = ' ';
				executedPcb->next = NULL;
				InsertList(over, executedPcb);

				if (EmptyQueue(consumerWaitingQueue) == -1)
				{
					rwflag = 'w';
					QueuePtr awakenPcb = GetHead(consumerWaitingQueue, rwflag);
					printf("\n进程执行完成，唤醒该消费者进程进入就绪队列\n");
					printf("进程名   进程类型   进程状态   进程产品\n");
					printf("———————————————————\n");
					printf("%d\t %d\t    %d\t       %c\n", awakenPcb->id, awakenPcb->type, awakenPcb->state, awakenPcb->product);
					awakenPcb->state = 0;
					DeQueue(consumerWaitingQueue);
					EnQueue(readyQueue, awakenPcb);
				}
				else
				{
					printf("\n进程执行完成，消费者等待队列为空，无需唤醒进程\n");
				}

				printf("\n【缓冲区】\n");
				printf("[");
				for (int i = 0; i < bufferSize; i++)
				{
					if (i == bufferSize - 1)
					{
						printf("%c]\n", buffer[i]);
					}
					else
					{
						printf("%c,", buffer[i]);
					}
				}
				printf("\n【就绪队列】\n");
				PrintQueue(readyQueue);
				printf("\n【生产者等待队列】\n");
				PrintQueue(producerWaitingQueue);
				printf("\n【消费者等待队列】\n");
				PrintQueue(consumerWaitingQueue);
				printf("\n【结束进程链表】\n");
				PrintList(over);
				printf("*******************************************\n");
			}
		}
	}
}

int main(void)
{	
	process();

	return 0;
}