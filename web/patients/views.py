from os import stat
import re
from django.http                import Http404
from django.urls                import reverse
from django.core.paginator      import Paginator, EmptyPage
from django.shortcuts           import redirect, render, HttpResponseRedirect
from django.db.models           import RestrictedError
from django.db.models.query     import QuerySet
from django.core.exceptions     import ValidationError

from rest_framework             import status, filters
from rest_framework.decorators  import api_view, renderer_classes, permission_classes
from rest_framework.response    import Response
from rest_framework.views       import APIView
from rest_framework.generics    import get_object_or_404, ListCreateAPIView, ListAPIView
from rest_framework.renderers   import TemplateHTMLRenderer
from rest_framework.permissions import IsAuthenticated, IsAdminUser, AllowAny
from uritemplate import partial

from .                          import forms
from .                          import pagination as patient_pagination
from .models                    import Patient, ClinicHistory
from .serializers               import PatientSerializer, PatientViewSerializer, HistoryViewSerializer, PatientHistoryDetailViewSerializer
from .                          import serializers as patient_serializer

from drf_yasg.utils             import swagger_auto_schema
from drf_yasg                   import openapi

from django_filters.rest_framework  import DjangoFilterBackend

from patients import serializers




# Create your views here.
class PatientView(ListCreateAPIView):
    """
    환자 목록, 생성 API View
    """

    queryset = Patient.objects.all()
    serializer_class = PatientViewSerializer
    pagination_class = patient_pagination.StandardResultsSetPagination
    template_name = 'patients/patient.html'
    renderer_classes = [TemplateHTMLRenderer]

    @swagger_auto_schema(responses={200: 'Created'})
    def get(self, request, *args, **kwargs):
        """ 환자 조회 """
        context = self.list(request, *args, **kwargs).data
        context['search'] = False
        
        return self.list(request, *args, **kwargs)
        

    @swagger_auto_schema(request_body=patient_serializer.PatientViewSerializer, responses={201: 'Created', 400: 'Bad Request'})
    def post(self, request):
        """ 환자 생성 """
        serializer = self.serializer_class(data=request.data)
        
        if serializer.is_valid():
            serializer.save()
            return redirect(reverse('patients:patient'))

        context = {
            'error': serializer.errors
        }
        return Response(context, status=status.HTTP_400_BAD_REQUEST)


class PatientSearchView(ListAPIView):
    """
    환자 검색 View
    """
    queryset = Patient.objects.all()
    serializer_class = PatientViewSerializer
    pagination_class = patient_pagination.StandardResultsSetPagination
    template_name = 'patients/patient.html'
    renderer_classes = [TemplateHTMLRenderer]

    def get_queryset(self):

        assert self.queryset is not None, (
            "'%s' should either include a `queryset` attribute, "
            "or override the `get_queryset()` method."
            % self.__class__.__name__
        )
        
        queryset = self.queryset
        request_query_params = self.request.query_params
        
        filter_args = {}

        filter_args['patient_id__startswith'] = request_query_params['patient_id']
        filter_args['patient_name__startswith'] = request_query_params['patient_name']
        sex = int(request_query_params['sex'])

        if sex != 0:
            filter_args['sex'] = sex
        
        if isinstance(queryset, QuerySet):
            # Ensure queryset is re-evaluated on each request.
            queryset = queryset.filter(**filter_args)
        return queryset


class PatientDetailView(APIView):
    """ 환자 상세정보 API View """

    template_name = 'patients/patient_detail.html'
    authentication_classes = []
    
    def get_object(self, patient_id):
        patient     = get_object_or_404(Patient, patient_id=patient_id)
        return patient

    @swagger_auto_schema(operation_description='특정 환자 정보의 조회', responses={200: 'Success'})
    def get(self, request, patient_id):
        """ GET patient info with patient_id """

        try:
            patient     = self.get_object(patient_id)
        except Patient.DoesNotExist:
            return redirect('/patient')
        
        context = {
            "patient_id": patient.patient_id,
            "patient_new_id": patient.patient_new_id,
            "patient_name": patient.patient_name,
            "sex": patient.sex,
            "age": patient.age,
        }

        return render(request, template_name=self.template_name, context=context)


    @swagger_auto_schema(operation_description='특정 환자 정보의 수정', request_body=patient_serializer.PatientSerializer)
    def post(self, request, patient_id):
        """ UPDATE patient info with patient_id """

        patient     = self.get_object(patient_id)
        serializer  = PatientSerializer(patient, data=request.data, partial=True)
        
        if not serializer.is_valid(raise_exception=True):
            return Response(status=status.HTTP_400_BAD_REQUEST)
        
        serializer.is_valid(raise_exception=True)
        data = serializer.validated_data
        
        serializer.save()
        
        if str(patient_id) != data['patient_id']:
            data['redirect'] = True
            return Response(data, status=status.HTTP_200_OK)

        data['redirect'] = False
        return Response(data, status=status.HTTP_200_OK)
    
    
    @swagger_auto_schema(operation_description='특정 환자 정보의 삭제')
    def delete(self, request, patient_id):
        """ DELETE patient info with patient_id """

        patient = self.get_object(patient_id)
        
        try:
            patient.delete()
        except RestrictedError:
            return Response(status=status.HTTP_400_BAD_REQUEST)

        return Response(status=status.HTTP_204_NO_CONTENT)
    

