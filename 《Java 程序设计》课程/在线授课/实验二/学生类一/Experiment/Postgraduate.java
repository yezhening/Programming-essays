package Experiment;

public class Postgraduate extends Student
{
	String teacher;
	String direction;
	public Postgraduate(int id, String name,double mathScore,double computerScore,String teacher,String direction)
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
	    return teacher;
	}
	
	public void setDirection(String direction)
	{
	    this.direction=direction;
	}
	
	public String getDirection()
	{
	    return direction;
	}
	
	public void print()
	{
    	System.out.printf("%d\t%s\t%.1f\t%.1f\t\n",getId(),getName(),getMathScore(),getComputerScore());
    }
}
