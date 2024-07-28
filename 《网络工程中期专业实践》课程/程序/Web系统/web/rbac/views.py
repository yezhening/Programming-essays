from django.shortcuts import render

# Create your views here.
from django.http import HttpResponseRedirect
from .forms import LoginForm, ConsumerForm, RoleForm, PermissionsForm
from .models import Consumer, ConsumerRole, Role, RolePermissons, Permissions
from packets.views import check_login

# 登录
def login(request):
    if request.method == 'POST':
        form = LoginForm(request.POST)
    
        if form.is_valid(): # 与表单定义合法
            # 取表单数据
            get_account = form.cleaned_data.get('account')
            get_password = form.cleaned_data.get('password')
            get_role = form.cleaned_data.get('role')
            
            # 取数据库对象
            consumer_object = Consumer.objects.get(account=get_account) # 账户
            consumerRole_object = ConsumerRole.objects.filter(consumer=consumer_object) # 可能有多对关系

            # 验证
            if consumer_object.password == get_password:
                for o in consumerRole_object: # 密码
                    if o.role.id == int(get_role):  # 角色
                        # 存储会话以传递
                        request.session['consumer_id'] = consumer_object.id
                        request.session['role_id'] = o.role.id
                        return HttpResponseRedirect('/rbac/homepage/')  # 绝对路径重定向
            else: # 验证失败
                form = LoginForm()
        else:  # 与表单定义不合法
            form = LoginForm()
    else:
        form = LoginForm()
    content = {'form': form}
    return render(request, 'rbac/login.html', content)

# 主页
def homepage(request):
    consumer_id = request.session.get('consumer_id') # 取用户id
    consumer_name = Consumer.objects.get(id=consumer_id).name # 取用户名
    role_id = request.session.get('role_id')  # 取角色id
    role_name = Role.objects.get(id=role_id).name  # 取角色名
    content = {
        'consumer_name': consumer_name,
        'role_name': role_name
        }
    return render(request, 'rbac/homepage.html', content)

# 账号管理
@check_login
def account_management(request):
    # 不用权限——功能字典
    content_list =[] # 存储内容 用户名，账号，密码，角色名，权限
    content_list2 = [] # 二元大列表

    consumer_objs=Consumer.objects.all() # 用户对象列表

    for consumer_obj in consumer_objs:
        content_list.append(consumer_obj.name) # 1
        content_list.append(consumer_obj.account) # 2
        content_list.append(consumer_obj.password) # 3

        con_ro_obj = ConsumerRole.objects.get(consumer=consumer_obj) # 用户-角色对象
        role_obj = con_ro_obj.role  # 角色对象 默认一个用户对应一个角色
        content_list.append(role_obj.name)  # 4


        per_list = []  # 权限列表
        ro_pe_objs= RolePermissons.objects.filter(role=role_obj) # 角色-权限对象
        for ro_pe_obj in ro_pe_objs:
            pe_obj = ro_pe_obj.permissons  # 权限对象 默认一个角色可有多种权限
            per_name = pe_obj.name  # 取权限名
            per_list.append(per_name)
        
        # 处理权限显示格式
        per_total = ""
        for per in per_list:
            if per_total == "":
                per_total = per + "\n"
            else:
                per_total = per_total + per + "\n"

        content_list.append(per_total)  # 5

        content_list2.append(content_list) # 添加大列表
        content_list=[] # 置空
    
    content = {'content_list2': content_list2}
    return render(request, 'rbac/account_management.html', content)

@check_login
# 创建用户
def create_consumer(request):
    if request.method == 'POST':
        # 取表单数据
        get_consumer_name = request.POST.get('consumer_name')
        get_account = request.POST.get('account')
        get_password = request.POST.get('password')
        get_role_id = request.POST.get('role_id')
       
        if get_consumer_name and get_account and get_password and get_role_id:  # 与表单定义合法
            # 模型存取出的表单数据
            consumer_object = Consumer(
                id=Consumer.objects.count()+1,  # 注意
                name=get_consumer_name,
                account=get_account,
                password=get_password)

            role_object = Role.objects.get(id=get_role_id)
            
            consumer_role_object = ConsumerRole(
                id=ConsumerRole.objects.count()+1,
                consumer=consumer_object,
                role=role_object)

            # 数据存数据库
            consumer_object.save()
            role_object.save()
            consumer_role_object.save()

            return HttpResponseRedirect('/rbac/account_management/')
    else:
        form = ConsumerForm()
    content = {'form': form}
    return render(request, 'rbac/create_consumer.html', content)

# 创建角色
@check_login
def create_role(request):
    if request.method == 'POST':
        # 取表单数据
        get_role_name = request.POST.get('role_name')
        checkbox_list = request.POST.getlist('permission_name') # 多选框取值 为form的字段名、html的"name"属性

        # 合法  表单设置的可输入最大长度在渲染时会限制
        if get_role_name and checkbox_list:
            # 模型存取出的表单数据  数据存数据库
            role_object = Role(
                id=Role.objects.count()+1,  # 注意
                name=get_role_name)

            role_object.save()

            for permissions_id in checkbox_list:
                permissions_obj = Permissions.objects.get(id=permissions_id) # 以每权限ID取权限对象

                role_permissions_object=RolePermissons( # 每权限对象与角色对象绑定
                    id=RolePermissons.objects.count()+1,  # 注意
                    role=role_object,
                    permissons=permissions_obj)

                role_permissions_object.save()

        return HttpResponseRedirect('/rbac/account_management/')
    else:
        form = RoleForm()
    content = {'form': form}
    return render(request, 'rbac/create_role.html', content)

# 创建权限
@check_login
def create_permissions(request):
    if request.method == 'POST':
        form = PermissionsForm(request.POST)
        
        if form.is_valid():
            # 取表单数据
            get_permissions_name = form.cleaned_data['permissions_name']
        
            # 模型存取出的表单数据  数据存数据库
            permissions_object = Permissions(
                id=Permissions.objects.count()+1,  # 注意
                name=get_permissions_name)

            # 数据存数据库
            permissions_object.save()

        return HttpResponseRedirect('/rbac/account_management/')
    else:
        form = PermissionsForm()
    content = {'form': form}
    return render(request, 'rbac/create_permissions.html', content)

# 登出
def logout(request):
    request.session.flush()  
    # 删除当前的会话数据并删除会话的Cookie（数据库和cookie都删），用于确保前面的会话数据不可以再次被用户的浏览器访问
    return HttpResponseRedirect('/rbac/login/')  # 绝对路径重定向
