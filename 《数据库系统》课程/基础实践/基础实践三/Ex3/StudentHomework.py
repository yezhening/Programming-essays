# 学生作业类
class StudentHomework:
    # 学生作业从表中课程号、学号是外码，课程主表中课程号、学生主表中学号是主码
    # 表反映的是对应关系，但并不是有课程号、学号就会有成绩
    # 应是有这门课程，有这个学生，学生选修课程才可能有成绩
    def __init__(self, course, student, h1s, h2s, h3s):  # 传对象
        self.__course = course
        self.__student = student
        self.__h1s = h1s
        self.__h2s = h2s
        self.__h3s = h3s

    @property
    def course(self):
        return self.__course

    @course.setter
    def course(self, course):
        self.__course = course

    @property
    def student(self):
        return self.__student

    @student.setter
    def student(self, student):
        self.__student = student

    @property
    def h1s(self):
        return self.__h1s

    @h1s.setter
    def h1s(self, h1s):
        self.__h1s = h1s

    @property
    def h2s(self):
        return self.__h2s

    @h2s.setter
    def h2s(self, h2s):
        self.__h2s = h2s

    @property
    def h3s(self):
        return self.__h3s

    @h3s.setter
    def h3s(self, h3s):
        self.__h3s = h3s