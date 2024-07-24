package userSystem;

import dataSet.*;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

import java.util.*;//ArrayList;

@SuppressWarnings("serial")
public class VirtualFoodOrder extends JFrame implements ActionListener
{
	//组件
		JLabel seeLabel=new JLabel("请输入：");
			
		JTextField input=new JTextField(24);
			
		JTextArea show=new JTextArea("说明：输入商家（餐馆）名字的全称，查看某一具体商家（餐馆）的信\n息；输入一个关键词，查看名字中包含此关键词的多个商家（餐馆）的\n信息。敲击回车查询。\n",4,36);
		JTextArea order=new JTextArea("在文字下方输入预定餐馆名称、菜品名称，菜品数量随机生成，中间以空\n格分隔，一份菜品即为一个订单，一个订单为一行；单击按钮进行预定:\n餐馆名称	菜品名称	菜品数量\n——————————————————\n",4,36);	
		
		JButton goBack=new JButton("想返回上一级");
		JButton confirm=new JButton("确定预定");
			
		//传参
		String account;
		ShopList shopList;
		Multithread multithread;
	
	//构造方法
	public VirtualFoodOrder(String account,ShopList shopList,Multithread multithread)
	{	
		this.account=account;
		this.shopList=shopList;
		this.multithread=multithread;	
		
		//北区
		JPanel north=new JPanel();
		north.add(seeLabel);
		north.add(input);
		input.addActionListener(this);
			
		this.add(north,BorderLayout.NORTH);
			
		//中区
		JScrollPane center1 =new JScrollPane(show);
		JScrollPane center2 =new JScrollPane(order);
		
		Box box=Box.createVerticalBox();
		box.add(center1);
		box.add(Box.createVerticalStrut(10));
		box.add(center2);
		
		this.add(box,BorderLayout.CENTER);

		//南区
		JPanel south=new JPanel();
		south.add(goBack);
		goBack.addActionListener(this);
		south.add(confirm);
		confirm.addActionListener(this);
			
		this.add(south,BorderLayout.SOUTH);
			
		//窗口
		setTitle("菜品预定");
		setLocation(200,200);
		setSize(400,500);
		setVisible(true);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
		
	//监听方法
	public void actionPerformed(ActionEvent e) 
	{
		//查询方法
		if(e.getSource()==input)
		{
			String inputContent=input.getText();//匹配信息
					
			String content="";
			if(inputContent.equals(""))
			{
				content = "请输入查询内容!";
			}
			else
			{
				content = "查询内容显示如下：\n";;//显示的所有内容+返回给显示界面的匹配到的商家信息			
				content=content+shopList.query(inputContent);
			}
			
			show.setText(content);
		}
		
		//预定方法
		if(e.getSource()==confirm)
		{
			show.setText("");//清空显示信息
			
			//处理预定信息
			String content=order.getText();//预定文本域内容
			String[] ordersContent=content.split("\n");//所有订单内容
			
			show.append("虚拟用户线程1预定菜品及数量：\n——————————————\n");
			ArrayList<Order> orderList1=new ArrayList<Order>();//虚拟用户1的订单顺序表
			for(int i=4;i<ordersContent.length;i++)//对每一份订单
			{
				String[] orderContent=ordersContent[i].split(" ");//订单内容
				String shop=orderContent[0];
				String foodName=orderContent[1];
				
				int randomFoodCount=(int)(Math.random()*10+1);//随机生成预定1-10份菜品数量
				String foodCount=String.valueOf(randomFoodCount);//int转换为String
				
				Order order=new Order(this.account,shop,foodName,foodCount);//一份订单
				orderList1.add(order);//添加
				
				show.append(shop+" "+foodName+" "+foodCount+"\n");
			}
			
			show.append("\n");
			
			show.append("虚拟用户线程2预定菜品及数量：\n——————————————\n");
			ArrayList<Order> orderList2=new ArrayList<Order>();//虚拟用户2的订单顺序表
			for(int i=4;i<ordersContent.length;i++)//对每一份订单
			{
				String[] orderContent=ordersContent[i].split(" ");//订单内容
				String shop=orderContent[0];
				String foodName=orderContent[1];
				
				int randomFoodCount=(int)(Math.random()*10+1);//随机生成预定1-10份菜品数量
				String foodCount=String.valueOf(randomFoodCount);//int转换为String
				
				Order order=new Order(this.account,shop,foodName,foodCount);//一份订单
				orderList2.add(order);//添加
				
				show.append(shop+" "+foodName+" "+foodCount+"\n");
			}
					
			//多线程
			//实现runnable接口的对象，每个虚拟用户线程应使用同一个实现runnable接口的对象
			Multithread multithread=new Multithread(this.account,this.shopList);
			multithread.setOrderList1(orderList1);
			multithread.setOrderList2(orderList2);
			
			//虚拟用户线程，一个实际用户线程启动两个虚拟用户线程
			Thread userThread1=new Thread(multithread);//创建虚拟用户线程
			Thread userThread2=new Thread(multithread);
			userThread1.setName("实际用户"+this.account+"的虚拟用户线程1");
			userThread2.setName("实际用户"+this.account+"的虚拟用户线程2");
			
			userThread1.start();//启动线程
			userThread2.start();
			
			//等待线程运行一会，再取得返回值
			try 
			{
				Thread.sleep(1000);//10s
			} 
			catch (InterruptedException e1) 
			{
				e1.printStackTrace();
			}
			
			String showText=multithread.passShowText();//从多线程运行传回来的文本显示内容
			
			show.append(showText);
		}
		
		//返回上一级
		if(e.getSource()==goBack)
		{
			this.dispose();
			new UserInterface(this.account,shopList,this.multithread);
		}		
	}
	
	//主方法
	public static void main(String[] args)
	{
		//需要参数
		//new VirtualFoodOrder();
	}
}