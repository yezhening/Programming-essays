package administratorSystem;

import dataSet.*;
import userSystem.*;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

@SuppressWarnings("serial")
public class UserInformationManagement extends JFrame implements ActionListener
{
	//组件
	JLabel operationsLabel=new JLabel("请选择要执行的功能");
		
	JButton seeAll=new JButton("查看所有用户的登录信息"); 
	JButton seeOne=new JButton("查询某一用户的登录信息"); 
	JButton deleteOne=new JButton("删除某一用户的登录信息");
	JButton goBack=new JButton("返回上一级");
		
	JLabel inputLabel=new JLabel("请输入:");
	
	JTextField input=new JTextField(18);
	
	JTextArea show=new JTextArea("说明：请在上方输入框输入某一用户账号信\n息，再点击查询、删除按钮执行功能。",7,25);
	
	//管理员账号
	String account;
	//商家信息表
	ShopList shopList;
	
	//构造方法
	public UserInformationManagement(String account,ShopList shopList)
	{
		this.account=account;
		this.shopList=shopList;
			
		//中区
		JPanel center1=new JPanel();
		center1.add(operationsLabel);
		center1.add(seeAll);
		seeAll.addActionListener(this);
		center1.add(seeOne);
		seeOne.addActionListener(this);
		center1.add(deleteOne);
		deleteOne.addActionListener(this);
		center1.add(goBack);
		goBack.addActionListener(this);
		
		JScrollPane showJSP=new JScrollPane(show);
		
		JPanel center2=new JPanel();
		center2.add(inputLabel);
		center2.add(input);
		center2.add(showJSP);
		
		JSplitPane center=new JSplitPane(JSplitPane.HORIZONTAL_SPLIT,center1,center2);//JSplitPane的使用
        center.setDividerLocation(200);
 		
		this.add(center,BorderLayout.CENTER);
				
		//窗口条件
		this.setTitle("用户信息管理");
		this.setLocation(200,200);
		this.setSize(500,200);
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}	

	public void actionPerformed(ActionEvent e) 
	{
		 UserLoginInformation userLoginInformation=new UserLoginInformation();
		
		if(e.getSource()==seeAll)
		{
			String showText=userLoginInformation.showUser();
			
			show.setText(showText);
		}
		
		if(e.getSource()==seeOne)
		{
			String seeAccount=input.getText();
			
			if(seeAccount.equals(""))
			{
				show.setText("请输入查询用户账号。");
			}
			else
			{
				//判断账号是否登录
				if(userLoginInformation.contains(seeAccount))//查询账号匹配
				{
					show.setText("该用户已登录。");
				
				}
				else
				{
					show.setText("该用户未登录。");
				}
			}	
		}
		
		//删除某一用户
		if(e.getSource()==deleteOne)
		{
			String deleteAccount=input.getText();
			
			if(deleteAccount.equals(""))
			{
				show.setText("请输入删除用户账号。");
			}
			else
			{
				if(userLoginInformation.contains(deleteAccount))
				{
					userLoginInformation.remove(deleteAccount);
					show.setText("已删除该用户登录信息。");
				}
				else
				{
					show.setText("该用户未登录。");
				}
			}
		}
		
		//返回上一级
		if(e.getSource()==goBack)
		{
			this.dispose();
			new AdministratorInterface(this.account,this.shopList);//跳转管理员界面
		}
	}	
	
	//主方法
	public static void main(String[] args)
	{
		//new UserInformationManagement();	
	}
}