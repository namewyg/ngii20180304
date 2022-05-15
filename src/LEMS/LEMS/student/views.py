from django.shortcuts import render, redirect
from user import models
import json
from django.http import JsonResponse, HttpResponse
from django.db.models import Q

def Home(request):
    userNum = request.session.get("userNum")
    if not userNum:
        return redirect('/')
    types = models.Equipment.TYPE_CHOICES
    user = models.Student.objects.get(userNum = userNum)
    return render(request, "student/home.html", {"user": user, "types":types})

def EquipmentsView(request, type):
    equipments = models.Equipment.objects.all()
    if type != "all":
        equipments = equipments.filter(eType=type)
    return render(request, "student/equipments.html", {"equipments": equipments})

def DrugsView(request, type):
    drugs = models.Drug.objects.all().filter(drugSafetyDegree=type)
    return render(request, "student/drugs.html", {"drugs": drugs})

def MyApplyRecord(request):
    """我借用的设备列表"""
    userNum = request.session.get("userNum")
    if not userNum:
        return HttpResponse("请重新登录")
    student = models.Student.objects.get(userNum=userNum)
    applyRecords = models.ApplyRecord.objects.all().filter(student = student)
    return render(request, "student/my-apply-record.html", {"applyRecords": applyRecords})


def ApplyRecordDetail(request):
    """设备预约详情"""
    userNum = request.session.get("userNum")
    if not userNum:
        return HttpResponse("请重新登录")
    id = request.GET["id"]
    applyRecord = models.ApplyRecord.objects.get(id=int(id))
    return render(request, "student/apply-record-detail.html", {"applyRecord": applyRecord})

def Apply(request):
    """预约设备"""
    eid = request.GET["eid"]
    return render(request, "student/apply.html", {"eid": eid})

def ApplyUpdate(request):
    """预约设备"""
    userNum = request.session.get("userNum")
    if not userNum:
        return HttpResponse("请重新登录")
    eid = request.POST["eid"]
    eDate = request.POST["eDate"]
    eBegin = request.POST["eBegin"]
    eEnd = request.POST["eEnd"]
    director = request.POST["director"]
    paymentMethod = request.POST["paymentMethod"]
    projectName = request.POST["projectName"]
    projectCode = request.POST["projectCode"]
    projectType = request.POST["projectType"]
    experimentContent = request.POST["experimentContent"]
    experimentNote = request.POST["experimentNote"]
    try:
        student = models.Student.objects.get(userNum=userNum)
        equipment = models.Equipment.objects.get(id=eid)
        models.ApplyRecord.objects.create(student=student,
                                            equipment=equipment,
                                            eDate=eDate,
                                            eBegin=eBegin,
                                            eEnd=eEnd,
                                            director=director,
                                            paymentMethod=paymentMethod,
                                            projectName=projectName,
                                            projectCode=projectCode,
                                            projectType=projectType,
                                            experimentContent=experimentContent,
                                            experimentNote=experimentNote)

        return JsonResponse({"code": 0})
    except ValueError:
        return JsonResponse({"code": 1})

def DrugCheck(request):
    drugs = models.Drug.objects.all()
    return render(request, "student/drug-check.html", {"drugs": drugs})

def DrugCheckUpdate(request):
    userNum = request.session.get("userNum")
    if not userNum:
        return HttpResponse("请重新登录")
    function = request.POST["function"]
    drugName = request.POST["drugName"]
    drugCAS = request.POST["drugCAS"]
    drugUnit = request.POST["drugUnit"]
    drugAmount = request.POST["drugAmount"]
    drugSafetyDegree = request.POST["drugSafetyDegree"]
    date = request.POST["date"]
    note = request.POST["note"]
 
    user = models.Student.objects.get(userNum = userNum)
    try:
        models.DrugCheck.objects.create(student=user.userName,
                                        function=function,
                                        drugName=drugName,
                                        drugCAS=drugCAS,
                                        drugUnit=drugUnit,
                                        drugAmount=drugAmount,
                                        drugSafetyDegree=drugSafetyDegree,
                                        date=date,
                                        note=note,
                                        state="待审核")
        return JsonResponse({"code": 0})
    except ValueError:
        return JsonResponse({"code": 1})  
         
def MyDrugCheck(request):
    userNum = request.session.get("userNum")
    if not userNum:
        return HttpResponse("请重新登录")
    student = models.Student.objects.get(userNum=userNum)
    drugChecks = models.DrugCheck.objects.all().filter(student = student.userName)
    return render(request, "student/my-drug-check.html", {"drugChecks": drugChecks})


def GetApplyRecordDate(request):
    """ """
    eid = request.POST["eid"]
    eDate = request.POST["eDate"]
    equipment = models.Equipment.objects.get(id=int(eid))
    applyRecords = models.ApplyRecord.objects.all().filter(Q(equipment=equipment)&Q(eDate=eDate))
    disabledDate = set()
    for apply in applyRecords:
        for i in range(apply.eBegin,apply.eEnd+1):
            disabledDate.add(i)
    return HttpResponse(json.dumps(list(disabledDate)))


