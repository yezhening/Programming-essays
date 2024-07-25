import pymssql

serverName = '127.0.0.1'
userName = 'yz'
passWord = 'yz'

conn = pymssql.connect(serverName, userName, passWord, "Study")
cursor = conn.cursor()

cursor.execute(""" 
IF OBJECT_ID('persons', 'U') IS NOT NULL 
DROP TABLE persons 
CREATE TABLE persons ( 
    id INT NOT NULL, 
    name VARCHAR(100), 
    salesrep VARCHAR(100), 
    PRIMARY KEY(id)) 
""")

cursor.executemany(
    "INSERT INTO persons VALUES (%d, %s, %s)",
    [(1, 'John Smith', 'John Doe'),
     (2, 'Jane Doe', 'Joe Dog'),
     (3, 'Mike T.', 'Sarah H.')])

conn.commit()

cursor.execute('SELECT * FROM persons WHERE salesrep=%s', 'John Doe')

row = cursor.fetchone()
while row:
    print("ID=%d, Name=%s" % (row[0], row[1]))
    row = cursor.fetchone()

# 获取结果示例
cursor.execute('SELECT * FROM persons')
row = cursor.fetchone()
while row:
    print("ID=%d, Name=%s" % (row[0], row[1]))
    row = cursor.fetchone()

cursor.execute('SELECT * FROM persons WHERE salesrep=%s', 'John Doe')
for row in cursor:
    # %r：输出对象本身
    # 逗号为了让解释器知道是一个元组，当元组只有一个元素时，要加个逗号
    print('row = %r' % (row,))

# 除了在建立连接时指定，还可以在这里指定as_dict=True
cursor = conn.cursor(as_dict=True)
cursor.execute('SELECT * FROM persons')
for row in cursor:
    print("ID=%d, Name=%s" % (row['id'], row['name']))

cursor.close()