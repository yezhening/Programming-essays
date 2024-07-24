import java.util.Scanner;

interface UnitPrice {
	public double unitprice(); // 返回商品的单价
}

class Television implements UnitPrice {
	public double unitprice() {
		return 1;
	}
}

class Computer implements UnitPrice {
	public double unitprice() {
		return 2;
	}
}

class Phone implements UnitPrice {
	public double unitprice() {
		return 3;
	}
}

public class Test {
	static double get(UnitPrice u) {
		return u.unitprice();
	}
	
	public static void main(String[] args) {
		/*static double get(UnitPrice u) {放主函数外面！
			return u.unitprice();
		}*/
		
		Scanner in=new Scanner(System.in);
		System.out.println("请输入电视、计算机和手机的销售个数：");
		int televisionCounts=in.nextInt();
		int computerCounts=in.nextInt();
		int phoneCounts=in.nextInt();
		
		Television te=new Television();
		Computer co=new Computer();
		Phone ph=new Phone();
		
		double televisionPrice= get(te);//接口回调 接口的变量=实现该接口的具体的类对象
		double computerPrice=get(co);//把实现了某个接口的不同的类对象分别发送给这个接口变量，这个接口变量将会分别为不同的类去服务
		double phonePrice=get(ph);
		
		double televisionSum=televisionCounts*televisionPrice;
		double computerSum=computerCounts*computerPrice;
		double PhoneSum=phoneCounts*phonePrice;
		
		System.out.println("电视的总销售额是："+televisionSum);
		System.out.println("计算机的总销售额是："+computerSum);
		System.out.println("手机的总销售额是："+PhoneSum);
	}
}