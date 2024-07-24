<!DOCtype HTML>

<html>

<head>
    <meta http-equiv="Content-Type" content="text/html';charest=gb2312">
    <!--<meta>标签可提供有关页面的元信息,属性定义了与文档相关联的名称/值对-->
    <!--http-equiv属性为名称/值对提供了名称。并指示服务器在发送实际的文档之前先在要传送给浏览器的 MIME 文档头部包含名称/值对。
    当服务器向浏览器发送文档时，会先发送许多名称/值对。虽然有些服务器会发送许多这种名称/值对，但是所有服务器都至少要发送一个：content-type:text/html。这将告诉浏览器准备接受一个HTML 文档。
    使用带有 http-equiv 属性的 <meta> 标签时，服务器将把名称/值对添加到发送给浏览器的内容头部。-->
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
    $idd = mysqli_connect("localhost", "n192705", "01234567")
        or die("could not connect:" . mysqli_error($idd));
    mysqli_select_db($idd, "n192705");
    mysqli_query($idd, "set names 'gb2312'");

    $resultt = mysqli_query($idd, "select * from User"); //执行SQL语句，并返回结果

    mysqli_close($idd); //关闭有参数
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
            <?php while ($rowData = mysqli_fetch_array($resultt, MYSQLI_ASSOC)) { //从结果集中获取所有数据,每次读取一行 MYSQLI_ASSOC:得到关联数组
            ?>
                <tr>
                    <td>
                        <?php $data = explode(":", $rowData['uid']); //用单引号 关联数组中各个字段用分号间隔
                        $uidData = (int)$data[0]; //取到的是值
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