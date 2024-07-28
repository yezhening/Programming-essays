import imp
from django.shortcuts import render

# Create your views here.
import functools
from rbac.models import Role, RolePermissons
from .models import Request, Response, Whois, DomainManagement
import whois # python-whois库
from .forms import HostTypeForm
from django.http import HttpResponseRedirect
from django.db.models import Max, Min, Avg
import requests

# 自定义的登录、功能验证装饰器
def check_login(func):
    # 不用权限——功能字典
    # 装饰器装饰一个函数会改变函数的名字 为'wrapper'
    # 恢复原函数名
    @functools.wraps(func)

    def wrapper(request, *args, **kwargs):
        role_id = request.session.get('role_id') # 取角色id
        try:
            role_obj=Role.objects.get(id=role_id) # 取角色对象
        except:
            return HttpResponseRedirect('/rbac/login/')  # 不成功重定向到登录界面
        role_perm_objs = RolePermissons.objects.filter(role=role_obj) # 取角色——权限对象集合
        func_name = func.__name__ # 被装饰函数名/功能名

        for role_perm_obj in role_perm_objs:
            permisson_name=role_perm_obj.permissons.name  # 取权限对象，权限名 
            if permisson_name == func_name: # 比对数据库中权限名和被装饰函数名是否执行响应功能函数
                return func(request, *args, **kwargs) # 成功可执行
    return wrapper

@check_login
#list出所有请求报文的特征，并在Host字段处链接显示另一页面，list出该host域名信息
def request_list(request):
    req_objs = Request.objects.all() # 所有请求报文
    content = {'req_objs': req_objs}
    return render(request, 'packets/request_list.html', content)

@check_login
# 显示域名信息
def show_host(request, packet_number):  # 参数名和url.py同，可和.html不同
    req_obj = Request.objects.get(number=packet_number) # 请求对象
    req_host = req_obj.host # 取host内容

    # 数据库存储信息
    if req_host.startswith('www.'):
        req_host = req_host[4:]  # 忽略"www."
    try:
        who_obj = Whois.objects.get(domain_name=req_host.lower()) 
    except:  # 捕捉异常 忽略大小写
        who_obj = Whois.objects.get(domain_name=req_host.upper())

      # 网络查询信息
    # 获取whois
    try:
       # info返回了所有的whois查询信息，可根据需要选择想要提取的查询方法
       who_info = whois.whois(req_host)
    except:
       pass

    content = {
        'who_obj': who_obj,
        'who_info':who_info
        }
    return render(request, 'packets/show_host.html', content)

@check_login
#list出所有响应报文的特征，并在Content-type链接显示另一页面，显示载荷内容
def response_list(request):
    res_objs = Response.objects.all()
    content = {'res_objs': res_objs}
    return render(request, 'packets/response_list.html', content)

@check_login
# 显示载荷内容
def show_payload(request, packet_number):
    res_obj = Response.objects.get(number=packet_number) # 响应对象
    file_path = res_obj.file_path  # 取文件路径

    #读文件
    fd = open(file_path)
    st = fd.read() # 字符串内容
    content = {'st': st}
    return render(request, 'packets/show_payload.html', content)

@check_login
# 统计各源地址发送的HTTPrequest数量
def request_sourceip_number(request): # 源地址依据IP
    dic = {}  # 一一对应的键值字典

    source_ip_objs = Request.objects.values_list('source_ip', flat=True).distinct()  # 不重复的源IP地址对象列表
    # flat 参数。如果为True，表示返回的结果为单个值而不是元组
    for temp in source_ip_objs:
        count = Request.objects.filter(source_ip=temp).count()  # 数量
        dic[temp]=count 

    content = {'dic': dic}
    return render(request, 'packets/request_sourceip_number.html', content)

@check_login
# 统计请求报文访问类型（饼图）
def request_type(request):
    # 可扩展
    # 各访问类型计数
    get_count = 0
    post_count = 0
    head_count = 0

    file_path_list = Request.objects.values_list('file_path', flat=True)  # 取文件路径对象列表
    for file_path in file_path_list:
        #读文件 计数
        fd = open(file_path)
        st = fd.read()
        if st.startswith('GET') == True:
            get_count = get_count+1
        if st.startswith('POST') == True:
            post_count = post_count+1
        if st.startswith('HEAD') == True:
            head_count = head_count+1

    # 字典存储
    dic = {}
    dic['GET'] = get_count
    dic['POST'] = post_count
    dic['HEAD'] = head_count

    #转换成Echart参数格式
    li = []  # 列表数据
    tu = zip(dic.keys(), dic.values())  # 转换成元组    
    # zip() 函数用于将可迭代的对象作为参数，将对象中对应的元素打包成一个个元组，然后返回由这些元组组成的列表
    s = ("name", "value")  # 定义键
    for t in tu:
        # 使用zip函数跟键结合转换成字典并放入列表  
        # 例：[{'name': '中层', 'values': 2421}, {'name': '低层', 'values': 1660}]
        li.append(dict(zip(s, t)))

    # 图例格式
    li2=[] # 列表数据
    for l in li: # 取字典
        li2.append(l['name']) # 取键值

    # 去其中字典键的引号
    li = str(li).replace("'name'", "name").replace("'values'", "values")

    content = {
        'li': li,
        'li2':li2
        }
    return render(request, 'packets/request_type.html', content)

