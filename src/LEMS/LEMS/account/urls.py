from django.urls import path
from . import views

app_name = 'account'
urlpatterns = [
    path('setting/<str:userNum>', views.Setting, name = "setting"),
    path('security/<str:userNum>', views.Security, name = "security"),
]