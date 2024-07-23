#include<stdio.h>
#include<string.h>//strcpy()
#include<stdlib.h>//srand()、rand()、exit()
#include<time.h>//time()

#define MEMORYSIZE 1024//内存大小为1MB，1024KB
#define FREESIZE 10//假定系统允许的空闲区表最大为FREESIZE
#define USEDSIZE 10//假定系统允许的最大作业数量为USEDSIZE

//————————————————————————————
//定义内存块（多余数据结构）
typedef struct Mb//Memory block
{
	float front;
	float size;
	char state[10];
}Mb;

//定义空闲区表
typedef struct FreeTable 
{
	float address;//空闲区起始地址
	float length;//空闲区长度，单位为字节
	int flag;//空闲区表登记栏目用0表示空栏目，1表示未分配
}FreeTable;//空闲区表

//定义分配区表
typedef struct UsedTable
{
	float address;//已分分区起始地址
	float length;//已分分区长度，单位为字节
	int flag;//已分配表区登记栏标志，用0表示空栏目，1表示已分配
}UsedTable;//已分配区表

//定义作业队列
typedef struct Job
{
	int id;
	float size;//表示作业所需要分配内存的大小
	int flag;//0表示没有分配到内存，1表示已分配到内存，2表示作业已经结束，内存已回收
	Job* next;
}Job,*QueuePtr;
typedef struct
{
	QueuePtr  front;
	QueuePtr  rear;
}LinkQueue;
typedef struct
{
	QueuePtr front;
	QueuePtr rear;
}LinkList;
//—————————————————————————————————————
void PrintFreeTable(FreeTable t[])//使用t[]		打印空闲区表
{
	printf("空闲区起始地址   空闲区长度   空闲区表登记栏目状态\n");
	printf("—————————————————————————\n");
	for (int i = 0; i < FREESIZE; i++)
	{
		printf("%-16.2f% -13.2f %d\n", t[i].address, t[i].length, t[i].flag);
	}
}

void PrintUsedTable(UsedTable t[])//打印已分配区表
{
	printf("已分配区表起始地址   已分配区表长度   已分配区表登记栏目状态\n");
	printf("——————————————————————————————\n");
	for (int i = 0; i < USEDSIZE; i++)
	{
		printf("%-20.2f% -17.2f %d\n", t[i].address, t[i].length, t[i].flag);
	}
}


//链队列操作
void InitQueue(LinkQueue& Q)//初始化链队列
{
	Q.front = Q.rear = new Job;
	Q.front->next = NULL;
}

void EnQueue(LinkQueue& Q, QueuePtr p)//入队
{
	Q.rear->next = p;
	Q.rear = p;
}

void PrintQueue(LinkQueue& Q)//打印链队列内容
{
	if (Q.front == Q.rear)
	{
		printf("队列为空\n");
	}
	else
	{
		QueuePtr p;
		p = Q.front->next;
		printf("作业名   作业所需分配内存大小	作业状态\n");
		printf("————————————————————\n");
		while (p != NULL)
		{
			printf("%d\t %-15.2f\t%d\n", p->id, p->size, p->flag);
			p = p->next;
		}
	}
}

int JudgeQueue(LinkQueue& Q)//判断作业是否都已分配到内存
{
	int flag = 1;//1表示都已分配到内存
	QueuePtr p;
	p = Q.front->next;

	while (p != NULL)
	{
		if (p->flag == 0)
		{
			flag = -1;//-1表示存在未分配内存的作业
		}
		p = p->next;
	}
	return flag;
}

int LengthQueue(LinkQueue& Q)//求链队列长度
{
	int length = 0;
	QueuePtr p;
	p = Q.front->next;
	while (p != NULL)
	{
		length++;
		p = p->next;
	}
	return length;
}

