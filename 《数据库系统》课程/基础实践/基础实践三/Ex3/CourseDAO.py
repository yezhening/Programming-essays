# 课程操作类
from DAO import *
# from Course import *

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
class CourseDAO:

    # 类变量作静态变量  SQL语句
    # 注意占位符的引号，SQL语句需要
    # 插入
    # cc、ch是数字，统一用%s屏蔽差异√   在修改出问题：因为在设计数据库时学分cc字段有效位数为两位
    insertSql = """
        INSERT INTO C(cno,cn,cc,ch,ct)
        VALUES('%s','%s','%s','%s','%s');
        """

    # 修改  根据学号
    updateSql = """
        UPDATE C
        SET cn='%s',cc='%s',ch='%s',ct='%s'
        WHERE cno='%s';
        """

    # 删除  根据学号
    deleteSql = """
        DELETE
        FROM C
        WHERE cno='%s';
        """

    # 查询  根据学号，未进行复杂替换
    querySql = """
        SELECT *
        FROM C
        where cno='%s'
        """

    def __init__(self, pool):
        # 连接池    委托
        self.pool = pool

    # 类变量只能由类方法使用×
    # 类方法作静态方法× 有用到self，即要与实例交互
    # 插入
    def insert(self, course):
        # SQL语句替换
        # StudentDAO.insertSql×  实例方法，用self引入
        sql = self.insertSql % (
            course.cno, course.cn, course.cc, course.ch, course.ct)

        connect = self.pool.get_connection()
        cursor = connect.cursor()

        cursor.execute(sql)
        connect.commit()  # DBUtils连接池需要手动提交

        cursor.close()
        connect.close()

    # 修改
    def update(self, course):
        sql = self.updateSql % (
            course.cn, course.cc, course.ch, course.ct, course.cno)  # 注意格式化顺序

        connect = self.pool.get_connection()
        cursor = connect.cursor()

        cursor.execute(sql)
        connect.commit()

        cursor.close()
        connect.close()

    # 删除
    def delete(self, cno):
        sql = self.deleteSql % cno

        connect = self.pool.get_connection()
        cursor = connect.cursor()

        cursor.execute(sql)
        connect.commit()

        cursor.close()
        connect.close()

    # 查询  根据学号
    def query(self, cno):
        sql = self.querySql % cno

        connect = self.pool.get_connection()
        cursor = connect.cursor()

        cursor.execute(sql)
        result = cursor.fetchone()  # 学号主键唯一

        # 查询结果有无判断
        # 小数用%s显示可精确到两位
        if result:
            print('cno=%s, cn=%s,cc=%s,ch=%s,ct=%s' %
                  (result[0], result[1], result[2], result[3], result[4]))
        else:
            print('无结果')

        cursor.close()
        connect.close()


# # 测试
# # 上面的from Course import *
# # 数据库连接 取连接池    一个
# connectPool = DAOBase(host='127.0.0.1', port=1433, user='yz',
#                       password='yz', database='education_rename', charset='utf8')

# #  课程操作   一个
# courseDAO = CourseDAO(connectPool)

# #  操作
# course = Course('S003', '数据库', '5.0', '60', '李五')
# course2 = Course('S003', '数据库2', '1.0', '64', '周五')
# cno = 'S003'

# courseDAO.insert(course)  # 插入
# courseDAO.query(cno)  # 查询
# courseDAO.update(course2)  # 修改
# courseDAO.query(cno)  # 查询
# courseDAO.delete(cno)  # 删除
# courseDAO.query(cno)  # 查询
