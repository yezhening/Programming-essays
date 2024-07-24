<html>
    <head><title>Addition</title></head>

    <body>
        <?php
            @$add1=$_GET['num1'];
            @$add2=$_GET['num2'];
        ?>

        <form name="addition" action="<?php echo $_SERVER['PHP_SELF']?>" method="GET">
            num1:
            <input type="text" name="num1"/>
            <br/>
            num2:
            <input type="text" name="num2"/>
            <br/>
            <input type="submit" name="submit" value="add"/>
            <input type="reset" name="reset" value="reset"/>
        </form>

        <?php
            if(!is_null($add1&&$add2))
            {
                $sum=$add1+$add2;
                printf("%d + %d = %d",$add1,$add2,$sum);
            }
        ?>
    </body>
</html>