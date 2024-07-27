<?php
header("Content-type: text/html; charset=utf-8");
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

//取数据
//对接口
$sql_statement  = "select * from interface"; //SQL语句

$result_set = mysqli_query($link, $sql_statement); //查询   得结果集

while ($row = mysqli_fetch_array($result_set)) //对每一行
{
    echo "<p>" . $row["name"]  . ": " . $row["data"] . "</p>\n";
}

//对主机
$sql_statement  = "select * from host"; //SQL语句

$result_set = mysqli_query($link, $sql_statement); //查询   得结果集

echo "<table>
        <tr>
            <th>process_id</th>
            <th>cpu_utility</th>
            <th>mem_utility (KBtyes)</th>
        </tr>";
while ($row = mysqli_fetch_array($result_set)) //对每一行
{
    echo "<tr>
    <td>" . $row["process_id"] . "</td>    
    <td>" . $row["cpu_utility"] . "</td>
    <td> " . $row["mem_utility"] . "</td>
    </tr>";
}
echo "</table>";
