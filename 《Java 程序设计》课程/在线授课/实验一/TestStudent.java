import java.util.*;

class Student//学生类
{
	int id;
	String name;
	double mathScore;
	double computerScore;
	
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
		return id;
	}
	
	public void setName(String name)
	{
		this.name=name;
	}
	
	public String getName()
	{
		return name;
	}
	
	public void setMathScore(double mathScore)
	{
		this.mathScore=mathScore;
	}
	
	public Double getMathScore()//Double不是double,因为compareTo()方法返回对象
	{
		return mathScore;
	}
	
	public void ComputerScore(double computerScore)
	{
		this.computerScore=computerScore;
	}
	
	public Double getComputerScore()
	{
		return computerScore;
	}
	
	public void print(Student s)//
	{
		System.out.printf("%d\t%s\t%.1f\t%.1f\t\n",s.getId(),s.getName(),s.getMathScore(),s.getComputerScore());
	}
}

//重写compareTo()方法只能比较一个成绩→将其放入compare方法中自定义比较器
class MathScoreComparator implements Comparator<Student>//自定义比较器,按数学成绩比较
{
	public int compare(Student s1,Student s2)//实现接口中的方法
	{
		return s1.getMathScore().compareTo(s2.getMathScore());
	}
}

class ComputerScoreComparator implements Comparator<Student>//自定义比较器,按计算机成绩比较
{
	public int compare(Student o1,Student o2)
	{
		return o1.getComputerScore().compareTo(o2.getComputerScore());
	}
}

public class TestStudent
{
	public static void main(String args[])
	{
		Scanner in=new Scanner(System.in);
		int count=3;
		//count=(int)(Math.random()*10)+1;
		int i;
		Student stu[]=new Student [count];//
		List<Student> list=new ArrayList<Student>();//注意是ArrayList
		
		System.out.println("请输入" + count + "位学生信息：");
		for(i=0;i<count;i++)
		{
			stu[i]=new Student(in.nextInt(),in.next(),in.nextDouble(),in.nextDouble());
			list.add(stu[i]);
		}
			
		System.out.println("排序前：");
		for(i=0;i<count;i++)
		{
			stu[i].print(stu[i]);
		}
		
		
		System.out.println("按照数学成绩排序后：");
		sortMathScore(list);//比较
		
		System.out.println("按照计算机成绩排序后：");
		sortCompuerScore(list);
		
	}
	
	public static void sortMathScore(List<Student> list)
	{
		Collections.sort(list,new MathScoreComparator());//Collections是一个工具类，sort是其中的静态方法，是用来对List类型进行排序的 不太清楚
		for(Student stu:list)//该种写法
		{
			stu.print(stu);
		}
	}
	
	public static void sortCompuerScore(List<Student> list)
	{
		Collections.sort(list,new ComputerScoreComparator());
		for(Student stu:list)
		{
			stu.print(stu);
		}
	}
}

class PostGraduate extends Student
{
	String teacherName;
	String researchDirection;
	
	PostGraduate(int id,String name,double mathScore,double computerScore,String teacherName,String researchDirection)
	{
		super(id,name,mathScore,computerScore);
		this.teacherName=teacherName;
		this.researchDirection=researchDirection;	
	}
	
	public void print(PostGraduate p)
	{
		System.out.printf("%d\t%s\t%.1f\t%.1f\t%s\t%s\t\n",p.getId(),p.getName(),p.getMathScore(),p.getComputerScore(),p.teacherName,p.researchDirection);
	}
}