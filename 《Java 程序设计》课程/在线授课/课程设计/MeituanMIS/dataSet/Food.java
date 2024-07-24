package dataSet;

import java.util.*;//ArrayList、Iterator

public class Food 
{
	private ArrayList<String> foodId = new ArrayList<String>();
	private ArrayList<String> foodName = new ArrayList<String>();
	private ArrayList<String> foodPrice = new ArrayList<String>();
	private ArrayList<String> foodCount = new ArrayList<String>();
	
	public Food(ArrayList<String> foodId,ArrayList<String> foodName,ArrayList<String> foodPrice,ArrayList<String> foodCount)
	{
		this.foodId=foodId;
		this.foodName=foodName;
		this.foodPrice=foodPrice;
		this.foodCount=foodCount;
	}
	
	public ArrayList<String> getFoodId()
	{
		return this.foodId;
	}
	
	public ArrayList<String> getFoodName()
	{
		return this.foodName;
	}
	
	public ArrayList<String> getFoodPrice()
	{
		return this.foodPrice;
	}
	
	public ArrayList<String> getFoodCount()
	{
		return this.foodCount;
	}
	
	//添加菜品序号、名字和价格
	public void addFoodId(String foodId)
	{
		this.foodId.add(foodId);
	}
	
	public void addFoodName(String foodName)
	{
		this.foodName.add(foodName);
	}
	
	public void addFoodPrice(String foodPrice)
	{
		this.foodPrice.add(foodPrice);
	}
	
	//删除菜品序号、名字和价格
	public void deleteFood(String foodName)
	{
		int index=-1;//记录匹配菜品名字的表索引（三个表的索引同）
		
		Iterator<String> foodNameIt=this.foodName.iterator();
		while(foodNameIt.hasNext())
		{
			index++;
			String thisFoodName=foodNameIt.next();
			
			if(thisFoodName.equals(foodName))//菜品名称匹配
			{
				foodNameIt.remove();//删除菜品名
				
				break;
			}
		}
		
		this.foodId.remove(index);//删除菜品序号
		this.foodPrice.remove(index);//删除菜品价格
	}
	
	//修改菜品序号、名字和价格
	public void modifyFood(String oldFoodName,String foodId,String foodName,String foodPrice)
	{
		int index=-1;//记录匹配菜品名字的表索引（三个表的索引同）
			
		Iterator<String> foodNameIt=this.foodName.iterator();
		while(foodNameIt.hasNext())
		{
			index++;
			String thisFoodName=foodNameIt.next();
				
			if(thisFoodName.equals(oldFoodName))//菜品名称匹配
			{
				this.foodName.set(index, foodName);//修改菜名	set()
				
				break;
			}
		}
			
		this.foodId.set(index,foodId);//修改菜品序号
		this.foodPrice.set(index,foodPrice);//修改菜品价格
	}
}