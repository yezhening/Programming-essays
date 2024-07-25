<?php
header('Content-Type:text/xml');
/*以原始形式将HTTP标头发送到客户端或浏览器
在将HTML，XML，JSON或其他输出发送到浏览器或客户端之前，
原始数据与服务器发出的请求（尤其是HTTP请求）一起作为标头信息发送出去。
HTTP标头提供了关于消息体(更准确地说是关于请求和响应)中发送的对象的所需信息。*/

$random=rand(1,100);//rand()为左右闭区间

//输出XML格式数据
echo "<?xml version=\"1.0\" ?><random>$random</random>";