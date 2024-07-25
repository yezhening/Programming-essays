from django.shortcuts import render

from .models import Student, Course, StudentHomework
from django.http import Http404


def showStudent(request):
    results = Student.objects.all()
    context = {'results': results}
    return render(request, 'course/showStudent.html', context)


def showCourse(request):
    results = Course.objects.all()
    context = {'results': results}
    return render(request, 'course/showCourse.html', context)


def showStudentHomework(request):
    results = StudentHomework.objects.all()
    context = {'results': results}
    return render(request, 'course/showStudentHomework.html', context)

# 路由调用视图
# 在showStudent视图超链接传学生学号，转到路由，路由调用视图


def showDetail(request, student_sno):
    try:
        # get()：字段匹配查询  使用字段名而不是pk  匹配字符串
        student = Student.objects.get(sno=student_sno)
        # 外键约束实际存的是对象，匹配字段
        # 创建StudentHomework对象时需要Course、Student对象作参数
        studentHomework = StudentHomework.objects.get(sno=student)
        course = Course.objects.get(cno=studentHomework.cno.cno)  # 匹配字段
        context = {#传多个键值对
            'student': student,
            'studentHomework':studentHomework,
            'course':course
            }
    except Student.DoesNotExist:
        raise Http404("Student's homeworks do not exist.")
    return render(request, 'course/showDetail.html', context)
