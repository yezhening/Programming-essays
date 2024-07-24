package userSystem;

import dataSet.*;
import loginSystem.*;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

@SuppressWarnings("serial")
public class UserRegister extends JFrame implements ActionListener
{
	JLabel newUserLabel=new JLabel("请填写新用户信息");
	
	JLabel newAccountLabel=new JLabel("请输入账号：");
	JTextField newAccount=new JTextField(20);
	
	JLabel newPasswordLabel=new JLabel("请输入密码：");
	JTextField newPassword=new JTextField(20);
	
	JLabel newContactLabel=new JLabel("请输入联系方式：");
	JTextField newContact=new JTextField(20);
	
	JButton goBack=new JButton("重新登录");
	JButton register=new JButton("确定注册");
	
	JLabel showLabel=new JLabel("无反馈信息");
	
	public UserRegister()
	{
		//北区
		JPanel north=new JPanel();
		north.add(newUserLabel);
		
		this.add(north,BorderLayout.NORTH);
		
		//中区
		Box box1=Box.createVerticalBox();
		box1.add(Box.createVerticalStrut(16));
		box1.add(newAccountLabel);
		box1.add(Box.createVerticalStrut(10));
		box1.add(newPasswordLabel);
		box1.add(Box.createVerticalStrut(10));
		box1.add(newContactLabel);
		
		Box box2=Box.createVerticalBox();
		box2.add(Box.createVerticalStrut(16));
		box2.add(newAccount);
		box2.add(Box.createVerticalStrut(10));
		box2.add(newPassword);
		box2.add(Box.createVerticalStrut(10));
		box2.add(newContact);
		
		Box box3=Box.createHorizontalBox();
		box3.add(goBack);
		goBack.addActionListener(this);
		box3.add(Box.createHorizontalStrut(30));
		box3.add(register);
		register.addActionListener(this);
		
		Box box12=Box.createHorizontalBox();
		box12.add(box1);
		box12.add(Box.createHorizontalStrut(30));
		box12.add(box2);
		
		Box box123=Box.createVerticalBox();
		box123.add(box12);
		box123.add(Box.createVerticalStrut(30));
		box123.add(box3);
		
		JPanel center=new JPanel();
		center.add(box123);
		
		this.add(center,BorderLayout.CENTER);
		
		//南区
		JPanel south=new JPanel();
		showLabel.setForeground(Color.blue);
		south.add(showLabel);
		
		this.add(south,BorderLayout.SOUTH);
	
		//窗体
		this.setTitle("欢迎来到用户注册界面");
		this.setLocation(100,100);
		this.setSize(400,300);
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	
	//监听方法
	public void actionPerformed(ActionEvent e) 
	{
		//重新登录
		if(e.getSource()==goBack)
		{
			this.dispose();
			new LoginInterface();
		}	
		
		//注册
		if(e.getSource()==register)
		{
			String account=newAccount.getText();
			String password=newPassword.getText();
			String contact=newContact.getText();
			
			if(account.equals("")||password.equals("")||contact.equals(""))//使用""而不是null
			{
				showLabel.setText("注册失败！请填写完整注册信息。");
			}
			else
			{
				UserList userList=new UserList();
				if(userList.contains(account))//已存在该用户账号
				{
					newAccount.setText("");
					showLabel.setText("注册失败！该用户账号已存在。");
				}
				else//不存在该用户账号
				{
					userList.register(account,password,contact);
					showLabel.setText("注册成功！");
				}
			}
		}	
	}
	
	//主方法
	public static void main(String[] args)
	{
		new UserRegister();
	}
}