package dataSet;

import java.util.*;//ArrayList、Iterator;
import java.io.*;

public class ShopList 
{
	ArrayList<Shop> shopList = new ArrayList<Shop>();//商家顺序表
	
	//构造方法
	public ShopList()
	{
		//读数据
		try
		{
			//File类
			File f=new File("C:\\Users\\DSHH\\Desktop\\数据集\\shop.txt");
		
			//输入流
			FileReader fr=new FileReader(f);
			BufferedReader br=new BufferedReader(fr);
		
			String line;//读一行数据
			String[] content;//一行数据中多个字符串构成的字符串数组
			
			String shopType;
			String shopId;
			String shopName;
			String shopPassword;
			String avgScore;
			String avePrice;
			String address;
			String phone;
			ArrayList<String> foodId = new ArrayList<String>();
			ArrayList<String> foodName = new ArrayList<String>();
			ArrayList<String> foodPrice = new ArrayList<String>();
			ArrayList<String> foodCount = new ArrayList<String>();
			ArrayList<String> comment = new ArrayList<String>();
			
			String[] tempFood;//用于处理food行分隔逗号
			String tempComment;//用于处理Comment去除首部空格
			
			//谨慎
			line=br.readLine();
			while(line!=null)
			{
				content=line.split(" ");//所读内容不为food和Comment	
				shopType=content[1];
				
				line=br.readLine();
				content=line.split(" ");
				shopId=content[1];
				
				line=br.readLine();
				content=line.split(" ");
				shopName=content[1];

				line=br.readLine();
				content=line.split(" ");
				shopPassword=content[1];

				line=br.readLine();
				content=line.split(" ");
				avgScore=content[1];	
				
				line=br.readLine();
				content=line.split(" ");
				avePrice=content[1];	
				
				line=br.readLine();
				content=line.split(" ");
				address=content[1];	
				
				line=br.readLine();
				content=line.split(" ");
				phone=content[1];
				
				while(true)
				{
					line=br.readLine();
					
					if((line==null)||((line.charAt(0))=='s'))//所读内容为shoptype或已读完
					{
						break;
					}
					
					if((line.charAt(0))=='f')//所读内容为food_id
					{
						content=line.split(" ");
						
						tempFood=content[1].split(",");
						foodId.add(tempFood[0]);//分隔逗号	
						
						tempFood=content[3].split(",");
						foodName.add(tempFood[0]);
						
						tempFood=content[5].split(",");
						foodPrice.add(tempFood[0]);
						
						foodCount.add("10");//生成菜品数量为十份
					}
					
					if((line.charAt(0))=='C')//所读内容为Comment
					{
						content=line.split(":");
						
						tempComment=content[1].stripLeading();//去除首部空格
						comment.add(tempComment);
					}
				}
				
				Food food=new Food(foodId,foodName,foodPrice,foodCount);//一个商家的所有菜品
				Shop shop=new Shop(shopType,shopId,shopName,shopPassword,avgScore,avePrice,address,phone,food,comment);//一个商家	
				shopList.add(shop);//商家顺序表
				
				//清空表，存入下一个商家信息,相比remove方法，不删除引用的方式清空表
				foodId=new ArrayList<String>();
				foodName=new ArrayList<String>();
				foodPrice=new ArrayList<String>();
				comment=new ArrayList<String>();
			}//每一个商家
		
			//关闭流
			br.close();
			fr.close();
		}
		catch(IOException ioe) 
		{
			System.out.println(ioe);
		}		
	}

	//使用类对象的成员变量
	public ArrayList<Shop> getShopList()
	{
		return this.shopList;
	}
	
