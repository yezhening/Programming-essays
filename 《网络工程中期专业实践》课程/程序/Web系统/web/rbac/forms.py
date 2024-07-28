from cProfile import label
from django import forms

from rbac.models import Role,Permissions

# 登录
class LoginForm(forms.Form):
    account = forms.CharField(max_length=10, label="账号")
    password = forms.CharField(widget=forms.PasswordInput(), max_length=10, label="密码")
    role = forms.CharField(widget=forms.RadioSelect(choices=[(1, "管理员"), (2, "普通用户"),]),initial=2,)

# 创建用户
class ConsumerForm(forms.Form):
    consumer_name = forms.CharField(max_length=10, label='名称')
    account = forms.CharField(max_length=10, label='账号')
    password = forms.CharField(max_length=10,label='密码')
    role_id = forms.ChoiceField(
        # choices=([('管理员', '管理员'), ('普通用户', '普通用户')]), 
        # initial='普通用户', 
        label="角色")
    # attribute = forms.IntegerField(label="贡献度排名")

    def __init__(self, *args, **kwargs):  # 实时根据数据库更新，每次运行时，都会执行__int__(),得到choices
        super(ConsumerForm, self).__init__(*args, **kwargs)
        self.fields['role_id'].widget.choices = Role.objects.values_list('id', 'name')

# 创建角色
class RoleForm(forms.Form):
    role_name = forms.CharField(max_length=10, label='角色名称')
    permission_name = forms.MultipleChoiceField(  # 多选框
        widget=forms.CheckboxSelectMultiple,
        label="可赋权限",
        # initial=[2, ],
        # choices=(('1', 'a'), (2, 'b'), (3, "c")),
    )

    def __init__(self, *args, **kwargs):  # 实时根据数据库更新，每次运行时，都会执行__int__(),得到choices
        super(RoleForm, self).__init__(*args, **kwargs)
        self.fields['permission_name'].widget.choices = Permissions.objects.values_list('id', 'name')
    
# 创建权限
class PermissionsForm(forms.Form):
    permissions_name = forms.CharField(max_length=100, label='权限名称')
