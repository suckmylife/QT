from django.db import models

# Create your models here.
# patient history catogory field
SEX_MALE = 1
SEX_FEMALE = 2

SEX_CHOICES = [
    (SEX_MALE, 'Male'),
    (SEX_FEMALE, 'Female'),
]

DIAGNOSIS_CHOICES = [
    (1, 'Stroke'),
    (2, 'SCI'),
    (3, 'MS'),
    (4, 'Degenerative'),
]

IMPAIRMENT_CHOICES = [
    (1, 'Rt. hemiplegia'),
    (2, 'Lt. hemiplegia'),
    (3, 'Paraplegia'),
    (4, 'Quadri or Tetraplegia'),
    (5, 'ETC'),
]

BMI_CHOICES = [
    (1, 'Low'),
    (2, 'Moderate'),
    (3, 'High'),
]

RISK_ASSESSMENT_CHOICES = [
    (1, 'Severe'),
    (2, 'High'),
    (3, 'Moderate'),
    (4, 'Mild'),
    (5, 'Low'),
]

BODY_POSITION_CHOICES = [
    (1, 'Supine'),
    (2, 'Rt. lateral'),
    (3, 'Lt. lateral'),
    (4, 'Prone'),
]

MATRESS_POSITION_CHOICES = [
    (1, '원점'),
    (2, '등판상승'),
    (3, '다리상승'),
    (4, '등판다리연동'),
    (5, '좌회전'),
    (6, '우회전'),
    (7, '트렌델렌버그'),
]


# 환자 
class Patient(models.Model):
    patient_id = models.CharField(unique=True, max_length=30)
    patient_new_id = models.CharField(max_length=30, blank=True, null=True)
    patient_name = models.CharField(max_length=30)
    sex = models.PositiveIntegerField(blank=True, null=True, choices=SEX_CHOICES)
    age = models.PositiveIntegerField(blank=True, null=True)
    created_at = models.DateTimeField(auto_now_add=True)
    updated_at = models.DateTimeField(auto_now=True)

    def __str__(self):
        return f'{self.patient_id}'
        
    class Meta:
        managed = False
        db_table = 'patient'
        app_label = 'patients'
        ordering = ['patient_id']



# 환자 내역 
class ClinicHistory(models.Model):
    patient = models.ForeignKey('Patient', related_name='patient_histories', on_delete=models.RESTRICT)
    memo = models.TextField(blank=True, null=True)
    diagnosis = models.PositiveIntegerField(blank=True, null=True, choices=DIAGNOSIS_CHOICES)
    impairment = models.PositiveIntegerField(blank=True, null=True, choices=IMPAIRMENT_CHOICES)
    bmi = models.FloatField(blank=True, null=True)
    bmi_cat = models.PositiveIntegerField(blank=True, null=True, choices=BMI_CHOICES)
    adm_duration = models.PositiveSmallIntegerField(blank=True, null=True)
    risk_assessment = models.PositiveIntegerField(blank=True, null=True)
    risk_assessment_cat = models.PositiveIntegerField(blank=True, null=True, choices=RISK_ASSESSMENT_CHOICES)
    body_position = models.PositiveIntegerField(blank=True, null=True, choices=BODY_POSITION_CHOICES)
    matress_position = models.PositiveIntegerField(blank=True, null=True, choices=MATRESS_POSITION_CHOICES)
    created_at = models.DateTimeField(auto_now_add=True)
    updated_at = models.DateTimeField(auto_now=True)
    
    def get_bmi_cateogory(self, bmi):
        if bmi is None:
            return None

        if bmi < 18.4:
            # 'low'
            return 1
        elif bmi >=18.4 and bmi <= 24.9:
            # 'moderate'
            return 2
        elif bmi > 24.9:
            # 'high'
            return 3
    
    def get_risk_assessment_category(self, risk_assessment):
        if risk_assessment is None:
            return None

        if risk_assessment <= 9:
            # 'severe'
            return 1
        elif 10 <= risk_assessment <= 12:
            # 'high'
            return 2
        elif 13 <= risk_assessment <= 14:
            # 'moderate'
            return 3
        elif 15 <= risk_assessment <= 18:
            # 'mild'
            return 4
        elif 19 <= risk_assessment <= 23:
            # 'low'
            return 5

    def save(self, *args, **kwargs) -> None:
        self.bmi_cat = self.get_bmi_cateogory(self.bmi)
        self.risk_assessment_cat = self.get_risk_assessment_category(self.risk_assessment)

        return super().save(*args, **kwargs) 
    
    class Meta:
        managed = False
        db_table = 'clinic_history'
        app_label = 'patients'
        ordering = ['-created_at', ]

# class Photo(models.Model):
#     clinic_history = models.ForeignKey('patients.ClinicHistory', on_delete=models.CASCADE)
#     caption = models.CharField(max_length=50)
#     image = models.TextField()
#     image_size = models.CharField(max_length=255, blank=True, null=True)
#     created_at = models.DateTimeField(auto_now_add=True)
#     updated_at = models.DateTimeField(auto_now=True)

#     class Meta:
#         managed = False
#         db_table = 'photo'