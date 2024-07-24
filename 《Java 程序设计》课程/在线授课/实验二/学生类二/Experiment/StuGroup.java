//【包的声明】
package Experiment;

public class StuGroup implements MyGroup //StuGroup实现了MyGroup接口
{
	 private Student[] stu;
	 private int length;   //实际容量
	
	public StuGroup(int len)
	{
		//【代码】
		//this.length=len;//错
		//注意：Cannot read the array length because "this.stu" is null
		this.stu=new Student[len];
		this.length=0;
	}
	
	private boolean isOverflow()    //辅助方法，判断数组是否溢出
	{
		if(this.length<this.stu.length)
		{
			return false;
		}
		else
		{
			return true;
		}			
	}
	
	public boolean isEmpty()    //判断数组是否为空
	{
		return this.length==0;
	}
	
	public boolean addStu(Student x)  //添加学生信息
	{
		if(isOverflow())
		{
			return false;
		}
		this.stu[this.length++]=x;
		return true;
	}
	
	public Student removeStu(int index)  //删除指定位置的学生信息
	{
		if(isEmpty())
		{
			System.out.println("学生表空的~~~删除失败~~~~");
			return null;//注意
		}
		//注意删除顺序表元素
		Student temp=this.stu[index-1];
		for(int i=index-1;i<this.length-1;i++)
		{
			this.stu[i]=this.stu[i+1];
		}
		this.length--;
		return temp;
	}
	
	//注意顺序表排序:冒泡
	public void sort(int x)   //按照成绩排序，参数1按数学排序，参数2按计算机排序
	{
		if(x==1)
		{
		    //【代码】
			for(int i=0;i<this.length-1;i++)
			{
				for(int j=0;j<this.length-i-1;j++)
				{
					
					if(this.stu[j].getMathScore()>this.stu[j+1].getMathScore())
					{
					
						Student temp;
						temp=this.stu[j];
						this.stu[j]=this.stu[j+1];
						this.stu[j+1]=temp;
					}
				}
			}
	    }
		else if(x==2)
		{
		    //【代码】
			for(int i=0;i<this.length-1;i++)
			{
				for(int j=0;j<this.length-i-1;j++)
				{
					if(this.stu[j].getComputerScore()>this.stu[j+1].getComputerScore())
					{
						Student temp;
						temp=this.stu[j];
						this.stu[j]=this.stu[j+1];
						this.stu[j+1]=temp;
					}
				}
			}
		 }
     }
	
	public void print()
	{
		//【代码】	
		for(int i=0;i<this.length;i++)
		{
			this.stu[i].print();
		}
		System.out.println();
	}		
}
