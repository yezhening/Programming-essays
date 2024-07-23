//头文件————————————————————
#include<stdio.h>//remove()
#include<stdlib.h>//malloc()、system();
#include<string.h>//strcpy_s()、memset()、strcmp()、strcat_s()、strlen()、strncpy_s()、strtok_s()

//宏————————————————————
#define ZERO 0//空文件
#define DIRECTORY 1//目录文件
#define GENERAL 2 //普通文件

//常量————————————————————
const int blockSize = 512;//虚拟磁盘块大小，单位：字节	Fat12文件系统一个扇区512B->一个磁盘块是一个扇区：512=515*2**0
const int blockCount = 128;//虚拟磁盘块数量	Fat12文件系统有2880个扇区,内存溢出->这里给128个
const int diskSize = blockCount * blockSize;//虚拟磁盘空间大小，共1474560B，1.44MB->65536B,	64KB
const char* diskName = "E:\\FileSystem"; //生成虚拟磁盘文件在实际磁盘中的名字	char类型指针
const int maxOpenFileNum = 5;//用户最大打开文件数量，默认为5

//全局变量————————————————————
char* baseAddress;//虚拟磁盘空间基地址
struct Disk* osPointer;//磁盘操作系统指针	全局变量可任意位置定义
FILE* fp;//文件指针，指向虚拟磁盘文件在实际磁盘中的地址
int currentDirectoryDiskNum = 19;//当前目录的磁盘块号,默认为根目录区
char currentPathShow[50] = "E:\\"; //当前路径显示，默认装50个字符
struct UserOpenFileList* openListPointer;//用户打开文件表指针
char command[10];//输入指令,默认装10个字符
char content[10];//指令后的内容，默认装10个字符
char userDiskName[13] = "E:\\"; //生成用户虚拟磁盘文件在实际磁盘中的名字	字符数3+10
FILE* userFp;//用户文件指针，指向用户虚拟磁盘文件在实际磁盘中的地址
int userDiskNum;//用户登录时记录用户目录文件的磁盘块号，用于查找文件
char user[10];//记录用户登录名，cd退出时需回退打开用户文件路径
int openFileCount = 1; //用户打开文件表打开文件数目	1存到5
struct directoryFile* directoryPointer;//目录指针
char findPath[50] = "E:\\";//查找路径，默认五十个字符
char findPaths[5][50];//记录所有查找文件的路径，默认五个
char findCount = 0;//找到的文件路径计数

//数据结构————————————————————
//文件控制块
struct Fcb
{
	//变量
	char name[10];//文件名，默认十个字符
	char type;//文件类型
	int fatherBlockNum;//文件的父目录的虚拟磁盘块号
	int currentBlockNum;//文件的当前目录的虚拟磁盘块号

	//方法
	//初始化文件控制块
	void init()
	{
		strcpy_s(name, "");
		type = ZERO;
		fatherBlockNum = -1;//使文件控制块的父、当前块号都为未知块号
		currentBlockNum = -1;
	}
};

//常量————————————————————
//fcbCount = 25，除去文件自身占0号块，剩余能存放24个文件
const int fcbCount = blockSize / sizeof(Fcb); //目录文件的最多的文件/Fcb数	默认一个目录文件大小最多为512B，占一个磁盘块	需要Fcb信息放在此位置

//目录文件
struct directoryFile
{
	//变量
	struct Fcb fcb[fcbCount];//一个目录文件所有的文件/文件控制块数量

	//方法
	//初始化目录文件
	void init(int fatherBlockNum, int currentBlockNum, const char* name) //父块号，当前块号，目录名
	{
		//目录文件有多个文件/文件控制块，第一个文件控制块记录自身的信息
		strcpy_s(fcb[0].name, name);
		fcb[0].type = DIRECTORY; //类型是目录文件
		fcb[0].fatherBlockNum = fatherBlockNum;
		fcb[0].currentBlockNum = currentBlockNum;

		//其他的文件控制块
		for (int i = 1; i < fcbCount; i++)
		{
			strcpy_s(fcb[i].name, "");
			fcb[i].type = 0;//类型是空文件
			fcb[i].fatherBlockNum = currentBlockNum; //目录中的文件的父块号是即为当前目录块号	
			fcb[i].currentBlockNum = -1;//仍为未知
		}
	}
};

//虚拟磁盘装载的文件系统：Fat12
struct Disk
{
	//变量
	//MBR引导记录扇区,512B,占用0扇区，一个磁盘块	未使用
	int fat1[blockCount]; //fat1区，记录磁块占用信息	占用1-9扇区，9个磁盘块	int类型一维数组
	int fat2[blockCount]; //fat2区，备份	占用10-18扇区，9个磁盘块
	struct directoryFile root;//根目录区	struct类型,根目录是一个目录文件	分配19-32个扇区，14个磁盘块
	char data[blockCount - 33][blockSize];//数据区 char类型二维数组	虚拟磁盘块数为原有的-MBR区-fat1区-fat2区-根目录区	将其转换为0开始的二维数组

	//方法
	//初始化/格式化磁盘
	void format()
	{
		//在fat区中记录文件信息
		for (int i = 0; i < 33; i++)
		{
			fat1[i] = -1; //0-32盘块号依次代表MBR，fat1，fat2，根目录区	-1表示不可用
			fat2[i] = -1;//fat2为备份区
		}

		for (int i = 33; i < blockCount; i++)
		{
			fat1[i] = 0; //fat1	填充整型值0,表示空表项
			fat2[i] = 0;//fat2
		}

		root.init(19, 19, "E:\\");//根目录区	父、当前目录的磁盘块号都为自身19，目录名在E://	调用结构体方法
		memset(data, 0, sizeof(data));//数据区	填充整型值0，表示空内容
	}
};

//用户打开文件表：管理打开的文件，只有打开的文件才能进行读写操作
struct UserOpenFileList//结构体类比类
{
	//成员变量
	int openFileNum = 0;//已打开文件数目	从0计数
	struct Fcb fcb[maxOpenFileNum]; //记录打开文件的文件控制块信息->用于管理	只存放五个文件控制块	从1计数

	//构造函数	创建时初始化表
	UserOpenFileList()
	{
		for (int i = 0; i < maxOpenFileNum; i++)
		{
			strcpy_s(fcb[i].name, "");
			fcb[i].type = GENERAL;
			fcb[i].fatherBlockNum = -1; //用于记录打开文件的父块号与当前操作块号的比较->判断是否已在打开表中
			fcb[i].currentBlockNum = -1;//无需使用该变量
		}
	}
};

//函数————————————————————
//函数声明
//命令函数
int format();
int logon(char* userName);//register是关键字
int login(char* userName);
int logout(char* userName);
int dir();
int cd(char* directoryName);
int mkdir(char* directoryName);
int rmdir(char* directoryName);
int create(char* fileName);
int del(char* fileName);//delete是关键字
int open(char* fileName);
int close(char* fileName);
int read(char* fileName);
int write(char* fileName);
int move(char* directoryPath, char* fileName);
int copy(char* directoryPath, char* fileName);
int find(char* name);
int head(int* num, char* fineName);
int tail(int* num, char* fileName);
int import(char* filePath);
int exp(char* fileName, char* filePath);//export是关键字

//系统函数
void startFileSystem();
void FirstLevel();
void SecondLevel();

//辅助函数
void DFS(int currentBlockNum, char* fileName);//查找文件时树的递归深度搜索

//磁盘初始化/格式化
int format()
{
	currentDirectoryDiskNum = 19;//当前目录的磁盘块号，默认为根目录区
	strcpy_s(currentPathShow, "E:\\"); //当前路径显示

	osPointer->format(); //操作系统指针、虚拟磁盘（数据结构）格式化	调用结构体函数

	delete openListPointer;//用户打开文件表初始化：删除后创建
	openListPointer = new UserOpenFileList();

	if (fopen_s(&fp, diskName, "w+") == 0)//成功创建可读写文件
	{
		if (fp != 0)//符合fwrite()、fclose规范
		{
			fwrite(baseAddress, sizeof(char), diskSize, fp);//创建并写文件	重写为空
			fclose(fp);
			printf("虚拟磁盘格式化成功。\n");
		}
	}

	return 1;
}

