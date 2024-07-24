<!DOCtype HTML>

<html>

<head>
    <meta http-equiv="Content-Type" content="text/html';charest=gb2312">
    <title>从数据库读取用户信息并显示</title>
    <!--bootstrap-->
    <link href="bootstrap2\bootstrap\css\bootstrap.min.css" rel="stylesheet" media="screen">
</head>

<body>
    <div class="navbar">
        <div class="navbar-inner">
            <h2 class="text-center">用户信息</h2>
        </div>
    </div>

    <?php
    $dsn = 'mysql:dbname=n192705;host=localhost';
    $user = 'n192705';
    $password = '01234567';

    try {
        $obj = new PDO($dsn, $user, $password);
        $obj->query("set names 'gb2312'"); //必须有
        $resultt = $obj->query("select * from User", PDO::FETCH_ASSOC); //结果集,print_r输出显示的是命令
    } catch (PDOException $e) {
        echo 'could not connect:' . $e->getMessage();
    }
    ?>

    <table class="table table-striped table-hover">
        <thead>
            <tr>
                <td>用户ID号</td>
                <td>昵称</td>
                <td>密码</td>
                <td>性别</td>
                <td>学历</td>
                <td>联系方式</td>
                <td>E-mail</td>
                <td>个人主页</td>
                <td>个人简介</td>
            </tr>
        </thead>
        <tbody>
            <?php while ($rowData = $resultt->fetch()) { //取一行
            ?>
                <tr>
                    <td>
                        <?php $data = explode(":", $rowData['uid']);
                        $uidData = (int)$data[0];
                        echo $uidData; ?>
                    </td>

                    <td>
                        <?php $data = explode(":", $rowData['name']);
                        $nameData = $data[0];
                        echo  $nameData; ?>
                    </td>

                    <td>
                        <?php $data = explode(":", $rowData['password']);
                        $passwordData = $data[0];
                        echo  $passwordData ?>
                    </td>

                    <td>
                        <?php $data = explode(":", $rowData['sex']);
                        $sexData = $data[0];
                        echo   $sexData ?>
                    </td>

                    <td>
                        <?php $data = explode(":", $rowData['education']);
                        $educationData = $data[0];
                        echo   $educationData ?>
                    </td>

                    <td>
                        <?php $data = explode(":", $rowData['telephone']);
                        $telephoneData = $data[0];
                        echo   $telephoneData ?>
                    </td>

                    <td>
                        <?php $data = explode(":", $rowData['email']);
                        $emailData = $data[0];
                        echo   $emailData ?>
                    </td>

                    <td>
                        <?php $data = explode(":", $rowData['homepage']);
                        $homepageData = $data[0];
                        echo   $homepageData ?>
                    </td>

                    <td>
                        <?php $data = explode(":", $rowData['introduction']);
                        $introductionData = $data[0];
                        echo  $introductionData ?>
                    </td>
                </tr>
            <?php } ?>

        </tbody>
    </table>

    <!--bootstrap-->
    <script src="bootstrap2\bootstrap\js\jquery.js"></script>
    <script src="bootstrap2\bootstrap\js\bootstrap.min.js"></script>
</body>

</html>