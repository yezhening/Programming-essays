# 学生类
class Student:
    def __init__(self, sno, sn, sex, majcla, bir, tel):
        self.__sno = sno  # 两个下划线为“真正”私有属性
        self.__sn = sn
        self.__sex = sex
        self.__majcla = majcla
        self.__bir = bir
        self.__tel = tel

    @property  # 装饰器    @property可以直接通过方法名访问方法，不需要在方法名后添加一对“（）”小括号
    def sno(self):
        return self.__sno

    @sno.setter  # @ *.setter 允许对已用@property装饰的函数赋值
    def sno(self, sno):
        self.__sno = sno

    @property
    def sn(self):
        return self.__sn

    @sn.setter
    def sn(self, sn):
        self.__sn = sn

    @property
    def sex(self):
        return self.__sex

    @sex.setter
    def sex(self, sex):
        self.__sex = sex

    @property
    def majcla(self):
        return self.__majcla

    @majcla.setter
    def majcla(self, majcla):
        self.__majcla = majcla

    @property
    def bir(self):
        return self.__bir

    @bir.setter
    def bir(self, bir):
        self.__bir = bir

    @property
    def tel(self):
        return self.__tel

    @tel.setter
    def tel(self, tel):
        self.__tel = tel
