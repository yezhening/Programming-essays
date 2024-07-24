<?php
$idd = mysqli_connect("localhost", "n192705", "Ye011013")
    or die("could not connect:" . mysqli_error($idd));
mysqli_select_db($idd, "n192705");
mysqli_query($idd, "set names utf8");

$resultt = mysqli_query($idd, "select * from User"); //执行SQL语句，并返回结果
while ($rowData = mysqli_fetch_array($resultt, MYSQLI_ASSOC)) //从结果集中获取所有数据,每次读取一行 MYSQLI_ASSOC:得到关联数组
{
    $data = explode(":", $rowData['uid']); //用单引号 关联数组中各个字段用分号间隔
    $uidData = (int)$data[0]; //取到的是值
    print_r($uidData);

    $data = explode(":", $rowData['name']);
    $nameData = $data[0];
    print_r($nameData);

    $data = explode(":", $rowData['password']);
    $passwordData = $data[0];
    print_r($passwordData);

    $data = explode(":", $rowData['sex']);
    $sexData = $data[0];
    print_r($sexData);

    $data = explode(":", $rowData['education']);
    $educationData = $data[0];
    print_r($educationData);

    $data = explode(":", $rowData['telephone']);
    $telephoneData = $data[0];
    print_r($telephoneData);

    $data = explode(":", $rowData['email']);
    $emailData = $data[0];
    print_r($emailData);

    $data = explode(":", $rowData['homepage']);
    $homepageData = $data[0];
    print_r($homepageData);

    $data = explode(":", $rowData['introduction']);
    $introductionData = $data[0];
    print_r($introductionData);

    mysqli_close($idd); //关闭有参数
}
