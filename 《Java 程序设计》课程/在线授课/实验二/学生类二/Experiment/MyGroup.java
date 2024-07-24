package Experiment;

public interface MyGroup 
{
	public abstract boolean isEmpty();
	public abstract boolean addStu(Student x);
	public abstract Student removeStu(int index);
	public abstract void sort(int x);
	public abstract void print();
}