QueuePtr GetHead(LinkQueue Q)//取队头元素，不会出现队列为空情况，出现时已结束程序
{
		QueuePtr p;
		p = Q.front->next;
		return p;
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

//链表操作
void InitList(LinkList& L)//初始化链表
{
	L.front = L.rear = new Job;
	L.front->next = NULL;
}
void InsertList(LinkList& L, QueuePtr p)//插入链表
{
	L.rear->next = p;
	L.rear = p;
}
void PrintList(LinkList& L)//打印链表
{
	if (L.front == L.rear)
	{
		printf("链表为空\n");
	}
	else
	{
		QueuePtr p;
		p = L.front->next;
		printf("作业名   作业所需分配内存大小	作业状态\n");
		printf("————————————————————\n");
		while (p != NULL)
		{
			printf("%d\t %-15.2f\t%d\n", p->id, p->size, p->flag);
			p = p->next;
		}
	}
}

//程序执行函数
void MemoryAllocation(LinkQueue jobQueue, FreeTable free_table[], UsedTable used_table[])//使用首次适应算法分配内存
{
	printf("****************************************\n");
	printf("二.内存分配\n");

	int length = LengthQueue(jobQueue);
	for (int count = 0; count < length; count++)
	{
		QueuePtr executedJob = GetHead(jobQueue);
		DeQueue(jobQueue);

		if (executedJob->flag == 0)
		{
			printf("\n为该作业分配内存\n");
			printf("作业名   作业所需分配内存大小	作业状态\n");
			printf("————————————————————\n");
			printf("%d\t %-15.2f\t%d\n", executedJob->id, executedJob->size, executedJob->flag);
		}

		for (int i = 0; i < FREESIZE + 1; i++)//查找在空闲区表
		{
			if (free_table[i].flag == 1)//有空闲表项
			{
				if (executedJob->size < free_table[i].length)
				{
					printf("\n该作业可分得所需内存\n");
					printf("该作业内存分配完成\n");

					//修改空闲区表
					free_table[i].length = free_table[i].length - executedJob->size;
					if (free_table[i].length == 0)//只有在刚好分配时才置空表目
					{
						free_table[i].flag = 0;
					}

					//修改已分配区表
					for (int j = 0; j < USEDSIZE; j++)
					{
						if (used_table[j].flag == 0)
						{
							used_table[j].address = free_table[i].address;
							used_table[j].length = executedJob->size;
							used_table[j].flag = 1;
							break;
						}
					}

					free_table[i].address = free_table[i].address + executedJob->size;

					executedJob->flag = 1;//已分配内存作业转去执行
					executedJob->next = NULL;
					EnQueue(jobQueue,executedJob);//重新挂入作业队列

					printf("\n【更新空闲区表】\n");
					PrintFreeTable(free_table);
					printf("\n【更新已分配区表】\n");
					PrintUsedTable(used_table);
					printf("\n【更新作业队列】\n");
					PrintQueue(jobQueue);

					break;
				}
			}
			if (i == FREESIZE)
			{
				printf("\n无法为该作业分配内存，作业被阻塞，重新挂入队列\n");
				executedJob->next = NULL;
				EnQueue(jobQueue, executedJob);//未分配内存的作业被阻塞，重新挂入队列
				printf("\n【更新作业队列】\n");
				PrintQueue(jobQueue);
				break;
			}
		}//查找空闲区表
	}
	printf("***************************************");
	printf("\n内存分配完成");
	printf("\n【空闲区表】\n");
	PrintFreeTable(free_table);
	printf("\n【已分配区表】\n");
	PrintUsedTable(used_table);
	printf("\n【作业队列】\n");
	PrintQueue(jobQueue);
	printf("****************************************\n");
}

void MergingFreePartitions(FreeTable free_table[])//合并空闲分区
{
	printf("\n合并空闲区\n");
	for (int freeNum = 0; freeNum < FREESIZE; freeNum++)
	{
		if (free_table[freeNum].flag == 1 && free_table[freeNum + 1].flag == 1)
		{
			if (free_table[freeNum].address + free_table[freeNum].length == free_table[freeNum + 1].address)
			{
				free_table[freeNum].length = free_table[freeNum].length + free_table[freeNum + 1].length;
				free_table[freeNum + 1].address = -1;
				free_table[freeNum + 1].length = 0;
				free_table[freeNum + 1].flag = 0;
			}
		}
	}
	printf("【更新空闲区表】\n");
	PrintFreeTable(free_table);
}

void SortFreePartitions(FreeTable free_table[])//排序空闲分区
{
	//把空表目换到后面
	printf("\n排序空闲区\n");
	float temp;
	for (int i = 0; i < FREESIZE; i++)
	{
		if (free_table[i].flag == 0)
		{
			for (int j = i + 1; j < FREESIZE; j++)
			{
				if (free_table[j].flag == 1)
				{
					temp = free_table[i].address;
					free_table[i].address = free_table[j].address;
					free_table[j].address = temp;

					temp = free_table[i].length;
					free_table[i].length = free_table[j].length;
					free_table[j].length = temp;

					free_table[i].flag = 1;
					free_table[j].flag = 0;
					break;
				}
			}
		}
	}

	//首次适应算法按低高地址冒泡排序
	for (int i = 0; i < FREESIZE; i++)
	{
		for (int j = i + 1; j < FREESIZE; j++)
		{
			if (free_table[i].flag == 1 && free_table[j].flag == 1)
			{
				if (free_table[i].address > free_table[j].address)
				{
					temp = free_table[i].address;
					free_table[i].address = free_table[j].address;
					free_table[j].address = temp;

					temp = free_table[i].length;
					free_table[i].length = free_table[j].length;
					free_table[j].length = temp;

					break;
				}
			}
		}
	}

	printf("【更新空闲区表】\n");
	PrintFreeTable(free_table);
}

void MemoryRecovery(LinkQueue jobQueue, FreeTable free_table[], UsedTable used_table[],LinkList over)//内存回收
{
	int length = LengthQueue(jobQueue);
	for (int count = 0; count < length; count++)//对每个作业
	{
		int random = -1;

		QueuePtr executedJob = GetHead(jobQueue);
		DeQueue(jobQueue);
		printf("\n对该做作业：\n");
		printf("作业名   作业所需分配内存大小	作业状态\n");
		printf("————————————————————\n");
		printf("%d\t %-15.2f\t%d\n", executedJob->id, executedJob->size, executedJob->flag);

		if (executedJob->flag == 1)//对已分配内存的作业回收
		{
			//合并空闲分区
			MergingFreePartitions(free_table);

			//把空表目换到后面
			//首次适应算法按低高地址冒泡排序
			SortFreePartitions(free_table);

			if (executedJob->flag == 1)
			{
				random = rand() % 2;
				printf("\n产生的随机数为：%d\n", random);

				if (random == 0)
				{
					printf("该作业未执行完毕，不回收该作业内存\n");
					executedJob->next = NULL;
					EnQueue(jobQueue, executedJob);//重新挂入队列
				}
				else
				{
					printf("该作业执行完毕，回收该作业内存\n");
					for (int usedNum = 0; usedNum < USEDSIZE; usedNum++)//在已分配区表中找到所使用内存的起始地址和大小
					{
						if (used_table[usedNum].flag == 1)
						{
							if (executedJob->size == used_table[usedNum].length)//找到已分配表项
							{
								printf("\n【该作业在已分配区表项】\n");
								printf("已分配区表起始地址   已分配区表长度   已分配区表登记栏目状态\n");
								printf("——————————————————————————————\n");
								printf("%-20.2f% -17.2f %d\n", used_table[usedNum].address, used_table[usedNum].length, used_table[usedNum].flag);

								//修改空闲区表
								for (int freeNum = 0; freeNum < FREESIZE + 1; freeNum++)
								{
									if (free_table[freeNum].address + free_table[freeNum].length == used_table[usedNum].address)//回收内存
									{
										free_table[freeNum].length = free_table[freeNum].length + used_table[usedNum].length;

										//修改已分配区表
										used_table[usedNum].address = -1;
										used_table[usedNum].length = 0;
										used_table[usedNum].flag = 0;

										//内存回收完成的作业挂入执行完成链表
										executedJob->flag = 2;//表示作业已回收内存
										executedJob->next = NULL;
										InsertList(over, executedJob);
										break;
									}
									if (freeNum == FREESIZE)//若未找到可匹配回收的空闲区，则新添空闲表项
									{
										for (int i = 0; i < FREESIZE + 1; i++)
										{
											if (free_table[i].flag == 0)
											{
												free_table[i].address = used_table[usedNum].address;
												free_table[i].length = used_table[usedNum].length;
												free_table[i].flag = 1;

												//修改已分配区表
												used_table[usedNum].address = -1;
												used_table[usedNum].length = 0;
												used_table[usedNum].flag = 0;

												//内存回收完成的作业挂入执行完成链表
												executedJob->flag = 2;//表示作业已回收内存
												executedJob->next = NULL;
												InsertList(over, executedJob);
												break;
											}
											if (i == FREESIZE)//未找到可填入的空闲区表项
											{
												printf("\n空闲区表已满，该作业内存记录不可填入空闲区表\n");
												printf("不回收该作业内存，将该作业重新挂至作业队列\n");
												executedJob->next = NULL;
												EnQueue(jobQueue, executedJob);
												break;
											}
										}
									}
								} // 对每一个空闲区表项
							}// 找到已分配表项
						}//如果已分配区表项为1，表示已分配
					}//在已分配区表中找到所使用内存的起始地址和大小
				}//如果随机数是1
			}
		}//对已分配内存的作业回收
		else//不执行操作/未分配内存的作业要重新入队
		{
			executedJob->next = NULL;
			EnQueue(jobQueue,executedJob);
		}
		if (random == 0)
		{
			printf("\n【空闲区表】\n");
			PrintFreeTable(free_table);
			printf("\n【已分配区表】\n");
			PrintUsedTable(used_table);
			printf("\n【更新作业队列】\n");
			PrintQueue(jobQueue);
			printf("\n【完成作业链表】\n");
			PrintList(over);
		}
		else
		{
			printf("\n【更新空闲区表】\n");
			PrintFreeTable(free_table);
			printf("\n【更新已分配区表】\n");
			PrintUsedTable(used_table);
			printf("\n【更新作业队列】\n");
			PrintQueue(jobQueue);
			printf("\n【更新完成作业链表】\n");
			PrintList(over);
		}
	}//对每个作业
}

int main(void)
{
	//配置随机参数
	srand((unsigned)time(NULL));

	//初始化内存块
	Mb mb[10];//初始化内存块个数为10个
	float initialRule = 512;//初始化规则，循环变化 512KB
	float assignedSize = 0;//已初始化的内存大小

	mb[0].front = 0;
	mb[0].size = MEMORYSIZE / initialRule * 1024;//单位为字节
	strcpy_s(mb[0].state,"free");
	//strcpy(mb[0].state,"free");
	for (int i = 1; i < 10; i++)
	{
		if (i != 9)
		{	
			initialRule = initialRule / 2;
			mb[i].front = mb[i - 1].front + mb[i - 1].size;
			mb[i].size = MEMORYSIZE / initialRule * 1024;
			assignedSize = assignedSize + (MEMORYSIZE / initialRule);
			strcpy_s(mb[i].state, "free");
			//strcpy(mb[0].state,"free");
		}
		else
		{
			mb[i].front = mb[i - 1].front + mb[i - 1].size;
			mb[i].size = (MEMORYSIZE - assignedSize) * 1024;
			strcpy_s(mb[i].state,"free");
			//strcpy(mb[0].state,"free");
		}
	}

	printf("****************************************\n");
	printf("一.初始化\n");
	printf("【初始内存块】\n");
	printf("内存块起始地址   内存块大小   内存块状态\n");
	printf("————————————————————\n");
	for(int i = 0; i < 10; i++)
	{
		printf("%-16.2f% -13.2f %s\n",mb[i].front,mb[i].size,mb[i].state);
	}

	//初始化空闲区表
	FreeTable free_table[FREESIZE];
	for (int i = 0; i < FREESIZE; i++)
	{
		free_table[i].address = mb[i].front;
		free_table[i].length = mb[i].size;
		free_table[i].flag = 1;//表示内存块未分配
	}
	printf("\n【初始化空闲区表】\n");
	PrintFreeTable(free_table);
	
	//初始化已分配区表
	UsedTable used_table[USEDSIZE];
	for (int i = 0; i < USEDSIZE; i++)
	{
		used_table[i].address = -1;
		used_table[i].length = 0;
		used_table[i].flag = 0;//表示空栏目
	}
	printf("\n【初始化已分配区表】\n");
	PrintUsedTable(used_table);

	//初始化作业队列
	LinkQueue jobQueue;
	InitQueue(jobQueue);

	Job job[10];
	for (int i = 0; i < 10; i++)
	{
		job[i].id = i;
		job[i].size = rand() % (1024 * 1024);
		job[i].flag = 0;
		job[i].next = NULL;
		
		EnQueue(jobQueue,&job[i]);
	}
	printf("\n【初始化作业队列】\n");
	PrintQueue(jobQueue);

	//初始化执行完成作业链表
	LinkList over;
	InitList(over);
	printf("\n【初始化完成作业链表】\n");
	PrintList(over);
	printf("****************************************\n");

	int judgement = JudgeQueue(jobQueue);//判断作业是否都已分配到内存
	int executedTimes = 1;//执行轮数
	while (judgement != 1)
	{
		printf("第%d轮执行\n", executedTimes);
		//使用首次适应算法
		//对作业进行内存分配
		MemoryAllocation(jobQueue, free_table, used_table);

		//对已分配内存的作业回收内存
		printf("****************************************\n");
		printf("三.内存回收\n");

		MemoryRecovery(jobQueue, free_table, used_table, over);
		executedTimes++;
		judgement = JudgeQueue(jobQueue);
	}
	printf("****************************************\n");
	printf("全部作业都已分配到空间，程序执行完毕\n");

	return 0;
}