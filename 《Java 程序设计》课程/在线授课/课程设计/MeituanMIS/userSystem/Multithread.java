package userSystem;

import dataSet.*;

import java.util.*;//Iterator、ArrayList;

public class Multithread implements Runnable
{	
	private String account;//用户账号
	//临界资源
	private ShopList shopList;//接收商家顺序表信息作为线程共享内容（临界资源）
	
	private	ArrayList<Order> orderList1;
	private	ArrayList<Order> orderList2;
	
	private String showText="\n***********************************************\n";//传回页面的显示内容
	
	//构造方法
	public Multithread(String account,ShopList shopList)//实际用户
	{
		this.account=account;
		this.shopList=shopList;
	}
	
	//适应虚拟用户,设置成员变量
	public void setOrderList1(ArrayList<Order> orderList1)
	{
		this.orderList1=orderList1;
	}
	
	//获取成员变量，线程运行后传给界面的内容
	public String passShowText()
	{
		return this.showText;
	}
	
	public void setOrderList2(ArrayList<Order> orderList2)
	{
		this.orderList2=orderList2;
	}
	
	//线程运行调用方法
	public void run()
	{
		start();
		foodOrder();
	}
	
	//启动方法
	public synchronized void start()
	{
		this.showText=this.showText+Thread.currentThread().getName()+"启动\n";//一般是虚拟用户线程1先启动
	}
	
	//菜品预定同步方法
	public synchronized void foodOrder()
	{
		this.showText=this.showText+Thread.currentThread().getName()+"开始预定\n\n";	
		
		//不同虚拟用户处理不同订单
		if(Thread.currentThread().getName().equals("实际用户"+this.account+"的虚拟用户线程1"))
		{
			this.showText=this.showText+shopList.handleOrder(orderList1);//“虚拟”商家表处理订单,传订单顺序表,取返回界面内容
		}
		else
		{
			this.showText=this.showText+this.shopList.handleOrder(orderList2);
		}
		
		this.showText=this.showText+"\n";
	}//预定方法
	
	//主方法
	public static void main(String[] args)
	{
		//需要参数
		//new Multithread();
	}
}