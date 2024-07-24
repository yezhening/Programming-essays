package userSystem;

import dataSet.*;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

@SuppressWarnings("serial")
public class PersonalInformationManagement extends JFrame implements ActionListener
{
	//组件
	JLabel operationsLabel=new JLabel("请选择要执行的功能");
	
	JButton see=new JButton("查看个人信息"); 
	JButton modify=new JButton("修改个人信息"); 
	JButton logout=new JButton("注销个人信息");
	JButton goBack=new JButton("想返回上一级");
	
	JTextArea show=new JTextArea("这里显示信息。",4,30);
	JTextField operate=new JTextField("这里操作信息。");
	
	String account;//用户界面需要传入用户账号作为成员对象
	ShopList shopList;
	Multithread multithread;
	
	//构造方法
	public PersonalInformationManagement(String account,ShopList shopList,Multithread multithread)
	{
		this.account=account;
		this.shopList=shopList;
		this.multithread=multithread;
		
		//北区
		JPanel north=new JPanel();
		north.add(operationsLabel);
		
		this.add(north,BorderLayout.NORTH);
		
		//中区
		Box box1=Box.createHorizontalBox();
		box1.add(see);
		see.addActionListener(this);
		box1.add(Box.createHorizontalStrut(20));
		box1.add(modify);
		modify.addActionListener(this);
		
		Box box2=Box.createHorizontalBox();
		box2.add(logout);
		box2.add(Box.createHorizontalStrut(20));
		logout.addActionListener(this);
		box2.add(goBack);
		goBack.addActionListener(this);
		
		Box box12=Box.createVerticalBox();
		box12.add(box1);
		box12.add(Box.createVerticalStrut(20));
		box12.add(box2);
		
		Box box=Box.createVerticalBox();
		box.add(Box.createVerticalStrut(10));
		box.add(box12);
		box.add(Box.createVerticalStrut(20));
		box.add(show);
		box.add(Box.createVerticalStrut(10));
		box.add(operate);
		operate.addActionListener(this);
	
		JPanel center=new JPanel();
		center.add(box);
		
		this.add(center,BorderLayout.CENTER);
		
		//窗口
		this.setTitle("个人信息管理");
		this.setLocation(200,200);
		this.setSize(400,300);
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}

	public void actionPerformed(ActionEvent e) 
	{
		UserList userList=new UserList();//用户顺序表
		
		//查看用户信息
		if(e.getSource()==see)
		{
			String line=userList.show(this.account);//取得用户信息
			String[] content=line.split(" ");
			String showText="用户信息显示如下：\n用户账号："+content[0]+"\n用户密码："+content[1]+"\n联系方式："+content[2];
			
			show.setText(showText);
		}
		
		//修改用户信息
		if(e.getSource()==modify)
		{
			show.setText("请在下方文本框输入修改后的账号、密码和联系方\n式，中间以空格分隔，敲击回车确认修改：\n");
			operate.setText("");
		}
		
		if(e.getSource()==operate)//在操作文本框敲击回车确认修改信息
		{
			String modifyLine=operate.getText();
			
			userList.modify(this.account,modifyLine);
			
			show.append("\n修改成功！注意：若修改账号需重新登录！");	
		}
		
		//注销用户信息
		if(e.getSource()==logout)
		{
			userList.logout(this.account);
			
			show.setText("注销成功！用户功能已不可用。");
		}
		
		//返回上一级
		if(e.getSource()==goBack)
		{
			this.dispose();
			new UserInterface(this.account,this.shopList,this.multithread);
		}
	}
	
	public static void main(String[] args)
	{
		//需要参数
		//new PersonalInformationManagement();
	}
}