	//查询商家信息
	public String query(String keyWord)
	{
		Iterator<Shop> it=this.shopList.iterator();
		int index=-1;//商家名称是否存在匹配关键词的标志位
		ArrayList<Shop> data=new ArrayList<Shop>();//将匹配到的商家信息存入顺序表中
		
		while(it.hasNext())
		{
			Shop s=it.next();//一个商家
			
			index =s.getShopName().indexOf(keyWord);
			if(index!=-1)//该商家名称存在匹配该关键词的
			{
				data.add(s);
				index=-1;//重置标志位
			}
		}	
		
		//从顺序表中取商家信息
		String content="\n";
		
		Iterator<Shop> it1=data.iterator();
		while(it1.hasNext())
		{
			Shop s=it1.next();//一个商家
			
			String content1="*******************************************************************************\n";
			String content2="商家类型："+s.getShopType()+"\n";
			String content3="商家名称："+s.getShopName()+"\n";
			String content4="平均评分："+s.getAvgScore()+"\n";
			String content5="平均价格："+s.getAvePrice()+"\n";
			String content6="地址："+s.getAddress()+"\n";
			String content7="电话："+s.getPhone()+"\n";
			String content8=content1+content2+content3+content4+content5+content6+content7+"\n";
			
			//格式化字符串，一个tab
			String content9="菜品名称	菜品价格	菜品数量	共有"+String.valueOf(s.getFood().getFoodName().size())+"种菜品\n";//int转换为字符串
			String content10="———————————————————————————————\n";
			String content11=content9+content10;
			
			for(int i=0;i<s.getFood().getFoodName().size();i++)
			{
				String content12=s.getFood().getFoodName().get(i)+"	"+s.getFood().getFoodPrice().get(i)+"	"+s.getFood().getFoodCount().get(i)+"	"+"\n";
				content11=content11+content12;
			}
			
			String content13="\n客户评价：\n";
			String content14="—————\n";
			String content15=content13+content14;
			
			if(s.getComment().size()==0)//没有评价
			{
				content15=content15+"该商家无客户评价\n";
			}
			else
			{
				for(int i=0;i<s.getComment().size();i++)
				{
					content15=content15+s.getComment().get(i)+"\n";
				}
			}
			content=content+content8+content11+content15;
		}	
		
		//将显示内容返回给图形界面
		return content;
	}
	
	//修改“虚拟”商家表，处理实际用户订单信息	整个函数一气呵成！
	public String handleOrder(ArrayList<Order> orderList)//传订单顺序表
	{
		ArrayList<Order> orderFlags=new ArrayList<Order>();//记录预定成功的订单
		String content="订单预定结果:\n";//获得内容传回界面
		
		//修改商家表中的菜品表
		Iterator<Shop> shopIt=this.shopList.iterator();//商家表
		while(shopIt.hasNext())
		{
			Shop shop=shopIt.next();//一个餐馆
			
			Iterator<Order> orderIt=orderList.iterator(); //订单表
			while(orderIt.hasNext())
			{
				Order order=orderIt.next();//一个订单
				
				if(shop.getShopName().equals(order.getShop()))//匹配餐馆
				{
					Iterator<String> foodNameIt=shop.getFood().getFoodName().iterator(); //餐馆取菜品取菜品名表
					int index=-1;//记录匹配菜名的索引
					
					while(foodNameIt.hasNext())
					{
						index++;
						String shopfoodName=foodNameIt.next();//一份菜品名
						
						if(shopfoodName.equals(order.getFoodName()))//匹配菜名
						{
							//修改数量
							//原来的
							String shopFoodCount=shop.getFood().getFoodCount().get(index);//从菜品数量表中取和菜品名相同索引的菜品数量
							int foodCount1=Integer.parseInt(shopFoodCount);//转换
							
							//订单的
							String orderFoodCount=order.getFoodCount();
							int foodCount2=Integer.parseInt(orderFoodCount);//转换
							
							foodCount1=foodCount1-foodCount2;//相减
							
							if(foodCount1>=0)//预定成功的订单
							{
								orderFlags.add(order);
								
								content=content+order.getAccount()+" "+order.getShop()+" "+order.getFoodName()+" "+order.getFoodCount()+" 订单预定成功！\n";
								
								//写回
								shopFoodCount=String.valueOf(foodCount1);//int转换为String
								
								shop.getFood().getFoodCount().remove(index);//删除原有元素
								shop.getFood().getFoodCount().add(index, shopFoodCount);//插入修改后元素
							}
							else//预定失败的订单
							{
								content=content+order.getAccount()+" "+order.getShop()+" "+order.getFoodName()+" "+order.getFoodCount()+" 订单预定失败！菜品份数已不够，请重新查询菜品信息。\n";
							}
						}//匹配菜名	
					}//一份菜品
				}//一个订单
			}//一家餐馆
		}//while
		
		//预定成功才写入预定信息文件
		try
		{
			//文件类
			File f=new File("C:\\Users\\DSHH\\Desktop\\数据集\\order.txt");
					
			//文件字符输出流，字符缓冲输出流
			FileWriter fw=new FileWriter(f,true);//追加
			BufferedWriter bw=new BufferedWriter(fw);
					
			String line="\n\n";		
			
			Iterator<Order> flagIt=orderFlags.iterator();
			while(flagIt.hasNext())
			{
				Order o=flagIt.next();//预定成功的订单
				
				line=o.getAccount()+" "+o.getShop()+" "+o.getFoodName()+" "+o.getFoodCount();//第一行多写入一个回车
				bw.write(line);
				bw.newLine();							
			}
					
			//关闭流
			bw.close();
			fw.close();
		}
		catch(IOException ioe)
		{
			System.out.println(ioe);
		}
		
		return content;//return语句写在函数末尾
	}//函数
			
