package shopSystem;

import dataSet.*;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

@SuppressWarnings("serial")
public class ShopOrdeManagement extends JFrame implements ActionListener
{
	JLabel seeLabel=new JLabel("商家菜品预定信息显示如下：");
	JLabel deleteLabel=new JLabel("<html>请输入需要删除的用户账号、预定菜品名称,中间以空格分隔,<br>一份菜品为一行:</html>");
	
	JTextArea seeOrder=new JTextArea(15,36);
	JTextArea deleteOrder=new JTextArea(14,36);
	
	JButton seeGoBack=new JButton("返回上一级");
	JButton seeConfirm=new JButton("确认查询");
	JButton deleteGoBack=new JButton("返回上一级");
	JButton deleteConfirm=new JButton("确认删除");
	
	//账号、商家表对象
	String account;
	ShopList shopList;

	//构造函数
	public ShopOrdeManagement(String account,ShopList shopList)
	{
		this.account=account;
		this.shopList=shopList;
		
		//中区
		//子窗体
		JPanel seePanel=new JPanel();
		seePanel.add(seeLabel);
		seePanel.add(seeOrder);		
		seePanel.add(seeGoBack);
		seeGoBack.addActionListener(this);
		seePanel.add(seeConfirm);
		seeConfirm.addActionListener(this);
		
		JPanel deletePanel=new JPanel();
		deletePanel.add(deleteLabel);
		deletePanel.add(deleteOrder);
		deletePanel.add(deleteGoBack);
		deleteGoBack.addActionListener(this);
		deletePanel.add(deleteConfirm);
		deleteConfirm.addActionListener(this);
		
		//选项卡
		JTabbedPane tabbedPane=new JTabbedPane();
		tabbedPane.addTab("查看菜品预定信息",seePanel);
		tabbedPane.addTab("删除菜品预定信息",deletePanel);
	
		this.add(tabbedPane,BorderLayout.CENTER);
		
		//窗口
		this.setTitle("商家预定管理");
		this.setLocation(200,200);
		this.setSize(400,400);
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	

	//监听方法
	public void actionPerformed(ActionEvent e) 
	{
		//查询订单
		if(e.getSource()==seeConfirm)
		{
			String seeText="商家菜品订单显示如下：\n\n用户 菜品名称 菜品数量\n———————————";
			String shopName="";
			
			//通过商家账号查询商家名称
			shopName=this.shopList.idToName(this.account);
			
			OrderList orderList=new OrderList();
			seeText=seeText+orderList.seeShopOrder(shopName);
			
			try
			{
				Thread.sleep(1000);//等待一秒获得传回内容
			} 
			catch (InterruptedException e1) 
			{	
				e1.printStackTrace();
			}
			
			seeOrder.setText(seeText);
		}
		
		//返回上一级
		if(e.getSource()==seeGoBack)
		{
				this.dispose();
				new ShopInterface(this.account,this.shopList);//跳转商家界面
		}
		
		//删除订单
		if(e.getSource()==deleteConfirm)
		{
			String deleteText=deleteOrder.getText();
			String[] orders=deleteText.split("\n");
			
			OrderList orderList=new OrderList();
			for(int i=0;i<orders.length;i++)
			{
				String[] order=orders[i].split(" ");
				String userAccount=order[0];//用户账号
				String foodName=order[1];
				
				orderList.deleteOrder(userAccount,foodName);
			}
			
			deleteOrder.append("\n\n删除成功！");
		}
		
		//返回上一级
		if(e.getSource()==deleteGoBack)
		{
			this.dispose();
			new ShopInterface(this.account,this.shopList);//跳转商家界面
		}
	}
	
	//主方法
	public static void main(String[] args) 
	{
		//需要参数
		//new ShopOrdeManagement();
	}
}