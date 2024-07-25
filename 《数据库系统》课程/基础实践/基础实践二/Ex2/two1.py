# 导入连接数据库所使用工具包
import pymssql

# 定义连接数据库基本信息
severName = '127.0.0.1'
userName = 'yz'
passWord = 'yz'
dataBase = 'education_rename'

# 生成连接数据库的connect对象、操作数据的cursor对象
connect = pymssql.connect(severName, userName, passWord, dataBase)
#定义自动提交
connect.autocommit(True)
cursor = connect.cursor()

# 执行SQL语句
cursor.execute("""
SELECT *
FROM S
""")

# 取结果集,取结果并向标准输出打印
row = cursor.fetchone()
while row:
    print("sno=%s,sn=%s,sex=%s,majcla=%s,bir=%s,tel=%s" %
          (row[0], row[1], row[2], row[3], row[4], row[5]))
    row = cursor.fetchone()

# 关闭对操作数据的cursor对象、connect对象的连接
cursor.close()
connect.close()
