# 学生操作类
# 文件是模块
# import：每个使用需带上“模块名.”前缀
# from import:不需
from DAO import *
#from Student import *

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
class StudentDAO:

    # 类变量作静态变量  SQL语句
    # 注意占位符的引号，SQL语句需要
    # 插入
    insertSql = """
        INSERT INTO S(sno,sn,sex,majcla,bir,tel)
        VALUES('%s','%s','%s','%s','%s','%s');
        """

    # 修改  根据学号
    updateSql = """
        UPDATE S
        SET sn='%s',sex='%s',majcla='%s',bir='%s',tel='%s'
        WHERE sno='%s';
        """

    # 删除  根据学号
    deleteSql = """
        DELETE
        FROM S
        WHERE sno='%s';
        """

    # 查询  根据学号，未进行复杂替换
    querySql = """
        SELECT *
        FROM S
        where sno='%s'
        """

    def __init__(self, pool):
        # 连接池    委托
        self.pool = pool

    # 类变量只能由类方法使用×
    # 类方法作静态方法× 有用到self，即要与实例交互
    # 插入
    def insert(self, student):
        # SQL语句替换
        # StudentDAO.insertSql×  实例方法，用self引入
        sql = self.insertSql % (
            student.sno, student.sn, student.sex, student.majcla, student.bir, student.tel)

        connect = self.pool.get_connection()
        cursor = connect.cursor()

        cursor.execute(sql)
        connect.commit()  # DBUtils连接池需要手动提交

        cursor.close()
        connect.close()

    # 修改
    def update(self, student):
        sql = self.updateSql % (
            student.sn, student.sex, student.majcla, student.bir, student.tel, student.sno)  # 注意格式化顺序

        connect = self.pool.get_connection()
        cursor = connect.cursor()

        cursor.execute(sql)
        connect.commit()

        cursor.close()
        connect.close()

    # 删除
    def delete(self, sno):
        sql = self.deleteSql % sno

        connect = self.pool.get_connection()
        cursor = connect.cursor()

        cursor.execute(sql)
        connect.commit()

        cursor.close()
        connect.close()

    # 查询  根据学号
    def query(self, sno):
        sql = self.querySql % sno

        connect = self.pool.get_connection()
        cursor = connect.cursor()

        cursor.execute(sql)
        result = cursor.fetchone()  # 学号主键唯一

        # 查询结果有无判断
        if result:
            print('sno=%s, sn=%s,sex=%s,majcla=%s,bir=%s,tel=%s' %
                  (result[0], result[1], result[2], result[3], result[4], result[5]))
        else:
            print('无结果')

        cursor.close()
        connect.close()


# 测试
# 上面的from Student import *
# 数据库连接 取连接池    一个
# connectPool = DAOBase(host='127.0.0.1', port=1433, user='yz',
#                       password='yz', database='education_rename', charset='utf8')

# # 学生操作   一个
# studentDAO = StudentDAO(connectPool)

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
