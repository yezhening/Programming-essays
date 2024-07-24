package shopSystem;

import dataSet.*;
import loginSystem.*;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

@SuppressWarnings("serial")
public class ShopRegister extends JFrame implements ActionListener
{
	JLabel newShopLabel=new JLabel("请填写新商家信息");
		
	JLabel newTypeLabel=new JLabel("请输入类型：");
	JTextField newType=new JTextField();
	
	JLabel newAccountLabel=new JLabel("请输入账号：");
	JTextField newAccount=new JTextField();
	
	JLabel newNameLabel=new JLabel("请输入名称：");
	JTextField newName=new JTextField();
		
	JLabel newPasswordLabel=new JLabel("请输入密码：");
	JTextField newPassword=new JTextField();
		
	JLabel newAvePriceLabel=new JLabel("请输入均价：");
	JTextField newAvePrice=new JTextField();
		
	JLabel newAddressLabel=new JLabel("请输入地址：");
	JTextField newAddress=new JTextField();
		
	JLabel newPhoneLabel=new JLabel("请输入电话：");
	JTextField newPhone=new JTextField();
		
	JLabel newFoodLabel1=new JLabel("请输入菜品的id号、名称和价格，中间以空格分隔,一份");
	JLabel newFoodLabel2=new JLabel("菜品为一行：");
	
	JTextArea newFood=new JTextArea(4,20);
	
	JButton goBack=new JButton("重新登录");
	JButton register=new JButton("确定注册");
		
	JLabel showLabel=new JLabel("无反馈信息");
		
	public ShopRegister()
	{
		//北区
		JPanel north=new JPanel();
		north.add(newShopLabel);
			
		this.add(north,BorderLayout.NORTH);
			
		//中区
		Box box1=Box.createVerticalBox();
		box1.add(newTypeLabel);
		box1.add(Box.createVerticalStrut(15));
		box1.add(newAccountLabel);
		box1.add(Box.createVerticalStrut(15));
		box1.add(newNameLabel);
		box1.add(Box.createVerticalStrut(15));
		box1.add(newPasswordLabel);
		box1.add(Box.createVerticalStrut(15));
		box1.add(newAvePriceLabel);
		box1.add(Box.createVerticalStrut(15));
		box1.add(newAddressLabel);
		box1.add(Box.createVerticalStrut(15));
		box1.add(newPhoneLabel);
		
		Box box2=Box.createVerticalBox();
		box2.add(newType);
		box2.add(Box.createVerticalStrut(10));
		box2.add(newAccount);
		box2.add(Box.createVerticalStrut(10));
		box2.add(newName);
		box2.add(Box.createVerticalStrut(10));
		box2.add(newPassword);
		box2.add(Box.createVerticalStrut(10));
		box2.add(newAvePrice);
		box2.add(Box.createVerticalStrut(10));
		box2.add(newAddress);
		box2.add(Box.createVerticalStrut(10));
		box2.add(newPhone);
				
		Box box12=Box.createHorizontalBox();
		box12.add(box1);
		box12.add(Box.createHorizontalStrut(10));
		box12.add(box2);
			
		Box box3=Box.createHorizontalBox();
		box3.add(newFoodLabel1);
		
		Box box4=Box.createHorizontalBox();
		box4.add(newFoodLabel2);
		
		JScrollPane area=new JScrollPane(newFood);
		
		Box box5=Box.createHorizontalBox();
		box5.add(goBack);
		goBack.addActionListener(this);
		box5.add(Box.createHorizontalStrut(80));
		box5.add(register);
		register.addActionListener(this);
		
		Box box=Box.createVerticalBox();
		box.add(box12);
		box.add(Box.createVerticalStrut(10));
		box.add(box3);
		box.add(box4);
		box.add(Box.createVerticalStrut(10));
		box.add(area);
		box.add(Box.createVerticalStrut(16));
		box.add(box5);
		
		JPanel center=new JPanel();
		center.add(box);
			
		this.add(center,BorderLayout.CENTER);
			
		//南区
		JPanel south=new JPanel();
		showLabel.setForeground(Color.blue);
		south.add(showLabel);
			
		this.add(south,BorderLayout.SOUTH);
		
		//窗体
		this.setTitle("欢迎来到商家注册界面");
		this.setLocation(100,100);
		this.setSize(400,500);
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	
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
			//获取界面内容
			String type=newType.getText();
			String id=newAccount.getText();
			String name=newName.getText();
			String password=newPassword.getText();
			String avgScore="0";//评分默认为0
			String avePrice=newAvePrice.getText();
			String address=newAddress.getText();
			String phone=newPhone.getText();
			String food=newFood.getText();
			
			if(type.equals("")||id.equals("")||name.equals("")||password.equals("")||avePrice.equals("")||address.equals("")||phone.equals("")||food.equals(""))
			{
				showLabel.setText("注册失败！请填写完整注册信息。");
			}
			else
			{
				ShopList shopList=new ShopList();
				if(shopList.contains(id))//已存在该商家账号
				{
					newAccount.setText("");
					showLabel.setText("注册失败！该用户账号已存在。");
				}
				else//不存在该用户账号
				{
					shopList.register(type,id,name,password,avgScore,avePrice,address,phone,food);
					showLabel.setText("注册成功！");
				}
			}		
		}
	}
	
	//主方法
	public static void main(String[] args) 
	{
		new ShopRegister();
	}
}