<?php
$dsn = 'mysql:dbname=n192705;host=localhost';
$user = 'n192705';
$password = '01234567';

try {
    $obj = new PDO($dsn, $user, $password);
    $obj->query("set names 'gb2312'"); //必须有

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
    }

    $result = $obj->query("insert into User(name,password,sex,education,telephone,email,homepage,introduction) values(\"$_POST[user]\",\"$_POST[pwd]\",\"$_POST[sex]\",\"$_POST[select]\",\"$_POST[tel]\",\"$_POST[email]\",\"$_POST[homepage]\",\"$_POST[instr]\")");

    echo "writing to table:User";
    sleep(60); //延迟执行当前脚本若干秒。   
} catch (PDOException $e) {
    echo 'could not connect:' . $e->getMessage();
}
