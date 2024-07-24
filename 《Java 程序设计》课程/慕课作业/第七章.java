import java.util.Scanner;

import java.io.*;

public class Test

{

	public static void main(String[] args) {

		Scanner scan = new Scanner(System.in);

// （1）从键盘上输入两个文件的路径名及其文件名。

		System.out.println("请输入第一个文件的路径名：");

		String pathName1 = scan.next();

		System.out.println("请输入第一个文件的文件名：");

		String fileName1 = scan.next();

		System.out.println("请输入第二个文件的路径名：");

		String pathName2 = scan.next();

		System.out.println("请输入第二个文件的文件名：");

		String fileName2 = scan.next();

// （2）从键盘上输入若干学生的信息（姓名，数学成绩，计算机成绩，各项之间用空格隔开），以finish结束，将这些学生的信息写入第一个文件。

// 读、写中文等用字符输入、输出流

		File f1 = new File(pathName1 + "\\" + fileName1);// 用文件路径、文件名创建文件对象 没有要手动创建文件夹 注意分隔符转义

		File f2 = new File(pathName2 + "\\" + fileName2);

		try {

			FileWriter out1 = new FileWriter(f1, true);// 用File对象创建一个文件字符输出流对象 true:追加模式写文件

			System.out.println("请输入学生的信息（姓名，数学成绩，计算机成绩，各项之间用空格隔开），以finish结束，一个学生的信息为一行");

			String content = "";// 初始化

// System.out.println(content);//测试回车

			content = scan.nextLine();// 该方法接受的字符串可包含空格，回车，会将缓冲区的回车读入

			content = scan.nextLine();// 连用两个吞掉回车

// System.out.println(content);//测试回车

			while (!content.equals("finish")) {

				out1.write(content);// 重载方法写入字符串

				out1.write((int) '\r');// 重载方法写入一个字符时只能是int类型

				out1.write((int) '\n');// 单独使用FileWriter类单独写入换行

				content = scan.nextLine();

			}

			scan.close();

// out1.close();//没使用完不关闭,流关闭前会自动刷新缓冲区内容

			out1.flush();// 强制新缓冲区内容

			System.out.println("已写入第一个文件");

// （3）将第一个文件作为输入流的源，找出姓“张”的学生，将他们的信息写入第二个文件。

// FileReader in=new

// FileReader(pathName1+"\\"+fileName1);//不重新建立文件对象，否则每次读只能读前一次文件的内容× 要强制刷新缓冲区内容

			FileReader in = new FileReader(f1);// 文件字符输入流无true参数

			BufferedReader br = new BufferedReader(in);

// 在文件中读取一行内容：将FileReader和BufferedReader类混合使用，使用BufferReader的readLine()方法

// 只使用文件字符输入流对象无法从文件中一行一行的读取内容，因为无法判断一行什么时候结束

			FileWriter out2 = new FileWriter(f2, true);

			BufferedWriter bw = new BufferedWriter(out2);

// FileWriter单独使用写回车((int)'\r')、((int)'\n')

// 将FileWriter和BufferedWriter类混合使用，使用BufferedWriterr的newLine()方法写回车

			String line = null;// 另一种初始化

			while ((line = br.readLine()) != null) {

// System.out.println(line);//测试

				if (line.startsWith("张")) {// String的startsWith()

					bw.write(line);

					bw.newLine();// 写回车

				}

			}

			br.close();// 先关闭缓冲流再关闭文件流

			bw.close();

			in.close();

			out1.close();

			out2.close();

			System.out.println("已写入第二个文件");

		} catch (IOException ioe) {

			System.out.println("输入输出错误！");

		}

	}

}
