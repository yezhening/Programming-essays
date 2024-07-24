package userSystem;

import java.util.*;//LinkedList
import java.io.*;

public class UserLoginInformation 
{
	//用户登录表
	LinkedList<String> usersLoginTable = new LinkedList<String>();
	
	//构造方法初始化链表
	public UserLoginInformation()
	{
		//读数据
		try
		{
			//File类
			File f=new File("C:\\Users\\DSHH\\Desktop\\数据集\\userLoginInformation.txt");
			
			//文件字符输入流、字符缓冲输入流
			FileReader fr=new FileReader(f);
			BufferedReader br=new BufferedReader(fr);
		
			String account=null;//读一行数据，存的是用户账号
			
			while((account=br.readLine())!=null)
			{
				usersLoginTable.add(account);//用户登录链表
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
	
	//用户是否登录（在登录表中）
	public boolean contains(String account)
	{
		return usersLoginTable.contains(account);
	}
	
	//添加登录用户账号
	public void add(String account)
	{
		//修改链表
		usersLoginTable.add(account);
		
		//修改文件
		try
		{
			File f=new File("C:\\Users\\DSHH\\Desktop\\数据集\\userLoginInformation.txt");
		
			FileWriter fw=new FileWriter(f,true);//追加
			BufferedWriter bw=new BufferedWriter(fw);
		
			bw.write(account);
			bw.newLine();
		
			bw.close();
			fw.close();
		}
		catch(IOException ioe) 
		{
			System.out.println(ioe);
		}		
	}
	
	//删除用户登录账号
	public void remove(String account)
	{
		//修改链表
		Iterator<String> it=this.usersLoginTable.iterator();
		while(it.hasNext())
		{
			String s=it.next();
			if(s.equals(account))
			{
				it.remove();
			}
		}
		
		//修改文件		
		try
		{
			File f=new File("C:\\Users\\DSHH\\Desktop\\数据集\\userLoginInformation.txt");
					
			//写数据
			FileWriter fw=new FileWriter(f);
			BufferedWriter bw=new BufferedWriter(fw);
					
			Iterator<String> it1=this.usersLoginTable.iterator();
			while(it1.hasNext())
			{
				String s=it1.next();
				bw.write(s);
				bw.newLine();
			}
					
			//关闭流
			bw.close();
			fw.close();
		}
		catch(IOException ioe)
		{
			System.out.println(ioe);
		}			
	}
	
	//查看所有用户登录信息
	public String showUser()
	{
		//返回界面内容
		String content="所有用户登录信息显示如下：\n\n用户账号\n————\n";
		
		Iterator<String> it=this.usersLoginTable.iterator();
		while(it.hasNext())
		{
			String s=it.next();//一个账号
			
			content=content+s+"\n";
		}
		
		return content;
	}
	
	//主方法
	public static void main(String[] args)
	{
		//new UserLoginInformation();
	}
}