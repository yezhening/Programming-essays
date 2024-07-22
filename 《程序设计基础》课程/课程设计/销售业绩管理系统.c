#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 25
struct Company
	{
		int num;
		char name[10];
		char manager[10];
		float sales[3];
		float sum;
		int rank;
	};
int main()
{	
	struct Company message[SIZE];//定义在主函数从而使用指针变量p
	struct Company *p;
	p=message;
	void menu(struct Company *p);
	menu(p);
	return 0;
}

void menu(struct Company *p)
{	
	void input(struct Company *p);
	void save(struct Company *p);
	void show(struct Company *p);
	void inquiry(struct Company *p);
	void revise(struct Company *p);
	void add(struct Company *p);
	void del(struct Company *p);
	void sum(struct Company *p);
	void sort(struct Company *p);
	void rank(struct Company *p);
	void aver(struct Company *p);
	void amount(struct Company *p);
	void esc(struct Company *p);
	while(1)//死循环
	{
		printf("\n");
		printf("============欢迎使用销售业绩管理系统!==========\n");
		printf("录入信息-------------------------------------1\n");
		printf("保存信息-------------------------------------2\n");
		printf("显示信息-------------------------------------3\n");
		printf("查询信息-------------------------------------4\n");
		printf("修改信息-------------------------------------5\n");
		printf("添加信息-------------------------------------6\n");
		printf("删除信息-------------------------------------7\n");	
		printf("求某个分公司的三类产品累计年销售总额---------8\n");
		printf("求所有分公司的年销售总额并排序---------------9\n");
		printf("求某个分公司在年度销售总额中的名次-----------10\n");
		printf("求所有分公司的某类产品的平均销售额-----------11\n");
    	printf("对所有分公司的某类产品年度销售额排序---------12\n");
		printf("退出-----------------------------------------0\n");
		printf("==============================================\n");
		printf("请输入您的选择(若首次进入,请先选择1):");
		int choice;
		scanf("%d",&choice);
		if(choice==1)
			input(p);
		else if(choice==2)
			save(p);
		else if(choice==3)
			show(p);
		else if(choice==4)
			inquiry(p);
		else if(choice==5)
			revise(p);
		else if(choice==6)
			add(p);
		else if(choice==7)
			del(p);
		else if(choice==8)
			sum(p);
		else if(choice==9)
			sort(p);
		else if(choice==10)
			rank(p);
		else if(choice==11)
			aver(p);
		else if(choice==12)
			amount(p);
		else//考虑到可能输入其他字符，用if-else而不是switch语句
		{
			esc(p);
			break;//退出函数方法
		}
	}
}

	void input(struct Company *p)//录入信息	注意参数内容
	{
		int i,j=1;
		for(i=0;i<SIZE;i++)
		{
			(p+i)->num=0;//使用指针箭头形式
			strcpy((p+i)->name,"NULL");
			strcpy((p+i)->manager,"NULL");
			(p+i)->sales[0]=0;
			(p+i)->sales[1]=0;
			(p+i)->sales[2]=0;
		}
		printf("\n");
		printf("请输入每个分公司的各项信息:\n");
		for(i=0;i<SIZE;i++,j++)
		{
			printf("\n");
			printf("公司名字:");
			scanf("%s",(p+i)->name);
			printf("公司经理名字:");
			scanf("%s",(p+i)->manager);
			printf("服务类产品销售额:");
			scanf("%f",&(p+i)->sales[0]);//不是数组需要加入取地址符号
			printf("货物类产品销售额:");
			scanf("%f",&(p+i)->sales[1]);
			printf("工程类产品销售额:");
			scanf("%f",&(p+i)->sales[2]);
			(p+i)->num=j;
			printf("\n");
			printf("是否输入完毕?是请输入y,否请输入n:");
			char inputjudge;
			getchar();//必须存在，在上方
			scanf("%c",&inputjudge);
			if(inputjudge=='y')
			{
				printf("\n");
				printf("录入成功\n");
				break;
			}
		}	
	}

	void save(struct Company *p)//保存信息
	{
		FILE *fp;
		if((fp=fopen("com.txt","w"))==NULL)
		{
			printf("cannot open file!\n");
			exit(0);
		}
		int i;
		for(i=0;i<SIZE;i++)
		{
			if(fwrite((p+i),sizeof(struct Company),1,fp)!=1)//(p+i)即地址
			{
				printf("保存失败");
				fclose(fp);
			}
		}
		printf("\n");
		printf("保存成功\n");
		fclose(fp);
	}

	void show(struct Company *p)//显示信息
	{
		FILE *fp;
		if((fp=fopen("com.txt","r"))==NULL)
		{
			printf("cannot open file!\n");
			exit(0);
		}
		printf("\n");
		printf("文件打开成功\n");//可要可不要
		printf("\n");
		printf("********************************************************************************\n");
		printf("公司号	 公司名   公司经理  服务类产品销售额  货物类产品销售额  工程类产品销售额\n");
		printf("********************************************************************************\n");
		int i;
		for(i=0;i<SIZE;i++)
		{
			if(fread((p+i),sizeof(struct Company),1,fp)==1)
			{
				if(strcmp((p+i)->name,"NULL")!=0)
				{
					printf("%2d %12s %10s %17.2f %17.2f %17.2f\n",(p+i)->num,(p+i)->name,(p+i)->manager,(p+i)->sales[0],(p+i)->sales[1],(p+i)->sales[2]);
					printf("\n");
				}
			}
			else
			{
				printf("读取文件错误\n");
				fclose(fp);
				return;//退出函数
			}
		}
		fclose(fp);
		printf("\n");
		printf("显示成功\n");
	}


	void inquiry(struct Company *p)//查询信息
	{
		FILE *fp;
		if((fp=fopen("com.txt","r"))==NULL)
		{
			printf("cannot open file!\n");
			exit(0);
		}
		printf("文件打开成功\n");
		printf("\n");
		char inquiryname[20];
		printf("请输入公司名:");
		scanf("%s",inquiryname);
		int flag;//灵活运用标志
		flag=0;
		int i;
		for(i=0;i<SIZE;i++)
		{
			fread((p+i),sizeof(struct Company),1,fp);//直接读取
			if(strcmp(inquiryname,(p+i)->name)==0)
			{
				printf("\n");
				printf("查询公司信息如下:\n");
				printf("********************************************************************************\n");
				printf("公司号	 公司名   公司经理  服务类产品销售额  货物类产品销售额  工程类产品销售额\n");
				printf("********************************************************************************\n");
				printf("%2d %12s %10s %17.2f %17.2f %17.2f\n",(p+i)->num,(p+i)->name,(p+i)->manager,(p+i)->sales[0],(p+i)->sales[1],(p+i)->sales[2]);
				flag=1;
				break;
			}
		}
		if(flag==0)
		{
			printf("\n");
			printf("不存在该公司名\n");
		}
		fclose(fp);
	}

	void revise(struct Company *p)
	{
		FILE *fp;
		if((fp=fopen("com.txt","r"))==NULL)
		{
			printf("cannot open file!\n");
			exit(0);
		}
		printf("\n");
		printf("文件打开成功\n");
		printf("\n");
		char revisename[20];
		printf("请输入需要修改的公司名:");
		scanf("%s",revisename);
		int flag;
		flag=0;
		int i;
		for(i=0;i<SIZE;i++)
		{
			fread((p+i),sizeof(struct Company),1,fp);
			if(strcmp(revisename,(p+i)->name)==0)
			{
				printf("\n");
				printf("公司信息显示如下:\n");
				printf("********************************************************************************\n");
				printf("公司号	 公司名   公司经理  服务类产品销售额  货物类产品销售额  工程类产品销售额\n");
				printf("********************************************************************************\n");
				printf("%2d %12s %10s %17.2f %17.2f %17.2f\n",(p+i)->num,(p+i)->name,(p+i)->manager,(p+i)->sales[0],(p+i)->sales[1],(p+i)->sales[2]);
				flag=1;
			}
		}
		if(flag==0)
		{
			printf("\n");
			printf("不存在该公司名\n");
			return;//return可以跳出函数
		}
		printf("\n");
		printf("请问您需要修改哪项信息?\n");
		printf("===================================================================================================\n");
		printf("a表示公司名   b表示公司经理   c表示服务类产品销售额   d表示货物类产品销售额   e表示工程类产品销售额\n");
		printf("===================================================================================================\n");
		printf("请输入相应字符:");
		char amend;
		getchar();//必须有
		scanf("%c",&amend);
		char rname[20],rmanager[20];
		float rs0,rs1,rs2;
		if(amend=='a')
		{
			printf("\n");
			printf("请输入新信息:");
			scanf("%s",rname);
			getchar();//必须有
			char revisejudge;
			printf("\n");
			printf("确定要修改吗?确定请输入y,不确定请输入n返回菜单:");
			scanf("%c",&revisejudge);
			if(revisejudge=='y')
			{
				for(i=0;i<SIZE;i++)
				{
					if(strcmp(revisename,(p+i)->name)==0)
						strcpy((p+i)->name,rname);
				}
				save(p);//调用保存，提示保存成功
				printf("\n");
				printf("修改成功\n");
			}
			else
			{
				printf("\n");
				printf("未进行修改\n");
			}
		}
		else if(amend=='b')
		{
			printf("\n");
			printf("请输入新信息:");
			scanf("%s",rmanager);
			getchar();
			char revisejudge;//仍然要定义
			printf("\n");
			printf("确定要修改吗?确定请输入y,不确定请输入n返回菜单:");
			scanf("%c",&revisejudge);
			if(revisejudge=='y')
			{
				for(i=0;i<SIZE;i++)
				{
					if(strcmp(revisename,(p+i)->name)==0)
						strcpy((p+i)->manager,rmanager);
				}
				save(p);
				printf("\n");
				printf("修改成功\n");
			}
			else
			{
				printf("\n");
				printf("未进行修改\n");
			}
		}
		else if(amend=='c')
		{
			printf("\n");
			printf("请输入新信息:");
			scanf("%f",&rs0);
			getchar();
			char revisejudge;
			printf("\n");
			printf("确定要修改吗?确定请输入y,不确定请输入n返回菜单:");
			scanf("%c",&revisejudge);
			if(revisejudge=='y')
			{
				for(i=0;i<SIZE;i++)
				{
					if(strcmp(revisename,(p+i)->name)==0)
						(p+i)->sales[0]=rs0;
				}
				save(p);
				printf("\n");
				printf("修改成功\n");
			}
			else
			{
				printf("\n");
				printf("未进行修改\n");
			}
		}
		else if(amend=='d')
		{
			printf("\n");
			printf("请输入新信息:");
			scanf("%f",&rs1);
			getchar();
			char revisejudge;
			printf("\n");
			printf("确定要修改吗?确定请输入y,不确定请输入n返回菜单:");
			scanf("%c",&revisejudge);
			if(revisejudge=='y')
			{
				for(i=0;i<SIZE;i++)
				{
					if(strcmp(revisename,(p+i)->name)==0)
						(p+i)->sales[1]=rs1;
				}
				save(p);
				printf("\n");
				printf("修改成功\n");
			}
			else
			{
				printf("\n");
				printf("未进行修改\n");
			}
		}
		else
		{
			printf("\n");
			printf("请输入新信息:");
			scanf("%f",&rs2);
			getchar();
			char revisejudge;
			printf("\n");
			printf("确定要修改吗?确定请输入y,不确定请输入n返回菜单:");
			scanf("%c",&revisejudge);
			if(revisejudge=='y')
			{
				for(i=0;i<SIZE;i++)
				{
					if(strcmp(revisename,(p+i)->name)==0)
						(p+i)->sales[2]=rs2;
				}
				save(p);
				printf("\n");
				printf("修改成功\n");
			}
			else
			{
				printf("\n");
				printf("未进行修改\n");
			}
		}
		fclose(fp);
	}

	void add(struct Company *p)
	{
		FILE *fp;
		if((fp=fopen("com.txt","w"))==NULL)
		{
			printf("cannot open file!\n");
			exit(0);
		}
		printf("文件打开成功\n");
		printf("\n");
		char aname[20],amanager[20];
		float asales[3];
		printf("请输入需要添加的信息:\n");
		printf("公司名字:");
		scanf("%s",aname);
		printf("公司经理名字:");
		scanf("%s",amanager);
		printf("服务类产品销售额:");
		scanf("%f",&asales[0]);
		printf("货物类产品销售额:");
		scanf("%f",&asales[1]);
		printf("工程类产品销售额:");
		scanf("%f",&asales[2]);
		printf("\n");
		printf("确定要添加吗？确定请输入y,不确定请输入n返回菜单:");
		char addjudge;
		getchar();
		scanf("%c",&addjudge);
		int i;
		if(addjudge=='y')
		{
			for(i=0;i<SIZE;i++)
			{
				if(strcmp((p+i)->name,"NULL")==0)
				{
					strcpy((p+i)->name,aname);
					strcpy((p+i)->manager,amanager);
					(p+i)->sales[0]=asales[0];
					(p+i)->sales[1]=asales[1];
					(p+i)->sales[2]=asales[2];
					if((p+i)->num==0)
						(p+i)->num=i+1;
					break;//添加完跳出添加循环
				}
			}
			save(p);//添加后保存
			printf("\n");
			printf("添加成功\n");
		}
		else
			printf("未进行添加\n");
		fclose(fp);
	}

	void del(struct Company *p)
	{
		FILE *fp;
		if((fp=fopen("com.txt","w"))==NULL)
		{
			printf("cannot open file!\n");
			exit(0);
		}
		printf("\n");
		printf("文件打开成功\n");
		printf("\n");
		char dname[20];
		printf("请输入需要删除信息的公司名:");
		scanf("%s",dname);
		int flag;
		flag=0;
		int i;
		for(i=0;i<SIZE;i++)
		{
			fread((p+i),sizeof(struct Company),1,fp);
			if(strcmp(dname,(p+i)->name)==0)
			{
				printf("\n");
				printf("公司信息显示如下:\n");
				printf("********************************************************************************\n");
				printf("公司号	 公司名   公司经理  服务类产品销售额  货物类产品销售额  工程类产品销售额\n");
				printf("********************************************************************************\n");
				printf("%2d %12s %10s %17.2f %17.2f %17.2f\n",(p+i)->num,(p+i)->name,(p+i)->manager,(p+i)->sales[0],(p+i)->sales[1],(p+i)->sales[2]);
				flag=1;
			}
		}
		if(flag==0)
		{
			printf("\n");
			printf("不存在该公司名\n");
			return;
		}
		printf("\n");
		printf("确定要删除吗?确定请输入y,不确定请输入n返回菜单:");
		char deljudge;
		getchar();//奇怪	习以为常
		scanf("%c",&deljudge);
		if(deljudge=='y')
		{	
			for(i=0;i<SIZE;i++)//重新将i初始化,无需定义新的循环变量
			{
				fread((p+i),sizeof(struct Company),1,fp);
				if(strcmp(dname,(p+i)->name)==0)
				{
						strcpy((p+i)->name,"NULL");
						strcpy((p+i)->manager,"NULL");
						(p+i)->sales[0]=0;
						(p+i)->sales[1]=0;
						(p+i)->sales[2]=0;	
				}	
			}
			save(p);
			printf("\n");
			printf("删除成功\n");
		}
		else
			{
				printf("\n");
				printf("未进行删除\n");
			}
		fclose(fp);	
	}

	void sum(struct Company *p)//求某个分公司的三类产品累计年销售总额
	{
		FILE *fp;
		if((fp=fopen("com.txt","r"))==NULL)
		{
			printf("cannot open file!\n");
			exit(0);
		}
		printf("文件打开成功\n");
		printf("\n");
		char sname[20];
		printf("请输入公司名:");
		scanf("%s",sname);
		printf("\n");
		int flag;
		flag=0;
		float ssum;
		int i;
		for(i=0;i<SIZE;i++)
		{
			fread((p+i),sizeof(struct Company),1,fp);
			if(strcmp(sname,(p+i)->name)==0)
			{
				ssum=(p+i)->sales[0]+(p+i)->sales[1]+(p+i)->sales[2];
				printf("该分公司的三类产品年销售总额为:");
				printf("%0.2f\n",ssum);
				flag=1;
			}
		}
		if(flag==0)
		{
			printf("不存在该公司名\n");
			return;
		}
		fclose(fp);
	}

	void sort(struct Company *p)//求所有分公司的年销售总额并排序
	{
		FILE *fp;
		if((fp=fopen("com.txt","r"))==NULL)
		{
			printf("cannot open file!\n");
			exit(0);
		}
		printf("\n");
		printf("文件打开成功\n");
		int i;
		for(i=0;i<SIZE;i++)
		{
			fread((p+i),sizeof(struct Company),1,fp);
			(p+i)->sum=(p+i)->sales[0]+(p+i)->sales[1]+(p+i)->sales[2];
		}
		printf("\n");
		printf("所有分公司年度销售总额计算成功\n");
		save(p);//保存后再次显示所有公司时会按排序后的输出
		printf("\n");
		int j,k;
		struct Company temp;//结构体中间变量直接交换所有成员
		for(i=0;i<SIZE-1;i++)//选择排序
		{
			k=i;
			for(j=i+1;j<SIZE;j++)
				if((p+j)->sum>(p+k)->sum)
				{
					k=j;
					temp=*(p+k);
					*(p+k)=*(p+i);
					*(p+i)=temp;
				}
		}
		printf("所有分公司年度销售总额排序成功\n");
		printf("\n");
		printf("年度销售总额排序由多到少为:\n");
		printf("\n");
		printf("公司信息显示如下:\n");
		printf("**************************************************************************************************\n");
		printf("公司号	 公司名   公司经理  服务类产品销售额  货物类产品销售额  工程类产品销售额  产品年度销售总额\n");
		printf("**************************************************************************************************\n");
		for(i=0;i<SIZE;i++)
		{	
			if(strcmp((p+i)->name,"NULL")!=0)
			{
				printf("%2d %12s %10s %17.2f %17.2f %17.2f %17.2f\n",(p+i)->num,(p+i)->name,(p+i)->manager,(p+i)->sales[0],(p+i)->sales[1],(p+i)->sales[2],(p+i)->sum);
				printf("\n");
			}
		}
		fclose(fp);
	}

	void rank(struct Company *p)//求某个分公司在年度销售总额中的名次
	{
		FILE *fp;
		if((fp=fopen("com.txt","r"))==NULL)
		{
			printf("cannot open file!\n");
			exit(0);
		}
		printf("\n");
		printf("文件打开成功\n");
		int i;
		for(i=0;i<SIZE;i++)
		{
			fread((p+i),sizeof(struct Company),1,fp);
			(p+i)->sum=(p+i)->sales[0]+(p+i)->sales[1]+(p+i)->sales[2];
		}
		printf("\n");
		printf("所有分公司年度销售总额计算成功\n");
		printf("\n");
		int j,k;
		struct Company temp;
		for(i=0;i<SIZE-1;i++)
		{
			k=i;
			for(j=i+1;j<SIZE;j++)
				if((p+j)->sum>(p+k)->sum)
				{
					k=j;
					temp=*(p+k);
					*(p+k)=*(p+i);
					*(p+i)=temp;
				}
		}
		printf("所有分公司年度销售总额排序成功\n");
		printf("\n");
		int order;
		for(i=0,order=1;i<SIZE;i++,order++)
		{
			(p+i)->rank=order;
		}
		printf("所有分公司年度销售总额排名成功\n");
		save(p);
		char rname[20];
		printf("\n");
		printf("请输入公司名:");
		scanf("%s",rname);
		printf("\n");
		int flag;
		flag=0;
		for(i=0;i<SIZE;i++)
		{
			if(strcmp(rname,(p+i)->name)==0)
			{
				printf("该分公司在年度销售总额中的名次是:");
				printf("%d\n",(p+i)->rank);
				flag=1;
				break;
			}
		}
		if(flag==0)
		{
			printf("不存在该公司名\n");
			return ;
		}
		fclose(fp);
	}

	void aver(struct Company *p)//求所有分公司的某类产品的平均销售额
	{
		FILE *fp;
		if((fp=fopen("com.txt","r"))==NULL)
		{
			printf("cannot open file!\n");
			exit(0);
		}
		printf("\n");
		printf("文件打开成功\n");
		printf("\n");
		char aname;
		int i;
		float asum=0;
		printf("请输入产品名(输入字母表示:s为服务类产品,g为货物类产品,p为工程类产品):");
		getchar();
		scanf("%c",&aname);
		if(aname=='s')
		{
			for(i=0;i<SIZE;i++)
			{
				fread((p+i),sizeof(struct Company),1,fp);
				if(strcmp((p+i)->name,"NULL")!=0)
				{
					asum=asum+(p+i)->sales[0];
				}
			}
		}
		else if(aname=='g')
		{
			for(i=0;i<SIZE;i++)
			{
				fread((p+i),sizeof(struct Company),1,fp);
				if(strcmp((p+i)->name,"NULL")!=0)
				{
					asum=asum+(p+i)->sales[1];
				}
			}
		}	
		else
		{
			for(i=0;i<SIZE;i++)
			{
				fread((p+i),sizeof(struct Company),1,fp);
				if(strcmp((p+i)->name,"NULL")!=0)
				{
					asum=asum+(p+i)->sales[2];
				}
			}
		}
		float count=0,aver;//开count计数求平均值
		for(i=0;i<SIZE;i++)
		{
			fread((p+i),sizeof(struct Company),1,fp);
			if(strcmp((p+i)->name,"NULL")!=0)
			{
				count++;
			}
		}
		aver=asum/count;
		printf("\n");
		printf("该类产品的平均销售额为:");
		printf("%0.2f\n",aver);
		fclose(fp);
	}

	void amount(struct Company *p)//对所有分公司的某类产品年度销售额排序
	{
		FILE *fp;
		if((fp=fopen("com.txt","r"))==NULL)
		{
			printf("cannot open file!\n");
			exit(0);
		}
		printf("\n");
		printf("文件打开成功\n");
		printf("\n");
		char aname;
		printf("请输入产品类别(输入字母表示:s为服务类产品,g为货物类产品,p为工程类产品):");
		getchar();
		scanf("%c",&aname);
		printf("\n");
		int i,j,k;
		struct Company temp;
		if(aname=='s')
		{
			for(i=0;i<SIZE;i++)
			{
				fread((p+i),sizeof(struct Company),1,fp);
				for(i=0;i<SIZE-1;i++)
				{
					k=i;
					for(j=i+1;j<SIZE;j++)
					{
						if((p+j)->sales[0]>(p+k)->sales[0])
						{
							k=j;
							temp=*(p+k);//整个结构体调换
							*(p+k)=*(p+i);
							*(p+i)=temp;
						}
					}
				}
			}
			printf("所有分公司服务类产品年度销售总额排序成功\n");
			printf("\n");
			printf("所有分公司服务类产品销售总额由大到小排序为:\n");
			printf("\n");
			printf("********************************************\n");
			printf("公司号	 公司名   公司经理  服务类产品销售额\n");
			printf("********************************************\n");
			for(i=0;i<SIZE;i++)//排序和输出循环虽分开但显得好看
			{
				if(strcmp((p+i)->name,"NULL")!=0)
				{
					printf("%2d %12s %10s %17.2f\n",(p+i)->num,(p+i)->name,(p+i)->manager,(p+i)->sales[0]);
				}	
			}
		}
		else if(aname=='g')
		{
			for(i=0;i<SIZE;i++)
			{
				fread((p+i),sizeof(struct Company),1,fp);
				for(i=0;i<SIZE-1;i++)
				{
					k=i;
					for(j=i+1;j<SIZE;j++)
					{
						if((p+j)->sales[1]>(p+k)->sales[1])
						{
							k=j;
							temp=*(p+k);
							*(p+k)=*(p+i);
							*(p+i)=temp;
						}
					}
				}
			}
			printf("所有分公司服务类产品年度销售总额排序成功\n");
			printf("\n");
			printf("所有分公司服务类产品销售总额由大到小排序为:\n");
			printf("\n");
			printf("********************************************\n");
			printf("公司号	 公司名   公司经理  货物类产品销售额\n");
			printf("********************************************\n");
			for(i=0;i<SIZE;i++)
			{
				if(strcmp((p+i)->name,"NULL")!=0)
				{
					printf("%2d %12s %10s %17.2f\n",(p+i)->num,(p+i)->name,(p+i)->manager,(p+i)->sales[1]);
				}	
			}
		}
		else
		{
			for(i=0;i<SIZE;i++)
			{
				fread((p+i),sizeof(struct Company),1,fp);
				for(i=0;i<SIZE-1;i++)
				{
					k=i;
					for(j=i+1;j<SIZE;j++)
					{
						if((p+j)->sales[2]>(p+k)->sales[2])
						{
							k=j;
							temp=*(p+k);
							*(p+k)=*(p+i);
							*(p+i)=temp;
						}
					}
				}
			}
			printf("所有分公司服务类产品年度销售总额排序成功\n");
			printf("\n");
			printf("所有分公司服务类产品销售总额由大到小排序为:\n");
			printf("\n");
			printf("********************************************\n");
			printf("公司号	 公司名   公司经理  工程类产品销售额\n");
			printf("********************************************\n");
			for(i=0;i<SIZE;i++)
			{
				if(strcmp((p+i)->name,"NULL")!=0)
				{
					printf("%2d %12s %10s %17.2f\n",(p+i)->num,(p+i)->name,(p+i)->manager,(p+i)->sales[2]);
				}	
			}
		}
		fclose(fp);
	}
	
	void esc(struct Company *p)
	{
		printf("\n");
		printf("确定要退出吗? 确定请输入y,不确定请输入n返回菜单:");
		char escjudge;
		void menu(struct Company *p);
		getchar();//这个放在scanf上面，否则扫描不进去
		scanf("%c",&escjudge);
		if(escjudge=='y')
		{
			exit(0);//都是因为上面那个getchar!让我花费一个多小时想方法	退出进程
		}
		else
			menu(p);
	}