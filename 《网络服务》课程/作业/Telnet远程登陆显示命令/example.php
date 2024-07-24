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

    <form name="message" action="<?php echo $_SERVER['PHP_SELF'] ?>" method="POST">
        User:
        <input type="text" name="user" value="n192705" />
        <br />
        Password:
        <input type="text" name="password" value="******" />
        <br />
        Command:
        <input type="text" name="command" />
        <br />
        <input type="submit" name="submit" value="Submit" />
        <input type="reset" name="reset" value="Reset" />
    </form>

    <?php
    require 'telnet.php';
    $inst = new Telnet();
    $inst->login("n192705", "01234567");
    //$execv = $inst->exec("more/proc/loadavg");
    //$execv = $inst->exec("ls homework/9 | sed -r \"s/\\x1B\[([0-9]{1,3}(;[0-9]{1,2})?)?[mGK]//g\"");
    //$execv = htmlspecialchars($execv);
    //echo "$execv" . "\n";

    $str = $str . " | sed -r \"s/\\x1B\[([0-9]{1,3}(;[0-9]{1,2})?)?[mGK]//g\"";
    echo "$str" . "\n";
    $execv = $inst->exec($str);
    $execv = htmlspecialchars($execv);
    echo "$execv" . "\n";
    ?>

    <!--bootstrap-->
    <script src="bootstrap2\bootstrap\js\jquery.js"></script>
    <script src="bootstrap2\bootstrap\js\bootstrap.min.js"></script>
</body>

</html>