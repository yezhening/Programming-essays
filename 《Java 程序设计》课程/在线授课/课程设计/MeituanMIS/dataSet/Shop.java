package dataSet;

import java.util.*;//ArrayList;

public class Shop 
{
	private String shopType;
	private String shopId;
	private String shopName;
	private String shopPassword;
	private String avgScore;
	private String avePrice;
	private String address;
	private String phone;
	private Food food;
	private ArrayList<String> comment = new ArrayList<String>();
	
	public Shop(String shopType,String shopId,String shopName,String shopPassword,String avgScore,String avePrice,String address,String phone,Food food,ArrayList<String> comment)
	{
		this.shopType=shopType;
		this.shopId=shopId;
		this.shopName=shopName;
		this.shopPassword=shopPassword;
		this.avgScore=avgScore;
		this.avePrice=avePrice;
		this.address=address;
		this.phone=phone;
		this.food=food;
		this.comment=comment;
	}
	
	//设置、获取成员变量
	public void setShopType(String shopType)
	{
		this.shopType=shopType;
	}
	
	public String getShopType()
	{
		return this.shopType;
	}
	
	public void setShopId(String shopId)
	{
		this.shopId=shopId;
	}
	
	public String getShopId()
	{
		return this.shopId;
	}
	
	public void setShopName(String shopName)
	{
		this.shopName=shopName;
	}
	
	public String getShopName()
	{
		return this.shopName;
	}
	
	public void setShopPassword(String shopPassword)
	{
		this.shopPassword=shopPassword;
	}
	
	public String getShopPassword()
	{
		return this.shopPassword;
	}
	
	public void setAvgScore(String avgScore)
	{
		this.avgScore=avgScore;
	}
	
	public String getAvgScore()
	{
		return this.avgScore;
	}
	
	public void setAvePrice(String avePrice)
	{
		this.avePrice=avePrice;
	}
	
	public String getAvePrice()
	{
		return this.avePrice;
	}
	
	public void setAddress(String address)
	{
		this.address=address;
	}
	
	public String getAddress()
	{
		return this.address;
	}
	
	public void setphone(String phone)
	{
		this.phone=phone;
	}
	
	public String getPhone()
	{
		return this.phone;
	}
	
	public Food getFood()
	{
		return this.food;
	}
	
	public ArrayList<String> getComment()
	{
		return this.comment;
	}
	
	//主方法
	public static void main(String[] args) 
	{
		//new Shop();
	}
}