//包引用语句
import java.util.Scanner;
import java.util.InputMismatchException;

//自定义异常类的声明
class RectangleMinusLengthException extends Exception//矩形边长为负值异常
{
	String name;
	public RectangleMinusLengthException(String str)
	{
		name=str;
	}
	public String toString()
	{
		return name;
	}
}

public class TestAllException

{
	// public static double calculateArea(double a, double b) throws
	// ...（...表示自定义异常类）
	public static double calculateArea(double a, double b) throws RectangleMinusLengthException
         {
			double area;
                   //判断什么时候声明异常
			if(a<0||b<0)
			{
				throw new RectangleMinusLengthException("负边长异常");
			}
                   //计算并且返回矩形面积
				area=a*b;
				return area;
          }

			public static void main(String[] args)

			{
				Scanner in = new Scanner(System.in);
				double length = 0;// 要初始化
				double width = 0;
				double area = 0;

				System.out.printf("请输入矩形的长和宽：");
				try {
					// 从键盘上接收矩形的长和宽
					length = in.nextDouble();
					width = in.nextDouble();
					
					// 调用 方法calculateArea
					area = calculateArea(length, width);
					
					// 输出矩形面积
					System.out.printf("矩形的面积是：%.2f\n", area);
					
					in.close();
				} catch (InputMismatchException ie) {
					System.out.printf("输入是数字，请改正！");
				} catch (RectangleMinusLengthException me) {
					System.out.printf("输入为负值，请改正！");
				}
				// 对于上述操作，注意捕获可能出现的异常
			}
}