@api_view(['GET'])
def clinic_histories_view(request):
    return render(request, 'histories/histories.html')


class HistoryView(ListCreateAPIView):

    queryset = ClinicHistory.objects.all()
    serializer_class = HistoryViewSerializer
    pagination_class = patient_pagination.StandardResultsSetPagination
    template_name = 'histories/histories.html'
    renderer_classes = [TemplateHTMLRenderer]
    permissions_classes = [AllowAny]

    def get_queryset(self):
        assert self.queryset is not None, (
            "'%s' should either include a `queryset` attribute, "
            "or override the `get_queryset()` method."
            % self.__class__.__name__
        )
        queryset = self.queryset
        
        # also : self.request.parser_context.get('kwargs')
        patient_id = int(self.kwargs.get('patient_id'))
        try:
            patient = Patient.objects.get(patient_id=patient_id)
        except Patient.DoesNotExist:
            return Response(status=status.HTTP_400_BAD_REQUEST)
            

        if isinstance(queryset, QuerySet):
            # Ensure queryset is re-evaluated on each request.
            queryset = queryset.filter(patient=patient.id)
        return queryset
        
    def get(self, request, *args, **kwargs):
        patient_id = int(self.kwargs.get('patient_id'))

        try:
            patient = Patient.objects.get(patient_id=patient_id)
        except Patient.DoesNotExist:
            return Response(status=status.HTTP_400_BAD_REQUEST)

        data = self.list(request, *args, **kwargs).data
        data['patient'] = patient
        
        return Response(data=data, status=status.HTTP_200_OK)
    
    # TODO post
    def create(self, request, *args, **kwargs):
        patient_id = request.POST['patient']

        try:
            patient = Patient.objects.get(patient_id=patient_id)
        except Patient.DoesNotExist:
            return Response(status=status.HTTP_400_BAD_REQUEST)

        request.POST._mutable = True
        request.POST['patient'] = patient.id
        request.POST._mutable = False
        
        serializer = self.get_serializer(data=request.data)
        
        if serializer.is_valid(raise_exception=True):
            self.perform_create(serializer)
            headers = self.get_success_headers(serializer.validated_data)
            return Response(serializer.validated_data, status=status.HTTP_201_CREATED, headers=headers)

        return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)



class PatientHistoryDetailView(APIView):
    
    authentication_classes = []
    
    def get_object(self, patient_id):
        patient     = get_object_or_404(Patient, patient_id=patient_id)
        return patient

    def get(self, request, patient_id, history_id):
        try:
            patient = self.get_object(patient_id)
        except Patient.DoesNotExist:
            raise ValidationError('patient not exist')

        try:
            queryset = ClinicHistory.objects.get(id=history_id)
        except ClinicHistory.DoesNotExist:
            raise ValidationError('history not exist')

        serializer = HistoryViewSerializer(queryset)

        context = {
            'histories': serializer.data,
        }

        return Response(context, status=status.HTTP_200_OK)
    
    
    def patch(self, request, patient_id, history_id):
        print('patch!!')
        """ 임상 내역 수정 """
        try:
            queryset = ClinicHistory.objects.get(id=history_id)
        except ClinicHistory.DoesNotExist:
            raise ValidationError('history not exist')

        serializer = HistoryViewSerializer(queryset, data=request.data, partial=True)

        if serializer.is_valid(raise_exception=True):
            serializer.save()
            headers = self.get_success_headers(serializer.validated_data)
            return Response(serializer.validated_data, status=status.HTTP_202_ACCEPTED, headers=headers)

        return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)


    
    def delete(self, request, patient_id, history_id):
        """ 임상 내역 삭제 """
        try:
            queryset = ClinicHistory.objects.get(id=history_id)
        except ClinicHistory.DoesNotExist:
            raise ValidationError('history not exist')

        try:
            queryset.delete()
        except RestrictedError:
            return Response(status=status.HTTP_400_BAD_REQUEST)
        
        return Response(status=status.HTTP_204_NO_CONTENT)

