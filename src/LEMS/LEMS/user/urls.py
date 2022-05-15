from django.urls import path
from . import views

urlpatterns = [
    path('', views.Index),
    path('register', views.Register),
    path('checklogin', views.Checklogin),
    path('checklogin-teacher', views.CheckloginTeacher),
]