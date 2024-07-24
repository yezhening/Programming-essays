package administratorSystem;

import dataSet.*;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

@SuppressWarnings("serial")
public class ShopInformationManagement extends JFrame implements ActionListener
{
	JLabel operationsLabel=new JLabel("请选择要执行的功能");
	JLabel inputLabel=new JLabel("请输入：");
	
	JButton see=new JButton("查看商家信息"); 
	JButton logout=new JButton("注销商家信息");
	JButton goBack=new JButton("想返回上一级");
	
	JTextField input=new JTextField(1);
	JTextArea show=new JTextArea("说明：在上方文本框输入查询商家名称，并点击查询商家按钮。\n在上方文本框输入注销商家名字，并点击注销商家按钮。",16,30);//第二个参数无意义，默认填充满容器
	
	//管理员界面需要传入商家账号作为成员对象
	String account;
	ShopList shopList;
	
	//构造方法
	public ShopInformationManagement(String account,ShopList shopList)
	{
		this.account=account;
		this.shopList=shopList;
		
		//中区
		Box box1=Box.createVerticalBox();
		box1.add(Box.createVerticalStrut(10));
		box1.add(operationsLabel);
		box1.add(Box.createVerticalStrut(10));
		box1.add(see);
		see.addActionListener(this);
		box1.add(Box.createVerticalStrut(10));
		box1.add(logout);
		logout.addActionListener(this);
		box1.add(Box.createVerticalStrut(10));
		box1.add(goBack);
		goBack.addActionListener(this);
			
		Box box2=Box.createHorizontalBox();
		box2.add(inputLabel);
		box2.add(Box.createHorizontalStrut(10));
		box2.add(input);
		
		JScrollPane showJSP =new JScrollPane(show);
		
		Box box3=Box.createHorizontalBox();
		box3.add(showJSP);
		
		Box box23=Box.createVerticalBox();
		box23.add(box2);
		box23.add(Box.createVerticalStrut(10));
		box23.add(box3);
		
		JSplitPane center=new JSplitPane(JSplitPane.HORIZONTAL_SPLIT,box1,box23);
        center.setDividerLocation(120);
			
		this.add(center,BorderLayout.CENTER);
	
		//窗口
		this.setTitle("商家信息管理：请选择要执行的功能");
		this.setLocation(300,300);
		this.setSize(600,200);
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}

	public void actionPerformed(ActionEvent e) 
	{
		//查询方法
		if(e.getSource()==see)
		{
			//取界面内容
			String inputContent=input.getText();
			String content="";
			
			content = "查询内容显示如下：\n";;//显示的所有内容
			content=content+this.shopList.query(inputContent);
			show.setText(content);
		}
		
		//注销商家信息
		if(e.getSource()==logout)
		{
			String shopName=input.getText();//商家名字
			
			String shopId=this.shopList.nameToId(shopName);//注销需要商家账号，查询时不显示商家账号，多写一个
			if(shopName.equals(""))
			{
				show.setText("请输入注销商家名字。");
			}
			else
			{
				this.shopList.logout(shopId);
				
				show.setText("注销成功。");
			}
		}
		
		//返回上一级
		if(e.getSource()==goBack)
		{
			this.dispose();
			new AdministratorInterface(this.account,this.shopList);
		}
	}
	
	//主方法
	public static void main(String[] args) 
	{
		//需要参数
		//new ShopInformationManagement();
	}
}