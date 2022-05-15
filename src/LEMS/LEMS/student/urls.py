from django.urls import path
from . import views

app_name = 'student'
urlpatterns = [
    path('home', views.Home, name= "home"),
    path('<str:type>/equipments', views.EquipmentsView, name = "type"),
    
    path('apply', views.Apply, name = "apply"),
    path('apply-update', views.ApplyUpdate, name = "apply-update"),
    path('my-apply-record', views.MyApplyRecord, name = "my-apply-record"),
    path('apply-record-detail', views.ApplyRecordDetail, name = "apply-record-detail"),
    path('get-apply-record-date', views.GetApplyRecordDate, name = "get-apply-record-date"),

    path('drugs-view/<str:type>', views.DrugsView, name = "drugs-view"),
    path('drug-check', views.DrugCheck, name = "drug-check"),
    path('drug-check-update', views.DrugCheckUpdate, name = "drug-check-update"),
    path('my-drug-check', views.MyDrugCheck, name = "my-drug-check"),
]