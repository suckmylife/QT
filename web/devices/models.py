from django.db import models
import ipaddress

# Create your models here.
class Module(models.Model):
    alias = models.CharField(unique=True, max_length=30)
    memo = models.CharField(blank=True, null=True, max_length=255)
    created_at = models.DateTimeField(auto_now_add=True)
    updated_at = models.DateTimeField(auto_now=True)

    def __str__(self):
        return self.alias

    def count_boards(self):
        return self.boards.count()

    def get_board_alias(self):
        boards = [b.alias for b in self.boards.all()]
        return boards
        
    get_board_alias.short_description = 'board alias'

    class Meta:
        managed = False
        db_table = 'module'
        app_label = 'devices'


GOOD_HEALTH = 'good'
BAD_HEALTH = 'bad'

HEALTH_CHOICES = (
    (GOOD_HEALTH, 'GOOD'),
    (BAD_HEALTH, 'BAD'),
)

class Board(models.Model):
    module = models.ForeignKey('Module', related_name='boards', on_delete=models.RESTRICT)
    location = models.PositiveIntegerField()
    alias = models.CharField(unique=True, max_length=30)
    serial_num = models.CharField(max_length=45)
    ip_address = models.PositiveIntegerField(unique=True)
    port = models.PositiveSmallIntegerField()
    mac_address = models.CharField(unique=True, max_length=20)
    health = models.CharField(max_length=10, blank=True, null=True, default='good', choices=HEALTH_CHOICES)
    version = models.CharField(max_length=4, blank=True, null=True)
    created_at = models.DateTimeField(auto_now_add=True)
    updated_at = models.DateTimeField(auto_now=True)

    def __str__(self):
        return f'{self.alias}'

    def save(self, *args, **kwargs):
        self.ip_address = int(ipaddress.IPv4Address(self.ip_address))
        return super().save(*args, **kwargs) 
    

    class Meta:
        managed = False
        db_table = 'board'
        app_label = 'devices'

class EventLog(models.Model):
    message = models.TextField()
    created_at = models.DateTimeField(auto_now_add=True)

    class Meta:
        managed = False
        db_table = 'event_log'
        app_label = 'devices'