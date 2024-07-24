<?php
error_reporting(0);
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
    for ($i = 0; $i < count($_POST['like']); $i++) //i包含判断
    {
        echo $_POST['like'][$i] . "&nbsp;&nbsp;";
    }
    echo "<br/>";
    if ($_FILES['photo']['name'] == true) {
        $path = "" . $_FILES['photo']['name'];
        move_uploaded_file($_FILES['photo']['tmp_name'], "img/" . $path);
        echo $path . " successfuly uploaded" . "<br/>";
    }
    echo $_POST['instr'];


    //读写文件
    $file = fopen("User.txt", "r+");
    $uid = 0; //注意
    while (!feof($file)) {
        fgets($file); //逐行读取
        $uid++;
    }

    if ($_POST['user']) {
        $content = "$uid,$_POST[user],";
    } else {
        $content = "$uid,null,";
    }
    fwrite($file, $content);
    if ($_POST['pwd']) {
        $content = "$_POST[pwd],";
    } else {
        $content = "null,";
    }
    fwrite($file, $content);
    if ($_POST['sex']) {
        $content = "$_POST[sex],";
    } else {
        $content = "null,";
    }
    fwrite($file, $content);
    if ($_POST['select']) {
        $content = "$_POST[select],";
    } else {
        $content = "null,";
    }
    fwrite($file, $content);
    if ($_POST['tel']) {
        $content = "$_POST[tel],";
    } else {
        $content = "null,";
    }
    fwrite($file, $content);
    if ($_POST['email']) {
        $content = "$_POST[email],";
    } else {
        $content = "null,";
    }
    fwrite($file, $content);
    if ($_POST['homepage']) {
        $content = "$_POST[homepage],";
    } else {
        $content = "null,";
    }
    fwrite($file, $content);
    if ($_POST['instr']) {
        $content = "$_POST[instr]\n";
    } else {

        $content = "null\n";
    }
    fwrite($file, $content);
    fclose($file);

    $file = fopen("Hobby.txt", "a+");
    for ($j = 0; $j < count($_POST['like']); $j++) {
        $hid = $j + 1;
        fwrite($file, "$hid,");
        fwrite($file, $_POST['like'][$j]);
        fwrite($file, "  ");
    }
    fwrite($file, "\n");
    fclose($file);

    $file = fopen("Relation.txt", "a+");
    for ($k = 0; $k < count($_POST['like']); $k++) {
        $hid = $k + 1;
        $content = "$uid,$hid\n";
        fwrite($file, $content);
    }
    fclose("$file");
}
