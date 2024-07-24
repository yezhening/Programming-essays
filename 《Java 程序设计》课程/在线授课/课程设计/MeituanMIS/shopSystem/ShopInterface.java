package shopSystem;

import dataSet.*;
import loginSystem.*;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

//商家界面
@SuppressWarnings("serial")
public class ShopInterface extends JFrame implements ActionListener
{	
	//组件
	JLabel operationsLabel=new JLabel("请选择要执行的功能");
	
	JButton shopInformationManagement=new JButton("商家信息管理"); 
	JButton shopFoodHanding=new JButton("商家菜品处理"); 
	JButton shopOrdeManagement=new JButton("商家预定管理");
	JButton goBack=new JButton("想要重新登录");
	
	//商家账号
	String account;
	//商家信息表
	ShopList shopList;
	
	//构造方法
	public ShopInterface(String account,ShopList shopList)
	{
		this.account=account;
		this.shopList=shopList;
		
		//中区
		Box box=Box.createVerticalBox();
		box.add(operationsLabel);
		box.add(Box.createVerticalStrut(30));
		box.add(shopInformationManagement);
		shopInformationManagement.addActionListener(this);
		box.add(Box.createVerticalStrut(20));
		box.add(shopFoodHanding);
		shopFoodHanding.addActionListener(this);
		box.add(Box.createVerticalStrut(20));
		box.add(shopOrdeManagement);
		shopOrdeManagement.addActionListener(this);
		box.add(Box.createVerticalStrut(20));
		box.add(goBack);
		goBack.addActionListener(this);
					
		JPanel center=new JPanel();
		center.add(box);
				
		this.add(center,BorderLayout.CENTER);
				
		//窗口条件
		this.setTitle("尊敬的商家，欢迎您！");
		this.setLocation(100,100);
		this.setSize(400,300);
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}

	//监听方法
	public void actionPerformed(ActionEvent e) 
	{
		ShopLoginInformation shopLoginInformation=new ShopLoginInformation();//商家登录信息
		
		//商家信息管理
		if(e.getSource()==shopInformationManagement)
		{
			this.dispose();
			new ShopInformationManagement(this.account,this.shopList);
		}
		
		//商家菜品处理
		if(e.getSource()==shopFoodHanding)
		{
			this.dispose();
			new ShopFoodHanding(this.account,this.shopList);
		}
		
		//商家预定管理
		if(e.getSource()==shopOrdeManagement)
		{
			this.dispose();
			new ShopOrdeManagement(this.account,this.shopList);
		}
		
		//重新登录
		if(e.getSource()==goBack)
		{
			shopLoginInformation.remove(account);//重新登录删除登录状态
			this.dispose();
			new LoginInterface();
		}
	}
	
	//主方法
	public static void main(String[] args)
	{
		//需要参数
		//new ShopInterface();
	}
}