package dataSet;

public class Order 
{
	private String account;
	private String shop;
	private String foodName;//无论账号商家，一行即为一个订单
	private String foodCount;
	
	public Order(String account,String shop,String foodName,String foodCount)
	{
		this.account=account;
		this.shop=shop;
		this.foodName=foodName;
		this.foodCount=foodCount;
	}
	
	public String getAccount()
	{
		return this.account;
	}
	
	public String getShop()
	{
		return this.shop;
	}
	
	public String getFoodName()
	{
		return this.foodName;
	}
	
	public String getFoodCount()
	{
		return this.foodCount;
	}
	
	//主方法
	public static void main(String[] args) 
	{
		//new Order();
	}
}