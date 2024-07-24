package Experiment;

public class Postgraduate extends Student
{
	private String teacher;
	private String direction;
	
	public Postgraduate(int id, String name, double mathScore, double computerScore,String teacher,String direction) 
	{
		super(id, name, mathScore, computerScore);
		this.teacher=teacher;
		this.direction=direction;
	}
	
	public String getTeacher() 
	{
		return teacher;
	}

	public void setTeacher(String teacher)
	{
		this.teacher = teacher;
	}

	public String getDirection()
	{
		return direction;
	}
	
	public void setDirection(String direction) 
	{
		this.direction = direction;
	}

	public void print()
	{
    	System.out.printf("%d\t%s\t%.1f\t%.1f\t%s\t%s\t\n",super.getId(),super.getName(),super.getMathScore(),super.getComputerScore(),this.getTeacher(),this.getDirection());
	}
}
