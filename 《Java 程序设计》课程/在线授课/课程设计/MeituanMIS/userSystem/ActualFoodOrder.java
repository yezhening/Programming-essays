package userSystem;

import dataSet.*;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

import java.util.*;//ArrayList、Iterator

@SuppressWarnings("serial")
public class ActualFoodOrder extends JFrame implements ActionListener
{
	//组件
	JLabel seeLabel=new JLabel("请输入：");
		
	JTextField input=new JTextField(24);
		
	JTextArea show=new JTextArea("说明：输入商家（餐馆）名字的全称，查看某一具体商家（餐馆）的信\n息；输入一个关键词，查看名字中包含此关键词的多个商家（餐馆）的\n信息。敲击回车查询。\n",4,36);
	JTextArea order=new JTextArea("在文字下方输入预定餐馆名称、菜品名称及数量，中间以空格分隔，一\n份菜品即为一个订单，一个订单为一行；单击按钮进行预定:\n餐馆名称	菜品名称	菜品数量\n———————————————————\n",4,36);	
	
	JButton goBack=new JButton("想返回上一级");
	JButton confirm=new JButton("确定预定");
		
	//传参
	String account;
	ShopList shopList;
	Multithread multithread;
		
	//构造方法
	public ActualFoodOrder(String account,ShopList shopList,Multithread multithread)
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
		Box box1=Box.createHorizontalBox();
		box1.add(goBack);
		goBack.addActionListener(this);
		box1.add(Box.createHorizontalStrut(100));
		box1.add(confirm);
		confirm.addActionListener(this);
		
		JPanel south=new JPanel();
		south.add(box1);
			
		this.add(south,BorderLayout.SOUTH);
			
		//窗口
		this.setTitle("菜品预定");
		this.setLocation(200,200);
		this.setSize(400,500);
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
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
		
		//预定菜品
		if(e.getSource()==confirm)
		{
			//处理预定信息
			String content=order.getText();//预定文本域内容
			String[] ordersContent=content.split("\n");//所有订单内容
			
			
			ArrayList<Order> orderList=new ArrayList<Order>();//订单顺序表
			for(int i=4;i<ordersContent.length;i++)//对每一份订单
			{
				String[] orderContent=ordersContent[i].split(" ");//订单内容
				String shop=orderContent[0];
				String foodName=orderContent[1];
				String foodCount=orderContent[2];
				
				Order order=new Order(this.account,shop,foodName,foodCount);//一份订单
				orderList.add(order);//添加
			}
			
			String showContent=this.shopList.handleOrder(orderList);//“虚拟”商家表处理订单,传订单顺序表,取返回界面内容
		
		
			show.setText(showContent);		
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
		//new ActualFoodOrder();
	}
}