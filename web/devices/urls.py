from django.urls import path, include
from . import views

app_name = 'devices'

urlpatterns = [
    # 모듈
    path('', views.ModuleView.as_view(), name="module"),
    path('<int:module_id>/', views.ModuleDetailView.as_view(), name="module-detail"),
    
    # 보드
    path('<int:module_id>/boards/', views.BoardView.as_view(), name='board'),
    path('<int:module_id>/boards/<int:board_id>/', views.BoardDetailView.as_view(), name='board-detail'),
    
    # 이벤트 로그
    path('events/', views.EventView.as_view(), name="event"),
    
]

