from django.shortcuts import render
from django.http import JsonResponse
from . import models
from django.contrib.auth.hashers import make_password, check_password
 
# 主页
def Index(request):
    return render(request, 'user/index.html')


# 注册
def Register(request):
    userName = request.POST['userName']
    userNum = request.POST['userNum']
    userPhone = request.POST['userPhone']
    userPwd1 = request.POST['userPwd1']
    userPwd2 = request.POST['userPwd2']

    # 判断两次密码是否一致
    if userPwd1 != userPwd2:
        return JsonResponse({"code": 1})
    # 判断输入是否合法
    try:
        int(userPhone)
        int(userNum)
    except ValueError:
        return JsonResponse({"code": 4})

    # 通过学号判断用户是否已经注册
    if models.Student.objects.filter(userNum=userNum):
        return JsonResponse({"code": 2})
    else:
        # 注册信息
        models.Student.objects.create(userName=userName, userNum=userNum, userPhone=userPhone, userPwd=make_password(userPwd1))
        return JsonResponse({"code": 3})


# 登录验证
def Checklogin(request):
    userNum = request.POST['userNum']
    userPwd = request.POST['userPwd']
    request.session["userNum"] = userNum
    request.session["isTeacher"] = 0
    request.session.set_expiry(3600) #设置5分钟后过期

    # 查询是否已经注册
    if models.Student.objects.filter(userNum=userNum):
        # 查询密码是否正确
        if check_password(userPwd, models.Student.objects.filter(userNum=userNum).get().userPwd):
            return JsonResponse({"code": 0})
        else:
            # 密码错误
            return JsonResponse({"code": 1})
    else:
        # 用户不存在
        return JsonResponse({"code": 2})


# 登录验证
def CheckloginTeacher(request):
    userNum = request.POST['userNum']
    userPwd = request.POST['userPwd']
    request.session["userNum"] = userNum
    request.session["isTeacher"] = 1
    request.session.set_expiry(3600) #设置5分钟后过期

        # 查询是否已经注册
    if models.Teacher.objects.filter(userNum=userNum):
        # 查询密码是否正确
        if check_password(userPwd, models.Teacher.objects.filter(userNum=userNum).get().userPwd):
            return JsonResponse({"code": 0})
        else:
            # 密码错误
            return JsonResponse({"code": 1})
    else:
        # 用户不存在
        return JsonResponse({"code": 2})