//用户注册
int logon(char* userName)
{
	struct directoryFile* currentDirectory;//当前目录指针

	currentDirectory = &(osPointer->root);//用户目录为第一级用户，只在根目录区下创建	表示法

	//在根目录下判断是否有同名用户/用户目录
	int i;
	for (i = 1; i < fcbCount; i++)//查询根目录下的其他文件/文件控制块
	{
		if ((currentDirectory->fcb[i].type == DIRECTORY) && (strcmp(currentDirectory->fcb[i].name, userName) == 0))//如果是目录文件且名字相同	逻辑
		{
			printf("已存在同名用户。\n");
			return 0;
		}
	}

	//判断是否目录已满：已经占有fcbCount的文件数量
	for (i = 1; i < fcbCount; i++)//查找空白fcb序号
	{
		if (currentDirectory->fcb[i].type == ZERO)//查找到
			break;
	}

	if (i == fcbCount)//未查找到
	{
		printf("根目录已满，无法注册用户。\n");
		return 0;
	}

	//判断虚拟磁盘是否已满：查fat文件分配表的磁盘块数
	int fcbI = i;//记录根目录下能存放用户目录的是第几个文件控制块
	for (i = 33; i < blockCount; i++)//从序号为33块磁盘查找	从0号开始，查找到127号
	{
		if (osPointer->fat1[i] == 0)//查找到未分配空表项
			break;
	}
	if (i == blockCount)//未查找到
	{
		printf("虚拟磁盘已满!\n");
		return 0;
	}

	//综上三个条件，未退出函数则满足注册用户/用户目录条件
	int fatI = i;//记录填充进fat区的磁盘块号

	//分配表项、磁盘块
	//虚拟磁盘空间的fat区记录用户目录文件
	osPointer->fat1[fatI] = osPointer->fat2[fatI] = 1; //1代表不为空表项

	//根目录区记录用户目录文件
	strcpy_s(currentDirectory->fcb[fcbI].name, userName);
	currentDirectory->fcb[fcbI].type = DIRECTORY;
	currentDirectory->fcb[fcbI].fatherBlockNum = currentDirectoryDiskNum;//当前目录即为用户目录的父目录，记录磁盘块号	默认为根目录区磁盘块号
	currentDirectory->fcb[fcbI].currentBlockNum = fatI;//目录文件的当前目录的磁盘块号就是fat区分配的磁盘块号

	//数据区初始化用户目录文件磁盘块
	currentDirectory = (struct directoryFile*)(osPointer->data[fatI - 33]); //在数据区定位到用户目录磁盘块号
	currentDirectory->init(currentDirectoryDiskNum, fatI, userName);//目录文件初始化
	printf("用户注册成功。\n");

	//保存文件
	//虚拟磁盘驱动器
	if (fopen_s(&fp, diskName, "w+") == 0)//"w+"：创建一个用于读写的空文件
	{
		//写文件
		if (fp != 0)//符合fwrite()、fclose规范
		{
			fwrite(baseAddress, sizeof(char), diskSize, fp);
			fclose(fp);
		}
	}

	//创建用户磁盘文件
	strcat_s(userDiskName, userName);//完善用户虚拟磁盘文件在实际磁盘中的名字
	if (fopen_s(&userFp, userDiskName, "w+") == 0)//"w+"：创建一个用于读写的空文件
	{
		//写文件
		if (userFp != 0)//符合fwrite()、fclose规范
		{
			fwrite(baseAddress, sizeof(char), diskSize, userFp);
			fclose(userFp);
		}
	}

	//注册完后将用户磁盘名回退，因为在登录时会再次完善用户磁盘名
	strncpy_s(userDiskName, userDiskName, strlen(userDiskName) - strlen(userName));

	return 1;
}

//用户登录
int login(char* userName)
{
	//用户登录时创建用户打开文件表
	openListPointer = new UserOpenFileList;//结构体类比类	创建对象

	struct directoryFile* currentDirectory;//当前目录指针

	currentDirectory = &(osPointer->root);//用户目录为第一级用户，只在根目录区下查找用户目录文件

	//判断当前目录/根目录下有用户目录
	int i, fcbI;
	for (i = 1; i < fcbCount; i++)//0记录根目录文件自身信息，从1开始找用户目录文件
	{
		if (currentDirectory->fcb[i].type == DIRECTORY && strcmp(currentDirectory->fcb[i].name, userName) == 0)//找到
		{
			fcbI = i;
			break;
		}
	}

	if (i == fcbCount)//未查找到
	{
		printf("不存在该用户。\n");
		return 0;
	}

	//修改当前目录文件信息
	currentDirectoryDiskNum = currentDirectory->fcb[fcbI].currentBlockNum;//当前目录磁盘块号赋值为用户目录文件当前的磁盘块号，后续在该目录下的操作会使用到该信息
	strcat_s(currentPathShow, currentDirectory->fcb[fcbI].name);//修改当前路径显示
	strcat_s(currentPathShow, "\\");

	//用户登录时记录用户目录文件的磁盘块号，用于查找文件
	userDiskNum = currentDirectoryDiskNum;

	printf("用户登录成功。\n\n");

	//记录用户登录名，cd退出时需回退打开用户文件路径
	strcpy_s(user, userName);

	//加载用户虚拟磁盘
	//读用户虚拟磁盘实际文件
	//完善用户实际磁盘文件名
	strcat_s(userDiskName, userName);

	//函数未退出，则用户存在，用户文件存在，直接打开即可
	if (fopen_s(&userFp, userDiskName, "r") == 0)
	{
		//读文件
		if (userFp != 0)//符合fread()、fclose规范
		{
			fread(baseAddress, sizeof(char), diskSize, userFp);
			fclose(userFp);

			printf("已加载用户磁盘文件(%s)。\n\n", userDiskName);

			//进入二级文件系统
			SecondLevel();
		}
	}
	else
	{
		printf("\n加载用户磁盘文件(%s)失败。\n", userDiskName);

		return 0;
	}

	return 1;
}

//用户注销
int logout(char* userName)
{
	struct directoryFile* currentDirectory;//当前目录指针

	//定位指针
	currentDirectory = &(osPointer->root);//在根目录区

	//查找用户目录文件
	int i;
	for (i = 1; i < fcbCount; i++)
	{
		if (currentDirectory->fcb[i].type == DIRECTORY && strcmp(currentDirectory->fcb[i].name, userName) == 0)//查找到
		{
			break;
		}
	}

	if (i == fcbCount)//未查找到
	{
		printf("不存在该用户。\n");
		return 0;
	}

	//定位到查找子目录的磁盘块号，不管用户下是否有文件信息（是否为空）
	int fcbI = currentDirectory->fcb[i].currentBlockNum;//i为查找到记录目录文件控制块序号

	//注销用户，二、三步骤和注册相反顺序
	//fat记录磁盘块号置空表项
	osPointer->fat1[fcbI] = osPointer->fat2[fcbI] = 0;

	//数据区格式化用户目录
	char* p = osPointer->data[fcbI - 33];//使用一个指针变量指向地址
	memset(p, 0, blockSize);//填充值0，表示无内容

	//当前目录的文件控制块记录信息清空
	currentDirectory->fcb[i].init();

	//保存文件
	//虚拟磁盘驱动器
	if (fopen_s(&fp, diskName, "w+") == 0)//"w+"：创建一个用于读写的空文件
	{
		//写文件
		if (fp != 0)//符合fwrite()、fclose规范
		{
			fwrite(baseAddress, sizeof(char), diskSize, fp);
			fclose(fp);
		}
	}

	//删除用户虚拟磁盘实际文件
	//完善用户实际磁盘文件名
	strcat_s(userDiskName, userName);

	if (remove(userDiskName) == 0)
	{
		printf("注销用户%s成功。\n", userName);
	}

	return 1;
}

//列文件目录
int dir()
{
	struct directoryFile* currentDirectory;//当前目录指针

	//定位目录
	if (currentDirectoryDiskNum == 19)//根目录
		currentDirectory = &(osPointer->root);
	else//数据区
		currentDirectory = (struct directoryFile*)(osPointer->data[currentDirectoryDiskNum - 33]);

	//显示
	printf("%-15s父目录\n", "..");//父目录
	printf("%-15s当前目录\n", ".");//当前目录

	//定位文件
	int  i, directoryFileCount = 0, generalFileCount = 0;
	for (i = 1; i < fcbCount; i++)
	{
		//查找目录文件
		if (currentDirectory->fcb[i].type == DIRECTORY)
		{
			directoryFileCount++;
			printf("%-15s文件夹\n", currentDirectory->fcb[i].name);//默认文件名最多十个字符 char content[10];
		}
		if (currentDirectory->fcb[i].type == GENERAL)
		{ //查找普通文件
			generalFileCount++;
			printf("%-15s文本文件\n", currentDirectory->fcb[i].name);
		}
	}

	printf("\n该目录下共有%d个文件夹，%d个文本文件。\n", directoryFileCount, generalFileCount);
	return 1;
}

