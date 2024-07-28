from django import forms

# 登录
class HostTypeForm(forms.Form):
    host_type_choices = (
        ('游戏公司', '游戏公司'),#前面存入，后面展示
        ('科技公司（不含游戏）', '科技公司（不含游戏）'),
        ('政府事业单位（不含高校）', '政府事业单位（不含高校）'),
        ('高校', '高校'),
        ('医疗机构', '医疗机构'),
    ) 

    # 设置form表单的必填项
    host_type = forms.ChoiceField(label='指定域名类型', widget=forms.Select(), choices=host_type_choices, initial=host_type_choices[0])