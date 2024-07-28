from django.db import models

# Create your models here.
class Permissions(models.Model):
    id = models.IntegerField(primary_key=True)
    name = models.CharField(max_length=100)

    class Meta:
        managed = False
        db_table = 'permissions'

class Role(models.Model):
    id = models.IntegerField(primary_key=True)
    name = models.CharField(max_length=10)

    class Meta:
        managed = False
        db_table = 'role'

class RolePermissons(models.Model):
    id = models.IntegerField(primary_key=True)
    role = models.ForeignKey(Role, models.DO_NOTHING)
    permissons = models.ForeignKey(Permissions, models.DO_NOTHING)

    class Meta:
        managed = False
        db_table = 'role_permissons'


class Consumer(models.Model):
    id = models.IntegerField(primary_key=True)
    name = models.CharField(max_length=10)
    account = models.CharField(max_length=10)
    password = models.CharField(max_length=10, blank=True, null=True)

    class Meta:
        managed = False
        db_table = 'consumer'


class ConsumerRole(models.Model):
    id = models.IntegerField(primary_key=True)
    consumer = models.ForeignKey(Consumer, models.DO_NOTHING)
    role = models.ForeignKey('Role', models.DO_NOTHING)

    class Meta:
        managed = False
        db_table = 'consumer_role'
