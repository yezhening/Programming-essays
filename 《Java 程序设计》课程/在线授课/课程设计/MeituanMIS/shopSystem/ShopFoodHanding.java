package shopSystem;

import dataSet.*;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

@SuppressWarnings("serial")
public class ShopFoodHanding extends JFrame implements ActionListener
{
    //基本组件
	JLabel addLabel=new JLabel("请输入添加的菜品序号、名称和价格，以空格分隔，一份菜品为一行");
	JLabel deleteLabel=new JLabel("请输入删除的菜品名称,一份菜品为一行");
	JLabel modifyLabel=new JLabel("<html>请输入原菜品名称，修改的菜品序号、名称和价格，以空格分隔，<br>一份菜品为一行</html>");
	
	JTextArea addInput=new JTextArea(15,36);
	JTextArea deleteInput=new JTextArea(15,36);
	JTextArea modifyInput=new JTextArea(14,36);

	JButton addGoBack=new JButton("返回上一级");//每个选项卡的显示需要不同的组件，否则会覆盖		不知如何判定选择选项卡
	JButton deleteGoBack=new JButton("返回上一级");
	JButton modifyGoBack=new JButton("返回上一级");
	
	JButton addConfirm=new JButton("确认添加");
	JButton deleteConfirm=new JButton("确认删除");	
	JButton modifyConfirm=new JButton("确认修改");
	
	//账号、商家表对象
	String account;
	ShopList shopList;
	
	//构造方法
	public ShopFoodHanding(String account,ShopList shopList)
	{
		this.account=account;
		this.shopList=shopList;
		
		//中区
		//子窗体
		JPanel addPanel=new JPanel();
		addPanel.add(addLabel);
		addPanel.add(addInput);
		addPanel.add(addGoBack);
		addGoBack.addActionListener(this);
		addPanel.add(addConfirm);
		addConfirm.addActionListener(this);
		
		JPanel deletePanel=new JPanel();
		deletePanel.add(deleteLabel);
		deletePanel.add(deleteInput);
		deletePanel.add(deleteGoBack);
		deleteGoBack.addActionListener(this);
		deletePanel.add(deleteConfirm);
		deleteConfirm.addActionListener(this);
		
		JPanel modifyPanel=new JPanel();
		modifyPanel.add(modifyLabel);
		modifyPanel.add(modifyInput);
		modifyPanel.add(modifyGoBack);
		modifyGoBack.addActionListener(this);
		modifyPanel.add(modifyConfirm);
		modifyConfirm.addActionListener(this);
		
		//选项卡
		JTabbedPane tabbedPane=new JTabbedPane();
		tabbedPane.addTab("添加菜品",addPanel);
		tabbedPane.addTab("删除菜品",deletePanel);
		tabbedPane.addTab("修改菜品",modifyPanel);
		
		this.add(tabbedPane,BorderLayout.CENTER);
		
		//窗口
		this.setTitle("商家菜品处理");
		this.setLocation(200,200);
		this.setSize(400,400);
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	
	//监听方法
	public void actionPerformed(ActionEvent e) 
	{
		String inputContent;
		String[] inputLine;
		String[] inputFood;
		
		String oldFoodName;
		String foodId;
		String foodName;
		String foodPrice;
		
		//添加菜品
		if(e.getSource()==addConfirm)
		{
			//获取界面内容
			inputContent=addInput.getText();
			inputLine=inputContent.split("\n");
			
			for(int i=0;i<inputLine.length;i++)//获取数组长度用length成员变量
			{
				inputFood=inputLine[i].split(" ");
				
				foodId=inputFood[0];
				foodName=inputFood[1];
				foodPrice=inputFood[2];
				
				//传入添加内容
				this.shopList.addFood(this.account, foodId, foodName, foodPrice);
			}
			
			addInput.setText("\n菜品添加成功！");
		}
		
		//返回上一级
		if(e.getSource()==addGoBack)
		{
			this.dispose();
			new ShopInterface(this.account,this.shopList);//跳转商家界面
		}
		
		if(e.getSource()==deleteConfirm)
		{
			//获取界面内容
			inputContent=deleteInput.getText();
			inputLine=inputContent.split("\n");
			
			for(int i=0;i<inputLine.length;i++)//一份菜品
			{	
				foodName=inputLine[i];
				
				//传入删除内容
				this.shopList.deleteFood(this.account,foodName);
			}
			
			deleteInput.setText("\n菜品删除成功！");
		}
		
		//返回上一级
		if(e.getSource()==deleteGoBack)
		{
			this.dispose();
			new ShopInterface(this.account,this.shopList);//跳转商家界面
		}
		
		//修改菜品
		if(e.getSource()==modifyConfirm)
		{
			//获取界面内容
			inputContent=modifyInput.getText();
			inputLine=inputContent.split("\n");
					
			for(int i=0;i<inputLine.length;i++)
			{
				inputFood=inputLine[i].split(" ");
						
				oldFoodName=inputFood[0];
				foodId=inputFood[1];
				foodName=inputFood[2];
				foodPrice=inputFood[3];
						
				//传入修改内容
				this.shopList.modifyFood(this.account, oldFoodName,foodId, foodName, foodPrice);
			}
			
			modifyInput.append("\n菜品修改成功！");
		}
		
		//返回上一级
		if(e.getSource()==modifyGoBack)
		{
			this.dispose();
			new ShopInterface(this.account,this.shopList);//跳转商家界面
		}
	}
	
	//主方法
	public static void main(String[] args) 
	{
		//需要参数
		//new ShopFoodHanding();
	}
}