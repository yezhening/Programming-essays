package dataSet;

import java.util.*;//ArrayList
import java.io.*;

public class UserList
{
	//用户顺序表
	ArrayList<User> userList = new ArrayList<User>();
	
	//构造方法初始化顺序表
	public UserList()
	{
		//读数据
		try
		{
			//File类
			File f=new File("C:\\Users\\DSHH\\Desktop\\数据集\\user.txt");
		
			//文件字符输入流、字符缓冲输入流
			FileReader fr=new FileReader(f);
			BufferedReader br=new BufferedReader(fr);
		
			String line=null;//读一行数据
			String[] content;//一行数据中三个字符串构成的字符串数组
			String account;
			String password;
			String contact;
		
			line=br.readLine();//不用第一行中文数据
			while((line=br.readLine())!=null)
			{
				content=line.split(" ");
			
				account=content[0];
				password=content[1];
				contact=content[2];
				
				User user=new User(account,password,contact);//一个用户
				userList.add(user);//添加到用户顺序表
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
	
	//判断是否包含某一用户
	public boolean contains(String account)
	{
		boolean flag=false;
		Iterator<User> it=this.userList.iterator();
				
		while(it.hasNext())
		{
			User u=it.next();
			if(u.getAccount().equals(account))//存在该用户账号时标志置位
			{
				flag=true;
				break;//不再比较
			}
		}
		return flag;
	}
	
	//判断用户账号、密码是否正确
	public String login(String account,String password)
	{
		//判断账号或密码错误
		String judgement="";
		boolean accountFlag=false;
		boolean passwordFlag=false;
		
		Iterator<User> it=this.userList.iterator();
		while(it.hasNext())//每个用户
		{
			User u=it.next();
			
			if(u.getAccount().equals(account))//账号匹配
			{
				accountFlag=true;
				
				if(u.getPassword().equals(password))//密码匹配
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
	
	//用户注册方法
	public void register(String account,String password,String contact)
	{
		//修改顺序表
		User user=new User(account,password,contact);
		userList.add(user);
			
		//修改文件
		try
		{
			//文件类
			File f=new File("C:\\Users\\DSHH\\Desktop\\数据集\\user.txt");
				
			//文件字符输出流，字符缓冲输出流
			FileWriter fw=new FileWriter(f,true);//追加
			BufferedWriter bw=new BufferedWriter(fw);
				
			String line=account+" "+password+" "+contact;
			bw.write(line);
			bw.newLine();
				
			//关闭流
			bw.close();
			fw.close();
		}
		catch(IOException ioe)
		{
			System.out.println(ioe);
		}
	}
	
	//根据用户账号查看用户信息
	public String show(String account)
	{
		Iterator<User> it=userList.iterator();
		String line=null;
		
		while(it.hasNext())
		{
			User u=it.next();//每一个用户
			if(u.getAccount().equals(account))//字符串String类值使用equals()方法比较
			{
				line=u.getAccount()+" "+u.getPassword()+" "+u.getContact();
				break;//不再匹配
			}	
		}
		return line;
	}
	
	//修改用户信息
	public void modify(String oldAccount,String newLine)
	{
		//获取新的用户信息
		String[] content=newLine.split(" ");
		String newAccount=content[0];
		String newPasssword=content[1];
		String newContact=content[2];
		
		//修改用户顺序表
		Iterator<User> it=userList.iterator();
		
		while(it.hasNext())
		{
			User u=it.next();
			if(u.getAccount().equals(oldAccount))//匹配旧帐号
			{
				u.setAccount(newAccount);
				u.setPassword(newPasssword);
				u.setContact(newContact);
				break;//不再匹配
			}
		}
		
		//写文件
		ArrayList<String> data=new ArrayList<String>();//使用顺序表存储数据，修改后再写入文件	因为有第一行中文
		try
		{
			File f=new File("C:\\Users\\DSHH\\Desktop\\数据集\\user.txt");
			
			//读数据
			FileReader fr=new FileReader(f);
			BufferedReader br=new BufferedReader(fr);
			
			String line=null;//读一行数据
			
			line=br.readLine();//第一行中文
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
				String s=it1.next();//每一行是一个用户信息
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
	
	//注销用户
	public void logout(String account)
	{
		//修改用户顺序表
		Iterator<User> it=userList.iterator();
				
		while(it.hasNext())
		{
			User u=it.next();
			if(u.getAccount().equals(account))//匹配账号
			{
				it.remove();//使用迭代器方法删除，不会破坏迭代器结构
			}
		}
		
		//修改文件
		ArrayList<String> data=new ArrayList<String>();
		try
		{
			File f=new File("C:\\Users\\DSHH\\Desktop\\数据集\\user.txt");
			
			//读数据
			FileReader fr=new FileReader(f);
			BufferedReader br=new BufferedReader(fr);
			
			String line=null;//读一行数据
			
			line=br.readLine();//第一行中文
			data.add(line);
			while((line=br.readLine())!=null)
			{	
				if(!(line.startsWith(account)))//不是要删除的信息都存入顺序表
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
				bw.newLine();//必须写入回车换行
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
		//UserList userList=new UserList();
	}
}