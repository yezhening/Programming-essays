import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class Test2 extends JFrame implements ActionListener
{
	JLabel  label1=new JLabel("获取[1,10)之间的随机数：");
	JButton	getRandomNum=new JButton("得到一个随机数");
			
	JLabel  label2=new JLabel("输入您的猜测：");
	JTextField guessRandomNum=new JTextField();
	
	JLabel label3=new JLabel("单击确定按钮：");
	JButton yes=new JButton("确定");
	
	JLabel show=new JLabel("无反馈信息");
	//show.//不可用
	
	Test2()
	{
		JPanel north=new JPanel();
	
		north.setLayout(new GridLayout(2,2,0,5));
		
		north.add(label1);
		north.add(getRandomNum);
		getRandomNum.addActionListener(this);
		
		north.add(label2);
		north.add(guessRandomNum);
		
		this.add(north,BorderLayout.NORTH);
		
		Box box1,box2,box;
		box1=Box.createHorizontalBox();
		box2=Box.createHorizontalBox();
		box=Box.createVerticalBox();
		
		box1.add(label3);
		box1.add(yes);
		yes.addActionListener(this);
		
		box2.add(show);
		show.setForeground(Color.blue);
		
		box.add(Box.createVerticalStrut(10));
		box.add(box1);
		box.add(Box.createVerticalStrut(30));
		box.add(box2);
		
		this.add(box,BorderLayout.CENTER);
		
		setTitle("猜数字小游戏");
		setBounds(100,100,400,300);
		setVisible(true);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	
	public void actionPerformed(ActionEvent e)
	{
		if(e.getSource()==getRandomNum)
		{
			int randomNum=(int) ((Math.random()*10)+1);//Math.randon()生成的数是大于等于0.0，小于1.0的
			show.setText("已生成随机数");
		}
		
		if(e.getSource()==yes)
		{
			int randomNum=(int) ((Math.random()*10)+1);//上面的值传不下来，投机取巧，一次性 因为每次都重新进这个函数
			
			String content=guessRandomNum.getText();//需要String类型
			int guessNum=Integer.parseInt(content);//字符串→整形类对象→整型数字
			
			
			if(guessNum>randomNum)
			{
				show.setText("猜大了");
			}
			else if(guessNum<randomNum)
			{
				show.setText("猜小了");
			}
			else
			{
				show.setText("猜对了");	
			}
		}
	}
	
	public static void main(String[] args)
	{
		new Test2();
	}
}