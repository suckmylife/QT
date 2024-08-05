from django.contrib import admin
from . import models

# Register your models here.
@admin.register(models.Patient)
class PatientAdmin(admin.ModelAdmin):
    """ Patient admin penal """

    list_display = (
        '__str__',
        'patient_name',
        'sex',
        'age',
        'count_histories',
        
    )

    list_filter = (
        'sex',
    )

    search_fields = (
        'id',
    )

    ordering = (
        '-id',
    )
    def get_patient_id(self, obj):
        return obj.patient_id

    def count_histories(self, obj):
        return obj.patient_histories.count()


@admin.register(models.ClinicHistory)
class PatientHistoryAdmin(admin.ModelAdmin):
    """ Patient history admin penal """

    readonly_fields = ('bmi_cat', 'risk_assessment_cat')

    fieldsets = (
        ('Patient ID', {'fields': ('patient', )}),
        ('메모', {'fields': ('memo',)}),
        ('진단 & 손상', {'fields': ('diagnosis', 'impairment',)}),
        ('체질', {'fields': ('bmi', 'bmi_cat', )}),
        ('입원 기간(일)', {'fields': ('adm_duration', )}),
        ('Braden scale', {'fields': ('risk_assessment', 'risk_assessment_cat', )}),
        ('자세', {'fields': ('body_position', 'matress_position', )}),
    )

    list_display = (
        'patient',
        'get_patient_name',
        'diagnosis',
        'impairment',
        'risk_assessment_cat',
    )

    list_filter = (
        'diagnosis',
        'impairment',
    )

    search_fields = (
        'patient',
    )

    ordering = (
        'patient',
    )

    raw_id_fields = (
        'patient',
    )

    def get_patient_name(self, obj):
        if obj.patient is None:
            return 'None'
        return obj.patient.patient_name

    get_patient_name.short_description = 'patient name'