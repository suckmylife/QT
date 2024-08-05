from os import stat
from django.http                import HttpResponse, HttpResponseRedirect
from django.shortcuts           import get_object_or_404, redirect
from django.db.models           import RestrictedError
from django.db.models.query     import QuerySet
from django.urls                import reverse
from requests                   import delete

from rest_framework             import status
from rest_framework.views       import APIView
from rest_framework.renderers   import TemplateHTMLRenderer
from rest_framework.response    import Response
from rest_framework.generics    import ListCreateAPIView, RetrieveUpdateDestroyAPIView, ListAPIView
from rest_framework.settings    import api_settings

from .                          import models
from .                          import serializers
from .                          import pagination as deivces_pag

from drf_yasg.utils             import swagger_auto_schema
from drf_yasg                   import openapi



class ModuleView(ListCreateAPIView):
    """
    Module REST API
    ---
    """

    queryset = models.Module.objects.all()
    serializer_class = serializers.ModuleSerializer
    pagination_class = deivces_pag.StandardResultsSetPagination

    template_name = 'devices/module.html'
    renderer_classes = [TemplateHTMLRenderer]

    @swagger_auto_schema(responses={200: 'OK', 404:'Not Found'}, tags=['[모듈] 조회 & 생성'])
    def get(self, request, *args, **kwargs):
        """
            # [GET] Module List를 가져오는 View
            ---
            ### Query string
            - page : paginated된 페이지의 번호
            - page_size : 한 페이지 당 보여줄 결과값의 수
            ---
            ### Return
            ```JSON
            {
                "count": 1,
                "next": null,
                "current": 1,
                "total": 1,
                "previous": null,
                "results": [
                    {
                        "id": 1,
                        "count": 3,
                        "board_list": [
                            "TEST_BOARD_01",
                            "TEST_BOARD_02",
                            "TEST_BOARD_03"
                        ],
                        "alias": "TEST_MODULE_01",
                        "memo": "test module 1",
                        "created_at": "2022-06-02T13:05:44",
                        "updated_at": "2022-06-20T15:59:59"
                    }
                ]
            }
            ```
            - count : 결과 object의 수
            - next : 다음 페이지의 url(ex. /modules?page=1&page_size=10)
            - current : 현재 페이지 번호
            - total : 총 페이지 수
            - previous : 이전 페이지의 url(ex. /modules?page=2&page_size=10)
            - results : 결과값
        """
        print(self.list(request, *args, **kwargs).data)
        return super().get(request, *args, **kwargs)

    @swagger_auto_schema(request_body=serializers.ModuleSerializer, responses={201: 'Created', 400: 'Bad Request'}, tags=['[모듈] 조회 & 생성'])
    def post(self, request, *args, **kwargs):
        """
            # [POST] Module을 생성하는 View
            ---
            ### 입력값
            - alias : 모듈의 별칭, 유일해야하고 필수값.
            - memo : 모듈에 대한 설명
        """
        super().post(request, *args, **kwargs)
        return redirect(reverse('devices:module'))


class ModuleDetailView(RetrieveUpdateDestroyAPIView):
    """
    Module REST API
    ---
    """

    authentication_classes = []
    queryset = models.Module.objects.all()
    serializer_class = serializers.ModuleSerializer
    pagination_class = None
    
    lookup_field = 'pk'
    lookup_url_kwarg = 'module_id'

    template_name = 'devices/module_detail.html'
    renderer_classes = [TemplateHTMLRenderer]
    
    @swagger_auto_schema(responses={200: 'OK', 404:'Not Found'}, tags=['[모듈] 조회 & 수정 & 삭제'])
    def get(self, request, *args, **kwargs):
        """
            # [GET] Module 세부 정보 조회 View
            ---
            ### Query string
            `/modules/<int:module_id>/`
            - module_id : module model의 pk
            ---
            ### Return
            ```JSON
            {
                "id": 1,
                "count": 3,
                "board_list": [
                    "TEST_BOARD_01",
                    "TEST_BOARD_02",
                    "TEST_BOARD_03"
                ],
                "alias": "TEST_MODULE_01",
                "memo": "test module 1",
                "created_at": "2022-06-02T13:05:44",
                "updated_at": "2022-06-20T15:59:59"
            }
            ```
            - id : module의 id
            - count : 모듈에 속해있는 보드들의 개수
            - board_list : 모듈에 속해있는 보드들의 alias
            - alias : 모듈의 별칭
            - memo : 모듈에 대한 설명
        """
        return super().get(request, *args, **kwargs)

    @swagger_auto_schema(responses={200: 'OK'}, tags=['[모듈] 조회 & 수정 & 삭제'])
    def put(self, request, *args, **kwargs):
        return super().put(request, *args, **kwargs)

    @swagger_auto_schema(responses={200: 'OK'}, tags=['[모듈] 조회 & 수정 & 삭제'])
    def patch(self, request, *args, **kwargs):
        """
            # [PATCH] Module 세부 정보 수정 View
            ---
        """
        return super().patch(request, *args, **kwargs)

    @swagger_auto_schema(responses={204: 'No Content'}, tags=['[모듈] 조회 & 수정 & 삭제'])
    def delete(self, request, *args, **kwargs):
        return super().delete(request, *args, **kwargs)

        


