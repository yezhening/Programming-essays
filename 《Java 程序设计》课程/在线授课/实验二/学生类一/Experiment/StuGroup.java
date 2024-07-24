package Experiment;

public class StuGroup implements MyGroup //StuGroup实现了MyGroup接口
{
	 private Student[] stu;
	 private int length;   //实际容量
	
	public StuGroup(int len)
	{
		//【代码】
		stu =new Student[len];
		length=0;	
	}
	
	private boolean isOverflow()    //辅助方法，判断数组是否溢出
	{
		if(this.length<stu.length)
			return false;
		else
			return true;
	}
	
	public boolean isEmpty()    //判断数组是否为空
	{
		return this.length==0;
	}
	
	public boolean addStu(Student x)  //添加学生信息
	{
		if(isOverflow())
			return false;
		stu[length++]=x;
		return true;
	}
	
	public Student removeStu(int index)  //删除指定位置的学生信息
	{
		if(isEmpty())
		{
			System.out.println("学生表空的~~~删除失败~~~~");
			return null;
		}
		Student temp=stu[index-1];
		for(int i=index-1;i<this.length-1;i++)
		{
			stu[i]=stu[i+1];
		}
		this.length--;
		return temp;
	}
	
	public void sort(int x)   //按照成绩排序，参数1按数学排序，参数2按计算机排序
	{
		if(x==1)
		{
		    //【代码】
			for(int i=0;i<length-1;i++)
			{
				for(int j=0;j<length-i-1;j++)
				{
					if(stu[j].getMathScore()>stu[j].getMathScore())
					{
						Student temp;
						temp=stu[j];
						stu[j]=stu[j+1];
						stu[j+1]=temp;
					}
				}
			}
	                 }
		else if(x==2)
		{
		      //【代码】
			for(int i=0;i<length-1;i++)
			{
				for(int j=0;j<length-i-1;j++)
				{
					if(stu[j].getComputerScore()>stu[j].getComputerScore())
					{
						Student temp;
						temp=stu[j];
						stu[j]=stu[j+1];
						stu[j+1]=temp;
					}
				}
			}
		 }
     }
	
	public void print()
	{
                             //【代码】	
		for(int i=0;i<length;i++)
			stu[i].print();
	}
		
}