//进入目录
int cd(char* directoryName)
{
	struct directoryFile* currentDirectory;//当前目录指针

	//进入用户目录文件时已修改当前目录的磁盘块号，直接去数据区查找当前目录文件
	currentDirectory = (struct directoryFile*)(osPointer->data[currentDirectoryDiskNum - 33]);

	//回到根目录/用户退出
	if (strcmp(directoryName, "\\") == 0)//字符转义
	{
		currentDirectoryDiskNum = 19;//修改当前磁盘块号
		strcpy_s(currentPathShow, "E:\\");//修改当前路径显示

		//用户退出将用户磁盘名回退，因为在登录时会再次完善用户磁盘名
		strncpy_s(userDiskName, userDiskName, strlen(userDiskName) - strlen(user));

		//退出后加载虚拟磁盘文件
		if ((fopen_s(&fp, diskName, "r")) == 0)
		{
			if ((baseAddress != 0) && (fp != 0))//fread()规范
			{
				fread(baseAddress, sizeof(char), diskSize, fp);
				fclose(fp);//必须先关闭，后保存文件时再指定模式重新打开
				printf("\n已加载磁盘文件(%s)。\n\n", diskName);

				//进入一级文件系统
				FirstLevel();
			}
		}
		else//一般进不到该选择项
		{
			printf("\n加载磁盘文件(%s)失败。\n", diskName);

			return 0;
		}
	}

	//回到父目录/用户退出
	if (strcmp(directoryName, "..") == 0)
	{
		currentDirectoryDiskNum = currentDirectory->fcb[0].fatherBlockNum;//同理，修改当前磁盘块号

		if (currentDirectoryDiskNum == 19)//如果退回的是根目录跳转
		{
			strcpy_s(currentPathShow, "E:\\");//修改当前路径显示

			//用户退出将用户磁盘名回退，因为在登录时会再次完善用户磁盘名
			strncpy_s(userDiskName, userDiskName, strlen(userDiskName) - strlen(user));

			//退出后加载虚拟磁盘文件
			if ((fopen_s(&fp, diskName, "r")) == 0)
			{
				if ((baseAddress != 0) && (fp != 0))//fread()规范
				{
					fread(baseAddress, sizeof(char), diskSize, fp);
					fclose(fp);//必须先关闭，后保存文件时再指定模式重新打开
					printf("\n已加载磁盘文件(%s)。\n\n", diskName);

					//进入一级文件系统
					FirstLevel();
				}
			}
			else
			{
				printf("\n加载磁盘文件(%s)失败。\n", diskName);

				return 0;
			}
		}
		else//如果退回的不是根目录
		{
			strncpy_s(currentPathShow, currentPathShow, strlen(currentPathShow) - strlen(currentDirectory->fcb[0].name) - 1);//修改当前路径显示 多减去一个字符"\"
		}

		return 1;
	}

	//查找子目录：在该目录下查找文件控制块
	int i, fcbI;
	for (i = 1; i < fcbCount; i++)
	{
		//查找该文件
		if (currentDirectory->fcb[i].type == DIRECTORY && strcmp(currentDirectory->fcb[i].name, directoryName) == 0)
		{
			fcbI = i;
			break;
		}
	}

	if (i == fcbCount)
	{
		printf("不存在该目录。\n");
		return 0;
	}

	//同理修改当前文件信息
	currentDirectoryDiskNum = currentDirectory->fcb[fcbI].currentBlockNum;//当前目录磁盘块号赋值为用户目录文件当前的磁盘块号，后续在该目录下的操作会使用到该信息
	strcat_s(currentPathShow, currentDirectory->fcb[fcbI].name);//修改当前路径显示
	strcat_s(currentPathShow, "\\");

	printf("进入当前目录下的子目录成功。\n");
	return 1;
}

//创建目录
int mkdir(char* directoryName)
{
	struct directoryFile* currentDirectory;//当前目录指针

	//进入用户目录文件时已修改当前目录的磁盘块号，直接去数据区查找当前目录文件
	currentDirectory = (struct directoryFile*)(osPointer->data[currentDirectoryDiskNum - 33]);

	//判断是否有同名目录
	int i;
	for (i = 1; i < fcbCount; i++)
	{
		if (currentDirectory->fcb[i].type == DIRECTORY && strcmp(currentDirectory->fcb[i].name, directoryName) == 0)
		{
			printf("已存在同名目录。\n");
			return 0;
		}
	}

	//判断目录是否已满，是否能装在文件
	for (i = 1; i < fcbCount; i++)
	{
		//查找空白fcb序号
		if (currentDirectory->fcb[i].type == ZERO)
			break;
	}

	if (i == fcbCount)
	{
		printf("该目录已满，请选择在新的目录下创建。\n");
		return 0;
	}

	//判断虚拟磁盘是否已满
	int fcbI = i;//记录创建的目录存放在当前目录下的第几个文件控制块
	for (i = 33; i < blockCount; i++)//从序号为33块磁盘查找	从0号开始，查找到127号
	{
		if (osPointer->fat1[i] == 0)//查找到未分配空表项
			break;
	}
	if (i == blockCount)//未查找到
	{
		printf("虚拟磁盘已满!\n");
		return 0;
	}

	//综上三个条件，未退出函数则满足创建目录条件
	int fatI = i;//记录填充进fat区的磁盘块号

	//分配表项、磁盘块
	//虚拟磁盘空间的fat区记录用户目录文件
	osPointer->fat1[fatI] = osPointer->fat2[fatI] = 1; //1代表不为空表项

	//数据区记录目录文件
	strcpy_s(currentDirectory->fcb[fcbI].name, directoryName);
	currentDirectory->fcb[fcbI].type = DIRECTORY;
	currentDirectory->fcb[fcbI].fatherBlockNum = currentDirectoryDiskNum;//当前目录即为所创建目录的父目录，记录磁盘块号
	currentDirectory->fcb[fcbI].currentBlockNum = fatI;//目录文件的当前目录的磁盘块号就是fat区分配的磁盘块号

	//初始化用户目录文件磁盘块
	currentDirectory = (struct directoryFile*)(osPointer->data[fatI - 33]); //在数据区定位到所创建目录磁盘块号
	currentDirectory->init(currentDirectoryDiskNum, fatI, directoryName);//目录文件初始化

	printf("创建目录“%s”成功。\n", directoryName);

	//保存文件
	if (fopen_s(&fp, diskName, "w+") == 0)//"w+"：创建一个用于读写的空文件
	{
		//写文件
		if (fp != 0)//符合fwrite()、fclose规范
		{
			fwrite(baseAddress, sizeof(char), diskSize, fp);
			fclose(fp);
		}
	}

	//写用户虚拟磁盘实际文件
	if (fopen_s(&userFp, userDiskName, "w+") == 0)
	{
		if (userFp != 0)//符合fwrite()、fclose规范
		{
			fwrite(baseAddress, sizeof(char), diskSize, userFp);
			fclose(userFp);
		}
	}

	return 1;
}

//删除目录
int rmdir(char* directoryName)
{
	struct directoryFile* currentDirectory;//当前目录指针

	//定位指针
	//进入用户目录文件时已修改当前目录的磁盘块号，直接去数据区查找当前目录文件
	currentDirectory = (struct directoryFile*)(osPointer->data[currentDirectoryDiskNum - 33]);

	//查找目录文件
	int i;
	for (i = 1; i < fcbCount; i++)
	{
		if (currentDirectory->fcb[i].type == DIRECTORY && strcmp(currentDirectory->fcb[i].name, directoryName) == 0)//查找到
		{
			break;
		}
	}

	if (i == fcbCount)//未查找到
	{
		printf("当前目录下不存在该子目录。\n");
		return 0;
	}

	int temp = i;//查找到记录目录文件控制块序号	注意表示含义

	//判断查找目录是否为空
	//定位到查找子目录的磁盘块号
	int fcbI = currentDirectory->fcb[temp].currentBlockNum;

	//定位到子目录文件
	struct directoryFile* sonDirectory; //查找子目录的指针
	sonDirectory = (struct directoryFile*)(osPointer->data[fcbI - 33]);

	//判断查找目录是否为空
	for (i = 1; i < fcbCount; i++)
	{
		if (sonDirectory->fcb[i].type != ZERO)//只要有一个不为空就退出函数
		{
			printf("该目录非空，为确保安全，请清空后再删除。\n");
			return 0;
		}
	}

	//删除子目录
	//fat记录磁盘块号置空表项
	osPointer->fat1[fcbI] = osPointer->fat2[fcbI] = 0;

	//已判断目录为空，则数据区格式化子目录
	char* p = osPointer->data[fcbI - 33];
	memset(p, 0, blockSize);//恢复0值，表示无内容

	//当前目录的文件控制块记录信息清空
	currentDirectory->fcb[temp].init();

	printf("删除目录“%s”成功。\n", directoryName);//字符串直接使用引用输出

	//保存文件
	if (fopen_s(&fp, diskName, "w+") == 0)//"w+"：创建一个用于读写的空文件
	{
		//写文件
		if (fp != 0)//符合fwrite()、fclose规范
		{
			fwrite(baseAddress, sizeof(char), diskSize, fp);
			fclose(fp);
		}
	}

	//写用户虚拟磁盘实际文件
	if (fopen_s(&userFp, userDiskName, "w+") == 0)
	{
		if (userFp != 0)//符合fwrite()、fclose规范
		{
			fwrite(baseAddress, sizeof(char), diskSize, userFp);
			fclose(userFp);
		}
	}

	return 1;
}

//创建文件
int create(char* fileName)
{
	struct directoryFile* currentDirectory;//当前目录指针

	//定位指针
	//进入用户目录文件时已修改当前目录的磁盘块号，直接去数据区查找当前目录文件
	currentDirectory = (struct directoryFile*)(osPointer->data[currentDirectoryDiskNum - 33]);

	//判断目录是否已满、该目录下是否有同名文件，记录分配的文件控制块
	int i, fcbI;
	int findFlag = 0;
	for (i = 1; i < fcbCount; i++) //查看目录是否已满//为了避免同名的文本文件
	{
		if (currentDirectory->fcb[i].type == ZERO && findFlag == 0)//文件控制块类型记录为空且未查找到标志为0
		{
			findFlag = 1;//查找到标志置位，不再进行该判断
			fcbI = i;//记录文件控制块序号

		}
		else if (currentDirectory->fcb[i].type == GENERAL && strcmp(currentDirectory->fcb[i].name, fileName) == 0)//即使查找到仍进行该判断，查看是否有同名文件
		{
			printf("无法在同一目录下创建同名文件。\n");
			return 0;
		}
	}

	//fat记录分配的磁盘块号
	int fatI;
	for (i = 33; i < blockCount; i++) //查找FAT表寻找空白区，用来分配磁盘块号
	{
		if (osPointer->fat1[i] == 0)//查找到
			break;
	}

	if (i == blockCount)
	{
		printf("虚拟磁盘已满!\n");
		return 0;
	}

	fatI = i;

	//fat区分配磁盘块
	osPointer->fat1[fatI] = osPointer->fat2[fatI] = 1;

	//目录区分配文件控制块
	strcpy_s(currentDirectory->fcb[fcbI].name, fileName);
	currentDirectory->fcb[fcbI].type = GENERAL;
	currentDirectory->fcb[fcbI].fatherBlockNum = currentDirectoryDiskNum;
	currentDirectory->fcb[fcbI].currentBlockNum = fatI;

	//初始化文本文件（最小单元）
	char* p = osPointer->data[fatI - 33];
	memset(p, 4, blockSize); //填充值4，表示Ctrl D结束/有值，显示内容时为空
	printf("创建文本文件“%s”成功。\n", fileName);

	//保存文件
	if (fopen_s(&fp, diskName, "w+") == 0)//"w+"：创建一个用于读写的空文件
	{
		//写文件
		if (fp != 0)//符合fwrite()、fclose规范
		{
			fwrite(baseAddress, sizeof(char), diskSize, fp);
			fclose(fp);
		}
	}

	//写用户虚拟磁盘实际文件
	if (fopen_s(&userFp, userDiskName, "w+") == 0)
	{
		if (userFp != 0)//符合fwrite()、fclose规范
		{
			fwrite(baseAddress, sizeof(char), diskSize, userFp);
			fclose(userFp);
		}
	}

	return 1;
}

