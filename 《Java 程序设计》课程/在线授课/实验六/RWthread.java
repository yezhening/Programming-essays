import java.util.*;

public class RWthread
{
    public static void main(String args[])
    {
        String readName="读线程",writeName="写线程";
        ReadWrite rw=new ReadWrite(readName,writeName);//只是多了传参，给予将要创建的线程命名	实现了Runnable接口中run()方法的类的对象
        
        rw.read.start();//注意引用
        rw.write.start();
   }
}

class ReadWrite implements Runnable
{  
   String ID=null;
   String name=null;   
   String readName,writeName;
   boolean flag = false;
   
   Thread read,write;//要放在构造函数外声明
   
   ReadWrite(String s1,String s)
   { 
      readName=s1;
      writeName=s;  
      
      read=new Thread(this);
      write=new Thread(this);
      
      read.setName(readName);
      write.setName(writeName);
   }
   
   public void run()
   {
	   while(true)//循环运行读写线程
	   {
		   if(Thread.currentThread().getName().equals(readName))//读线程
	       {  
			  Scanner reader=new Scanner(System.in); 
			   
			  if(!flag)//如果flag为false,调用读线程
			  {
				  System.out.println("请输入学号：");
		          ID=reader.nextLine(); 
		          
		          if(ID.equals("finish"))
	        	  {
	        		  System.out.println("\n读线程和写线程工作结束！");
	        		  flag=true;
	        		  write.interrupt();
	        		  reader.close(); 
	        		  return;
	        	  }
		          
		          System.out.println("请输入姓名：");
		          name=reader.nextLine();           
		          flag=true;//修改标志，必须调用一次写线程才能再次调用读线程
		          write.interrupt();//调用一次读线程后，调用一次写线程
			  }
	       }  
	          
		   
	      else if(Thread.currentThread().getName().equals(writeName))//写线程
	      {    
	    	  if(!flag)//如果flag为false,未调用读线程，则先调用读线程	如果flag为true,已经调用了一次读线程,则调用一次写线程
	    	  {
	    		  try{  
		              read.join();//不能用读线程.join()	每一次循环都先调用读线程
		           } catch(InterruptedException e) { }
	    	  }
	    	 
	    	  if(ID.equals("finish"))
	    	  {
	    		  return;
	    	  }
	    	  
	          System.out.println("\n输出学号："+ID+"，输出姓名："+name);
	          System.out.println();  
	          flag=false;//修改标志，必须调用一次读线程才能再次调用写线程
	      }     
	   } 
   }  
}
  

import java.util.*;

public class RWthread
{
    public static void main(String args[])
    {
        String readName="读线程",writeName="写线程";
        ReadWrite rw=new ReadWrite(readName,writeName);
        
        rw.read.start();//注意引用
        rw.write.start();
   }
}

class ReadWrite implements Runnable
{  
   String ID=null;
   String name=null;   
   String readName,writeName;
   boolean flag = false;
   
   Thread read,write;//要放在构造函数外声明
   
   ReadWrite(String s1,String s)
   { 
      readName=s1;
      writeName=s;  
      
      read=new Thread(this);
      write=new Thread(this);
      
      read.setName(readName);
      write.setName(writeName);
   }
   
   public void run()
   {
	   while(true)//循环运行读写线程
	   {
		   if(Thread.currentThread().getName().equals(readName))//读线程
	       {  
			  Scanner reader=new Scanner(System.in); 
			   
			  if(!flag)//如果flag为false,调用读线程
			  {
				  System.out.println("请输入学号：");
		          ID=reader.nextLine(); 
		          
		          if(ID.equals("finish"))
	        	  {
	        		  System.out.println("\n读线程和写线程工作结束！");
	        		  flag=true;
	        		  write.interrupt();
	        		  reader.close(); 
	        		  return;
	        	  }
		          
		          System.out.println("请输入姓名：");
		          name=reader.nextLine();           
		          flag=true;//修改标志，必须调用一次写线程才能再次调用读线程
		          write.interrupt();//调用一次读线程后，调用一次写线程
			  }
	       }  
	          
		   
	      else if(Thread.currentThread().getName().equals(writeName))//写线程
	      {    
	    	  if(!flag)//如果flag为false,未调用读线程，则先调用读线程	如果flag为true,已经调用了一次读线程,则调用一次写线程
	    	  {
	    		  try{  
		              read.join();//不能用读线程.join()	每一次循环都先调用读线程
		           } catch(InterruptedException e) { }
	    	  }
	    	 
	    	  if(ID.equals("finish"))
	    	  {
	    		  return;
	    	  }
	    	  
	          System.out.println("\n输出学号："+ID+"，输出姓名："+name);
	          System.out.println();  
	          flag=false;//修改标志，必须调用一次读线程才能再次调用写线程
	      }     
	   } 
   }  
}
  