@check_login
# 统计各目的地址接收到的HTTP报文的数量
def destinationip_number(request):
    dic = {}  # 一一对应的键值字典

    response_destination_ip_objs = Response.objects.values_list(
        'destination_ip', flat=True).distinct()  # 响应不重复的目的IP地址
    for temp in response_destination_ip_objs:
        count = Response.objects.filter(destination_ip=temp).count()  # 数量
        dic[temp] = count

    content = {'dic': dic}
    return render(request, 'packets/destinationip_number.html', content)

@check_login
# 统计响应报文载荷的类型（饼图）
def response_content_type(request):
    dic = {}  # 一一对应的键值字典
    content_type_objs = Response.objects.values_list(
        'content_type', flat=True).distinct()  # 不重复的载荷类型对象列表
    for temp in content_type_objs:
        count = Response.objects.filter(content_type=temp).count()  # 数量
        dic[temp] = count

    #转换成Echart参数格式
    li = []  # 列表数据
    tu = zip(dic.keys(), dic.values())  # 转换成元组
    # zip() 函数用于将可迭代的对象作为参数，将对象中对应的元素打包成一个个元组，然后返回由这些元组组成的列表
    s = ("name", "value")  # 定义键
    for t in tu:
        # 使用zip函数跟键结合转换成字典并放入列表
        # 例：[{'name': '中层', 'values': 2421}, {'name': '低层', 'values': 1660}]
        li.append(dict(zip(s, t)))

    # 图例格式
    li2 = []  # 列表数据
    for l in li:  # 取字典
        li2.append(l['name'])  # 取键值

    # 去其中字典键的引号
    li = str(li).replace("'name'", "name").replace("'values'", "values")

    content = {
        'li': li,
        'li2': li2
    }
    return render(request, 'packets/response_content_type.html', content)


@check_login
#根据域名及域名注册公司，指定域名类型
def specify_host_type(request):
    #显示所有域名信息
    host_list=[] # 存host内容列表
    req_objs = Request.objects.all()  # 请求对象列表
    for req_obj in req_objs:
        req_host = req_obj.host  # 取host内容
        if req_host.startswith('www.'):
            req_host = req_host[4:]  # 忽略"www."
        host_list.append(req_host) # 存列表
    set_list = set(host_list) # 去重

    # 数据库存储信息
    who_objs=[] # 存储whois对象列表
    for set_host in set_list:
        try:
            who_obj = Whois.objects.get(domain_name=set_host.lower()) # whois对象
        except:  # 捕捉异常 忽略大小写
            who_obj = Whois.objects.get(domain_name=set_host.upper())
        who_objs.append(who_obj)  # 存列表

    content = {'who_objs': who_objs}
    return render(request, 'packets/specify_host_type.html', content)

@check_login
# 修改
def alter_host_type(request, who_obj_domain_name):
    if request.method == 'POST':
        form = HostTypeForm(request.POST)
        if form.is_valid():
            do_name = who_obj_domain_name # 域名
            try:
                who_obj = Whois.objects.get(domain_name=do_name.lower())  # 忽略大小写 whois对象
            except:
                who_obj = Whois.objects.get(domain_name=do_name.upper())  # 忽略大小写
            r_o = who_obj.registrant_organization # 注册人组织
            host_type = request.POST.get('host_type') #域名类型

            # 模型按取出的表单数据修改
            do_man_obj = DomainManagement(
                domain_name=do_name,  # 判断要修改的数据
                registrant_organization=r_o,
                domain_type=host_type)

            # 保存数据库
            do_man_obj.save()
            # 重定向到显示界面
            return HttpResponseRedirect('/packets/specify_host_type/')
    else:
        host_type_form = HostTypeForm()

        content = {'host_type_form': host_type_form}
        return render(request, 'packets/alter_host_type.html', content)

@check_login
#根据请求报文，list出每个源地址的host兴趣爱（公司类型）
def show_host_type(request):
    host_li = []  # 域名列表
    content_li=[] # 内容列表
    content_li2=[] # 三内容列表

    sou_li = Request.objects.values_list('source_ip', flat=True).distinct() # 不重复的源IP地址列表
    for sou_ip in sou_li:
        ho_name_objs = Request.objects.filter(source_ip=sou_ip) # 域名请求对象列表 一个源IP可能对应多个域名
        for ho_name_obj in ho_name_objs: 
            ho_name = ho_name_obj.host  # 取相对应的域名
            host_li.append(ho_name) # 存列表
            set_host_li=set(host_li) # 去重

        # 取域名类型
        for ho_name in set_host_li:
            if str(ho_name).startswith('www.'):
                ho_name = ho_name[4:]  # 忽略www.
            try:
                ho_type = DomainManagement.objects.get(domain_name=ho_name.lower()).domain_type    # 域名管理对象
            except:
                ho_type = '无信息'  # 可能存在未录入
            try:
                ho_type = DomainManagement.objects.get(domain_name=ho_name.upper()).domain_type
            except:
                ho_type = '无信息'  # 可能存在未录入
            
            # 存储为[[源IP地址，域名，域名类型],]格式
            content_li2.append(sou_ip)
            content_li2.append(ho_name)
            content_li2.append(ho_type)

            content_li.append(content_li2)
            content_li2 = []  # 置空

    context = {'content_li': content_li}
    return render(request, 'packets/show_host_type.html', context)

