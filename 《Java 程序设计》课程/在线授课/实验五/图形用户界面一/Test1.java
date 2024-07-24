import javax.swing.*;
import java.awt.event.*;
import java.awt.*;
import java.io.*;

public class Test1 extends JFrame implements ActionListener
{
	JLabel path=new JLabel("请输入文件路径和名称：");
	JTextField filePath=new JTextField();
	
	JLabel append=new JLabel("请输入需要追加的内容：");
	JTextField appendContent=new JTextField();
	//appendContent.addActionListener(this);//不可用
	
	JButton button=new JButton("将文本区的内容写入文件");
	
	JTextArea area=new JTextArea();
	
	Test1()
	{
		JPanel north=new JPanel();
		north.setLayout(new GridLayout(3,2,0,5));//GridLayout()中组件平均占据空间，会随着窗口大小变化
		
		north.add(path);
		north.add(filePath);
		
		north.add(append);
		north.add(appendContent);
		appendContent.addActionListener(this);//要在构造方法中才可为组件添加监听器
		
		north.add(button);
		button.addActionListener(this);
		
		JScrollPane center=new JScrollPane(area);
		//area.addActionListener(this);//不可用
		//center.add(area);//不可用
		
		this.add(north,BorderLayout.NORTH);
		this.add(center,BorderLayout.CENTER);
		
		setLocation(100,100);
		setSize(400,300);
		setVisible(true);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	
	public void actionPerformed(ActionEvent e)
	{
		if(e.getSource()==appendContent)//监听多个不同组件时
		{
			String allContent=area.getText();
			String content=appendContent.getText();
			
			appendContent.setText("");
			if(allContent.equals(""))//String类的比较用equals()
			{
				allContent=allContent+content;
			}
			else
			{
				allContent=allContent+"\n"+content;
			}
			area.setText(allContent);
		}
		
		if(e.getSource()==button)
		{
			String fp=filePath.getText();
			File f=new File(fp);
			String content=area.getText();
			
			try {
				FileWriter out=new FileWriter(fp,true);
				out.write(content);
				out.close();
			}catch(IOException ioe){
				System.out.println("输入输出错误！");
			}
			
			filePath.setText("");
			String allContent="文件路径和名称："+fp+"\n"+"内容：\n"+content+"\n"+"成功写入文件！";
			area.setText(allContent);
		}
	}
	
	public static void main(String[] args)
	{
		new Test1();
	}
}