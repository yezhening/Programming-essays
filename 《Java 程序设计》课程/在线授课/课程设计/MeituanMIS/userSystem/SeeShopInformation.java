package userSystem;

import dataSet.*;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

@SuppressWarnings("serial")
public class SeeShopInformation extends JFrame implements ActionListener
{
	//组件
	JLabel seeLabel=new JLabel("请输入：");
	
	JTextField input=new JTextField(20);
	
	JTextArea show=new JTextArea("说明：输入商家（餐馆）名字的全称，查看某一具体商家（餐馆）的信\n息；输入一个关键词，查看名字中包含此关键词的多个商家（餐馆）的\n信息。敲击回车查询。",11,36);
	
	JButton goBack=new JButton("想返回上一级");
	
	String account;
	ShopList shopList;
	Multithread multithread;
	
	//构造方法
	public SeeShopInformation(String account,ShopList shopList,Multithread multithread)
	{	
		this.account=account;
		this.shopList=shopList;
		this.multithread=multithread;
		
		//北区
		JPanel north=new JPanel();
		north.add(seeLabel);
		north.add(input);
		input.addActionListener(this);
		
		this.add(north,BorderLayout.NORTH);
		
		//中区
		JScrollPane center =new JScrollPane(show);//构造方法中添加组件，add()方法添加中间容器
		
		this.add(center,BorderLayout.CENTER);

		//南区
		JPanel south=new JPanel();
		south.add(goBack);
		goBack.addActionListener(this);
		
		this.add(south,BorderLayout.SOUTH);
		
		//窗口
		this.setTitle("查看商家信息");
		this.setLocation(200,200);
		this.setSize(400,400);
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	
	//监听方法
	public void actionPerformed(ActionEvent e) 
	{
		//查询方法
		if(e.getSource()==input)
		{
			String inputContent=input.getText();//匹配信息
			
			String content="";
			if(inputContent.equals(""))
			{
				content="请输入查询内容!";
			}
			else
			{
				content="查询内容显示如下：\n";;//显示的所有内容+返回给显示界面的匹配到的商家信息			
				content=content+shopList.query(inputContent);
			}
			
			show.setText(content);
		}
		
		//返回上一级
		if(e.getSource()==goBack)
		{
			this.dispose();
			new UserInterface(this.account,shopList,this.multithread);
		}	
	}
	
	//主方法
	public static void main(String[] args)
	{
		//需要参数
		//new SeeShopInformation();
	}
}