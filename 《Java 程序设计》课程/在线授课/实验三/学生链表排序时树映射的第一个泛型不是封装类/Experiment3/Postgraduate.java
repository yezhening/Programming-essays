package Experiment3;

public class Postgraduate extends Student
{
	//【代码】
	String tutor;
	String researchArea;
	
	public Postgraduate(int id,String name,double mathScore,double computerScore,String tutor,String researchArea)
	{
		//【代码】
		super(id,name,mathScore,computerScore);
		this.tutor=tutor;
		this.researchArea=researchArea;
	}
	
	public void print()
	{
		//【代码】
		System.out.printf("%d\t%s\t%.2f\t%.2f\t%s\t%s\n",super.getId(),super.getName(),super.getMathScore(),super.getComputerScore(),this.getTutor(),this.getTutor());
	}

	public String getTutor()
	{
		return tutor;
	}

	public void setTutor(String tutor)
	{
		this.tutor = tutor;
	}

	public String getResearchArea()
	{
		return researchArea;
	}

	public void setResearchArea(String researchArea)
	{
		this.researchArea = researchArea;
	}

}
