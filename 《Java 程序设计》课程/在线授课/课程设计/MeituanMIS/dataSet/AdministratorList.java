package dataSet;

import java.util.*;
import java.io.*;

public class AdministratorList
{
	//管理员顺序表
	ArrayList<Administrator> administratorList = new ArrayList<Administrator>();
	
	//构造方法初始化顺序表
	public AdministratorList()
	{
		//读数据
		try
		{
			//File类
			File f=new File("C:\\Users\\DSHH\\Desktop\\数据集\\administrator.txt");
		
			//文件字符输入流、字符缓冲输入流
			FileReader fr=new FileReader(f);
			BufferedReader br=new BufferedReader(fr);
		
			String line=null;//读一行数据
			String[] content;//一行数据中两个字符串构成的字符串数组
			String account;
			String password;
		
			line=br.readLine();//不用第一行中文数据
			while((line=br.readLine())!=null)
			{
				content=line.split(" ");
			
				account=content[0];
				password=content[1];
				
				Administrator administrator=new Administrator(account,password);//一个管理员
				this.administratorList.add(administrator);//添加到用户顺序表
			}
		
			//关闭流
			br.close();
			fr.close();
		}
		catch(IOException ioe) 
		{
			System.out.println(ioe);
		}		
	}

	//获取成员变量，管理员顺序表...
	public ArrayList<Administrator> getAdministratorList()
	{
		return this.administratorList;
	}
	
	//判断管理员账号、密码是否正确
	public String login(String account,String password)
	{
		//判断账号或密码错误
		String judgement="";
		boolean accountFlag=false;
		boolean passwordFlag=false;
			
		Iterator<Administrator> it=this.administratorList.iterator();
		while(it.hasNext())//每个管理员
		{
			Administrator a=it.next();
				
			if(a.getAccount().equals(account))//账号匹配
			{
				accountFlag=true;
					
				if(a.getPassword().equals(password))//密码匹配
				{
					passwordFlag=true;
						
					break;//匹配成功退出循环
				}
			}
		}
			
		//设置返回值
		if(accountFlag&&passwordFlag)
		{
			judgement="11";
		}
		else if(!accountFlag&&passwordFlag)
		{
			judgement="01";
		}
		else if(accountFlag&&!passwordFlag)
		{
			judgement="10";
		}
		else
		{
			judgement="00";
		}
			
		return judgement;
	}
	
	//根据管理员账号查看管理员信息
	public String show(String account)
	{
		Iterator<Administrator> it=administratorList.iterator();
		String content=null;
			
		while(it.hasNext())
		{
			Administrator a=it.next();
			if(a.getAccount().equals(account))
			{
				content="账号："+a.getAccount()+"\n密码："+a.getPassword();
				break;
			}	
		}
		
		return content;
	}
	
	//修改管理员信息
	public void modify(String oldAccount,String newLine)
	{
		String[] content=newLine.split(" ");
		String newAccount=content[0];
		String newPasssword=content[1];
		
		//修改管理员顺序表
		Iterator<Administrator> it=administratorList.iterator();
			
		while(it.hasNext())
		{
			Administrator a=it.next();
			if(a.getAccount().equals(oldAccount))//账号匹配
			{
				a.setAccount(newAccount);
				a.setPassword(newPasssword);
				break;
			}
		}
		
		//修改文件
		ArrayList<String> data=new ArrayList<String>();//使用顺序表存储数据，修改后再写入文件
		try
		{
			File f=new File("C:\\Users\\DSHH\\Desktop\\数据集\\administrator.txt");
			
			//读数据
			FileReader fr=new FileReader(f);
			BufferedReader br=new BufferedReader(fr);
			
			String line=null;//读一行数据
			
			line=br.readLine();
			data.add(line);
			while((line=br.readLine())!=null)
			{
				
				if(line.startsWith(oldAccount))//如果是要修改的账号，则写入新信息
				{
					data.add(newLine);
				}
				else//否则写入原来信息
				{
					data.add(line);
				}
			}
	
			//写数据
			FileWriter fw=new FileWriter(f);
			BufferedWriter bw=new BufferedWriter(fw);
			
			Iterator<String> it1=data.iterator();
			while(it1.hasNext())
			{
				String s=it1.next();
				bw.write(s);
				bw.newLine();
			}
			
			//关闭流
			br.close();
			fr.close();
		
			bw.close();
			fw.close();	
		}
		catch(IOException ioe)
		{
			System.out.println(ioe);
		}
	}
	
	//主方法
	public static void main(String[] args)
	{
		//new AdministratorList();
	}
}