	//判断商家账号、密码是否正确
	public String login(String account,String password)
	{
		//判断账号或密码错误
		String judgement="";
		boolean accountFlag=false;
		boolean passwordFlag=false;
			
		Iterator<Shop> it=this.shopList.iterator();
		while(it.hasNext())//每个商家
		{
			Shop s=it.next();
				
			if(s.getShopId().equals(account))//账号匹配
			{
				accountFlag=true;
					
				if(s.getShopPassword().equals(password))//密码匹配
				{
					passwordFlag=true;
						
					break;//匹配成功退出循环
				}
			}
		}
			
		//设置返回值
		if(accountFlag&&passwordFlag)
		{
			judgement="11";
		}
		else if(!accountFlag&&passwordFlag)
		{
			judgement="01";
		}
		else if(accountFlag&&!passwordFlag)
		{
			judgement="10";
		}
		else
		{
			judgement="00";
		}
			
		return judgement;
	}
		
	//判断是否存在商家账号
	public boolean contains(String account)
	{
		boolean flag=false;
		Iterator<Shop> it=this.shopList.iterator();
				
		while(it.hasNext())
		{
			Shop s=it.next();
			if(s.getShopId().equals(account))//存在该商家账号时标志置位
			{
				flag=true;
				break;//不再比较
			}
		}
		return flag;
	}
	
	//商家注册方法
	public void register(String type,String id,String name,String password,String avgScore,String avePrice,String address,String phone,String food)
	{
		//取菜品数据
		ArrayList<String> foodId = new ArrayList<String>();
		ArrayList<String> foodName = new ArrayList<String>();
		ArrayList<String> foodPrice = new ArrayList<String>();
		ArrayList<String> foodCount = new ArrayList<String>();
		ArrayList<String> comment = new ArrayList<String>();//Shop类对象构造需要菜品数量参数，不写入文件即可
		
		String[] foodLines=food.split("\n");//所有菜品
		
		for(int i=0;i<foodLines.length;i++)//对每一份菜品
		{
			String[] foodContent=foodLines[i].split(" ");
			
			foodId.add(foodContent[0]);
			foodName.add(foodContent[1]);
			foodPrice.add(foodContent[2]);
			foodCount.add("0");//Food类对象构造需要菜品数量参数，默认构造0，不写入文件即可
		}

		Food food1=new Food(foodId,foodName,foodPrice,foodCount);//所有菜品
		Shop shop=new Shop(type,id,name,password,avgScore,avePrice,address,phone,food1,comment);//一个商家
		this.shopList.add(shop);
		
		//修改文件
		try
		{
			//文件类
			File f=new File("C:\\Users\\DSHH\\Desktop\\数据集\\shop.txt");
					
			//文件字符输出流，字符缓冲输出流
			FileWriter fw=new FileWriter(f,true);//追加
			BufferedWriter bw=new BufferedWriter(fw);
				
			bw.newLine();//第一行多写入一个换行
			
			String line="shoptype: "+type;
			bw.write(line);
			bw.newLine();
			
			line="shopId: "+id;
			bw.write(line);
			bw.newLine();
			
			line="shopName: "+name;
			bw.write(line);
			bw.newLine();
			
			line="shopPassword: "+password;
			bw.write(line);
			bw.newLine();
			
			line="avgScore: "+password;
			bw.write(line);
			bw.newLine();
			
			line="avePrice: "+password;
			bw.write(line);
			bw.newLine();
				
			line="address: "+password;
			bw.write(line);
			bw.newLine();
				
			line="phone: "+password;
			bw.write(line);
			bw.newLine();
			
			int index=-1;//序号、名字和价格表为相同索引
			Iterator<String> foodIdIt=foodId.iterator();
			while(foodIdIt.hasNext())
			{
				index++;
				String foodId1=foodIdIt.next();
				
				String foodName1=foodName.get(index);
				String foodPrice1=foodPrice.get(index);
				
				line="food_id: "+foodId1+", food_name: "+foodName1+", food_price: "+foodPrice1;
				bw.write(line);
				bw.newLine();
			}
			
			
			//关闭流
			bw.close();
			fw.close();
		}
		catch(IOException ioe)
		{
			System.out.println(ioe);
		}
	}
	
