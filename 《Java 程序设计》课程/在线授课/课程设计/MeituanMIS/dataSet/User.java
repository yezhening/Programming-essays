package dataSet;

public class User 
{
	private String account;
	private String password;
	private String contact;
	
	//构造方法
	public User(String account,String password,String contact)
	{
		this.account=account;
		this.password=password;
		this.contact=contact;
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
	
	public void setContact(String contact)
	{
		this.contact=contact;
	}
	
	public String getContact()
	{
		return this.contact;
	}
	
	//主方法
	public static void main(String[] args)
	{
		//需要参数
		//new User();
	}
}
