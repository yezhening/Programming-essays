import pymssql 

connect = pymssql.connect('(local)', 'yz', 'yz', 'Study')
if connect:
    print("连接成功!")

cursor = connect.cursor()

# sql = "select name, sex from C_test"  
# 改表名C_test
sql = "select name, salesrep from persons"
cursor.execute(sql)

row = cursor.fetchone()
while row:
    print("Name=%s, Sex=%s" % (row[0], row[1])) 
    row = cursor.fetchone()

cursor.close()
connect.close()