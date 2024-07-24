package dataSet;

public class Administrator 
{
	private String account;
	private String password;
	
	//构造方法
	public Administrator(String account,String password)
	{
		this.account=account;
		this.password=password;
	}
	
	//设置、获取成员变量
	public void setAccount(String account)
	{
		this.account=account;
	}
	
	public String getAccount()
	{
		return this.account;
	}
	
	public void setPassword(String password)
	{
		this.password=password;
	}
	
	public String getPassword()
	{
		return this.password;
	}
	
	//主方法
	public static void main(String[] args)
	{
		//需要参数
		//new Administrator();
	}
}
