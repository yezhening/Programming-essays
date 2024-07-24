package Experiment;

public interface MyGroup {
	public boolean isEmpty();    //判断数组是否为空
	public boolean addStu(Student x);  //添加学生信息
	public Student removeStu(int index);  //删除指定位置的学生信息
	public void sort(int x);   //按照成绩排序，参数1按数学排序，参数2按计算机排序
	public void print();	
}
