import ipaddress
from django.contrib import admin
from . import models

# Register your models here.
@admin.register(models.Module)
class ModuleAdmin(admin.ModelAdmin):
    """ Module admin definition"""

    list_display = (
        'alias',
        'count_boards',
        'get_board_alias',
        'created_at',
    )
    


@admin.register(models.Board)
class BoardAdmin(admin.ModelAdmin):
    """ Board admin definition"""

    list_display = (
        "alias",
        "module",
        "location",
        "serial_num",
        "get_ipaddress",
        "mac_address",
        "health",
    )

    list_filter = (
        'module',
        'health',
    )

    ordering = ('module',)

    search_fields = ('serial_num',)

    def get_ipaddress(self, obj):
        return str(ipaddress.IPv4Address(obj.ip_address))

    get_ipaddress.short_description = 'ip address'