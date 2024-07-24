package administratorSystem;

import java.util.*;
import java.io.*;

public class AdministratorLoginInformation 
{
	//用户登录表
	LinkedList<String> administratorsLoginTable = new LinkedList<String>();
	
	//构造方法初始化链表
	public AdministratorLoginInformation()
	{
		//读数据
		try
		{
			//File类
			File f=new File("C:\\Users\\DSHH\\Desktop\\数据集\\administratorLoginInformation.txt");
			
			//文件字符输入流、字符缓冲输入流
			FileReader fr=new FileReader(f);
			BufferedReader br=new BufferedReader(fr);
		
			String account=null;//读一行数据，存的是用户账号
			
			while((account=br.readLine())!=null)
			{
				this.administratorsLoginTable.add(account);//用户登录链表
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
	
	//管理员是否登录（在登录表中）
	public boolean contains(String account)
	{
		return this.administratorsLoginTable.contains(account);
	}
	
	//添加登录用户账号
	public void add(String account)
	{
		//修改链表
		this.administratorsLoginTable.add(account);
		
		//修改文件
		try
		{
			File f=new File("C:\\Users\\DSHH\\Desktop\\数据集\\administratorLoginInformation.txt");
		
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
		Iterator<String> it=administratorsLoginTable.iterator();
		while(it.hasNext())
		{
			String s=it.next();
			if(s.equals(account))
			{
				it.remove();
			}
		}
		
		//修改文件
		ArrayList<String> data=new ArrayList<String>();
				
		try
		{
			File f=new File("C:\\Users\\DSHH\\Desktop\\数据集\\administratorLoginInformation.txt");
					
			//读数据
			FileReader fr=new FileReader(f);
			BufferedReader br=new BufferedReader(fr);
					
			String line=null;//读一行数据
					
			line=br.readLine();
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
}