package dataSet;

import java.io.*;
import java.util.*;//ArrayList、Iterator

public class OrderList 
{
	//订单顺序表
	ArrayList<Order> orderList = new ArrayList<Order>();
	
	//构造方法初始化顺序表
	public OrderList()
	{
		//读数据
		try
		{
			//File类
			File f=new File("C:\\Users\\DSHH\\Desktop\\数据集\\order.txt");
			
			//文件字符输入流、字符缓冲输入流
			FileReader fr=new FileReader(f);
			BufferedReader br=new BufferedReader(fr);
			
			String line=null;//读一行数据
			String[] content;//一行数据中四个字符串构成的字符串数组
			
			String account;
			String shop;
			String foodName;
			String foodCount;
			
			if((line=br.readLine())!=null)//第一行数据
			{
				content=line.split(" ");
				
				account=content[0];
				shop=content[1];
				foodName=content[2];
				foodCount=content[3];
				
				Order order=new Order(account,shop,foodName,foodCount);//一个订单
				orderList.add(order);//订单表
			}
			while((line=br.readLine())!=null)
			{
				content=line.split(" ");
				
				account=content[0];
				shop=content[1];
				foodName=content[2];
				foodCount=content[3];
				
				Order order=new Order(account,shop,foodName,foodCount);//一个订单
				orderList.add(order);//订单表
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
	
	//取成员
	public ArrayList<Order> getOrderList()
	{
		return this.orderList;
	}
	
	//用户查看订单
	public String seeUserOrder(String account)
	{
		Iterator<Order> it=this.orderList.iterator();
		String content="用户账号 预定餐馆 预定菜品 预定数量\n—————————————————\n";
		boolean orderFlag=false;//判断是否查询到订单	
		
		while(it.hasNext())
		{
			Order o=it.next();
				
			if(o.getAccount().equals(account))//用户账号匹配
			{
				orderFlag=true;
				content=content+o.getAccount()+" "+o.getFoodName()+" "+o.getFoodCount()+"\n";
			}
		}
		if(!orderFlag)//用户无订单信息
		{
			content="该用户无订单信息。";
		}
		
		return content;
	}
	
	//商家查看订单
	public String seeShopOrder(String shop)
	{
		Iterator<Order> it=this.orderList.iterator();
		
		//传回界面的内容
		String content="";
		
		while(it.hasNext())
		{
			Order o=it.next();
			
			if(o.getShop().equals(shop))//商家名称匹配
			{
				content=content+"\n"+o.getAccount()+" "+o.getFoodName()+" "+o.getFoodCount();
			}
		}
		
		return content;
	}
	
	//商家删除订单
	public void deleteOrder(String userAccount,String foodName)
	{
		//修改顺序表
		Iterator<Order> it=this.orderList.iterator();
			
		while(it.hasNext())
		{
			Order o=it.next();
				
			if(o.getAccount().equals(userAccount))//用户帐号匹配
			{
				if(o.getFoodName().equals(foodName))//订单菜品匹配
				{
					it.remove();
				}
			}
		}
		
		//修改文件
		try
		{
			//File类
			File f=new File("C:\\Users\\DSHH\\Desktop\\数据集\\order.txt");
					
			//输出流
			FileWriter fw=new FileWriter(f);
			BufferedWriter bw=new BufferedWriter(fw);
			
			Iterator<Order> it1=this.orderList.iterator();
			while(it1.hasNext())
			{
				Order o=it1.next();
				
				String line=o.getAccount()+" "+o.getShop()+" "+o.getFoodName()+" "+o.getFoodCount();
				bw.write(line);
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
		//new OrderList();
	}
}