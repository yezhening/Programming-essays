//【包的声明】
package Experiment;

public abstract class Student 
{
    //抽象学生类的属性有学号（int）,姓名（String）,数学成绩（double），计算机成绩（double）
    //【成员变量的声明】 
	private int id;
	private String name;
	private double mathScore;
	private double computerScore;
	
    //【构造方法】 
	public Student(int id,String name,double mathScore,double computerScore)
	{
		this.id=id;
		this.name=name;
		this.mathScore=mathScore;
		this.computerScore=computerScore;
	}
	
    //【获取学号】
	public int getId()
	{
		return this.id;
	}
	
    //【设置学号】
	public void setId(int id)
	{
		this.id=id;
	}
	
    //【获取姓名】
	public String getName()
	{
		return this.name;
	}
	
    //【设置姓名】
	public void setName(String name)
	{
		this.name=name;
	}
	
    //【获取数学成绩】
	public double getMathScore()
	{
		return this.mathScore;
	}
	
    //【设置数学成绩】
	public void setMathScore(double mathScore)
	{
		this.mathScore=mathScore;
	}
	
    //【获取计算机成绩】
	public double getComputerScore()
	{
		return this.computerScore;
	}
	
    //【设置计算机成绩】
	public void setComputerScore(double computerScore)
	{
		this.computerScore=computerScore;
	}
	
    public abstract void print(); //抽象方法，子类要实现这个方法，也就是输出学生的信息
}
