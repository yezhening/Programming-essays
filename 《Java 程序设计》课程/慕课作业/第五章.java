import java.util.Scanner;

class CountSubString
{
      //【变量声明】
	  int subCount;//检索子串个数
	  int subIndex=0;//记录子串下标移动的指针
	  int strLength;//记录主串的字符长度
	  int subLength;
	  
      //方法实现
      void countSubString(String s, String substring) //s代表整个字符串，substring代表子串
      {
           //【代码】
    	  strLength=s.length();
    	  subLength=substring.length();
    	  
    	  do
    	  {
    		  subIndex=s.indexOf(substring,subIndex); 
    		  if(subIndex==-1)
    		  {
    			  System.out.println(subCount);
    			  break;
    		  }
    		  else
    		  { 
    			  //System.out.println(subIndex);
        		  subIndex=subIndex+subLength;
        		  subCount++;
    		  }
    	  }while(subIndex<strLength);
      }
}

public class CountString
{
       public static void main(String args[])
       {
              Scanner reader=new Scanner(System.in);
              System.out.println("请输入字符串：");
              String s=reader.nextLine();
              System.out.println("请输入你要检索的子串：");
              String substring=reader.nextLine();
              CountSubString a=new CountSubString();
              a.countSubString(s, substring);
       }
}