//删除文件
int del(char* fileName)
{
	//判断文件是否在用户打开文件表中
	int i;
	for (i = 0; i < maxOpenFileNum; i++)
	{
		if ((openListPointer->fcb[i].type == GENERAL) && (strcmp(openListPointer->fcb[i].name, fileName) == 0))//找到文件
		{
			//判断是否是当前目录下需移动的文件
			if (openListPointer->fcb[i].fatherBlockNum == currentDirectoryDiskNum)//是
			{
				printf("该文件已打开，无法进行删除操作。\n");
				return 0;
			}
		}
	}

	struct directoryFile* currentDirectory;//当前目录指针

	//定位指针
	//进入用户目录文件时已修改当前目录的磁盘块号，直接去数据区查找当前目录文件
	currentDirectory = (struct directoryFile*)(osPointer->data[currentDirectoryDiskNum - 33]);

	//查找文件
	int fcbI;
	for (i = 1; i < fcbCount; i++)
	{
		if (currentDirectory->fcb[i].type == GENERAL && strcmp(currentDirectory->fcb[i].name, fileName) == 0)//查找到
		{
			fcbI = i;//记录目录的分配文件控制块序号
			break;
		}
	}

	if (i == fcbCount)//未查找到
	{
		printf("当前目录下不存在该文件。\n");
		return 0;
	}

	int fatI = currentDirectory->fcb[fcbI].currentBlockNum;//文件所使用的磁盘块号

	//fat区将使用磁盘块标为未使用
	osPointer->fat1[fatI] = osPointer->fat2[fatI] = 0;

	//目录区将文件所使用的文件控制块初始化
	currentDirectory->fcb[fcbI].init();

	//数据区将内容置为0表示未使用
	char* p = osPointer->data[fatI - 33];
	memset(p, 0, blockSize);	 //清除原文本文件的内容
	printf("删除文本文件成功。\n");

	//保存文件
	if (fopen_s(&fp, diskName, "w+") == 0)//"w+"：创建一个用于读写的空文件
	{
		//写文件
		if (fp != 0)//符合fwrite()、fclose规范
		{
			fwrite(baseAddress, sizeof(char), diskSize, fp);
			fclose(fp);
		}
	}

	//写用户虚拟磁盘实际文件
	if (fopen_s(&userFp, userDiskName, "w+") == 0)
	{
		if (userFp != 0)//符合fwrite()、fclose规范
		{
			fwrite(baseAddress, sizeof(char), diskSize, userFp);
			fclose(userFp);
		}
	}

	return 1;
}

//打开文件
int open(char* fileName)
{
	//判断用户打开文件表是否已满
	if (openListPointer->openFileNum == maxOpenFileNum)
	{
		printf("打开文件数目达到上限，无法再打开新文件。\n");
		return 0;
	}

	//查找用户打开文件表，查看文件是否已被打开，三重判断
	int i;
	for (i = 0; i < maxOpenFileNum; i++)
	{
		//宏“未知类型”，直接比较
		if (openListPointer->fcb[i].type == GENERAL && strcmp(openListPointer->fcb[i].name, fileName) == 0)//在用户打开文件表中：类型是普通文件且名字同，即为要打开的文件
		{
			if (openListPointer->fcb[i].fatherBlockNum == currentDirectoryDiskNum)//要打开文件的父磁盘块号是当前目录磁盘块号
			{
				printf("该文件已经被打开。\n");
				return 0;
			}
		}
	}

	//判断是否有该文件
	struct directoryFile* currentDirectory;//当前目录指针

	//定位指针
	//进入用户目录文件时已修改当前目录的磁盘块号，直接去数据区查找当前目录文件
	currentDirectory = (struct directoryFile*)(osPointer->data[currentDirectoryDiskNum - 33]);

	//查找打开文件
	int fcbI;
	for (i = 1; i < fcbCount; i++)
	{
		if (currentDirectory->fcb[i].type == GENERAL && (strcmp(currentDirectory->fcb[i].name, fileName) == 0))
		{
			fcbI = i;//记录目录下要打开文件的文件控制块号
			break;
		}
	}

	if (i == fcbCount)
	{
		printf("当前目录下不存在该文件。\n");
		return 0;
	}

	//将新文件装载进用户打开文件表
	openListPointer->openFileNum++;
	openListPointer->fcb[openFileCount] = currentDirectory->fcb[fcbI]; //Fcb拷贝
	openFileCount++;//下一个打开文件的文件控制块继续存放

	printf("文件打开成功。\n");

	return 1;
}

//关闭文件
int close(char* fileName)
{
	//在用户打开文件表中查找已打开的文件进行关闭，因为是打开文件就将其记录，需判断不同目录的同名文件
	int i, fcbI;
	for (i = 0; i < maxOpenFileNum; i++)
	{
		if ((openListPointer->fcb[i].type == GENERAL) && (strcmp(openListPointer->fcb[i].name, fileName) == 0))//找到文件
		{
			//判断是否是当前目录下需关闭的文件
			if (openListPointer->fcb[i].fatherBlockNum == currentDirectoryDiskNum)//是
			{
				fcbI = i;//记录下文件的文件控制块序号
				break;
			}
			else//否
			{
				printf("该文件已打开，但未在当前目录下，无法关闭。\n");
				return 0;
			}
		}
	}

	//判断文件是否在用户打开文件表中
	if (i == maxOpenFileNum)
	{
		printf("该文件未在用户打开文件表中。\n");
		return 0;
	}

	//修改用户文件打开表信息
	openListPointer->openFileNum--;
	openListPointer->fcb[fcbI].init();

	printf("文件已关闭。\n");

	return 1;
}

int read(char* fileName)
{
	//在用户打开文件表中查找已打开的文件进行读操作，因为是打开文件就将其记录，需判断不同目录的同名文件
	int i, fcbI;
	for (i = 0; i < maxOpenFileNum; i++)
	{
		if ((openListPointer->fcb[i].type == GENERAL) && (strcmp(openListPointer->fcb[i].name, fileName) == 0))//找到文件
		{
			//判断是否是当前目录下需读的文件
			if (openListPointer->fcb[i].fatherBlockNum == currentDirectoryDiskNum)//是
			{
				fcbI = i;//记录下文件的文件控制块序号
				break;
			}
			else//否
			{
				printf("该文件未在当前目录下，无法进行读操作。\n");
				return 0;
			}
		}
	}

	//判断文件是否在用户打开文件表中
	if (i == maxOpenFileNum)
	{
		printf("该文件未打开。\n");
		return 0;
	}

	//将内容从数据区读出
	//在用户打开文件表的文件控制块中记录读文件的磁盘块号
	int	fatI = openListPointer->fcb[fcbI].currentBlockNum;

	//读文件指针定位到该文件数据区地址段
	char* startPointer, * endPointer;
	startPointer = osPointer->data[fatI - 33];
	endPointer = osPointer->data[fatI - 33 + 1];//一个文件大小只占用一个扇区/磁盘块：512B

	if ((*startPointer) == 4)
	{
		printf("该文件的内容为空。");
	}
	else
	{
		printf("该文件的内容为：\n");
		while ((*startPointer) != 4 && (startPointer < endPointer))//未读到结束符，未读到数据区下一磁盘块
		{
			putchar(*startPointer);
			startPointer++;
		}
	}

	printf("\n\n读文件成功。\n");

	return 1;
}

