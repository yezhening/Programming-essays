<?php
//捕获数据————————————————————
$hrSWRunPerf = snmp2_real_walk("localhost", "bj18", "HOST-RESOURCES-MIB::hrSWRunPerf"); //主机性能
//snmp2_getnext()获取多个SNMP对象

//处理数据————————————————————
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
// print_r($hrSWRunPerf); //数组不能连接换行符，否则只输出"\n"
// //print_r()以易于理解的格式打印变量
// print("\n");

// print("host_array:\n");
// print_r($host_array);
// print("\n");

//存取数据————————————————————
$memcache_obj = memcache_connect('localhost', 113113); //连接服务器

memcache_set($memcache_obj, 'host_array', $host_array, 0, 30); //存 可直接存数组

$host_array1 = memcache_get($memcache_obj, 'host_array'); //取

memcache_close($memcache_obj); //关闭连接

//显示————————————————————
echo "<table>
        <tr>
            <th>process_id</th>
            <th>cpu_utility</th>
            <th>mem_utility (KBtyes)</th>
        </tr>";
foreach ($host_array1 as $h_a1) {
    echo "<tr>
    <td>" . $h_a1[0] . "</td>    
    <td>" . $h_a1[1] . "</td>
    <td> " . $h_a1[2] . "</td>
    </tr>";
}
echo "</table>";