	//通过商家账号查询商家名称
	public String idToName(String account)
	{
		String shopName="";
			
		Iterator<Shop> it=this.shopList.iterator();
		while(it.hasNext())
		{
			Shop shop=it.next();//一个商家
							
			if(shop.getShopId().equals(account))//匹配账号
			{
				shopName=shop.getShopName(); 
			}
		}
		return shopName;
	}
	
	//修改商家信息
	public String modify(String account,String text)
	{
		//返回界面的内容
		String showText="";
		
		//取修改内容
		String[] line=text.split("\n");
		
		String type="";
		String id="";
		String name="";
		String password="";
		String avePrice="";
		String address="";
		String phone="";
		for(int i=0;i<line.length;i++)//对每一行
		{
			String[] content=line[0].split(":");
			type=content[1];
			id=content[1];
			name=content[1];
			password=content[1];
			avePrice=content[1];
			address=content[1];
			phone=content[1];
		}
		
		//修改商家顺序表
		Iterator<Shop> it=this.shopList.iterator();
		while(it.hasNext())
		{
			Shop s=it.next();
			
			if(s.getShopId().equals(account))//匹配商家
			{
				s.setShopType(type);
				
				//判断修改后的账号是否重复
				boolean accountFlag=false;
				Iterator<Shop> it1=this.shopList.iterator();
				while(it1.hasNext())
				{
					Shop s1=it1.next();
					
					if(s1.getShopId().equals(id))//存在重复的账号
					{
						accountFlag=true;
						showText="存在重复的账号，未修改账号信息。\n";
					}
				}
				if(!accountFlag)//不存在重复的账号
				{
					s.setShopId(id);
					showText=showText+"信息修改成功！修改账号后请重新登录。";
				}
				else
				{
					showText=showText+"其余信息修改成功！";
				}
				s.setShopName(name);
				s.setShopPassword(password);
				s.setAvePrice(avePrice);
				s.setAddress(address);
				s.setphone(phone);
			}
		}
		
		//修改文件
		try
		{
			//File类
			File f=new File("C:\\Users\\DSHH\\Desktop\\数据集\\shop.txt");
		
			//输出流
			FileWriter fw=new FileWriter(f);
			BufferedWriter bw=new BufferedWriter(fw);
			
			//写修改后的顺序表
			Iterator<Shop> it1=shopList.iterator();
			while(it1.hasNext())
			{
				Shop s=it1.next();
				String content="";
				
				String content1="shopType: "+s.getShopType()+"\n";
				String content2="shopId: "+s.getShopId()+"\n";
				String content3="shopName: "+s.getShopName()+"\n";
				String content4="shopPassword: "+s.getShopPassword()+"\n";
				String content5="avgScore: "+s.getAvgScore()+"\n";
				String content6="avePrice: "+s.getAvePrice()+"\n";
				String content7="address: "+s.getAddress()+"\n";
				String content8="phone: "+s.getPhone()+"\n";
				content=content1+content2+content3+content4+content5+content6+content7+content8;
				
				String content9="";
				String content10="";
				for(int i=0;i<s.getFood().getFoodId().size();i++)
				{
					content9="food_id: "+s.getFood().getFoodId().get(i)+", "+"food_name: "+s.getFood().getFoodName().get(i)+", "+"food_price: "+s.getFood().getFoodPrice().get(i)+"\n";
					content=content+content9;
				}
				
				for(int i=0;i<s.getComment().size();i++)
				{
					content10="Comment"+i+": "+s.getComment().get(i)+"\n";
					content=content+content10;
				}
				
				bw.write(content);
			}
			
			//关闭流
			bw.close();
			fw.close();
		}
		catch(IOException ioe) 
		{
			System.out.println(ioe);
		}	
		
		return showText;
	}

