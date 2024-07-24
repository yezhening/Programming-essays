import java.util.Scanner;
import java.io.*;

public class Test2 {
	public static void main(String[] args) {
		Scanner scan = new Scanner(System.in);

		// （1）将第一部分产生的文件中的姓名“李善”改为“王山”。（就在原文件中修改。）
		System.out.println("请输入所修改文件的路径名和文件名：");
		String pathName = scan.next();
		File f = new File(pathName);
		
		try {
			RandomAccessFile raf = new RandomAccessFile(f, "rw");
			
			// 解决读取乱码问题
			//String s = null;
			//Exception in thread "main" java.lang.NullPointerException: Cannot invoke "String.getBytes(String)" because "s" is null at Test2.main(Test2.java:17)
			String s = "";
			byte[] b = s.getBytes("ISO-8859-1");//返回字符串的"iso-8858-1"编码字节。
			//RandomAccessFile 读写文件时，不管文件中保存的数据编码格式是什么   使用 RandomAccessFile对象方法的 readLine() 都会将编码格式转换成 ISO-8859-1 所以 输出显示是还要在进行一次转码
			String s1 = new String(b);//新建字符串。

			// （1）将第一部分产生的文件中的姓名“李善”改为“王山”。（就在原文件中修改。）
			String s2 = "李善";
			String s3 = "王山";
			
			long beforeLastPointer = 0;// 记录上一次文件指针位置
			long nowLastPointer = 0;// 记录这一次文件指针位置
			
			//解码显示全部文件内容
			/*byte[] buf=new byte[(int)raf.length()];
		    raf.read(buf);//将内容写入buf字节数组中 读取字节数据
		    String str=new String(buf,"gbk");
		    System.out.println(str);*/
		    
			while ((s1 = raf.readLine()) != null) {
				//解码显示一行文件内容
				//System.out.println(s1);//要转码显示
				//System.out.println(new String(s1.getBytes("ISO-8859-1"),"gbk"));//需要重新转码才能正常显示 匿名类 utf-8不行 测试
				//System.out.println(new String(s1.getBytes("ISO-8859-1")));//重新解析
				
			    //修改    
				nowLastPointer = raf.getFilePointer();//取这一次文件指针位置
				String str1=new String(s1.getBytes("ISO-8859-1"),"gbk");
				if (str1.startsWith(s2))// String contains()
				{
					//System.out.println(str1);//测试
					
					String str = str1.replace(s2, s3);// String replace() 只是替换，仍然为一行内容
					//System.out.println(str);//测试
					
					raf.seek(beforeLastPointer);// 定位到上一次文件指针位置进行写入
					//raf.writeChars(str);
					//raf.writeBytes(str);
					raf.write(str.getBytes());//重新解析
				}
				beforeLastPointer=nowLastPointer;//更新指针
				//raf.seek(nowLastPointer);// 定位到这一次文件指针位置，继续往下读 多余，写入后指针已经移位到最后了
			}
			
			System.out.println("修改完成。");
			
			//（2）之后，在文件的最后继续添加学生信息。
			System.out.println("请输入添加学生的信息（姓名，数学成绩，计算机成绩，各项之间用空格隔开），以finish结束，一个学生的信息为一行");
			String content = "";
			content = scan.nextLine();
			content = scan.nextLine();
			
			while (!content.equals("finish")) {
				raf.write(content.getBytes());
				raf.write((int) '\r');//单独写
				raf.write((int) '\n');
				content = scan.nextLine();
			}
			
			System.out.println("添加完成。");
			scan.close();
			raf.close();
		} catch (IOException ioe) {
			System.out.println("输入输出错误！");
		}
	}
}

			
			
			
			
			
		