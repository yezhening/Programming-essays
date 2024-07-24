package administratorSystem;

import dataSet.*;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

@SuppressWarnings("serial")
public class AdministratorInformationManagement extends JFrame implements ActionListener
{
	//组件
	JLabel operationsLabel=new JLabel("请选择要执行的功能");
	
	JButton see=new JButton("查看管理员信息"); 
	JButton modify=new JButton("修改管理员信息"); 
	JButton goBack=new JButton("想返回上一级");
	
	JTextArea show=new JTextArea("这里显示信息。",5,30);
	JTextField operate=new JTextField("这里操作信息。");
	
	//传参
	String account;
	ShopList shopList;
	
	//构造方法
	public AdministratorInformationManagement(String account,ShopList shopList)
	{
		this.account=account;
		this.shopList=shopList;
		
		//北区
		JPanel north=new JPanel();
		north.add(operationsLabel);
		
		this.add(north,BorderLayout.NORTH);
		
		//中区
		Box box1=Box.createHorizontalBox();
		box1.add(see);
		see.addActionListener(this);
		box1.add(Box.createHorizontalStrut(10));
		box1.add(modify);
		modify.addActionListener(this);
		
		Box box=Box.createVerticalBox();
		box.add(Box.createVerticalStrut(5));
		box.add(box1);
		box.add(Box.createVerticalStrut(20));
		box.add(show);
		box.add(Box.createVerticalStrut(14));
		box.add(operate);
		operate.addActionListener(this);
	
		JPanel center=new JPanel();
		center.add(box);
		
		this.add(center,BorderLayout.CENTER);
		
		//南区
		JPanel south=new JPanel();
		south.add(goBack);
		goBack.addActionListener(this);
		
		this.add(south,BorderLayout.SOUTH);
		
		//窗口
		this.setTitle("个人信息管理");
		this.setLocation(200,200);
		this.setSize(400,300);
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}

	public void actionPerformed(ActionEvent e) 
	{
		AdministratorList administratorList=new AdministratorList();
		
		//查看管理员信息
		if(e.getSource()==see)
		{
			//获取内容
			String showText=administratorList.show(this.account);
				
			show.setText(showText);
		}
		
		//修改管理员信息
		if(e.getSource()==modify)
		{
			show.setText("请在下方文本框输入要修改后的账号、密码，中间以\n空格分隔，敲击回车确认修改：\n");
			operate.setText("");
		}
				
		if(e.getSource()==operate)//在操作文本框敲击回车确认修改信息
		{
			String modifyLine=operate.getText();
					
			administratorList.modify(this.account,modifyLine);
					
			show.append("\n修改成功！若修改账号，请重新登录。");	
		}
		
		//返回上一级
		if(e.getSource()==goBack)
		{
			this.dispose();
			new AdministratorInterface(this.account,this.shopList);
		}
	}
	
	public static void main(String[] args)
	{
		//需要参数
		//new AdministratorInformationManagement();
	}
}