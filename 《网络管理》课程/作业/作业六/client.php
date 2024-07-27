<?php
//捕获数据————————————————————
$before_ifInOctets = snmp2_get('localhost', 'bj18', 'IF-MIB::ifInOctets.2'); //前入站流量
// snmp2_getnext()获取单个SNMP对象的值   SNMP Agent IP地址，团体名/密码，对象标识符 .2是网卡eth0的index索引表示 
//可能可用127.0.0.1、211.71.149.251  
$before_ifOutOctets = snmp2_get('localhost', 'bj18', 'IF-MIB::ifOutOctets.2'); //前出站流量

$before_ifInUcastPkts = snmp2_get('localhost', 'bj18', 'IF-MIB::ifInUcastPkts.2'); //前入站单播数据帧数量
$before_ifOutUcastPkts = snmp2_get('localhost', 'bj18', 'IF-MIB::ifOutUcastPkts.2'); //前出站单播数据帧数量

sleep(120); //睡眠120秒   以获取2分钟内的数据

$after_ifInOctets = snmp2_get('localhost', 'bj18', 'IF-MIB::ifInOctets.2'); //后入站流量
$after_ifOutOctets = snmp2_get('localhost', 'bj18', 'IF-MIB::ifOutOctets.2'); //后出站流量

$after_ifInUcastPkts = snmp2_get('localhost', 'bj18', 'IF-MIB::ifInUcastPkts.2'); //后入站单播数据帧数量
$after_ifOutUcastPkts = snmp2_get('localhost', 'bj18', 'IF-MIB::ifOutUcastPkts.2'); //后出站单播数据帧数量

$hrSWRunPerf = snmp2_real_walk("localhost", "bj18", "HOST-RESOURCES-MIB::hrSWRunPerf"); //主机性能
//snmp2_getnext()获取多个SNMP对象

//处理数据————————————————————
//对接口
$temp = explode(" ", $before_ifInOctets); //Counter32: 662439788
$temp1 = explode(" ", $after_ifInOctets);
$two_mins_ifInOctets = $temp1[1] - $temp[1]; //两分钟内入站流量  
// echo gettype($two_mins_ifInOctets);
//PHP字符串数字加减会自动转换为数字再运算   运算后为integer类型

$temp = explode(" ", $before_ifOutOctets);
$temp1 = explode(" ", $after_ifOutOctets);
$two_mins_ifOutOctets = $temp1[1] - $temp[1]; //两分钟内出站流量 

$temp = explode(" ", $before_ifInUcastPkts);
$temp1 = explode(" ", $after_ifInUcastPkts);
$two_mins_ifInUcastPkts = $temp1[1] - $temp[1]; //两分钟内入站单播数据帧数量

$temp = explode(" ", $before_ifOutUcastPkts);
$temp1 = explode(" ", $after_ifOutUcastPkts);
$two_mins_ifOutUcastPkts = $temp1[1] - $temp[1]; //两分钟内出站单播数据帧数量

//对主机
// $host_array = []; //二维数组   存放[进程号，CPU占用率，内存占用率]一维数组  
//版本不兼容    "PHP -v"显示版本为5.3.6<5.4

$host_array = array();

foreach ($hrSWRunPerf as $key => $value) //循环获取每一对象-值对
//[HOST-RESOURCES-MIB::hrSWRunPerfCPU.31015] => INTEGER: 152
//[HOST-RESOURCES-MIB::hrSWRunPerfMem.1] => INTEGER: 3375104 KBytes
{
    $temp = explode(".", $key);
    $process_id = $temp[1]; //进程号    1

    $judge = substr($key, 31, 3); //CPU或Mem

    $temp1 = explode(" ", $value);

    if ($judge == "CPU") //CPU占用率键值在前，先创建一维数组
    {
        $cpu_utility = $temp1[1]; //CPU占用率   2

        $pro_cpu = array($process_id, $cpu_utility); //创建一维数组  只有两元素
        array_push($host_array, $pro_cpu); //将一维数组添加至二维数组
    }
    if ($judge == "Mem")  //内存占用率键值在后，向一维数组补充数据
    {
        $mem_utility  = $temp1[1];

        foreach ($host_array as &$pro_cpu) //遍历二维数组找进程号同的一维数组  使用'&'可改变原一维数组
        {
            if ($pro_cpu[0] == $process_id) {

                $pro_cpu[2] = $mem_utility; //内存占用率    3
            }
        }
    }
}

