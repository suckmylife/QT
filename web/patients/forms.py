from django import forms


class PatientForm(forms.Form):
    patient_id = forms.CharField()
    patient_new_id = forms.CharField()
    patient_name = forms.CharField()
    sex = forms.CharField()
    age = forms.CharField()