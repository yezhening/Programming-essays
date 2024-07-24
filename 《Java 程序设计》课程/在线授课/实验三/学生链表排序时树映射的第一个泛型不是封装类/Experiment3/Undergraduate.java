//【代码：包声明语句】
package Experiment3;

public class Undergraduate extends Student
{
	public Undergraduate(int id,String name,double mathScore,double computerScore)
	{
		//【代码】
		super(id,name,mathScore,computerScore);//直接继承
	}

	public void print()
	{	
		//【代码】
		System.out.printf("%d\t%s\t%.2f\t%.2f\n",super.getId(),super.getName(),super.getMathScore(),super.getComputerScore());//子类调用父类方法
	}

}
