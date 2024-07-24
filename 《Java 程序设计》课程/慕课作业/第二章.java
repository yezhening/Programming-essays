package y;



public class TestContinue

{

      public static void main(String args[])

    {

              int a[][]= {{85,90,96},{90,85},{90,90,80,95}};

              int i;

              

              for(i=0;i<=2;i++)

              {     

                   System.out.print(a[0][i]+" ");

              }

              System.out.print("\n");

              for(i=0;i<=1;i++)

              {     

                   System.out.print(a[1][i]+" ");

              }

              System.out.print("\n");

              for(i=0;i<=3;i++)

              {     

                   System.out.print(a[2][i]+" ");

              }

              

              System.out.print("\n");

              System.out.print("\n");

              

              int b[][]= {{85,90,96},{90,85},{90,90,80,95}};

              b=new int[3][];

              b[0]=new int[3];

              b[1]=new int[2];

              b[2]=new int[4];

              b[0][0]=85;

              b[0][1]=90;

              b[0][2]=96;

              b[1][0]=90;

              b[1][1]= 85;

              b[2][0]= 90;

              b[2][1]= 90;

              b[2][2]= 80;

              b[2][3]= 95;

              

              for(i=0;i<=2;i++)

              {     

                   System.out.print(a[0][i]+" ");

              }

              System.out.print("\n");

              for(i=0;i<=1;i++)

              {     

                   System.out.print(a[1][i]+" ");

              }

              System.out.print("\n");

              for(i=0;i<=3;i++)

              {     

                   System.out.print(a[2][i]+" ");

              }

    }

}

