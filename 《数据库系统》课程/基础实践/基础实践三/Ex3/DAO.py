import abc  # 关于抽象类
import pymssql  # 关于数据库连接
from dbutils.persistent_db import PersistentDB  # 关于连接池

# 装饰器的核心内容就是将函数作为参数传递给另一个函数 理解
# 自定义函数装饰器实现单实例    懒汉式


def singleton(cls):
    # 字典：不可变的类地址为键，实例为值
    _instance = {}

    def inner(**kwargs):  # 带传入类的参数
        if cls not in _instance:
            _instance[cls] = cls(**kwargs)  # 创建实例  带传入类的参数
        return _instance[cls]
    return inner

# 数据库连接抽象类


class DAO(metaclass=abc.ABCMeta):
    @abc.abstractclassmethod
    def get_connection(self):
        pass

# 单实例类
# 数据库连接具体类  默认->可扩展


@singleton
class DAOBase(DAO):  # 继承
    def __init__(self, **kwargs):
        # 配置数据库连接参数
        # 连接池
        self.pool = PersistentDB(
            creator=pymssql,  # 使用链接数据库的模块
            maxusage=None,  # 一个链接最多被重复使用的次数，None表示无限制
            setsession=[],  # 开始会话前执行的命令列表
            ping=0,  # ping SQL Server服务端，检查是否服务可用
            closeable=False,
            # 如果为False时， conn.close() 实际上被忽略，供下次使用，在线程关闭时，才会自动关闭链接。
            # （pool.steady_connection()可以获取一个新的链接）
            threadlocal=None,  # 本线程独享值的对象，用于保存链接对象，如果链接对象被重置
            host=kwargs.get('host', '127.0.0.1'),  # 所连接数据库参数
            port=kwargs.get('port', 3389),
            user=kwargs.get('user', 'root'),
            password=kwargs.get('password', 123456),
            database=kwargs.get('database', 'test'),
            charset=kwargs.get('charset', 'utf8'))

    # 获取数据库连接 统一
    def get_connection(self):
        # 从连接池取数据库连接
        connect = self.pool.connection()
        return connect


# # 测试
# # 数据库连接 取连接池    一个
# connectPool = DAOBase(host='127.0.0.1', port=1433, user='yz',
#                       password='yz', database='education_rename', charset='utf8')
# if connectPool:
#     print('数据库连接成功')

# #验证单实例
# connectPool2 = DAOBase(host='127.0.0.1', port=1433, user='yz',
#                        password='yz', database='Study', charset='utf8')

# print(id(connectPool))
# print(id(connectPool2))