	//注销商家信息
	public void logout(String account)
	{
		Iterator<Shop> it=this.shopList.iterator();
		
		//修改商家顺序表
		while(it.hasNext())
		{
			Shop s=it.next();
			
			if(s.getShopId().equals(account))//迭代器删除表信息
			{
				it.remove();
			}
		}
		
		//修改文件
		try
		{
			//File类
			File f=new File("C:\\Users\\DSHH\\Desktop\\数据集\\shop.txt");
		
			//输出流
			FileWriter fw=new FileWriter(f);
			BufferedWriter bw=new BufferedWriter(fw);
			
			//读修改后的顺序表
			Iterator<Shop> it1=shopList.iterator();
			while(it1.hasNext())
			{
				Shop s=it1.next();
				String content="";
				
				String content1="shopType: "+s.getShopType()+"\n";
				String content2="shopId: "+s.getShopId()+"\n";
				String content3="shopName: "+s.getShopName()+"\n";
				String content4="shopPassword: "+s.getShopPassword()+"\n";
				String content5="avgScore: "+s.getAvgScore()+"\n";
				String content6="avePrice: "+s.getAvePrice()+"\n";
				String content7="address: "+s.getAddress()+"\n";
				String content8="phone: "+s.getPhone()+"\n";
				content=content1+content2+content3+content4+content5+content6+content7+content8;
				
				String content9="";
				String content10="";
				for(int i=0;i<s.getFood().getFoodId().size();i++)
				{
					content9="food_id: "+s.getFood().getFoodId().get(i)+", "+"food_name: "+s.getFood().getFoodName().get(i)+", "+"food_price: "+s.getFood().getFoodPrice().get(i)+"\n";
					content=content+content9;
				}
				
				for(int i=0;i<s.getComment().size();i++)
				{
					content10="Comment"+i+": "+s.getComment().get(i)+"\n";
					content=content+content10;
				}
				
				bw.write(content);
			}
			
			//关闭流
			bw.close();
			fw.close();
		}
		catch(IOException ioe) 
		{
			System.out.println(ioe);
		}	
	}
	
	//添加菜品
	public void addFood(String account,String foodId,String foodName,String foodPrice)
	{
		//修改商家表
		Iterator<Shop> it=this.shopList.iterator();
		while(it.hasNext())
		{
			Shop s=it.next();//一个商家
			
			if(s.getShopId().equals(account))//匹配账号
			{
				Food food=s.getFood();//所有菜品
				
				food.addFoodId(foodId);
				food.addFoodName(foodName);
				food.addFoodPrice(foodPrice);
			}
		}
		
		//写文件
		try
		{
			//File类
			File f=new File("C:\\Users\\DSHH\\Desktop\\数据集\\shop.txt");
				
			//输出流
			FileWriter fw=new FileWriter(f);
			BufferedWriter bw=new BufferedWriter(fw);
					
			//读修改后的顺序表
			Iterator<Shop> it1=this.shopList.iterator();
			while(it1.hasNext())
			{
				Shop s=it1.next();
				String content="";
						
				String content1="shopType: "+s.getShopType()+"\n";
				String content2="shopId: "+s.getShopId()+"\n";
				String content3="shopName: "+s.getShopName()+"\n";
				String content4="shopPassword: "+s.getShopPassword()+"\n";
				String content5="avgScore: "+s.getAvgScore()+"\n";
				String content6="avePrice: "+s.getAvePrice()+"\n";
				String content7="address: "+s.getAddress()+"\n";
				String content8="phone: "+s.getPhone()+"\n";
				content=content1+content2+content3+content4+content5+content6+content7+content8;
						
				String content9="";
				String content10="";
				for(int i=0;i<s.getFood().getFoodId().size();i++)
				{
					content9="food_id: "+s.getFood().getFoodId().get(i)+", "+"food_name: "+s.getFood().getFoodName().get(i)+", "+"food_price: "+s.getFood().getFoodPrice().get(i)+"\n";
					content=content+content9;
				}
						
				for(int i=0;i<s.getComment().size();i++)
				{
					content10="Comment"+i+": "+s.getComment().get(i)+"\n";
					content=content+content10;
				}
						
				bw.write(content);
			}
					
			//关闭流
			bw.close();
			fw.close();
		}
		catch(IOException ioe) 
		{
			System.out.println(ioe);
		}
	}
	
