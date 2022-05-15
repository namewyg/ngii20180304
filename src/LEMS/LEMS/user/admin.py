from django.contrib import admin
from . import models
from django.contrib.auth.hashers import make_password

class StudentAdmin(admin.ModelAdmin):
    list_display = ['userName','userNum']
    def save_model(self, request, obj, form, change) -> None:
        obj.userPwd = make_password(request.POST['userPwd'])
        return super(StudentAdmin,self).save_model(request, obj, form, change)

class TeacherAdmin(admin.ModelAdmin):
    list_display = ['userName','userNum']
    def save_model(self, request, obj, form, change) -> None:
        obj.userPwd = make_password(request.POST['userPwd'])
        return super(TeacherAdmin,self).save_model(request, obj, form, change)

class EquipmentAdmin(admin.ModelAdmin):
    list_display = ['eName','eModel','eLocation','eDirector']
    def eDirector(self, obj):
        return obj.eDirector.userName

class ApplyRecordAdmin(admin.ModelAdmin):
    list_display = ['student','equipment','eDate']

class DrugAdmin(admin.ModelAdmin):
    list_display = ['drugName','drugCAS','drugAmount','drugUnit']

class DrugCheckAdmin(admin.ModelAdmin):
    list_display = ['student','function','drugName','drugCAS','drugAmount','drugUnit','date']
# Register your models here.
admin.site.register(models.Student,StudentAdmin)
admin.site.register(models.Teacher,TeacherAdmin)
admin.site.register(models.Equipment,EquipmentAdmin )
admin.site.register(models.ApplyRecord, ApplyRecordAdmin)
admin.site.register(models.Drug, DrugAdmin)
admin.site.register(models.DrugCheck, DrugCheckAdmin)