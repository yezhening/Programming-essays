# 导入复核DB-API 2的数据库驱动器和DBUtils所使用的数据库工具包
import pymssql
from dbutils.persistent_db import PersistentDB

# 连接池类


class ConnectionPool(object):
    # 初始化
    def __init__(self, **kwargs):
      # 配置数据库连接参数
        self.pool = PersistentDB(
            creator=pymssql,  # 使用链接数据库的模块
            maxusage=None,  # 一个链接最多被重复使用的次数，None表示无限制
            setsession=[],  # 开始会话前执行的命令列表
            ping=0,  # ping SQL Server服务端，检查是否服务可用
            closeable=False,
            # 如果为False时， conn.close() 实际上被忽略，供下次使用，在线程关闭时，才会自动关闭链接。
            # 如果为True时， conn.close()则关闭链接，那么再次调用pool.connection时就会报错，因为已经真的关闭了连接
            # （pool.steady_connection()可以获取一个新的链接）
            threadlocal=None,  # 本线程独享值的对象，用于保存链接对象，如果链接对象被重置
            host=kwargs.get('host', '127.0.0.1'),  # 所连接数据库参数
            port=kwargs.get('port', 3389),
            user=kwargs.get('user', 'root'),
            password=kwargs.get('password', 123456),
            database=kwargs.get('database', 'test'),
            charset=kwargs.get('charset', 'utf8'))

    # 执行sql函数
    def exec_sql(self, sql):
        # 使用DBUtils
        conn = self.pool.connection()
        cursor = conn.cursor()

        cursor.execute(sql)
        result = cursor.fetchall()
        return result

        # conn.close()#不关闭,即使调用也不关闭,不用

    # 删除连接池
    def __del__(self):
        print('连接池已被删除')


# 使用DBUtils优化已有程序
# 创建连接池对象
connectionPool = ConnectionPool(host='127.0.0.1', port=1433, user='yz',
                                password='yz', database='education_rename', charset='utf8')

# 每业务使用连接池中的连接
# 一.遍历课程表
sql = """
    SELECT *
    FROM C;
"""
result = connectionPool.exec_sql(sql)
for r in result:
    print("cno=%s,cn=%s,cc=%f,ch=%d,ct=%s" %
          (r[0], r[1], r[2], r[3], r[4]))
print('\n')

# 二.查找所有男同学各门作业的平均成绩
sql = """
    SELECT sn,(h1s+h2s+h3s)/3 AS hs_avg
    FROM S,SH
    WHERE S.sno=SH.sno AND sex='男'
"""
result = connectionPool.exec_sql(sql)
for r in result:
    print("sn=%s,hs_avg=%f" % (r[0], r[1]))
print('\n')

# 删除链接池 手动回收(可自动回收)
del connectionPool