	//删除菜品
	public void deleteFood(String account,String foodName)
	{
		//修改商家表
		Iterator<Shop> it=this.shopList.iterator();
		while(it.hasNext())
		{
			Shop shop=it.next();//一个商家
					
			if(shop.getShopId().equals(account))//匹配账号
			{
				Food food=shop.getFood();//所有菜品
						
				food.deleteFood(foodName);
			}
		}
		
		//写文件
		try
		{
			//File类
			File f=new File("C:\\Users\\DSHH\\Desktop\\数据集\\shop.txt");
						
			//输出流
			FileWriter fw=new FileWriter(f);
			BufferedWriter bw=new BufferedWriter(fw);
							
			//写修改后的顺序表
			Iterator<Shop> it1=this.shopList.iterator();
			while(it1.hasNext())
			{
				Shop s=it1.next();
				String content="";
								
				String content1="shopType: "+s.getShopType()+"\n";
				String content2="shopId: "+s.getShopId()+"\n";
				String content3="shopName: "+s.getShopName()+"\n";
				String content4="shopPassword: "+s.getShopPassword()+"\n";
				String content5="avgScore: "+s.getAvgScore()+"\n";
				String content6="avePrice: "+s.getAvePrice()+"\n";
				String content7="address: "+s.getAddress()+"\n";
				String content8="phone: "+s.getPhone()+"\n";
				content=content1+content2+content3+content4+content5+content6+content7+content8;
								
				String content9="";
				String content10="";
				for(int i=0;i<s.getFood().getFoodId().size();i++)
				{
					content9="food_id: "+s.getFood().getFoodId().get(i)+", "+"food_name: "+s.getFood().getFoodName().get(i)+", "+"food_price: "+s.getFood().getFoodPrice().get(i)+"\n";
					content=content+content9;
				}
								
				for(int i=0;i<s.getComment().size();i++)
				{
					content10="Comment"+i+": "+s.getComment().get(i)+"\n";
					content=content+content10;
				}
								
				bw.write(content);
			}
							
			//关闭流
			bw.close();
			fw.close();
		}
		catch(IOException ioe) 
		{
			System.out.println(ioe);
		}
	}
	
	//修改菜品
	public void modifyFood(String account,String oldFoodName,String foodId,String foodName,String foodPrice)
	{
		//修改商家表
		Iterator<Shop> it=this.shopList.iterator();
		while(it.hasNext())
		{
			Shop shop=it.next();//一个商家
				
			if(shop.getShopId().equals(account))//匹配账号
			{
				Food food=shop.getFood();//所有菜品
					
				food.modifyFood(oldFoodName,foodId,foodName,foodPrice);
			}
		}
			
		//写文件
		try
		{
			//File类
			File f=new File("C:\\Users\\DSHH\\Desktop\\数据集\\shop.txt");
					
			//输出流
			FileWriter fw=new FileWriter(f);
			BufferedWriter bw=new BufferedWriter(fw);
						
			//读修改后的顺序表
			Iterator<Shop> it1=this.shopList.iterator();
			while(it1.hasNext())
			{
				Shop s=it1.next();
				String content="";
							
				String content1="shopType: "+s.getShopType()+"\n";
				String content2="shopId: "+s.getShopId()+"\n";
				String content3="shopName: "+s.getShopName()+"\n";
				String content4="shopPassword: "+s.getShopPassword()+"\n";
				String content5="avgScore: "+s.getAvgScore()+"\n";
				String content6="avePrice: "+s.getAvePrice()+"\n";
				String content7="address: "+s.getAddress()+"\n";
				String content8="phone: "+s.getPhone()+"\n";
				content=content1+content2+content3+content4+content5+content6+content7+content8;
							
				String content9="";
				String content10="";
				for(int i=0;i<s.getFood().getFoodId().size();i++)
				{
					content9="food_id: "+s.getFood().getFoodId().get(i)+", "+"food_name: "+s.getFood().getFoodName().get(i)+", "+"food_price: "+s.getFood().getFoodPrice().get(i)+"\n";
					content=content+content9;
				}
							
				for(int i=0;i<s.getComment().size();i++)
				{
					content10="Comment"+i+": "+s.getComment().get(i)+"\n";
					content=content+content10;
				}
							
				bw.write(content);
			}
						
			//关闭流
			bw.close();
			fw.close();
		}
		catch(IOException ioe) 
		{
			System.out.println(ioe);
		}
	}
	
	//通过商家名称查询商家账号
	public String nameToId(String shopName)
	{
		String shopId="";
				
		Iterator<Shop> it=this.shopList.iterator();
		while(it.hasNext())
		{
			Shop shop=it.next();//一个商家
								
			if(shop.getShopName().equals(shopName))//匹配名称
			{
					shopId=shop.getShopId(); 
			}
		}
		return shopId;
	}
	
	//主方法
	public static void main(String[] args)
	{
		//new ShopList();
	}
}