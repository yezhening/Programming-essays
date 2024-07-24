<!DOCTYPE html>
<html>

<head>
    <title>Client</title>
    <!--bootstrap-->
    <link href="bootstrap2\bootstrap\css\bootstrap.min.css" rel="stylesheet" media="screen">
</head>

<body>
    <?php
    @$str = $_POST['command'];
    ?>

    <?php
    require 'telnet.php';
    $inst = new Telnet();
    $inst->login("n192705", "Ye011013");
    //$execv = $inst->exec("more/proc/loadavg");
    //$execv = $inst->exec("ls homework/9 | sed -r \"s/\\x1B\[([0-9]{1,3}(;[0-9]{1,2})?)?[mGK]//g\"");
    //$execv = htmlspecialchars($execv);
    //echo "$execv" . "\n";

    $str = $str . " | sed -r \"s/\\x1B\[([0-9]{1,3}(;[0-9]{1,2})?)?[mGK]//g\"";
    $execv = $inst->exec($str);
    $execv = htmlspecialchars($execv);
    //echo "$execv" . "\n";
    ?>

    <!--页首-->
    <div class="navbar">
        <div class="navbar-inner">
            <h2 class="text-center">网络服务作业</h2>
        </div>
    </div>

    <!--内容-->
    <div class="well">
        <form name="message" action="<?php echo $_SERVER['PHP_SELF'] ?>" method="POST" class="form-horizontal">
            <div class="control-group">
                <label class="control-label">Username:</label>
                <div class="controls">
                    <input type="text" placeholder="Text input" name="user" value="n192705" />
                </div>
                <br />
                <label class="control-label">Password:</label>
                <div class="controls">
                    <input type="text" placeholder="Text input" name="password" value="******" />
                </div>
                <br />
                <label class="control-label">Command:</label>
                <div class="controls">
                    <input type="text" placeholder="Text input" name="command" />
                </div>
                <br />
                <div class="controls">
                    <input type="submit" name="submit" value="Submit" class="btn  btn-primary" />
                    <input type="reset" name="reset" value="Reset" class="btn" />
                </div>
                <br>
                <div class="controls">
                    <label class="control-label">显示文本框可拖动"拉大"以显示内容：</label>
                </div>
                <br>
                <div class="controls">
                    <textarea rows="3"><?php echo "$execv" . "\n" ?></textarea>
                </div>
        </form>
    </div>

    <!--bootstrap-->
    <script src="bootstrap2\bootstrap\js\jquery.js"></script>
    <script src="bootstrap2\bootstrap\js\bootstrap.min.js"></script>
</body>

</html>