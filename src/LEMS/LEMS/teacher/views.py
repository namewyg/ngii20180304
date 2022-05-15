from django.shortcuts import render, redirect
from user import models
from django.http import JsonResponse, HttpResponse
from django.db.models import Q

def HomeView(request):
    """教师界面"""
    userNum = request.session.get("userNum")
    if not userNum:
        return redirect('/')
    types = models.Equipment.TYPE_CHOICES
    user = models.Teacher.objects.get(userNum = userNum)
    return render(request, "teacher/home.html", {"user": user, "types":types})

def EquipmentsView(request, type):
    equipments = models.Equipment.objects.all()
    if type != "all":
        equipments = equipments.filter(eType=type)
    return render(request, "teacher/equipments.html", {"equipments": equipments})

def DrugsView(request, type):
    drugs = models.Drug.objects.all().filter(drugSafetyDegree=type)
    return render(request, "teacher/drugs.html", {"drugs": drugs})

def AddEquipment(request):
    """添加新增设备"""
    teachers = models.Teacher.objects.all()
    types = models.Equipment.TYPE_CHOICES
    return render(request, "teacher/add-equipment.html", {"teachers": teachers, "types":types})

def AddEquipmentUpdate(request):
    """提交新增设备信息"""
    eType = request.POST["eType"]
    eName = request.POST["eName"]
    eModel = request.POST["eModel"]
    eLocation = request.POST["eLocation"]
    eWarranty = request.POST["eWarranty"]
    eDirector = models.Teacher.objects.get(userNum = request.POST["userNum"])
    try:
        models.Equipment.objects.create(eType=eType,eName=eName,eModel=eModel,eLocation=eLocation,eWarranty=eWarranty,eDirector=eDirector)
    except Exception:
        return JsonResponse({"code": 1})
    else:
        return JsonResponse({"code": 0})

def DeleteEquipment(request):
    """删除设备"""
    eid = request.GET["eid"]
    equipment = models.Equipment.objects.get(id=int(eid))
    try:
        equipment.delete()
    except Exception:
        return HttpResponse("删除失败")
    else:
        return HttpResponse("删除成功")

def EditEquipment(request):
    """编辑设备信息"""
    eid = request.GET['eid']
    equipment = models.Equipment.objects.get(id=int(eid))
    types = models.Equipment.TYPE_CHOICES
    return render(request, "teacher/edit-equipment.html", {"equipment": equipment, "types":types})

def EditEquipmentUpdate(request):
    """提交更新设备信息"""
    eid = request.GET["eid"]
    e = models.Equipment.objects.get(id=eid)
    e.eType = request.POST["eType"]
    e.eName = request.POST["eName"]
    e.eModel = request.POST["eModel"]
    e.eLocation = request.POST["eLocation"]
    e.eState = request.POST["eState"]
    try:
        e.save()
    except Exception:
        return JsonResponse({"code": 1})
    else:
        return JsonResponse({"code": 0})

def ApplyRecord(request, state):
    """设备预约列表"""
    userNum = request.session.get("userNum")
    if not userNum:
        return HttpResponse("请重新登录")
    if state == 'review':
        applyRecords = models.ApplyRecord.objects.all().filter(Q(equipment__eDirector__userNum = userNum) & Q(eState="待审核"))
    elif state == 'passed':
        applyRecords = models.ApplyRecord.objects.all().filter(Q(equipment__eDirector__userNum = userNum) & Q(eState="审核通过"))
    elif state == 'failed':
        applyRecords = models.ApplyRecord.objects.all().filter(Q(equipment__eDirector__userNum = userNum) & Q(eState="审核未通过"))
    elif state == 'expired':
        applyRecords = models.ApplyRecord.objects.all().filter(Q(equipment__eDirector__userNum = userNum) & Q(eState="审核过期"))
    else:
        applyRecords = models.ApplyRecord()
    return render(request, "teacher/apply-record.html", {"applyRecords": applyRecords})

def ApplyRecordDetail(request):
    """设备预约详情"""
    userNum = request.session.get("userNum")
    if not userNum:
        return HttpResponse("请重新登录")
    id = request.GET["id"]
    applyRecord = models.ApplyRecord.objects.get(id=int(id))
    return render(request, "teacher/apply-record-detail.html", {"applyRecord": applyRecord})

def ApplyRecordOperate(request, operation):
    """设备预约处理"""
    id = request.GET["id"]
    applyRecord = models.ApplyRecord.objects.get(id=int(id))
    if operation == 'pass':
        applyRecord.eState = "审核通过"
    elif operation == 'fail':
        applyRecord.eState = "审核未通过"

    try:
        applyRecord.save()
    except Exception:
        return HttpResponse("操作失败")
    else:
        return HttpResponse("操作成功")

def DrugCheck(request, state):
    userNum = request.session.get("userNum")
    if not userNum:
        return HttpResponse("请重新登录")
    if state == 'review':
        drugChecks = models.DrugCheck.objects.all().filter(state="待审核")
    elif state == 'passed':
        drugChecks = models.DrugCheck.objects.all().filter(state="审核通过")
    elif state == 'failed':
        drugChecks = models.DrugCheck.objects.all().filter(state="审核未通过")
    elif state == 'expired':
        drugChecks = models.DrugCheck.objects.all().filter(state="审核过期")
    else:
        drugChecks = models.DrugCheck()
    return render(request, "teacher/drug-check.html", {"drugChecks": drugChecks})

def DrugCheckOperate(request, operation):
    id = request.GET["id"]
    drugCheck = models.DrugCheck.objects.get(id=int(id))
    if operation == 'pass':
        drugCheck.state = "审核通过"
        try:
            drugCheck.save()
        except Exception:
            return HttpResponse("审核状态更新失败")

        try:
            drug = models.Drug.objects.get(Q(drugCAS=drugCheck.drugCAS) & Q(drugUnit=drugCheck.drugUnit))
        except models.Drug.DoesNotExist:
            drug = models.Drug.objects.create(drugName=drugCheck.drugName,
                            drugCAS=drugCheck.drugCAS,
                            drugUnit=drugCheck.drugUnit,
                            drugAmount=0,
                            drugSafetyDegree=drugCheck.drugSafetyDegree)
        if drugCheck.function == "设备使用":
            drug.drugAmount -= drugCheck.drugAmount
        elif drugCheck.function == "设备入库":
            drug.drugAmount += drugCheck.drugAmount
        try:
            drug.save()
        except Exception:
            return HttpResponse("设备更新失败")

    elif operation == 'fail':
        drugCheck.state = "审核未通过"
        try:
            drugCheck.save()
        except Exception:
            return HttpResponse("操作失败")
    
    return HttpResponse("操作成功")