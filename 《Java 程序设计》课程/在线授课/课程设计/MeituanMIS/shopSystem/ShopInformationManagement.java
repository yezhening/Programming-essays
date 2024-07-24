package shopSystem;

import dataSet.*;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

@SuppressWarnings("serial")
public class ShopInformationManagement extends JFrame implements ActionListener
{
	JLabel operationsLabel=new JLabel("请选择要执行的功能");
	
	JButton see=new JButton("查看商家信息"); 
	JButton modify=new JButton("修改商家信息"); 
	JButton logout=new JButton("注销商家信息");
	JButton goBack=new JButton("想返回上一级");
	JButton confirm=new JButton("确定修改信息");
	
	JTextArea show=new JTextArea("这里显示信息。");
	JTextArea operate=new JTextArea("这里操作信息。");
	
	//商家界面需要传入商家账号作为成员对象
	String account;
	ShopList shopList;
	
	//构造方法
	public ShopInformationManagement(String account,ShopList shopList)
	{
		this.account=account;
		this.shopList=shopList;
		
		//北区
		JPanel north=new JPanel();
		north.add(see);
		see.addActionListener(this);
		north.add(modify);
		modify.addActionListener(this);
		north.add(logout);
		logout.addActionListener(this);
		
		this.add(north,BorderLayout.NORTH);
		
		//中区	
		JScrollPane center1 =new JScrollPane(show);
		JScrollPane center2 =new JScrollPane(operate);
				
		Box box=Box.createVerticalBox();
		box.add(center1);
		box.add(Box.createVerticalStrut(10));
		box.add(center2);
				
		this.add(box,BorderLayout.CENTER);

		//南区
		JPanel south=new JPanel();
		south.add(goBack);
		goBack.addActionListener(this);
		south.add(confirm);
		confirm.addActionListener(this);
					
		this.add(south,BorderLayout.SOUTH);
		
		//窗口
		this.setTitle("商家信息管理：请选择要执行的功能");
		this.setLocation(200,200);
		this.setSize(400,400);
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}

	public void actionPerformed(ActionEvent e) 
	{
		//查看商家信息
		if(e.getSource()==see)
		{
			String shopName=this.shopList.idToName(this.account);//匹配信息
			
			String content="";
			if(shopName.equals(""))
			{
				content = "请输入查询内容!";
			}
			else
			{
				content = "查询内容显示如下：\n";;//显示的所有内容+返回给显示界面的匹配到的商家信息			
				content=content+shopList.query(shopName);
			}
			
			show.setText(content);
		}
		
		//修改商家信息
		if(e.getSource()==modify)
		{		
			show.setText("请在下方文本域输入修改内容：");		
			operate.setText("shoptype:\nshopId:\nshopName:\nshopPassword:\navePrice:\naddress:\nphone:");
		}
		
		if(e.getSource()==confirm)
		{
			//获取修改内容
			String content=operate.getText();
			
			String showText=this.shopList.modify(this.account, content);
			show.setText(showText);
		}
		
		//注销商家信息
		if(e.getSource()==logout)
		{
			this.shopList.logout(this.account);
				
			show.setText("注销成功，商家功能已不可用");
		}
		
		//返回上一级
		if(e.getSource()==goBack)
		{
			this.dispose();
			new ShopInterface(this.account,this.shopList);
		}
	}
	
	
	public static void main(String[] args) 
	{
		//需要参数
		//new ShopInformationManagement();
	}
}