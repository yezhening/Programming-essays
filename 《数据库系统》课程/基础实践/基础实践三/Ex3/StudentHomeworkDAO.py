# 学生作业操作类
from DAO import *
# from Student import *
# from Course import *
# from StudentHomework import *

# 自定义函数装饰器实现单实例    懒汉式


def singleton(cls):
    # 字典：不可变的类地址为键，实例为值
    _instance = {}

    def inner(pool):  # 带传入类的参数
        if cls not in _instance:
            _instance[cls] = cls(pool)  # 创建实例  带传入类的参数
        return _instance[cls]
    return inner


@singleton
class StudentHomeworkDAO:

    # 类变量作静态变量  SQL语句
    # 注意占位符的引号，SQL语句需要
    # 插入
    insertSql = """
        INSERT INTO SH(cno,sno,h1s,h2s,h3s)
        VALUES('%s','%s','%s','%s','%s');
        """

    # 修改  根据课程号和学号
    updateSql = """
        UPDATE SH
        SET h1s='%s',h2s='%s',h3s='%s'
        WHERE cno='%s' AND sno='%s';
        """

    # 删除  根据课程号和学号
    deleteSql = """
        DELETE
        FROM SH
        WHERE cno='%s' AND sno='%s';
        """

    # 查询  根据课程号和学号，未进行复杂替换
    querySql = """
        SELECT *
        FROM SH
        where cno='%s' AND sno='%s'
        """

    def __init__(self, pool):
        # 连接池    委托
        self.pool = pool

    # 类变量只能由类方法使用×
    # 类方法作静态方法× 有用到self，即要与实例交互
    # 插入
    def insert(self, course, student, studentHomework):
        # SQL语句替换
        # StudentDAO.insertSql×  实例方法，用self引入
        sql = self.insertSql % (
            course.cno, student.sno, studentHomework.h1s, studentHomework.h2s, studentHomework.h3s)

        connect = self.pool.get_connection()
        cursor = connect.cursor()

        cursor.execute(sql)
        connect.commit()  # DBUtils连接池需要手动提交

        cursor.close()
        connect.close()

    # 修改
    def update(self, cno, sno, studentHomework):
        sql = self.updateSql % (studentHomework.h1s, studentHomework.h2s,
                                studentHomework.h3s, cno, sno)  # 注意格式化顺序

        connect = self.pool.get_connection()
        cursor = connect.cursor()

        cursor.execute(sql)
        connect.commit()

        cursor.close()
        connect.close()

    # 删除
    def delete(self, cno, sno):
        sql = self.deleteSql % (cno, sno)

        connect = self.pool.get_connection()
        cursor = connect.cursor()

        cursor.execute(sql)
        connect.commit()

        cursor.close()
        connect.close()

    # 查询  根据学号
    def query(self, cno, sno):
        sql = self.querySql % (cno, sno)  # 需要括号括住格式化参数

        connect = self.pool.get_connection()
        cursor = connect.cursor()

        cursor.execute(sql)
        result = cursor.fetchone()  # 课程号、学号外键唯一

        # 查询结果有无判断
        if result:
            print('cno=%s,sno=%s,h1s=%s,h2s=%s,h3s=%s' %
                  (result[0], result[1], result[2], result[3], result[4]))
        else:
            print('无结果')

        cursor.close()
        connect.close()


# # 测试
# # 上面的
# # from Student import *
# # from Course import *
# # from StudentHomework import *

# # 数据库连接 取连接池    一个
# connectPool = DAOBase(host='127.0.0.1', port=1433, user='yz',
#                       password='yz', database='education_rename', charset='utf8')

# # 学生作业操作   一个
# studentHomeworkDAO = StudentHomeworkDAO(connectPool)

# # 操作
# course = Course('S001', '数据库', '1.0', '50', '周无')  # 这里随意
# student = Student('0592', '钱三', '男', '数据01', '2000-1-1', '17788881013')

# studentHomework = StudentHomework(course, student, '90.0', '90.0', '90.0')
# studentHomework2 = StudentHomework(course, student, '70.0', '70.0', '70.0')
# cno = 'S001'
# sno = '0592'

# # 约束错误   会出现error message 20018
# studentHomeworkDAO.insert(course, student, studentHomework)  # 插入
# studentHomeworkDAO.query(cno,sno)  #查询
# studentHomeworkDAO.update(cno, sno ,studentHomework2)  # 修改
# studentHomeworkDAO.query(cno,sno)  #查询
# studentHomeworkDAO.delete(cno, sno)  # 删除
# studentHomeworkDAO.query(cno,sno)  #查询