class BoardView(ListCreateAPIView):
    """
    Board REST API
    ---
    """

    queryset = models.Board.objects.all()
    serializer_class = serializers.BoardSerializer
    pagination_class = None

    lookup_field = 'module'
    lookup_url_kwarg = 'module_id'
    
    def get_queryset(self):
        assert self.queryset is not None, (
            "'%s' should either include a `queryset` attribute, "
            "or override the `get_queryset()` method."
            % self.__class__.__name__
        )

        queryset = self.queryset
        if isinstance(queryset, QuerySet):
            # Ensure queryset is re-evaluated on each request.
            queryset = queryset.filter(module=self.kwargs['module_id'])
        return queryset

    @swagger_auto_schema(responses={200: 'OK', 404: 'Not Found'}, tags=['[보드] 조회 & 생성'])
    def get(self, request, *args, **kwargs):
        return super().get(request, *args, **kwargs)

    def create(self, request, *args, **kwargs):
        serializer = self.get_serializer(data=request.data)
        serializer.is_valid(raise_exception=True)
        self.perform_create(serializer)
        headers = self.get_success_headers(serializer.data)
        
        return HttpResponseRedirect(redirect_to=f'/modules/{serializer.data["module"]}', headers=headers)
        return Response(serializer.data, status=status.HTTP_201_CREATED, headers=headers)

    @swagger_auto_schema(responses={201: 'Created'}, tags=['[보드] 조회 & 생성'])
    def post(self, request, *args, **kwargs):
        return super().post(request, *args, **kwargs)


class BoardDetailView(RetrieveUpdateDestroyAPIView):
    """
    Board REST API
    ---
    """
    authentication_classes = []
    queryset = models.Board.objects.all()
    serializer_class = serializers.BoardSerializer
    pagination_class = None

    lookup_field = 'id'
    lookup_url_kwarg = 'board_id'

    template_name = 'devices/board_detail.html'
    renderer_classes = [TemplateHTMLRenderer]
    
    @swagger_auto_schema(responses={200: 'OK', 404: 'Not Found'}, tags=['[보드] 조회 & 수정 & 삭제'])
    def get(self, request, *args, **kwargs):
        return super().get(request, *args, **kwargs)

    @swagger_auto_schema(responses={200: 'OK'}, tags=['[보드] 조회 & 수정 & 삭제'])
    def put(self, request, *args, **kwargs):
        return super().put(request, *args, **kwargs)

    @swagger_auto_schema(responses={200: 'OK'}, tags=['[보드] 조회 & 수정 & 삭제'])
    def patch(self, request, *args, **kwargs):
        return super().patch(request, *args, **kwargs)

    @swagger_auto_schema(responses={204: 'No Content'}, tags=['[보드] 조회 & 수정 & 삭제'])
    def delete(self, request, *args, **kwargs):
        return super().delete(request, *args, **kwargs)





class EventView(ListAPIView):
    queryset = models.EventLog.objects.all()
    serializer_class = serializers.EventSerializer
    pagination_class = deivces_pag.StandardResultsSetPagination

    template_name = 'devices/event.html'
    renderer_classes = [TemplateHTMLRenderer]

    @swagger_auto_schema(responses={200: 'OK', 404: 'Not Found'}, tags=['[보드] 조회 & 수정 & 삭제'])
    def get(self, request, *args, **kwargs):
        print(self.list(request, *args, **kwargs).data)
        return super().get(request, *args, **kwargs)
    