//显示————————————————————
print("before_ifInOctets: " . $before_ifInOctets . "\n");
//print()输出字符串
print("before_ifOutOctet: " . $before_ifOutOctets . "\n\n");

print("before_ifInUcastPkts: " . $before_ifInUcastPkts . "\n");
print("before_ifOutUcastPkt: " . $before_ifOutUcastPkts . "\n\n");

print("after_ifInOctets: " . $after_ifInOctets . "\n");
print("after_ifOutOctets: " . $after_ifOutOctets . "\n\n");

print("after_ifInUcastPkts: " . $after_ifInUcastPkts . "\n");
print("after_ifOutUcastPkts: " . $after_ifOutUcastPkts . "\n\n");

print("two_mins_ifInOctets: " . $two_mins_ifInOctets . "\n");
print("two_mins_ifOutOctets: " . $two_mins_ifOutOctets . "\n\n");

print("two_mins_ifInUcastPkts: " . $two_mins_ifInUcastPkts . "\n");
print("two_mins_ifOutUcastPkts: " . $two_mins_ifOutUcastPkts . "\n\n");

print_r($hrSWRunPerf); //数组不能连接换行符，否则只输出"\n"
//print_r()以易于理解的格式打印变量
print("\n");

print("host_array:\n");
print_r($host_array);
print("\n");

//连接MySQL数据库————————————————————
$mysql_server_addr = 'localhost'; //服务端地址
$mysql_username = 'n192705'; //用户名
$mysql_password = '123456'; //密码
$mysql_database = 'n192705'; //数据库名

$link = mysqli_connect($mysql_server_addr, $mysql_username, $mysql_password, $mysql_database); //连接数据库
//PHP5.0开始不推荐使用mysql_connect()，PHP7.0废弃，PHP7.0及以上版本用mysqli_connect()代替
//提示信息
if (!$link) {
    echo "Error: Unable to connect to MySQL." . PHP_EOL;
    echo "Debugging errno: " . mysqli_connect_errno() . PHP_EOL;
    echo "Debugging error: " . mysqli_connect_error() . PHP_EOL;
    exit;
}

//存数据————————————————————
//对接口
$sql_statement = "INSERT INTO interface (name, data) 
VALUES ('two_mins_ifInOctets', $two_mins_ifInOctets);"; //SQL语句

$result = mysqli_query($link, $sql_statement); //查询
if ($result == false) {
    echo "Query failure\n";
}


$sql_statement = "INSERT INTO interface (name, data) 
VALUES ('two_mins_ifOutOctets', $two_mins_ifOutOctets);";

$result = mysqli_query($link, $sql_statement);
if ($result == false) {
    echo "Query failure\n";
}

$sql_statement = "INSERT INTO interface (name, data) 
VALUES ('two_mins_ifInUcastPkts', $two_mins_ifInUcastPkts);";

$result = mysqli_query($link, $sql_statement);
if ($result == false) {
    echo "Query failure\n";
}

$sql_statement = "INSERT INTO interface (name, data) 
VALUES ('two_mins_ifOutUcastPkts', $two_mins_ifOutUcastPkts);";

$result = mysqli_query($link, $sql_statement);
if ($result == false) {
    echo "Query failure\n";
}

//对主机
foreach ($host_array as $host) {
    $sql_statement = "INSERT INTO host (process_id,cpu_utility,mem_utility) 
    VALUES ($host[0],$host[1],$host[2]);";

    $result = mysqli_query($link, $sql_statement);
    if ($result == false) {
        echo "Query failure\n";
    }
}

mysqli_close($link);//关闭连接