@check_login
# 统计请求报文的最大帧大小、最小帧大小、平均帧大小
def request_frame_size(request):
    max_frame_size = Request.objects.aggregate(Max('frame_size'))  # 最大帧大小
    min_frame_size = Request.objects.aggregate(Min('frame_size'))  # 最小帧大小
    avg_frame_size = Request.objects.aggregate(Avg('frame_size'))  # 平均帧大小

    content = {
        'max_frame_size': max_frame_size,
        'min_frame_size':  min_frame_size,
        'avg_frame_size': avg_frame_size}
    return render(request, 'packets/request_frame_size.html', content)

@check_login
# 统计响应报文的最大帧大小、最小帧大小、平均帧大小
def response_frame_size(request):
    max_frame_size = Response.objects.aggregate(Max('frame_size'))  # 最大帧大小
    min_frame_size = Response.objects.aggregate(Min('frame_size'))  # 最小帧大小
    avg_frame_size = Response.objects.aggregate(Avg('frame_size'))  # 平均帧大小

    context = {
        'max_frame_size': max_frame_size,
        'min_frame_size':  min_frame_size,
        'avg_frame_size': avg_frame_size}
    return render(request, 'packets/response_frame_size.html', context)

@check_login
# 采用关键字检索，若响应报文为.html或.txt，尝试分析该目的主机检索的网页内容性质
def analysis_webpage(request):
    content_list=[] # 传递内容列表
    content_list2=[] # 响应报文编号，载荷

    res_objs=Response.objects.all() # 响应报文对象列表
    for res_obj in res_objs:  # 响应报文对象
        cont_typ=res_obj.content_type # content-type内容

        # 若响应报文为.html或.txt
        if cont_typ=='text/html':
            fi_pa = res_obj.file_path # 文件路径
            # 读文件
            fd=open(fi_pa)
            payload=fd.read() # 取载荷

            # 取响应体
            count=payload.find('....') # 计取响应体需切片数
            count= count + 4
            res_body = payload[count:]

            # 取网址
            res_body1 = res_body  # 可切变量
            http_content1 = ""  # 存储网址
            http_content2 = [] 

            while(1): # 对一个响应包的载荷
                http_count = res_body1.find('http')  # 计取http需切片数
                if http_count == -1:  # 没有则退出
                    break
                http_content = res_body1[http_count:]  # 先切头
                res_body1 = http_content  # 记录剩下内容，进行循环切片

                http_count2 = http_content.find('"')  # 再找结束符   可根据需要扩展
                if http_count2 == -1:
                    http_count2 = http_content.find('<')
                http_content = http_content[:http_count2]  # 切尾
                res_body1 = res_body1[http_count2:]  # 记录剩下内容

                if http_content1 == "":
                    http_content1 = http_content
                else:
                    http_content1 = http_content1 + "\n" + http_content  # 增加换行以显示

                http_content2.append(http_content) # 用于分析域名的遍历
                # print(http_content2)

            # print(http_content2)
            # 分析域名
            nature_content = ""  # 存储性质
            for http in http_content2:
                if http.find('org'):
                    if nature_content.find('非盈利组织') != -1: # 已经存在
                        pass
                    elif nature_content == "":
                        nature_content = "非盈利组织\n"
                    else:
                        nature_content = nature_content + "非盈利组织\n"
                if http.find('com'):
                    if nature_content.find('公司企业') != -1:  # 已经存在
                        pass
                    elif nature_content == "":
                        nature_content = "公司企业\n"
                    else:
                        nature_content = nature_content + "公司企业\n"

            # 分析网页源代码
            code = ""
            for http in http_content2:
                if code == "":
                    code = http + "\n"# 网站名称  记录网页源代码
                else:
                    code = code + http + "\n"
                try:
                    html = requests.get(http, timeout=1) # 访问超时时间 有的网站访问不了
                    code = code + html.text + "\n" + "----------\n"
                except:
                    pass

                # 判断是否访问到 即变量是否被赋值
                try:
                    html
                except NameError:
                    html_exists = False
                else:
                    html_exists = True
                if html_exists == False:
                    html = "None"
                    code = code + html + "\n" + "----------\n"

                del html # 清空变量

            print(code)  # 后台监控网络请求是否跑完

            content_list2.append(res_obj.number)  # 存储1
            content_list2.append(res_body)  # 2
            content_list2.append(http_content1)  # 3
            content_list2.append(code)  # 4
            content_list2.append(nature_content)  # 5

            content_list.append(content_list2)
            content_list2=[] # 置空

    content = {'content_list': content_list}
    return render(request, 'packets/analysis_webpage.html', content)