//写文件
int write(char* fileName)
{
	//在用户打开文件表中查找已打开的文件进行写操作，因为是打开文件就将其记录，需判断不同目录的同名文件
	int i, fcbI;
	for (i = 0; i < maxOpenFileNum; i++)
	{
		if ((openListPointer->fcb[i].type == GENERAL) && (strcmp(openListPointer->fcb[i].name, fileName) == 0))//找到文件
		{
			//判断是否是当前目录下需写的文件
			if (openListPointer->fcb[i].fatherBlockNum == currentDirectoryDiskNum)//是
			{
				fcbI = i;//记录下文件的文件控制块序号
				break;
			}
			else//否
			{
				printf("该文件未在当前目录下，无法进行写操作。\n");
				return 0;
			}
		}
	}

	//判断文件是否在用户打开文件表中
	if (i == maxOpenFileNum)
	{
		printf("该文件未打开。\n");
		return 0;
	}

	//将内容写进数据区
	//在用户打开文件表的文件控制块中记录写文件的磁盘块号
	int	fatI = openListPointer->fcb[fcbI].currentBlockNum;

	//写文件指针定位到该文件数据区地址段
	char* startPointer, * endPointer;
	startPointer = osPointer->data[fatI - 33];
	endPointer = osPointer->data[fatI - 33 + 1];//一个文件大小只占用一个扇区/磁盘块：512B

	printf("请输入文本,以Ctrl D号结束：\n");//Ctrl D：代表传输结束，ASCII字符十进制为4

	char input;
	input = getchar();//吸收回车
	while (((input = getchar()) != 4))//该数据区段填入不为Ctrl D结束符
	{
		if (startPointer < endPointer - 1)//未达到文件容量
		{
			*startPointer = input;//填写数据区	先赋值，再指针移动
			startPointer++;
		}
		else//达到文件容量
		{
			printf("达到文件最大容量。");
			*startPointer = 4;//在endPointer - 1赋值4,表示结束	endPointer是下一个扇区
			break;
		}
	}

	printf("\n写入文件成功。\n");

	//保存文件
	if (fopen_s(&fp, diskName, "w+") == 0)//"w+"：创建一个用于读写的空文件
	{
		//写文件
		if (fp != 0)//符合fwrite()、fclose规范
		{
			fwrite(baseAddress, sizeof(char), diskSize, fp);
			fclose(fp);
		}
	}

	//写用户虚拟磁盘实际文件
	if (fopen_s(&userFp, userDiskName, "w+") == 0)
	{
		if (userFp != 0)//符合fwrite()、fclose规范
		{
			fwrite(baseAddress, sizeof(char), diskSize, userFp);
			fclose(userFp);
		}
	}

	return 1;
}

//移动文件
int move(char* directoryPath, char* fileName)
{
	//判断文件是否在用户打开文件表中
	int i;
	for (i = 0; i < maxOpenFileNum; i++)
	{
		if ((openListPointer->fcb[i].type == GENERAL) && (strcmp(openListPointer->fcb[i].name, fileName) == 0))//找到文件
		{
			//判断是否是当前目录下需移动的文件
			if (openListPointer->fcb[i].fatherBlockNum == currentDirectoryDiskNum)//是
			{
				printf("该文件已打开，无法进行移动操作。\n");
				return 0;
			}
		}
	}

	//查找文件
	struct directoryFile* currentDirectory;//当前目录指针

	//进入用户目录文件时已修改当前目录的磁盘块号，直接去数据区查找当前目录文件
	currentDirectory = (struct directoryFile*)(osPointer->data[currentDirectoryDiskNum - 33]);

	int fileFcbI, fileFatI;//注意各个变量的对应关系
	for (i = 1; i < fcbCount; i++)
	{
		if (currentDirectory->fcb[i].type == GENERAL && strcmp(currentDirectory->fcb[i].name, fileName) == 0)//查找到
		{
			fileFcbI = i;//记录当前目录下文件所在文件控制块序号
			fileFatI = currentDirectory->fcb[i].currentBlockNum;//记录下所移动文件占用的磁盘块号
			break;
		}
	}

	if (i == fcbCount)
	{
		printf("当前目录下不存在移动文件。\n");
		return 0;
	}

	//查找目录
	//分解目录路径名
	char* directoryBuf = NULL;//分隔字符串后剩余内容的缓冲区
	char* directoryName;//每一个目录名，指向字符串数组的指针

	directoryName = strtok_s(directoryPath, "\\", &directoryBuf);//转义

	//在数据区找移动目录文件
	struct directoryFile* moveDirectory;//移动目录指针
	moveDirectory = (struct directoryFile*)(osPointer->data[userDiskNum - 33]);//第一次定位到用户目录文件	使用了可能未初始化的本地指针变量“moveDirectory”

	//循环对用户目录下移动目录路径名->树的路径
	int moveDirectoryBlockNum = userDiskNum;//记录移动目录的磁盘块号，因为需要根据磁盘块号在数据区定位指针
	int userDirectoryFlag = 0;//只在移动到用户目录下时使用"../"
	int directoryFcbI;//记录目录所在文件控制块序号

	while (directoryName != NULL)
	{
		//继续向下个目录
		moveDirectory = (struct directoryFile*)(osPointer->data[moveDirectoryBlockNum - 33]);//第一次定位到用户目录文件

		if (strcmp(directoryName, "..") == 0)//移动到用户目录下
		{
			userDirectoryFlag = 1;//标志置位
			break;//退出循环
		}

		for (i = 1; i < fcbCount; i++)//不移动到用户目录下
		{
			if (moveDirectory->fcb[i].type == DIRECTORY && strcmp(moveDirectory->fcb[i].name, directoryName) == 0)
			{
				directoryFcbI = i;//记录目录所在文件控制块序号
				moveDirectoryBlockNum = moveDirectory->fcb[i].currentBlockNum;//记录目录文件控制块的磁盘块号
				break;
			}
		}

		if (i == fcbCount)
		{
			printf("不存在移动目录，请检查路径。\n");
			return 0;
		}

		directoryName = strtok_s(directoryBuf, "\\", &directoryBuf);//继续向下查找目录
	}

	//定位进入所找到的目录，在找到目录一层的下一层：因为要修改文件控制块
	if (userDirectoryFlag != 1)//不移动到用户目录下
	{
		moveDirectory = (struct directoryFile*)(osPointer->data[moveDirectoryBlockNum - 33]);
	}

	//装载文件
	int findFlag = 0;
	int fcbI;//记录文件控制块序号

	for (i = 1; i < fcbCount; i++) //查看目录是否已满//为了避免同名的文本文件
	{
		if (moveDirectory->fcb[i].type == ZERO && findFlag == 0)//文件控制块类型记录为空且未查找到标志为0
		{
			findFlag = 1;//查找到标志置位，不再进行该判断
			fcbI = i;//记录文件控制块序号
		}
		else if (moveDirectory->fcb[i].type == GENERAL && strcmp(moveDirectory->fcb[i].name, fileName) == 0)//即使查找到仍进行该判断，查看是否有同名文件
		{
			printf("无法在同一目录下存入同名文件。\n");
			return 0;
		}
	}

	//移动目录区分配文件控制块
	strcpy_s(moveDirectory->fcb[fcbI].name, fileName);
	moveDirectory->fcb[fcbI].type = GENERAL;
	moveDirectory->fcb[fcbI].fatherBlockNum = moveDirectoryBlockNum;
	moveDirectory->fcb[fcbI].currentBlockNum = fileFatI;

	//删除原有记录文件
	//当前目录区将文件所使用的文件控制块初始化
	currentDirectory->fcb[fileFcbI].init();

	//保存文件
	if (fopen_s(&fp, diskName, "w+") == 0)//"w+"：创建一个用于读写的空文件
	{
		//写文件
		if (fp != 0)//符合fwrite()、fclose规范
		{
			fwrite(baseAddress, sizeof(char), diskSize, fp);
			fclose(fp);
		}
	}

	//写用户虚拟磁盘实际文件
	if (fopen_s(&userFp, userDiskName, "w+") == 0)
	{
		if (userFp != 0)//符合fwrite()、fclose规范
		{
			fwrite(baseAddress, sizeof(char), diskSize, userFp);
			fclose(userFp);
		}
	}

	printf("移动文件“%s”成功。\n", fileName);

	return 1;
}

