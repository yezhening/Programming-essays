from django.db import models


class Student(models.Model):
    sno = models.CharField(max_length=4)  # 默认not null
    # 其他参数的使用   文档建议：使用blank而不是null
    sn = models.CharField(max_length=10, blank=True)
    sex = models.CharField(max_length=1, blank=True)
    majcla = models.CharField(max_length=10, blank=True)
    bir = models.DateField(blank=True)  # 类带括号：实例化
    tel = models.CharField(max_length=11, blank=True)

    # __str__()也是直接返回对象，展示对象的直观信息
    def __str__(self):
        return self.sn


class Course(models.Model):
    cno = models.CharField(max_length=4)
    cn = models.CharField(max_length=10, blank=True)
    cc = models.DecimalField(max_digits=2, decimal_places=1, blank=True)
    ch = models.IntegerField(blank=True)
    ct = models.CharField(max_length=10, blank=True)

    def __str__(self):
        return self.cn


class StudentHomework(models.Model):
    # 外键约束：是多对一的关系，课程和学生共同决定作业关系
    cno = models.ForeignKey(Course, on_delete=models.CASCADE)
    sno = models.ForeignKey(Student, on_delete=models.CASCADE)
    h1s = models.DecimalField(max_digits=4, decimal_places=2, blank=True)
    h2s = models.DecimalField(max_digits=4, decimal_places=2, blank=True)
    h3s = models.DecimalField(max_digits=4, decimal_places=2, blank=True)
