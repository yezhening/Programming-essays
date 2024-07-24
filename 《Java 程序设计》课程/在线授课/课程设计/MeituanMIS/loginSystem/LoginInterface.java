package loginSystem;

import dataSet.*;
import userSystem.*;
import shopSystem.*;
import administratorSystem.*;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

//登录界面
@SuppressWarnings("serial")
public class LoginInterface extends JFrame implements ActionListener
{
	//组件
	//图片
	ImageIcon image = new ImageIcon("C:\\Users\\DSHH\\Desktop\\数据集\\图片\\美团外卖.jpg");
	JLabel imageLabel=new JLabel(image);
	
	JLabel identityLabel=new JLabel("请选择您的身份：");
	JRadioButton user=new JRadioButton("用户");
	JRadioButton shop=new JRadioButton("商家");
	JRadioButton administrator=new JRadioButton("管理员");
	
	JLabel accountLabel=new JLabel("账号：");
	JTextField account=new JTextField();
	
	JLabel passwordLabel=new JLabel("密码：");
	JPasswordField password=new JPasswordField();
	
	JButton register=new JButton("注册");
	JButton login=new JButton("登录");
	
	JLabel show=new JLabel("欢迎来到美团餐馆菜品预定系统");
	
	//每一个用户、商家使用同一个商家表对象（临界资源）
	//若是读文件，则每次都可创建一个对象，数据共享
	//但是有自动生成菜品数量，若重复创建对象会重复初始化菜品数量，则其不为临界资源
	ShopList shopList=new ShopList();

	//构造方法
	public LoginInterface()
	{
		//北区
		image.setImage(image.getImage().getScaledInstance(500,200,Image.SCALE_DEFAULT));//这里设置图片大小，目前是20*20
		
		JPanel north=new JPanel();
		north.add(imageLabel);
	    
		this.add(north,BorderLayout.NORTH);
		
		//中区
		//单选按钮组
		ButtonGroup group = new ButtonGroup();
		group.add(user);
		group.add(shop);
		group.add(administrator);
		
		Box box1=Box.createHorizontalBox();
		box1.add(identityLabel);
		box1.add(user);
		box1.add(shop);
		box1.add(administrator);
		
		Box box2=Box.createHorizontalBox();
		box2.add(accountLabel);
		box2.add(account);
		
		Box box3=Box.createHorizontalBox();
		box3.add(passwordLabel);
		box3.add(password);
		
		Box box4=Box.createHorizontalBox();
		box4.add(register);
		register.addActionListener(this);
		box4.add(Box.createHorizontalStrut(100));
		box4.add(login);
		login.addActionListener(this);
		
		Box box5=Box.createHorizontalBox();
		show.setForeground(Color.blue);
		box5.add(show);
		
		Box box=Box.createVerticalBox();
		box.add(Box.createVerticalStrut(10));
		box.add(box1);
		box.add(Box.createVerticalStrut(30));
		box.add(box2);
		box.add(Box.createVerticalStrut(30));
		box.add(box3);
		box.add(Box.createVerticalStrut(30));
		box.add(box4);
		box.add(Box.createVerticalStrut(30));
		box.add(box5);
		
		JPanel center=new JPanel();
	    center.add(box);
	    
		this.add(center,BorderLayout.CENTER);
		
		//窗口设置
		this.getContentPane().setBackground(Color.yellow);
		this.setTitle("美团餐馆菜品预定系统");
		this.setLocation(200,200);
		this.setSize(500,500);
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}