//拷贝文件
int copy(char* directoryPath, char* fileName)
{
	//判断文件是否在用户打开文件表中
	int i;
	for (i = 0; i < maxOpenFileNum; i++)
	{
		if ((openListPointer->fcb[i].type == GENERAL) && (strcmp(openListPointer->fcb[i].name, fileName) == 0))//找到文件
		{
			//判断是否是当前目录下需移动的文件
			if (openListPointer->fcb[i].fatherBlockNum == currentDirectoryDiskNum)//是
			{
				printf("该文件已打开，无法进行拷贝操作。\n");
				return 0;
			}
		}
	}

	//查找文件
	struct directoryFile* currentDirectory;//当前目录指针

	//进入用户目录文件时已修改当前目录的磁盘块号，直接去数据区查找当前目录文件
	currentDirectory = (struct directoryFile*)(osPointer->data[currentDirectoryDiskNum - 33]);

	int fileFcbI, fileFatI;//注意各个变量的对应关系
	for (i = 1; i < fcbCount; i++)
	{
		if (currentDirectory->fcb[i].type == GENERAL && strcmp(currentDirectory->fcb[i].name, fileName) == 0)//查找到
		{
			fileFcbI = i;//记录当前目录下文件所在文件控制块序号
			fileFatI = currentDirectory->fcb[i].currentBlockNum;//记录下移动文件占用的磁盘块号
			break;
		}
	}

	if (i == fcbCount)
	{
		printf("当前目录下不存在拷贝文件。\n");
		return 0;
	}

	//查找目录
	//分解目录路径名
	char* directoryBuf = NULL;//分隔字符串后剩余内容的缓冲区
	char* directoryName;//每一个目录名，指向字符串数组的指针

	directoryName = strtok_s(directoryPath, "\\", &directoryBuf);//转义

	//在数据区找到拷贝目录文件
	struct directoryFile* copyDirectory;//拷贝目录指针
	copyDirectory = (struct directoryFile*)(osPointer->data[userDiskNum - 33]);//第一次定位到用户目录文件	使用了可能未初始化的本地指针变量“moveDirectory”

	//循环对用户目录下拷贝目录路径名->树的路径
	int copyDirectoryBlockNum = userDiskNum;//记录拷贝目录的磁盘块号，因为需要根据磁盘块号在数据区定位指针
	int userDirectoryFlag = 0;//只在拷贝到用户目录下时使用"../"
	int directoryFcbI;//记录目录所在文件控制块序号

	while (directoryName != NULL)
	{
		//继续向下个目录
		copyDirectory = (struct directoryFile*)(osPointer->data[copyDirectoryBlockNum - 33]);//第一次定位到用户目录文件

		if (strcmp(directoryName, "..") == 0)//拷贝到用户目录下
		{
			userDirectoryFlag = 1;//标志置位
			break;//退出循环
		}

		for (i = 1; i < fcbCount; i++)//不拷贝到用户目录下
		{
			if (copyDirectory->fcb[i].type == DIRECTORY && strcmp(copyDirectory->fcb[i].name, directoryName) == 0)
			{
				directoryFcbI = i;//记录目录所在文件控制块序号
				copyDirectoryBlockNum = copyDirectory->fcb[i].currentBlockNum;//记录目录文件控制块的磁盘块号
				break;
			}
		}

		if (i == fcbCount)
		{
			printf("不存在拷贝目录，请检查路径。\n");
			return 0;
		}

		directoryName = strtok_s(directoryBuf, "\\", &directoryBuf);//继续向下查找目录
	}

	//进入所找到的目录，在找到目录一层的下一层：因为要修改文件控制块
	if (userDirectoryFlag != 1)//不拷贝到用户目录下
	{
		copyDirectory = (struct directoryFile*)(osPointer->data[copyDirectoryBlockNum - 33]);
	}

	//装载文件
	int findFlag = 0;
	int fcbI;
	for (i = 1; i < fcbCount; i++) //查看目录是否已满//为了避免同名的文本文件
	{
		if (copyDirectory->fcb[i].type == ZERO && findFlag == 0)//文件控制块类型记录为空且未查找到标志为0
		{
			findFlag = 1;//查找到标志置位，不再进行该判断
			fcbI = i;//记录文件控制块序号
		}
		else if (copyDirectory->fcb[i].type == GENERAL && strcmp(copyDirectory->fcb[i].name, fileName) == 0)//即使查找到仍进行该判断，查看是否有同名文件
		{
			printf("无法在同一目录下存入同名文件。\n");
			return 0;
		}
	}

	//拷贝目录区分配文件控制块
	strcpy_s(copyDirectory->fcb[fcbI].name, fileName);
	copyDirectory->fcb[fcbI].type = GENERAL;
	copyDirectory->fcb[fcbI].fatherBlockNum = copyDirectoryBlockNum;
	copyDirectory->fcb[fcbI].currentBlockNum = fileFatI;

	//保存文件
	if (fopen_s(&fp, diskName, "w+") == 0)//"w+"：创建一个用于读写的空文件
	{
		//写文件
		if (fp != 0)//符合fwrite()、fclose规范
		{
			fwrite(baseAddress, sizeof(char), diskSize, fp);
			fclose(fp);
		}
	}

	//写用户虚拟磁盘实际文件
	if (fopen_s(&userFp, userDiskName, "w+") == 0)
	{
		if (userFp != 0)//符合fwrite()、fclose规范
		{
			fwrite(baseAddress, sizeof(char), diskSize, userFp);
			fclose(userFp);
		}
	}

	printf("拷贝文件成功。\n");

	return 1;
}

//查找文件
int find(char* fileName)
{
	int currentBlockNum = userDiskNum;//当前目录所在磁盘块号，第一次从用户目录寻找

	directoryPointer = (struct directoryFile*)(osPointer->data[currentBlockNum - 33]);//第一次定位到用户目录文件

	strcat_s(findPath, directoryPointer->fcb[0].name);//用户目录路径
	strcat_s(findPath, "\\");

	//递归对用户目录下每个目录->树的深度优先遍历
	DFS(currentBlockNum, fileName);

	if (findCount == 0)
	{
		printf("\n不存在该文件。\n");
		return 0;
	}

	printf("查找文件路径如下：\n");
	for (int i = 0; i < findCount; i++)
	{
		printf("%s\n", findPaths[i]);
	}

	printf("\n查找文件“%s”成功。\n", fileName);

	return 1;
}

//递归对用户目录下每个目录->树的深度优先遍历
void DFS(int currentBlockNum, char* fileName)
{
	//定位指针
	//在数据区找目录文件
	directoryPointer = (struct directoryFile*)(osPointer->data[currentBlockNum - 33]);//第一次定位到用户目录文件	重复定位

	//更新路径
	if (currentBlockNum != userDiskNum)//第一次用户目录不用更新
	{
		strcat_s(findPath, directoryPointer->fcb[0].name);
		strcat_s(findPath, "\\");
		//printf("路径：	%s\n", findPath);
	}

	int i;
	for (i = 1; i < fcbCount; i++)
	{
		//目录文件则递归
		if (directoryPointer->fcb[i].type == DIRECTORY)
		{
			currentBlockNum = directoryPointer->fcb[i].currentBlockNum;//记录目录文件控制块的磁盘块号

			DFS(currentBlockNum, fileName);//递归查找
		}

		//文本文件则查找
		if (directoryPointer->fcb[i].type == GENERAL && strcmp(directoryPointer->fcb[i].name, fileName) == 0)//查找到文件
		{
			//装载路径
			char filePath[50] = "";
			strcat_s(filePath, findPath);
			strcat_s(filePath, fileName);

			strcpy_s(findPaths[findCount], filePath);
			findCount++;
		}
	}

	//一个目录搜索完就回退
	if (i == fcbCount)//多回退一次到E:\，不需管
	{
		strncpy_s(findPath, findPath, strlen(findPath) - strlen(directoryPointer->fcb[0].name) - 1);//回退当前路径显示 多减去一个字符"\"
		//printf("回退路径：	%s\n", findPath);
	}

	return;
}

//显示文件的前 num 行
int head(int* num, char* fileName)
{
	//在用户打开文件表中查找已打开的文件进行读操作，因为是打开文件就将其记录，需判断不同目录的同名文件
	int i, fcbI;
	for (i = 0; i < maxOpenFileNum; i++)
	{
		if ((openListPointer->fcb[i].type == GENERAL) && (strcmp(openListPointer->fcb[i].name, fileName) == 0))//找到文件
		{
			//判断是否是当前目录下需读的文件
			if (openListPointer->fcb[i].fatherBlockNum == currentDirectoryDiskNum)//是
			{
				fcbI = i;//记录下文件的文件控制块序号
				break;
			}
			else//否
			{
				printf("该文件未在当前目录下，无法进行读操作。\n");
				return 0;
			}
		}
	}

	//判断文件是否在用户打开文件表中
	if (i == maxOpenFileNum)
	{
		printf("该文件未打开。\n");
		return 0;
	}

	//将内容从数据区读出
	//在用户打开文件表的文件控制块中记录读文件的磁盘块号
	int	fatI = openListPointer->fcb[fcbI].currentBlockNum;

	//读文件指针定位到该文件数据区地址段
	char* startPointer, * endPointer;
	startPointer = osPointer->data[fatI - 33];
	endPointer = osPointer->data[fatI - 33 + 1];//一个文件大小只占用一个扇区/磁盘块：512B

	printf("该文件前%d行内容为：\n", *num);

	int lineCount = 1;//行计数
	if ((*startPointer) != 4 && (startPointer < endPointer))//先判断是否有内容，有则先输出"1 "
	{
		printf("%d	", lineCount);
	}
	while ((*startPointer) != 4 && (startPointer < endPointer))//未读到结束符，未读到数据区下一磁盘块
	{
		if (*startPointer == '\n')//读到换行符
		{
			lineCount++; //行数 + 1

			//结束读
			if (lineCount > * num)//读过需要的行数
			{
				printf("\n");

				return 1;
			}

			putchar(*startPointer);//输出换行符
			startPointer++;
			printf("%d	", lineCount);//输出行数
		}
		else//输出字符
		{
			putchar(*startPointer);
			startPointer++;
		}
	}
	printf("\n");

	return 1;
}

