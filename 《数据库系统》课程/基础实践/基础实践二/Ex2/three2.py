# 构建朴素的数据库连接池程序
# 导包
import queue
import pymssql

# 连接池类


class ConnectionPool(object):
    # 初始化
    def __init__(self, **kwargs):  # kwargs:不定长度的键值对/字典
        # 若未指定size键的值,则默认使用10。size指定存放连接数据库的connect对象的队列的长度
        self.size = kwargs.get('size', 10)
        self.kwargs = kwargs  # 字典
        self.conn_queue = queue.Queue(maxsize=self.size)  # 创建存放connect对象的空队列
        for i in range(self.size):
            self.conn_queue.put(self._create_new_conn())  # 创建connect对象并将其加入队列

    # 创建connect对象
    def _create_new_conn(self):
        return pymssql.connect(
            host=self.kwargs.get('host', '127.0.0.1'),
            user=self.kwargs.get('user'),
            password=self.kwargs.get('password'),  # 1.是password而不是passwd
            port=self.kwargs.get('port', 3306),)  # 有个逗号,无碍
        # connect_timeout=5  2.无connect_timeout属性

    # 将connect对象加入队列
    def _put_conn(self, conn):
        self.conn_queue.put(conn)

    # 从队列connect对象取
    def _get_conn(self):
        conn = self.conn_queue.get()
        if conn is None:  # 如果没有就创建
            # self._create_new_conn()    4.错误
            self.conn_queue.put(self._create_new_conn())  # 若没有,先创建,放入队列,再取
            conn = self.conn_queue.get()
        return conn

    # 使用connect对象创建cursor对象,执行SQL语句
    def exec_sql(self, sql):
        conn = self._get_conn()
        try:
            # with conn as cur:  # with as 语句操作上下文管理器，能够自动分配并且释放资源 3.不能用with as
            cur = conn.cursor()
            cur.execute(sql)
            return cur.fetchall()
        except pymssql.ProgrammingError as e:  # 程序异常
            raise e
        except pymssql.OperationalError as e:  # 运算操作异常  重新创建
            conn = self._create_new_conn()
            raise e
        finally:
            self._put_conn(conn)  # 使用完放回连接池

    # 删除连接池
    def __del__(self):
        try:
            while True:
                conn = self.conn_queue.get_nowait()  # 非阻塞取
                if conn:
                    conn.close()  # 关闭连接
        except queue.Empty:
            pass  # 不捕获队列为空异常


# 创建连接池对象
connectionPool = ConnectionPool(
    host='127.0.0.1', user='yz', password='yz', port=1433)  # 键不需要引号

# 注意指定使用数据库
sql = """
    USE education_rename
    SELECT sn,(h1s+h2s+h3s)/3 AS hs_avg
    FROM S,SH
    WHERE S.sno=SH.sno AND sex='男'
"""
# 连接数据库,执行SQL语句
row = connectionPool.exec_sql(sql)  # 列表,每条记录是一个元组
# print(row)

# 打印输出
for r in row:  # 对每个元组
    print("sn=%s,hs_avg=%f" % (r[0], r[1]))  # 输出的是元组

# 关闭连接
del connectionPool
