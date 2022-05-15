from django.shortcuts import get_object_or_404, render
from user import models
from django.http import JsonResponse, HttpResponse
from django.contrib.auth.hashers import make_password, check_password
# Create your views here.


def Setting(request, userNum):
    if request.session["userNum"] == userNum:
        if request.session["isTeacher"] == 0:
            user = get_object_or_404(models.Student, userNum = userNum)
        else:
            user = get_object_or_404(models.Teacher, userNum = userNum)
    
        if request.method == 'GET':
            return render(request, "account/setting.html", {"user": user})
        if request.method == 'POST':
            user.userName = request.POST['userName']
            user.userPhone = request.POST['userPhone']
            try:
                user.save()
            except Exception:
                return JsonResponse({"code": 1})      # fail
            else:
                return JsonResponse({"code": 0})      # success
                    
    else:
        return HttpResponse("未登录！")

def Security(request, userNum):
    if request.session["userNum"] == userNum:
        if request.session["isTeacher"] == 0:
            user = get_object_or_404(models.Student, userNum = userNum)
        else:
            user = get_object_or_404(models.Teacher, userNum = userNum)

        if request.method == 'GET':
            return render(request, "account/security.html", {"user": user})
        if request.method == 'POST':
            userPwd = request.POST['userPwd']
            userNewPwd1 = request.POST['userNewPwd1']
            userNewPwd2 = request.POST['userNewPwd2']
        
        # 判断密码是否正确
        if request.session["isTeacher"] == 0 and not check_password(userPwd, models.Student.objects.filter(userNum=userNum).get().userPwd):
            return JsonResponse({"code": 1})

        if request.session["isTeacher"] == 1 and not check_password(userPwd, models.Teacher.objects.filter(userNum=userNum).get().userPwd):
            return JsonResponse({"code": 1})
        
        # 判断两次密码是否一致
        if userNewPwd1 != userNewPwd2:
            return JsonResponse({"code": 2})
        
        user.userPwd = make_password(userNewPwd1)
        try:
            user.save()
        except Exception:
            return JsonResponse({"code": 3})      # fail
        else:
            return JsonResponse({"code": 0})      # success

    else:
        return HttpResponse("未登录！")