//显示文件的后num行
int tail(int* num, char* fileName)
{
	//在用户打开文件表中查找已打开的文件进行读操作，因为是打开文件就将其记录，需判断不同目录的同名文件
	int i, fcbI;
	for (i = 0; i < maxOpenFileNum; i++)
	{
		if ((openListPointer->fcb[i].type == GENERAL) && (strcmp(openListPointer->fcb[i].name, fileName) == 0))//找到文件
		{
			//判断是否是当前目录下需读的文件
			if (openListPointer->fcb[i].fatherBlockNum == currentDirectoryDiskNum)//是
			{
				fcbI = i;//记录下文件的文件控制块序号
				break;
			}
			else//否
			{
				printf("该文件未在当前目录下，无法进行读操作。\n");
				return 0;
			}
		}
	}

	//判断文件是否在用户打开文件表中
	if (i == maxOpenFileNum)
	{
		printf("该文件未打开。\n");
		return 0;
	}

	//将内容从数据区读出
	//在用户打开文件表的文件控制块中记录读文件的磁盘块号
	int	fatI = openListPointer->fcb[fcbI].currentBlockNum;

	//读文件指针定位到该文件数据区地址段
	char* startPointer, * endPointer;
	startPointer = osPointer->data[fatI - 33];
	endPointer = osPointer->data[fatI - 33 + 1];//一个文件大小只占用一个扇区/磁盘块：512B

	printf("该文件的后%d行内容为：\n", *num);

	int lineNum = 1;//行总数
	int lineCount = 1;//行计数

	//统计文件行数
	while ((*startPointer) != 4 && (startPointer < endPointer))//未读到结束符，未读到数据区下一磁盘块
	{
		if (*startPointer == '\n')
		{
			lineNum++;
		}

		startPointer++; //勿漏循环条件
	}

	//重新定位
	startPointer = osPointer->data[fatI - 33];

	//如果要读行数大于等于文件行数则全部读出
	if (*num >= lineNum)//先*后>=
	{
		if ((*startPointer) != 4 && (startPointer < endPointer))//先判断是否有内容，有则先输出"1 "
		{
			printf("%d	", lineCount);
		}
		while ((*startPointer) != 4 && (startPointer < endPointer))//未读到结束符，未读到数据区下一磁盘块
		{
			if (*startPointer == '\n')//读到换行符
			{
				lineCount++;//行数增

				putchar(*startPointer);//输出换行符
				startPointer++;
				printf("%d	", lineCount);//输出行数
			}
			else//输出字符
			{
				putchar(*startPointer);
				startPointer++;
			}
		}

		printf("\n");

		return 1;
	}

	//脑热，逻辑理了好久
	while ((*startPointer) != 4 && (startPointer < endPointer))//未读到结束符，未读到数据区下一磁盘块
	{
		//记录行数
		if (*startPointer == '\n')//读到换行符
		{
			lineCount++;//行数增

			//到应该显示的行
			if (lineCount + *num == lineNum + 1)//注意判断
			{
				printf("%d	", lineCount);//输出行数
				startPointer++;//换行符的下一字符，第一行显示不需要换行

				break;//退出循环，到下一阶段开始输出字符
			}
		}

		startPointer++;//移动指针直到找到需要输出的行数
	}

	//到了需要输出行，按部就班输出即可
	while ((*startPointer) != 4 && (startPointer < endPointer))//未读到结束符，未读到数据区下一磁盘块
	{
		if (*startPointer == '\n')//读到换行符
		{
			lineCount++;//行数增

			putchar(*startPointer);//输出换行符
			startPointer++;
			printf("%d	", lineCount);//输出行数
		}
		else//输出字符
		{
			putchar(*startPointer);
			startPointer++;
		}
	}

	printf("\n");

	return 1;
}

int import(char* filePath)
{
	FILE* localFp;//新定义文件指针
	char line[512];//一行文件内容
	char buffer[512] = "";//将文件内容读到缓冲区	char类型一个字符占一个字节 默认一个文件占一个扇区/磁盘块，512B	字符串拼接要初始化

	//读本地磁盘实际文件
	if ((fopen_s(&localFp, filePath, "r")) != 0)//无则报错	"r"：打开一个用于读取的文件。该文件必须存在
	{
		printf("本地磁盘不存在该文件。\n");
		return 0;
	}
	else//有
	{
		if (localFp != 0)//符合fgets()、fclose()规范
		{
			while (fgets(line, sizeof(line), localFp) != 0)//文件存储编码为GBK，显示到屏幕正确
			{
				strcat_s(buffer, line);//将文件内容存到缓冲区
			}

			fclose(localFp);
		}
	}

	//取文件名
	char* fileBuf = NULL;//分隔字符串后剩余内容的缓冲区
	char* fileName;//每一个目录名，指向字符串数组的指针

	fileName = strtok_s(filePath, "\\", &fileBuf);//E:\a.txt->a.txt
	fileName = strtok_s(fileBuf, ".", &fileBuf);//a.txt->a

	//导入文件
	struct directoryFile* currentDirectory;//当前目录指针

	//定位指针
	//进入用户目录文件时已修改当前目录的磁盘块号，直接去数据区查找当前目录文件
	currentDirectory = (struct directoryFile*)(osPointer->data[currentDirectoryDiskNum - 33]);

	//判断目录是否已满、该目录下是否有同名文件，记录分配的文件控制块
	int i, fcbI;
	int findFlag = 0;
	for (i = 1; i < fcbCount; i++) //查看目录是否已满//为了避免同名的文本文件
	{
		if (currentDirectory->fcb[i].type == ZERO && findFlag == 0)//文件控制块类型记录为空且未查找到标志为0
		{
			findFlag = 1;//查找到标志置位，不再进行该判断
			fcbI = i;//记录文件控制块序号
		}
		else if (currentDirectory->fcb[i].type == GENERAL && strcmp(currentDirectory->fcb[i].name, fileName) == 0)//即使查找到仍进行该判断，查看是否有同名文件
		{
			printf("无法在同一目录下导入同名文件。\n");
			return 0;
		}
	}

	//fat记录分配的磁盘块号
	int fatI;
	for (i = 33; i < blockCount; i++) //查找FAT表寻找空白区，用来分配磁盘块号j
	{
		if (osPointer->fat1[i] == 0)//查找到
			break;
	}

	if (i == blockCount)
	{
		printf("虚拟磁盘已满!\n");
		return 0;
	}

	fatI = i;

	//fat区分配磁盘块
	osPointer->fat1[fatI] = osPointer->fat2[fatI] = 1;

	//目录区分配文件控制块
	strcpy_s(currentDirectory->fcb[fcbI].name, fileName);
	currentDirectory->fcb[fcbI].type = GENERAL;
	currentDirectory->fcb[fcbI].fatherBlockNum = currentDirectoryDiskNum;
	currentDirectory->fcb[fcbI].currentBlockNum = fatI;

	//写入文本文件内容
	//写文件指针定位到该文件数据区地址段
	char* startPointer, * endPointer;
	startPointer = osPointer->data[fatI - 33];
	endPointer = osPointer->data[fatI - 33 + 1];//一个文件大小只占用一个扇区/磁盘块：512B

	unsigned int j;
	for (j = 0; j < strlen(buffer); j++)//使用strlen	“ < ”: 有符号 / 无符号不匹配

	{
		if (startPointer < endPointer - 1)//未达到文件容量
		{
			*startPointer = buffer[j];//填写数据区	先赋值，再指针移动
			startPointer++;
		}
		else//达到文件容量
		{
			printf("达到文件最大容量。");
			*startPointer = 4;//在endPointer - 1赋值4,表示结束	endPointer是下一个扇区
			break;
		}
	}

	//保存文件
	if (fopen_s(&fp, diskName, "w+") == 0)//"w+"：创建一个用于读写的空文件
	{
		//写文件
		if (fp != 0)//符合fwrite()、fclose规范
		{
			fwrite(baseAddress, sizeof(char), diskSize, fp);
			fclose(fp);
		}
	}

	//写用户虚拟磁盘实际文件
	if (fopen_s(&userFp, userDiskName, "w+") == 0)
	{
		if (userFp != 0)//符合fwrite()、fclose规范
		{
			fwrite(baseAddress, sizeof(char), diskSize, userFp);
			fclose(userFp);
		}
	}

	printf("文件“%s”导入成功。\n", fileName);

	return 1;
}

int exp(char* fileName, char* filePath)
{
	//判断文件是否在用户打开文件表中
	int i;
	for (i = 0; i < maxOpenFileNum; i++)
	{
		if ((openListPointer->fcb[i].type == GENERAL) && (strcmp(openListPointer->fcb[i].name, fileName) == 0))//找到文件
		{
			//判断是否是当前目录下需导出的文件
			if (openListPointer->fcb[i].fatherBlockNum == currentDirectoryDiskNum)//是
			{
				printf("该文件已打开，无法进行导出操作。\n");
				return 0;
			}
		}
	}

	//查找文件
	struct directoryFile* currentDirectory;//当前目录指针

	//进入用户目录文件时已修改当前目录的磁盘块号，直接去数据区查找当前目录文件
	currentDirectory = (struct directoryFile*)(osPointer->data[currentDirectoryDiskNum - 33]);

	int fatI;//注意各个变量的对应关系
	for (i = 1; i < fcbCount; i++)
	{
		if (currentDirectory->fcb[i].type == GENERAL && strcmp(currentDirectory->fcb[i].name, fileName) == 0)//查找到
		{
			fatI = currentDirectory->fcb[i].currentBlockNum;//记录下导出文件占用的磁盘块号
			break;
		}
	}

	if (i == fcbCount)
	{
		printf("当前目录下不存在导出文件。\n");
		return 0;
	}

	//将内容从数据区读出
	char buffer[blockSize] = "";//文件内容放入缓冲区	初始化为空

	//读文件指针定位到该文件数据区地址段
	char* startPointer, * endPointer;
	startPointer = osPointer->data[fatI - 33];
	endPointer = osPointer->data[fatI - 33 + 1];//一个文件大小只占用一个扇区/磁盘块：512B

	i = 0;
	while ((*startPointer) != 4 && (startPointer < endPointer))//未读到结束符，未读到数据区下一磁盘块
	{
		buffer[i] = *startPointer;//有内容就存值
		i++;
		startPointer++;
	}

	//导出文件
	//完善本地文件路径
	char localFilePath[10] = "";//本地文件完整路径
	strcat_s(localFilePath, filePath);
	strcat_s(localFilePath, fileName);
	strcat_s(localFilePath, ".txt");

	FILE* localFp;
	if (fopen_s(&localFp, localFilePath, "w+") == 0)//"w+"：创建一个用于读写的空文件
	{
		//写文件
		if (localFp != 0)//符合fwrite()、fclose规范
		{
			fwrite(buffer, sizeof(char), blockSize, localFp);
			fclose(fp);
		}
	}

	printf("导出文件成功。\n");

	return 1;
}

