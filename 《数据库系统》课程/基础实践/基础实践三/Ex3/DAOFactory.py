# DAO工厂类
from StudentDAO import *
from CourseDAO import *
from StudentHomeworkDAO import *
from Student import *

# 自定义函数装饰器实现单实例    懒汉式


def singleton(cls):
    # 字典：不可变的类地址为键，实例为值
    _instance = {}

    def inner(connectPool):  # 带传入类的参数
        if cls not in _instance:
            _instance[cls] = cls(connectPool)  # 创建实例   带传入类的参数
        return _instance[cls]
    return inner

# 单实例类
# 数据库操作工厂类


@singleton
class DAOFactory:
    def __init__(self, connectPool):
        self.connectPool = connectPool  # 委托

    def getStudentDAO(self, databaseType):  # 判断数据库类型
        if databaseType == 'sqlserver':
            studentDAO = StudentDAO(self.connectPool)
            return studentDAO

    def getCoursetDAO(self, databaseType):
        if databaseType == 'sqlserver':
            courseDAO = CourseDAO(self.connectPool)
            return courseDAO

    def getStudentHomeworkDAO(self, databaseType):
        if databaseType == 'sqlserver':
            studentHomeworkDAO = StudentHomeworkDAO(self.connectPool)
            return studentHomeworkDAO

# # 测试
# # 上面的from Student import *

# # 数据库连接 取连接池    一个
# connectPool = DAOBase(host='127.0.0.1', port=1433, user='yz',
#                       password='yz', database='education_rename', charset='utf8')

# # DAO工厂   一个
# daoFactory = DAOFactory(connectPool)

# # 学生操作   一个
# studentDAO = daoFactory.getStudentDAO('sqlserver')

# # 操作
# student = Student('1003', '钱三', '男', '数据库01', '2000-1-1', '17788881013')
# student2 = Student('1003', '孙四', '女', '数据库02', '2021-1-1', '18888881013')
# sno = '1003'

# studentDAO.insert(student)  # 插入
# studentDAO.query(sno)  # 查询
# studentDAO.update(student2)  # 修改
# studentDAO.query(sno)  # 查询
# studentDAO.delete(sno)  # 删除
# studentDAO.query(sno)  # 查询
