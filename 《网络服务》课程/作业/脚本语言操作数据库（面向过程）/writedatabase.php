<?php
header("Content-Type: text/html; charset=utf-8");
$id = mysqli_connect("localhost", "n192705", "01234567")
    or die("could not connect:" . mysqli_error($id)); //$id:文件描述符
mysqli_select_db($id, "n192705");
mysqli_query($id, "set names 'gb2312'"); //设置客户端字符集：是建表的字符集× 想了好久

//提交数据
if ($_POST['Submit'] == true) {

    if ($_POST['user']) {
        echo $_POST['user'] . "<br/>";
        //echo打印取到的值  "<br/>"换行
    }
    if ($_POST['pwd']) {
        echo $_POST['pwd'] . "<br/>";
    }
    if ($_POST['sex']) {
        echo $_POST['sex'] . "<br/>";
    }
    if ($_POST['select']) {
        echo $_POST['select'] . "<br/>";
    }
    if ($_POST['tel']) {
        echo $_POST['tel'] . "<br/>";
    }
    if ($_POST['email']) {
        echo $_POST['email'] . "<br/>";
    }
    if ($_POST['homepage']) {
        echo $_POST['homepage'] . "<br/>";
    }

    if ($_FILES['photo']['name'] == true) {
        $path = "" . $_FILES['photo']['name'];
        move_uploaded_file($_FILES['photo']['tmp_name'], "img/" . $path);
        echo $path . " successfuly uploaded" . "<br/>";
    }
    if ($_POST['instr']) {
        echo $_POST['instr'] . "<br/>";
    }

    $result = mysqli_query($id, "insert into User(name,password,sex,education,telephone,email,homepage,introduction) values(\"$_POST[user]\",\"$_POST[pwd]\",\"$_POST[sex]\",\"$_POST[select]\",\"$_POST[tel]\",\"$_POST[email]\",\"$_POST[homepage]\",\"$_POST[instr]\")"); //字段无单引号了 插入字符串要加单引号！
    if (!$result) {
        die("write error" . mysqli_error($id)); //给参数
        break;
    }
    echo "writing to table:User";
    sleep(60); //延迟执行当前脚本若干秒。
}

mysqli_close($id);
