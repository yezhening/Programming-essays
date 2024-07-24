package shopSystem;

import java.util.*;
import java.io.*;

public class ShopLoginInformation 
{
	//商家登录表
	LinkedList<String> shopsLoginTable = new LinkedList<String>();
		
	//构造方法初始化链表
	public ShopLoginInformation()
	{
		//读数据
		try
		{
			//File类
			File f=new File("C:\\Users\\DSHH\\Desktop\\数据集\\shopLoginInformation.txt");
				
			//文件字符输入流、字符缓冲输入流
			FileReader fr=new FileReader(f);
			BufferedReader br=new BufferedReader(fr);
			
			String account=null;//读一行数据，存的是商家账号
				
			while((account=br.readLine())!=null)
			{
				shopsLoginTable.add(account);//商家登录链表
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
	
	//商家是否登录（在登录表中）
	public boolean contains(String account)
	{
		return shopsLoginTable.contains(account);
	}
	
	//添加登录商家账号
	public void add(String account)
	{
		//修改链表
		shopsLoginTable.add(account);
			
		//修改文件
		try
		{
			File f=new File("C:\\Users\\DSHH\\Desktop\\数据集\\shopLoginInformation.txt");
			
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
	
	//删除商家登录账号
	public void remove(String account)
	{
		//修改链表
		Iterator<String> it=this.shopsLoginTable.iterator();
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
			File f=new File("C:\\Users\\DSHH\\Desktop\\数据集\\shopLoginInformation.txt");
						
			//写数据
			FileWriter fw=new FileWriter(f);
			BufferedWriter bw=new BufferedWriter(fw);
						
			Iterator<String> it1=this.shopsLoginTable.iterator();
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
	
	//主方法
	public static void main(String[] args) 
	{
		new ShopLoginInformation();
	}
}