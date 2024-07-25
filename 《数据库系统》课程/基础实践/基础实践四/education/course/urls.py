from django.urls import path

from . import views

urlpatterns=[#是小写变量,有s
    path('showStudent/',views.showStudent,name='showStudent'),#路由需要目录正斜杠
    path('showCourse/',views.showCourse,name='showCourse'),
    path('showStudentHomework/',views.showStudentHomework,name='showStudentHomework'),
    path('<str:student_sno>/', views.showDetail,name='showDetail')#路径转换器是str，不是string
]