//启动文件系统
void startFileSystem()
{
	baseAddress = (char*)malloc(diskSize);//在实际内存中申请虚拟磁盘空间
	osPointer = (struct Disk*)(baseAddress); //虚拟磁盘初始化，操作系统指针指向实际磁盘空间，建立映射

	//读虚拟磁盘实际文件
	if ((fopen_s(&fp, diskName, "r")) != 0)//无则格式化	"r"：打开一个用于读取的文件。该文件必须存在
	{
		printf("第一次使用该文件系统。\t正在初始化...\n");

		//格式化
		format();

		//进入一级文件系统
		FirstLevel();
	}
	else//有则加载
	{
		if ((baseAddress != 0) && (fp != 0))//fread()规范
		{
			fread(baseAddress, sizeof(char), diskSize, fp);
			fclose(fp);//必须先关闭，后保存文件时再指定模式重新打开
			printf("已加载磁盘文件(%s)。\n\n", diskName);

			//进入一级文件系统
			FirstLevel();
		}
	}
}

//一级文件系统
void FirstLevel()
{
	printf("********************欢迎使用一级文件系统********************\n");//一边20个字符
	printf("            命令格式                    说明                \n");
	printf("            logon                       用户注册            \n");
	printf("            login                       用户登录            \n");
	printf("            logout                      用户注销            \n");
	printf("            format                      虚拟磁盘格式化      \n");
	printf("************************************************************\n");

	while (1)
	{
		printf("\n%s", currentPathShow);
		scanf_s("%s", command, sizeof(command));//等待输入命令

		if (strcmp(command, "logon") == 0)
		{
			printf("请输入注册用户名：");
			scanf_s("%s", content, sizeof(content));//等待输入内容/注册用户名
			logon(content);
		}
		else if (strcmp(command, "login") == 0)
		{
			printf("请输入登录用户名：");
			scanf_s("%s", content, sizeof(content));//登录用户名
			login(content);
		}
		else if (strcmp(command, "logout") == 0)
		{
			//验证管理员
			printf("请输入管理员账号：");
			char account[10] = "";//可能没有为字符串“account”添加字符串零终止符。

			scanf_s("%s", account, sizeof(account));//默认为root

			if (strcmp(account, "root") == 0)//是管理员
			{
				printf("请输入注销用户名：");
				scanf_s("%s", content, sizeof(content));//注销用户名
				logout(content);
			}
			else
			{
				printf("管理员账号验证错误。\n");
			}
		}
		else if (strcmp(command, "format") == 0)
		{
			//验证管理员
			printf("请输入管理员账号：");
			char account[10] = "";
			scanf_s("%s", account, sizeof(account));//默认为root

			if (strcmp(account, "root") == 0)//是管理员
			{
				format();
			}
			else
			{
				printf("管理员账号验证错误。\n");
			}
		}
		else
		{
			printf("无效指令, 请重新输入。\n");
		}
	}
}

//二级文件系统
void SecondLevel()
{
	printf("********************欢迎使用二级文件系统********************\n");
	printf("            命令格式                    说明                \n");
	printf("            dir                         列文件目录          \n");
	printf("            cd                          进入目录            \n");
	printf("            mkdir                       创建目录            \n");
	printf("            rmdir                       删除目录            \n");
	printf("            create                      创建文件            \n");
	printf("            delete                      删除文件            \n");
	printf("            open                        打开文件            \n");
	printf("            close                       关闭文件            \n");
	printf("            read                        读文件              \n");
	printf("            write                       写文件              \n");
	printf("            move                        移动文件            \n");
	printf("            copy                        拷贝文件            \n");
	printf("            find                        查找文件            \n");
	printf("            head -num                   显示文件的前 num 行 \n");
	printf("            tail -num                   显示文件的后 num 行 \n");
	printf("            import                      将本地文件导入到目录\n");
	printf("            export                      将文件导出到本地磁盘\n");
	printf("            clear                       清屏			    \n");
	printf("************************************************************\n");

	while (1)
	{
		printf("\n%s", currentPathShow);
		scanf_s("%s", command, sizeof(command));//等待输入命令

		if (strcmp(command, "dir") == 0)
		{
			dir();
		}
		else if (strcmp(command, "cd") == 0)
		{
			printf("请输入切换目录名：");
			scanf_s("%s", content, sizeof(content));//等待输入内容/切换目录名
			cd(content);
		}
		else if (strcmp(command, "mkdir") == 0)
		{
			printf("请输入创建目录名：");
			scanf_s("%s", content, sizeof(content));//等待输入内容/创建目录名
			mkdir(content);
		}
		else if (strcmp(command, "rmdir") == 0)
		{
			printf("请输入删除目录名：");
			scanf_s("%s", content, sizeof(content));//等待输入内容/删除目录名
			rmdir(content);
		}
		else if (strcmp(command, "create") == 0)
		{
			printf("请输入创建文件名：");
			scanf_s("%s", content, sizeof(content));//等待输入内容/创建文件名
			create(content);
		}
		else if (strcmp(command, "delete") == 0)
		{
			printf("请输入删除文件名：");
			scanf_s("%s", content, sizeof(content));//等待输入内容/删除文件名
			del(content);
		}
		else if (strcmp(command, "open") == 0)
		{
			printf("请输入打开文件名：");
			scanf_s("%s", content, sizeof(content));//等待输入内容/打开文件名
			open(content);
		}
		else if (strcmp(command, "close") == 0)
		{
			printf("请输入关闭文件名：");
			scanf_s("%s", content, sizeof(content));//等待输入内容/关闭文件名
			close(content);
		}
		else if (strcmp(command, "read") == 0)
		{
			printf("请输入读取文件名：");
			scanf_s("%s", content, sizeof(content));//等待输入内容/读取文件名
			read(content);
		}
		else if (strcmp(command, "write") == 0)
		{
			printf("请输入写入文件名：");
			scanf_s("%s", content, sizeof(content));//等待输入内容/写入文件名
			write(content);
		}
		else if (strcmp(command, "move") == 0)
		{
			char directoryName[10];//移动目录名

			printf("移动到用户目录下为“..\\”，请输入移动目录名：");
			scanf_s("%s", directoryName, sizeof(directoryName));//等待输入内容/移动目录名

			printf("请输入移动文件名：");
			scanf_s("%s", content, sizeof(content));//等待输入内容/移动文件名
			move(directoryName, content);
		}
		else if (strcmp(command, "copy") == 0)
		{
			char directoryName[10];//移动目录名

			printf("拷贝到用户目录下为“..\\”，请输入拷贝目录名：");
			scanf_s("%s", directoryName, sizeof(directoryName));//等待输入内容/拷贝目录名

			printf("请输入拷贝文件名：");
			scanf_s("%s", content, sizeof(content));//等待输入内容/拷贝文件名
			copy(directoryName, content);
		}
		else if (strcmp(command, "find") == 0)
		{
			printf("请输入查找文件名：");
			scanf_s("%s", content, sizeof(content));//等待输入内容/查找文件名
			find(content);
		}
		else if (strcmp(command, "head") == 0)
		{
			char unusedChar;
			unusedChar = getchar();//读一个字符" "
			unusedChar = getchar();//读一个字符"-"

			int num;
			scanf_s("%d", &num);//读文件行数

			printf("请输入读取文件名：");
			scanf_s("%s", content, sizeof(content));//等待输入内容/读取文件名
			head(&num, content);
		}
		else if (strcmp(command, "tail") == 0)
		{
			char unusedChar;
			unusedChar = getchar();//读一个字符" "
			unusedChar = getchar();//读一个字符"-"

			int num;
			scanf_s("%d", &num);//读文件行数

			printf("请输入读取文件名：");
			scanf_s("%s", content, sizeof(content));//等待输入内容/读文件名
			tail(&num, content);
		}
		else if (strcmp(command, "import") == 0)
		{
			printf("请输入导入本地文件路径：");
			scanf_s("%s", content, sizeof(content));//等待输入内容/导入本地文件路径
			import(content);
		}
		else if (strcmp(command, "export") == 0)
		{
			char fileName[10];//默认十个字符

			printf("请输入导出文件名：");
			scanf_s("%s", fileName, sizeof(fileName));

			printf("请输入导出本地文件路径：");
			scanf_s("%s", content, sizeof(content));//等待输入内容/导出本地文件路径
			exp(fileName, content);
		}
		else if (strcmp(command, "clear") == 0)
		{
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");//二十七个	不用system("cls");以可以查看之间的消息记录
		}
		else
		{
			printf("无效指令, 请重新输入。\n");
		}
	}
}

//主函数————————————————————
int main(void)
{
	startFileSystem();//启动文件系统

	return 0;
}