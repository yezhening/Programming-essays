package userSystem;

import dataSet.*;
import loginSystem.*;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

@SuppressWarnings("serial")
public class UserInterface extends JFrame implements ActionListener
{
	JLabel operationsLabel=new JLabel("请选择要执行的功能");
	
	JButton personalInformationManagement=new JButton("个人信息管理"); 
	JButton seeShopInformation=new JButton("查看商家信息"); 
	JButton foodOrder=new JButton("想要预定菜品");
	JButton orderInquiry=new JButton("查询用户订单");
	JButton goBack=new JButton("想要重新登录");
	
	String account;//用户界面需要传入用户账号作为成员对象
	ShopList shopList;
	Multithread multithread;
	
	//构造方法
	public UserInterface(String account,ShopList shopList,Multithread multithread)
	{
		this.account=account;
		this.shopList=shopList;
		this.multithread=multithread;
		
		//中区
		Box box=Box.createVerticalBox();
		box.add(operationsLabel);
		box.add(Box.createVerticalStrut(30));
		box.add(personalInformationManagement);
		personalInformationManagement.addActionListener(this);
		box.add(Box.createVerticalStrut(10));
		box.add(seeShopInformation);
		seeShopInformation.addActionListener(this);
		box.add(Box.createVerticalStrut(10));
		box.add(foodOrder);
		foodOrder.addActionListener(this);
		box.add(Box.createVerticalStrut(10));
		box.add(orderInquiry);
		orderInquiry.addActionListener(this);
		box.add(Box.createVerticalStrut(10));
		box.add(goBack);
		goBack.addActionListener(this);
	
		JPanel center=new JPanel();
		center.add(box);
		
		this.add(center,BorderLayout.CENTER);
		
		//窗口
		this.setTitle("尊敬的用户，欢迎您！");
		this.setLocation(100,100);
		this.setSize(400,300);
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}

	//监听方法
	public void actionPerformed(ActionEvent e) 
	{
		UserLoginInformation userLoginInformation=new UserLoginInformation();//用户登录信息
		
		//个人信息管理
		if(e.getSource()==personalInformationManagement)
		{
			this.dispose();
			new PersonalInformationManagement(this.account,this.shopList,this.multithread);
		}
		
		//查看商家信息
		if(e.getSource()==seeShopInformation)
		{
			this.dispose();
			new SeeShopInformation(this.account,this.shopList,this.multithread);
		}
		
		//菜品预定
		if(e.getSource()==foodOrder)
		{	
			//实际用户线程
			//this.dispose();
			//new ActualFoodOrder(account,shopList,multithread);
			
			//虚拟用户线程
			this.dispose();
			new VirtualFoodOrder(account,shopList,multithread);
		}
		
		//查询用户订单
		if(e.getSource()==orderInquiry)
		{
			this.dispose();
			new SeeOrder(account,shopList,multithread);
		}
		
		//重新登录
		if(e.getSource()==goBack)
		{
			userLoginInformation.remove(account);//重新登录删除登录状态
			this.dispose();
			new LoginInterface();
		}
	}
	
	//主方法
	public static void main(String[] args)
	{
		//需要参数
		//new UserInterface();
	}
}