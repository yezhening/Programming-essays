<!DOCTYPE html>
<html>

<head>
    <title>Relation</title>
    <!--bootstrap-->
    <link href="bootstrap2\bootstrap\css\bootstrap.min.css" rel="stylesheet" media="screen">
</head>

<body>
    <!--数据处理-->
    <?php
    error_reporting(0);
    $uid1=array();
    //echo count($u1);
    $uid2=array();
    $uid3=array();
    $uid4=array();
    
    $file=fopen("/home/n192705/homework/5/Relation.txt","r");
    while(!feof($file))
    {
        $line=fgets($file);
        //$line=fread($file,"1");//1B enough
        //echo $line;
        $content=explode(",",$line);
        //print_r($content);
        switch($content[1])
        //Notice: Undefined offset: 1 in /home/n192705/homework/5/relation.php on line 28
        {
            case 1:
                array_push($uid1,$content[0]);
                break;//易漏
            case 2:
                array_push($uid2,$content[0]);
                break;
            case 3:
                array_push($uid3,$content[0]);
                break;
            case 4:
                array_push($uid4,$content[0]);
                break;
        }
    }
    //print_r($uid1);
    //print_r($uid2);
    //print_r($uid3);
    //print_r($uid4);
    fclose($file);

    $u1=array();
    $u2=array();
    $u3=array();
    $u4=array();

    $file=fopen("/home/n192705/homework/5/User.txt","r");
    while(!feof($file))
    {
        $line=fgets($file);
        //echo $line;
        $content=explode(",",$line);
        //print_r($content);
        for($i=0;$i<count($uid1);$i++)
        {
            if($content[0]==$uid1[$i])
            {
                array_push($u1,$content[1]);
            }
        }       
        for($i=0;$i<count($uid2);$i++)
        {
            if($content[0]==$uid2[$i])
            {
                array_push($u2,$content[1]);
            }
        } 
        for($i=0;$i<count($uid3);$i++)
        {
            if($content[0]==$uid3[$i])
            {
                array_push($u3,$content[1]);
            }
        }
        for($i=0;$i<count($uid4);$i++)
        {
            if($content[0]==$uid4[$i])
            {
                array_push($u4,$content[1]);
            }
        }        
    }
    //print_r($u1);
    //print_r($u2);
    //print_r($u3);
    //print_r($u4);
    fclose($file);

?>



    <!--表格-->
    <table class="table table-striped table-bordered table-hover">
        <tr>
            <th>序号</th>
            <th>爱好</th>
            <th>用户</th>
        </tr>
        <tr>
            <td>1</td>
            <td>购物</td>
            <td>
                <?php 
                    //echo $a[0].$a[1]; 
                    for($i=0;$i<count($u1);$i++)
                    {
                        if($i+1!=count($u1))
                        {
                            echo $u1[$i].",";
                        }
                        else
                        {
                            echo $u1[$i];
                        }  
                    }
                ?>
            </td>
        </tr>
        <tr>
            <td>2</td>
            <td>上网</td>
            <td>
                <?php 
                    for($i=0;$i<count($u2);$i++)
                    {
                        if($i+1!=count($u2))
                        {
                            echo $u2[$i].",";
                        }
                        else
                        {
                            echo $u2[$i];
                        }  
                    }
                ?>
            </td>
        </tr>
        <tr>
            <td>3</td>
            <td>看电影</td>
            <td>
                <?php 
                    for($i=0;$i<count($u3);$i++)
                    {
                        if($i+1!=count($u3))
                        {
                            echo $u3[$i].",";
                        }
                        else
                        {
                            echo $u3[$i];
                        }  
                    }
                ?>
            </td>
        </tr>
        <tr>
            <td>4</td>
            <td>旅游</td>
            <td>
                <?php 
                    for($i=0;$i<count($u4);$i++)
                    {
                        if($i+1!=count($u4))
                        {
                            echo $u4[$i].",";
                        }
                        else
                        {
                            echo $u4[$i];
                        }  
                    }
                ?>
            </td>
        </tr>
    </table>

    <!--bootstrap-->
    <script src="bootstrap2\bootstrap\js\jquery.js"></script>
    <script src="bootstrap2\bootstrap\js\bootstrap.min.js"></script>
</body>

</html>