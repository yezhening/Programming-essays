//【包的引用】
import Experiment.*;

import java.util.*;

public class Main
{
	public static void main(String[] args)
	{
		Scanner in=new Scanner(System.in);
		
		StuGroup group=new StuGroup(20);  //group可以容纳20个学生
		int num;  //实际输入的学生信息
		char choice;
		
		System.out.println("要创建本科生信息表还是研究生信息表？\nA.本科生\tB.研究生");
		choice=in.next().charAt(0);
		
		switch(choice)//【代码】)
		{
			case 'A':
			case 'a':
				System.out.println("请问要输入多少个本科生的信息？");
				//【代码】
				num=in.nextInt();
				Student stuA[] = new Undergraduate[num];//注意形式 stuA[]对象数组是子类Undergraduate对象的上转型对象
				
				for(int i=0;i<num;i++)
				{
					System.out.println("请输入第"+(i+1)+"位学生的学号，姓名，数学、计算机成绩");
					//【代码】
					stuA[i]=new Undergraduate(in.nextInt(),in.next(),in.nextDouble(),in.nextDouble());
					group.addStu(stuA[i]);
				}
				
				group.print();//运行时多态
				group.addStu(new Undergraduate(1312,"shgd",99,98.5));
				group.addStu(new Undergraduate(1316,"ddsa",89,88.5));
				group.print();//运行时多态
				break;
				
			case 'B':
			case 'b':
				System.out.println("请问要输入多少个研究生的信息？");
				//【代码】
				num=in.nextInt();
				Student stuB[] = new Postgraduate[num];//stuB[]对象数组是子类Postgraduate对象的上转型对象
				
				for(int i=0;i<num;i++)
				{
					System.out.println("请输入第"+(i+1)+"位学生的学号，姓名，数学、计算机成绩，导师和研究方向，以空格隔开");
					//【代码】
					stuB[i]=new Postgraduate(in.nextInt(),in.next(),in.nextDouble(),in.nextDouble(),in.next(),in.next());
					group.addStu(stuB[i]);
				}
				
				group.print();//运行时多态 
				group.addStu(new Postgraduate(1312,"shgd",99,98.5,"xyy","os"));
				group.addStu(new Postgraduate(1316,"ddsa",89,88.5,"wcl","rgzn"));
				group.print();
				break;
			default:
				System.out.println("输入错误！！！");
				break;
		}
		
		in.close();
		
		group.removeStu(2);
		group.print();
		group.sort(1);
		group.print();
		group.sort(2);
		group.print();
	}
}
