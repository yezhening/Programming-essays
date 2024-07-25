#课程类
class Course:
    def __init__(self, cno, cn, cc, ch, ct):
        self.__cno = cno
        self.__cn = cn
        self.__cc = cc
        self.__ch = ch
        self.__ct = ct

    @property
    def cno(self):
        return self.__cno

    @cno.setter
    def cno(self, cno):
        self.__cno = cno

    @property
    def cn(self):
        return self.__cn

    @cn.setter
    def cn(self, cn):
        self.__cn = cn

    @property
    def cc(self):
        return self.__cc

    @cc.setter
    def cc(self, cc):
        self.__cc = cc

    @property
    def ch(self):
        return self.__ch

    @ch.setter
    def ch(self, ch):
        self.__ch = ch

    @property
    def ct(self):
        return self.__ct

    @ct.setter
    def ct(self, ct):
        self.__ct = ct