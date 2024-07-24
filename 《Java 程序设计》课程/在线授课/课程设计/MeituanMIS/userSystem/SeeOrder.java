package userSystem;

import dataSet.*;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

@SuppressWarnings("serial")
public class SeeOrder extends JFrame implements ActionListener
{
	JLabel seeLabel=new JLabel("用户订单显示如下：");
	
	JTextArea show=new JTextArea();
	
	JButton goBack=new JButton("想返回上一级");
	
	String account;
	ShopList shopList;
	Multithread multithread;
	
	//构造方法
	public SeeOrder(String account,ShopList shopList,Multithread multithread)
	{
		this.account=account;
		this.shopList=shopList;
		this.multithread=multithread;
		
		//取订单信息内容
		String showContent="";//不能放在try里
		OrderList orderList=new OrderList();
		showContent=showContent+orderList.seeUserOrder(this.account);
		
		//窗体界面
		//北区
		JPanel north=new JPanel();
		north.add(seeLabel);
		
		this.add(north,BorderLayout.NORTH);
		
		//中区
		JScrollPane center=new JScrollPane(show);
		show.setText(showContent);
		
		this.add(center,BorderLayout.CENTER);
		
		//南区
		JPanel south=new JPanel();
		south.add(goBack);
		goBack.addActionListener(this);
		
		this.add(south,BorderLayout.SOUTH);
		
		//窗口
		this.setTitle("查询用户订单");
		this.setLocation(100,100);
		this.setSize(400,300);
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}

	//监听方法
	public void actionPerformed(ActionEvent e) 
	{
		//返回上一级
		if(e.getSource()==goBack)
		{
			this.dispose();
			new UserInterface(this.account,this.shopList,this.multithread);
		}
	}
	
	//主方法
	public static void main(String[] args)
	{
		//需要参数
		//new SeeOrder();
	}
}