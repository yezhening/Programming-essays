package Experiment;

public class Undergraduate extends Student
{
	public Undergraduate(int id, String name,double mathScore,double computerScore)
	{
		super(id,name,mathScore,computerScore);
	}
	
	public void print()
	{
    	System.out.printf("%d\t%s\t%.1f\t%.1f\t\n",getId(),getName(),getMathScore(),getComputerScore());
    }
}
