from django.urls import path
from . import views


app_name = 'teacher'
urlpatterns = [
    path('home', views.HomeView),
    path('<str:type>/equipments', views.EquipmentsView, name = "type"),
    path('add-equipment', views.AddEquipment, name = "add-equipment"),
    path('add-equipment-update', views.AddEquipmentUpdate, name = "add-equipment-update"),
    path('delete-equipment', views.DeleteEquipment, name = "delete-equipment"),
    path('edit-equipment', views.EditEquipment, name = "edit-equipment"),
    path('edit-equipment-update', views.EditEquipmentUpdate, name = "edit-equipment-update"),

    path('apply-record-detail', views.ApplyRecordDetail, name = "apply-record-detail"),
    path('apply-record/<str:state>', views.ApplyRecord, name = "apply-record-state"),
    path('apply-record-operate/<str:operation>', views.ApplyRecordOperate, name = "apply-record-operate"),

    path('drugs-view/<str:type>', views.DrugsView, name = "drugs-view"),
    path('drug-check/<str:state>', views.DrugCheck, name = "drug-check-state"),
    path('drug-check-operate/<str:operation>', views.DrugCheckOperate, name = "drug-check-operate"),
]

