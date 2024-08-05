from rest_framework                     import serializers
from rest_framework.exceptions          import ValidationError
from . import models

class PatientSerializer(serializers.ModelSerializer):
    class Meta:
        model = models.Patient
        fields = '__all__'


class PatientViewSerializer(serializers.Serializer):
    # TODO 사용자 입력값에 대해 valid 함수 추가
    
    patient_id      = serializers.CharField(max_length=30)
    patient_new_id  = serializers.CharField(max_length=30, allow_blank=True, allow_null=True)
    patient_name    = serializers.CharField(max_length=30)
    sex             = serializers.IntegerField(max_value=2, min_value=1)
    age             = serializers.IntegerField(max_value=200, min_value=0)
    t_sex           = serializers.SerializerMethodField()

    def validate_patient_id(self, value):
        if not value.isdigit():
            raise ValidationError('patient id는 숫자로만 구성되어야 함.')
        return value

    def create(self, validated_data):
        return models.Patient.objects.create(**validated_data)

    def get_t_sex(self, obj):
        if obj.sex == 1:
            return '남'
        return '여'
    


class HistoryViewSerializer(serializers.ModelSerializer):
    t_diagnosis = serializers.SerializerMethodField()
    t_impairment = serializers.SerializerMethodField()
    t_bmi_cat = serializers.SerializerMethodField()
    t_risk_assessment_cat = serializers.SerializerMethodField()
    t_body_position = serializers.SerializerMethodField()
    t_matress_position = serializers.SerializerMethodField()
    t_created_at = serializers.SerializerMethodField()

    class Meta:
        model       = models.ClinicHistory
        fields      = ('__all__')


    def get_t_diagnosis(self, obj):
        diagnosis_dict = {
            1: 'stroke',
            2: 'SCI',
            3: 'MS',
            4: 'degenerative'
        }
        return diagnosis_dict.get(obj.diagnosis, obj.diagnosis)

    def get_t_impairment(self, obj):
        impairment_dict = {
            1: 'Rt. hemiplegia',
            2: 'Lt. hemiplegia',
            3: 'Paraplegia',
            4: 'Tetraplegia',
            5: 'ETC'
        }
        return impairment_dict[obj.impairment]
        
    def get_t_bmi_cat(self, obj):
        bmi_dict = {
            1: 'low',
            2: 'moderate',
            3: 'high',
        }
        return bmi_dict.get(obj.bmi_cat, obj.bmi_cat)

    def get_t_risk_assessment_cat(self, obj):
        risk_assessment_dict = {
            1: 'severe',
            2: 'high',
            3: 'moderate',
            4: 'mild',
            5: 'low',
        }
        return risk_assessment_dict.get(obj.risk_assessment_cat, obj.risk_assessment_cat)

    def get_t_body_position(self, obj):
        body_position_dict = {
            1: 'supine',
            2: 'Rt. Lateral',
            3: 'Lt. Lateral',
            4: 'prone',
        }
        return body_position_dict[obj.body_position]

    def get_t_matress_position(self, obj):
        matress_position_dict = {
            1: '원점',
            2: '등판 상승',
            3: '다리 상승',
            4: '등판 & 다리 연동',
            5: '좌회전',
            6: '우회전',
            7: '트렌델렌버그',
        }
        return matress_position_dict[obj.matress_position]

    def get_t_created_at(self, obj):
        return obj.created_at


class PatientHistoryDetailViewSerializer(serializers.ModelSerializer):
    class Meta:
        model       = models.ClinicHistory
        exclude     = ['bmi_cat', 'risk_assessment_cat']