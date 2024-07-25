#导入复核DB-API 2的数据库驱动器和DBUtils所使用的数据库工具包
import pymssql
from dbutils.persistent_db import PersistentDB

#配置数据库连接参数
POOL = PersistentDB(
    creator=pymssql,  # 使用链接数据库的模块
    maxusage=None,  # 一个链接最多被重复使用的次数，None表示无限制
    setsession=[],  # 开始会话前执行的命令列表
    ping=0,  # ping SQL Server服务端，检查是否服务可用
    closeable=False,  
    # 如果为False时， conn.close() 实际上被忽略，供下次使用，在线程关闭时，才会自动关闭链接。
    # 如果为True时， conn.close()则关闭链接，那么再次调用pool.connection时就会报错，因为已经真的关闭了连接
    # （pool.steady_connection()可以获取一个新的链接）
    threadlocal=None,  # 本线程独享值的对象，用于保存链接对象，如果链接对象被重置 
    host='127.0.0.1', #所连接数据库参数
    port=1433, 
    user='yz', 
    password='yz', 
    database='education_rename', 
    charset='utf8'
)

#使用DBUtils的PersistentDB优化已有程序
#一.遍历课程表
conn = POOL.connection() 
cursor = conn.cursor() 

sql = """
    SELECT *
    FROM C;
"""
cursor.execute(sql) 
result = cursor.fetchall() 

for r in result:  
    print("cno=%s,cn=%s,cc=%f,ch=%d,ct=%s" %
          (r[0], r[1], r[2], r[3], r[4]))

conn.close()
print('\n')

#二.查找所有男同学各门作业的平均成绩
conn = POOL.connection()
cursor = conn.cursor()

sql = """
    SELECT sn,(h1s+h2s+h3s)/3 AS hs_avg
    FROM S,SH
    WHERE S.sno=SH.sno AND sex='男'
"""
cursor.execute(sql)
result = cursor.fetchall()

for r in result: 
    print("sn=%s,hs_avg=%f" % (r[0], r[1]))  

conn.close()