	//监听方法
	public void actionPerformed(ActionEvent e) //注意:每次响应都会调用一次监听方法
	{
		//获取界面内容
		String accountContent=account.getText();
		String passwordContent=new String(password.getPassword());//getPassword()返回char[]
			
		//用户身份
		if(user.isSelected())//单选按钮的选择判断
		{	
			UserLoginInformation userLoginInformation=new UserLoginInformation();//用户登录信息对象
			UserList userList=new UserList();//用户表对象
			
			//登录
			if(e.getSource()==login)
			{
				if(userLoginInformation.contains(accountContent))//用户已登录
				{
					show.setText("登陆失败！用户已登录。");
				}
				else//用户未登录
				{					
					String judgement=userList.login(accountContent,passwordContent);
					
					//判断账号、密码错误
					//设置返回值
					if(judgement.equals("11"))
					{
						userLoginInformation.add(accountContent);//将用户登录信息写入文件
						
						show.setText("登录成功！");
						
						//用户多线程
						//实现runnable接口的对象，每个实际用户线程应使用同一个实现runnable接口的对象
						Multithread multithread=new Multithread(accountContent,shopList);
						
						new UserInterface(accountContent,this.shopList,multithread);//跳转用户界面
					}
					else if(judgement.equals("01"))
					{
						show.setText("账号错误，登录失败！请重新输入账号。");
						account.setText("");
					}
					else if(judgement.equals("10"))
					{
						show.setText("密码错误，登录失败！请重新输入密码。");
						password.setText("");
					}
					else
					{
						show.setText("账号、密码错误，登录失败！请重新输入账号、密码。");
						account.setText("");
						password.setText("");
					}
				}//用户未登录
			}//登录	
			
			if(e.getSource()==register)//注册
			{
				new UserRegister();
			}
		}//用户身份
		
		//商家身份
		if(shop.isSelected())
		{	
			ShopLoginInformation shopLoginInformation=new ShopLoginInformation();//商家登录信息
			
			//登录
			if(e.getSource()==login)
			{
				if(shopLoginInformation.contains(accountContent))//商家已登录
				{
					show.setText("登陆失败！商家已登录。");
				}
				else//商家未登录
				{
					String judgement=this.shopList.login(accountContent,passwordContent);
					
					//判断账号、密码错误
					//设置返回值
					if(judgement.equals("11"))
					{
						shopLoginInformation.add(accountContent);//将商家登录信息写入文件
						
						show.setText("登录成功！");
						
						new ShopInterface(accountContent,this.shopList);//跳转商家界面
					}
					else if(judgement.equals("01"))
					{
						show.setText("账号错误，登录失败！请重新输入账号。");
						account.setText("");
					}
					else if(judgement.equals("10"))
					{
						show.setText("密码错误，登录失败！请重新输入密码。");
						password.setText("");
					}
					else
					{
						show.setText("账号、密码错误，登录失败！请重新输入账号、密码。");
						account.setText("");
						password.setText("");
					}
				}//商家未登录
			}//登录
			
			//注册
			if(e.getSource()==register)//注册
			{
				new ShopRegister();
			}
		}//商家身份	
				
		//管理员身份
		if(administrator.isSelected())
		{	
			AdministratorList administratorList=new AdministratorList();
			AdministratorLoginInformation administratorLoginInformation=new AdministratorLoginInformation();//管理员登录信息
			
			//登录
			if(e.getSource()==login)
			{
				if(administratorLoginInformation.contains(accountContent))//管理员已登录
				{
					show.setText("登录失败！管理员已登录。");
				}
				else//管理员未登录
				{			
					String judgement=administratorList.login(accountContent,passwordContent);
					
					//判断账号、密码错误
					//设置返回值
					if(judgement.equals("11"))
					{
						administratorLoginInformation.add(accountContent);//将管理员登录信息写入文件
						
						show.setText("登录成功！");
						
						new AdministratorInterface(accountContent,this.shopList);//跳转管理员界面
					}
					else if(judgement.equals("01"))
					{
						show.setText("账号错误，登录失败！请重新输入账号。");
						account.setText("");
					}
					else if(judgement.equals("10"))
					{
						show.setText("密码错误，登录失败！请重新输入密码。");
						password.setText("");
					}
					else
					{
						show.setText("账号、密码错误，登录失败！请重新输入账号、密码。");
						account.setText("");
						password.setText("");
					}
				}//管理员未登录
			}//登录
		}//管理员身份	
	}//监听方法
	
	public static void main(String[] args)
	{
		new LoginInterface();
	}
}