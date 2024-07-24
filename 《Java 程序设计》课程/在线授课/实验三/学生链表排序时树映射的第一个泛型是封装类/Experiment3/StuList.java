//【代码：包声明语句和包引用语句】
package Experiment3;

import java.util.*;//使用泛型

public class StuList implements MyList {
	// 【代码】 //使用双链表泛型类对象存储学生信息
	LinkedList<Student> list = new LinkedList<Student>();

	public StuList()// 接口不写构造方法
	{
		// 【代码】 Main程序中使用无参构造函数创建对象
	}

	public int size() {
		// 【代码】
		return list.size();
	}

	public boolean addStu(Student x) // 添加学生信息 注意返回值
	{
		// 【代码】
		list.add(x);
		return true;
	}

	public Student removeStu(int index) // 删除指定位置的学生信息
	{
		// 【代码】
		return list.remove(index);
	}

	public void clear() {
		// 【代码】
		list.clear();
	}

	public void sort(int x) // 使用树映射泛型类对象将学生信息按照成绩排序并且输出，参数1按数学排序，参数2按计算机排序
	{
		TreeMap<Double, Student> treemap = new TreeMap<Double, Student>();// 还有一种Map，它在内部会对Key进行排序，这种Map就是SortedMap。注意到SortedMap是接口，它的实现类是TreeMap。
		if (x == 1) {
			// 【代码】
			// 创建树图,在图中已排好序
			Iterator<Student> it = list.iterator();
			while (it.hasNext()) {
				Student t = it.next();
				treemap.put(t.getMathScore(), t);
			}
			System.out.println("按照数学成绩排序后为：");
			// 应遍历树图输出
			Collection<Student> collections = treemap.values();// 获得树图的Collection视图，创建集合（有序）
			Iterator<Student> itc = collections.iterator();
			while (itc.hasNext()) {
				Student t = itc.next();
				t.print();
			}
		} else if (x == 2) {
			// 【代码】
			Iterator<Student> it = list.iterator();
			while (it.hasNext()) {
				Student t = it.next();
				treemap.put(t.getComputerScore(), t);
			}
			System.out.println("按照计算机成绩排序后为：");
			Collection<Student> collections = treemap.values();
			Iterator<Student> itc = collections.iterator();
			while (itc.hasNext()) {
				Student t = itc.next();
				t.print();
			}

		}
	}

	public void print() {
		// 【代码】
		// Iterator<Student> it = new Iterator<Student>();//使用迭代器遍历
		Iterator<Student> it = list.iterator();
		while (it.hasNext()) {
			Student t = it.next();
			t.print();
		}
	}

}
