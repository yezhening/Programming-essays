<?php
//捕获数据————————————————————
$hrStorageTable = snmp2_real_walk("localhost", "bj18", "HOST-RESOURCES-MIB::hrStorageTable"); //主机存储

//数据处理结构————————————————————
//使用链表而不是数据库
//单链表结点类
class Node  //PHP无结构体，用类表示
{
    public $data;
    public $next;   //PHP弱类型，变量相当于指向结点的指针

    public function __construct($data = null, $next = null)  //function __construct间有空格 有默认值
    {
        $this->data = $data;    //是data不是date！！！
        $this->next = $next;
    }
}

//单链表类
class LinkList
{
    public $header;   //链表头结点，头节点必须存在
    private $size = 0;   //链表大小

    public function __construct()
    {
        $this->header = new Node(null);    //可只提供一个参数   因为有默认值
    }

    public function insert(Node $node)  //插入结点
    {
        $current = $this->header;   //PHP弱类型，直接创建初始化一结点变量（的指针）
        while ($current->next != null) {   //不是if，是while   尾插法，将指针移到表尾
            $current = $current->next;
        }

        $current->next = $node;

        return ++$this->size;
    }

    public function show()   // 遍历链表
    {
        echo 'LinkList size: ' . $this->size;   //echo不能拼接换行符'\n'   能拼接'<br/>'
        //命令行'<br/>'无作用   web页面"PHP_EOL"无作用
        echo '<br/>';   //换行符
        echo PHP_EOL;    //换行符

        $current = $this->header;
        for ($i = 0; $i < $this->size; $i++) {

            echo $current->next->data;
            echo '<br/>';
            echo PHP_EOL;    //换行符

            $current = $current->next;
        }
    }

    public function clear() //清空链表
    {
        $this->header = null;
    }
}

//处理数据————————————————————
$link_list = new LinkList();    //创建链表

foreach ($hrStorageTable as $key => $value) //循环获取每一对象-值对
// [HOST-RESOURCES-MIB::hrStorageIndex.1] => INTEGER: 1
{
    $content = "[" . $key . "] => " . $value; //拼接内容

    $node = new Node($content); //创建结点

    $link_list->insert($node);  //插入链表  用->而不是.调用
}

$link_list->show();  //遍历链表显示
$link_list->clear();    //清空链表
