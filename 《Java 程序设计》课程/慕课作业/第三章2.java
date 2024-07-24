package a;
import java.util.*;

public class Test
{
	public static void main(String[] args)
	{
		Scanner in=new Scanner(System.in);
		
		Student stu=new Student(in.nextInt(),in.next(),in.nextDouble(),in.nextDouble());
		stu.print();
		
		PostGraduate post=new PostGraduate(in.nextInt(),in.next(),in.nextDouble(),in.nextDouble(),in.next(),in.next());
		post.print();
	}
}

class Student
{
	private int id;
	private String name;
	private double mathScore;
	private double computerScore;
	
	public Student()
	{}
	
	public Student(int id,String name,double mathScore,double computerScore)
	{
		this.id=id;
		this.name=name;
		this.mathScore=mathScore;
		this.computerScore=computerScore;
	}
	
	public void setId(int id)
	{
		this.id=id;
	}
	
	public int getId()
	{
		return this.id;
	}
	
	public void setName(String name)
	{
		this.name=name;
	}
	
	public String getName()
	{
		return this.name;
	}
	
	public void setMathScore(double mathScore)
	{
		this.mathScore=mathScore;
	}
	
	public double getMathScore()
	{
		return this.mathScore;
	}
	
	public void setComputerScore(double computerScore)
	{
		this.computerScore=computerScore;
	}
	
	public double getComputerScore()
	{
		return this.computerScore;
	}
	
	public void print()
	{
		System.out.printf("%d %s %f %f\n",this.getId(),this.getName(),this.getMathScore(),this.getComputerScore());
	}
}

class PostGraduate extends Student
{
	private String teacher;
	private String direction;
	
	PostGraduate()
	{}
	
	PostGraduate(int id,String name,double mathScore,double computerScore,String teacher,String direction)
	{
		super(id,name,mathScore,computerScore);
		this.teacher=teacher;
		this.direction=direction;
	}
	
	public void setTeacher(String teacher)
	{
		this.teacher=teacher;
	}
	
	public String getTeacher()
	{
		return this.teacher;
	}
	
	public void setDirection(String direction)
	{
		this.direction=direction;
	}
	
	public String getDirection()
	{
		return this.direction;
	}
	
	public void print()
	{
		System.out.printf("%d %s %f %f %s %s\n",this.getId(),this.getName(),this.getMathScore(),this.getComputerScore(),this.getTeacher(),this.getDirection());
	}
}