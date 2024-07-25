from DAO import *
from DAOFactory import *
from Student import *

# 数据库连接 取连接池    一个
connectPool = DAOBase(host='127.0.0.1', port=1433, user='yz',
                      password='yz', database='education_rename', charset='utf8')

# 学生操作   一个
daoFactory = DAOFactory(connectPool)
studentDAO = daoFactory.getStudentDAO('sqlserver')

# 操作
stu = (1, 2, 3, 4, 5, 6)
result = studentDAO.insert(stu)
print(result)
