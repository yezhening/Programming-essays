//头文件————————————————————
#include <libmemcached/memcached.h>
//在Ubuntu 20.04安装：sudo apt install libmemcached-dev
//安装无法获得锁解决：sudo rm /var/lib/dpkg/lock
//在Ubuntu 20.04安装：sudo apt install memcached    默认已启动，端口11211
#include <stdio.h>
// #include <string.h>  //不需要

//主函数————————————————————
int main(int argc, char **argv)
{
    //初始化
    // memcached_servers_parse (char *server_strings);  parse：解析
    memcached_server_st *servers = NULL; //缓存服务器    是一个分布式“集群”，即有多个，所以用“*”
    memcached_return rc;                 //返回的操作状态    如连接connect()、设置set()、获取get()
    memcached_st *memc;                  //连接句柄    对1.套接字，2.数据

    servers = memcached_server_list_append(servers, "localhost", 113113, &rc); //增加缓存服务器结点
    memc = memcached_create(NULL);                                             //建立连接句柄    对1.套接字，2.数据
    rc = memcached_server_push(memc, servers);                                 //将缓存服务器放入句柄

    if (rc == MEMCACHED_SUCCESS)
        fprintf(stderr, "Added server successfully\n");
    else
        fprintf(stderr, "Couldn't add server: %s\n", memcached_strerror(memc, rc));
    //将缓存服务器放入句柄的失败信息

    //设置数据
    // char *key = "keystring";
    // char *value = "keyvalue";
    char *key = "time";  //设置数据的键
    char *value = "2022.4.20"; //设置数据的值

    char *retrieved_value; //获取数据的值
    size_t value_length;   //数据的值的长度
    uint32_t flags;        // 设置set操作的标志，在获取get操作中表明  是否使用zlib压缩，0无1有   一般不用

    rc = memcached_set(memc, key, strlen(key), value, strlen(value), (time_t)0, (uint32_t)0);
    //设置set将数据写入句柄->套接字->缓存中
    //参数：句柄，数据的键，数据的键的长度，数据的值，数据的值的长度，数据过期时间：0永不过期，压缩标志：0无1有

    if (rc == MEMCACHED_SUCCESS)
        fprintf(stderr, "Key stored successfully\n");
    else
        fprintf(stderr, "Couldn't store key: %s\n", memcached_strerror(memc, rc));

    //获取数据
    retrieved_value = memcached_get(memc, key, strlen(key), &value_length, &flags, &rc); //按数据的键取数据的值
    printf("Yay!\n");

    if (rc == MEMCACHED_SUCCESS)
    {
        fprintf(stderr, "Key retrieved successfully\n"); // retrieved：检索
        printf("The key '%s' returned value '%s'.\n", key, retrieved_value);

        free(retrieved_value); //需释放
    }
    else
        fprintf(stderr, "Couldn't retrieve key: %s\n", memcached_strerror(memc, rc));

    return 0;
}