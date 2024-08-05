from django.urls        import path, include
from .views             import PatientView, PatientDetailView
from .views             import HistoryView, PatientHistoryDetailView, clinic_histories_view
from .views             import PatientSearchView

app_name = 'patients'

urlpatterns = [
    # """ 환자 정보 관리 """
    path('', PatientView.as_view(), name="patient"),
    path('<int:patient_id>/', PatientDetailView.as_view(), name="patient-detail"),
    path('search/', PatientSearchView.as_view(), name='patient-search'), 

    # """ 임상 내역 관리 """
    path('histories/', clinic_histories_view, name='history-search'),
    path('<int:patient_id>/histories/', HistoryView.as_view(), name="history"),
    path('<int:patient_id>/histories/<int:history_id>', PatientHistoryDetailView.as_view(), name="history-detail"),
]

