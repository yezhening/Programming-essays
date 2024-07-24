<?php
    $uid1=array();
    //echo count($u1);
    $uid2=array();
    $uid3=array();
    $uid4=array();
    
    $file=fopen("C:\Users\DSHH\Desktop\Relation.txt","r");
    while(!feof($file))
    {
        $line=fgets($file);
        //$line=fread($file,"1");//1B enough
        //echo $line;
        $content=explode(",",$line);
        //print_r($content);
        switch($content[1])
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

    $file=fopen("C:\Users\DSHH\Desktop\User.txt","r");
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