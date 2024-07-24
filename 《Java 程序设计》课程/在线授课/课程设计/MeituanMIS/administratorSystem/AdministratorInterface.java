package administratorSystem;

import dataSet.*;
import loginSystem.*;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

//管理员界面
@SuppressWarnings("serial")
public class AdministratorInterface extends JFrame implements ActionListener
{	
	//组件
	JLabel operationsLabel=new JLabel("请选择要执行的功能");
	
	JButton administratorInformationManagement=new JButton("个人信息管理"); 
	JButton userInformationManagement=new JButton("用户信息管理"); 
	JButton shopInformationManagement=new JButton("商家信息管理");
	JButton goBack=new JButton("想要重新登录");
	
	//管理员账号
	String account;
	//商家信息表
	ShopList shopList;
	
	//构造方法
	public AdministratorInterface(String account,ShopList shopList)
	{
		this.account=account;
		this.shopList=shopList;
		
		//北区
		JPanel north=new JPanel();
		north.add(operationsLabel);
		
		this.add(north,BorderLayout.NORTH);
		
		//中区
		Box box=Box.createVerticalBox();
		box.add(Box.createVerticalStrut(10));
		box.add(administratorInformationManagement);
		administratorInformationManagement.addActionListener(this);
		box.add(Box.createVerticalStrut(30));
		box.add(userInformationManagement);
		userInformationManagement.addActionListener(this);
		box.add(Box.createVerticalStrut(30));
		box.add(shopInformationManagement);
		shopInformationManagement.addActionListener(this);
		box.add(Box.createVerticalStrut(30));
		box.add(goBack);
		goBack.addActionListener(this);
		
		JPanel center=new JPanel();
		center.add(box);
		
		this.add(center,BorderLayout.CENTER);
				
		//窗口条件
		this.setTitle("尊敬的管理员，欢迎您！");
		this.setLocation(200,200);
		this.setSize(400,300);
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}

	//监听方法
	public void actionPerformed(ActionEvent e) 
	{
		//系统管理员个人信息管理
		if(e.getSource()==administratorInformationManagement)
		{
			this.dispose();
			new AdministratorInformationManagement(this.account,this.shopList);
		}
		
		//用户信息管理
		if(e.getSource()==userInformationManagement)
		{
			this.dispose();
			new UserInformationManagement(this.account,this.shopList);
		}
			
		//商家信息管理
		if(e.getSource()==shopInformationManagement)
		{
			this.dispose();
			new ShopInformationManagement(this.account,this.shopList);
		}
		
		//重新登录
		if(e.getSource()==goBack)
		{
			AdministratorLoginInformation administratorLoginInformation=new AdministratorLoginInformation();
			administratorLoginInformation.remove(this.account);
			
			this.dispose();
			new LoginInterface();
		}
	}
	
	//主方法
	public static void main(String[] args)
	{
		//需要参数
		//new AdministratorInterface();
	}
}
