from django.urls import path

from . import views

app_name = 'rbac'
urlpatterns = [
    path('login/', views.login, name='login'),
    path('homepage/', views.homepage, name='homepage'),
    path('logout/', views.logout, name='logout'),

    path('account_management/', views.account_management, name='account_management'),
    path('create_consumer/', views.create_consumer, name='create_consumer'),
    path('create_role/', views.create_role, name='create_role'),
    path('create_permissions/', views.create_permissions, name='create_permissions'),
]
