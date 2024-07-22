#include <stdio.h>
#include <string.h>
int main()
{
	void mystrcat(char *s1,char *s2,char *new_s);
	char *s1,*s2,*new_s;
	char c1[20],c2[20],sum[40];
	printf("请输入字符串1:");
	scanf("%s",c1);
	printf("\n");
	printf("请输入字符串2:");
	scanf("%s",c2);
	s1=c1;
	s2=c2;
	new_s=sum;
	printf("\n");
	printf("交叉连接后的字符串为:");
	mystrcat(s1,s2,new_s);
	return 0;
}

	void mystrcat(char *s1,char *s2,char *new_s)
	{
		int len1,len2,temp,i,j,k,g,h,f,m;
		len1=strlen(s1);
		len2=strlen(s2);
		m=(len2-1)/2;//首尾元素交换
		for(i=0;i<=m;i++)
		{
			j=len2-1-i;
			temp=s2[i];//指针，引用时应为*(s2+i)
			s2[i]=s2[j];
			s2[j]=temp;
		}
		if(*s1!='\0'&&*s2!='\0')
		{
			k=0,g=0,h=0;
			while(k<len1+len2)
			{
				new_s[k]=s1[g];
				new_s[k+1]=s2[h];
				k=k+2;
				g++;
				h++;
				if(s1[g]=='\0')//进行移位后的元素判断，补齐多余元素
				{
					while(s2[h]!='\0')
					{
						new_s[k]=s2[h];
						k++;
						h++;
					}
				}
				if(s2[h]=='\0')
				{
					while(s1[h]!='\0')
					{
						new_s[k]=s1[g];
						k++;
						g++;
					}
				}
			}	
		}
		new_s[k]='\0';//易漏
		printf("%s\n",new_s);
	}