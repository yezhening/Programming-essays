import java.util.Scanner;

abstract class ObjectVolume {
	abstract double getVolume();// 返回物体的体积
}

class Sphere extends ObjectVolume {//子类继承不用public修饰
	final double PI = 3.14;
	double radius;

	public Sphere(double radius) {
		this.radius = radius;
	}

	public double getVolume() {
		return 4 / 3 * PI * radius * radius * radius;
	}
}

class Cube extends ObjectVolume {
	double side;

	public Cube(double side) {
		this.side = side;
	}

	public double getVolume() {
		return side * side * side;
	}
}

class Cylinder extends ObjectVolume {
	final double PI = 3.14;
	double radius;
	double length;

	public Cylinder(double radius, double length) {
		this.radius = radius;
		this.length = length;
	}

	public double getVolume() {
		return PI * radius * radius * length;
	}
}

public class Test {

	static void get(ObjectVolume obj) {
		double result = obj.getVolume();//使用
		System.out.println(result);
	}

	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);
		System.out.println("请输入整数1、2或3：");
		int select = in.nextInt();

		if (select == 1) {
			System.out.println("请输入球体半径：");
			double radius = in.nextDouble();
			// Sphere sp = new Sphere(radius);
			ObjectVolume obj = new Sphere(radius);// 上转型
			get(obj);
		} else if (select == 2) {
			System.out.println("请输入立方体边长：");
			double side = in.nextDouble();
			// Cube cu = new Cube(side);
			ObjectVolume obj = new Cube(side);
			get(obj);
		} else {
			System.out.println("请输入圆柱体半径和高：");
			double radius = in.nextDouble();
			double length = in.nextDouble();
			// Cylinder cy = new Cylinder(radius, length);
			ObjectVolume obj = new Cylinder(radius, length);
			get(obj);
		}

	}

}