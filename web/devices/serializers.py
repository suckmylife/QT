from dataclasses import field
import ipaddress

from rest_framework import serializers
from core import serializers as core_serializers
from devices import models


class CustomIpAddressField(serializers.Field):
    """ Custom IP Address Field """

    # TODO input validation
    # 안들어왔을때, 숫자 범위가 벗어날 경우

    def to_representation(self, value):
        return str(ipaddress.IPv4Address(value))

    def to_internal_value(self, data):
        return int(ipaddress.IPv4Address(data))



class BoardSerializer(serializers.ModelSerializer):
    ip_address = CustomIpAddressField()
    created_at = core_serializers.CustomCreatedAtField(read_only=True)
    updated_at = core_serializers.CustomUpdatedAtField(read_only=True)
    
    class Meta:
        model = models.Board
        fields = '__all__'


class ModuleSerializer(serializers.ModelSerializer):
    count = serializers.SerializerMethodField()
    boards = BoardSerializer(many=True, read_only=True)
    
    class Meta:
        model = models.Module
        fields= '__all__'
        read_only_field = ['count', 'boards']

    def get_count(self, obj):
        return obj.count_boards()


class EventSerializer(serializers.ModelSerializer):
    class Meta:
        model = models.EventLog
        fields = '__all__'