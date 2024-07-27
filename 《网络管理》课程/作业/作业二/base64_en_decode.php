<?php
$file_path = "./deer.jpeg";
$file_type = pathinfo($file_path, PATHINFO_EXTENSION);   //pathinfo()以数组形式返回关于文件路径的信息 获取文件扩展名
// echo $file_type;

$file_data = file_get_contents($file_path); //获取文件内容
// echo $file_data;

$base64_data = base64_encode($file_data);   //base64编码
echo $base64_data;

$file_data1 = base64_decode($base64_data);   //base64解码
// echo $file_data1;

$file_path1 = "./deer1.jpeg";
file_put_contents($file_path1, $file_data1);  //放入新图片文件路径显示

// 直接显示
echo "
  <!DOCTYPE html>
    <html>
       <head>
         <title>Show decode image</title>
       </head>
       <body>
         <img src='$file_path1'/> 
       </body>
    </html>
  ";
