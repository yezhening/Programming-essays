from django.db import models

# Create your models here.
class Request(models.Model):
    number = models.IntegerField(primary_key=True)
    source_ip = models.CharField(max_length=20)
    destination_ip = models.CharField(max_length=20)
    source_port = models.IntegerField()
    destination_port = models.IntegerField()
    host = models.CharField(max_length=100)
    url = models.CharField(max_length=500)
    user_agent = models.CharField(max_length=100)
    cookie = models.CharField(max_length=500, blank=True, null=True)
    frame_size = models.IntegerField()
    file_path = models.CharField(
        max_length=100, db_collation='utf8_general_ci')

    class Meta:
        managed = False
        db_table = 'request'


class Response(models.Model):
    number = models.IntegerField(primary_key=True)
    source_ip = models.CharField(max_length=20)
    destination_ip = models.CharField(max_length=20)
    source_port = models.IntegerField()
    destination_port = models.IntegerField()
    content_type = models.CharField(max_length=100)
    frame_size = models.IntegerField()
    file_path = models.CharField(max_length=100)

    class Meta:
        managed = False
        db_table = 'response'


class Whois(models.Model):
    domain_name = models.CharField(primary_key=True, max_length=100)
    registrant_organization = models.CharField(
        max_length=100, blank=True, null=True)
    registrar = models.CharField(max_length=100, blank=True, null=True)
    registrar_phone = models.CharField(max_length=50, blank=True, null=True)

    class Meta:
        managed = False
        db_table = 'whois'


class DomainManagement(models.Model):
    domain_name = models.CharField(primary_key=True, max_length=100)
    registrant_organization = models.CharField(
        max_length=100, blank=True, null=True)
    domain_type = models.CharField(max_length=50, blank=True, null=True)

    class Meta:
        managed = False
        db_table = 'domain_management'
