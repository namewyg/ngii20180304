# Generated by Django 3.1.5 on 2021-02-25 16:31

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('user', '0001_initial'),
    ]

    operations = [
        migrations.AlterField(
            model_name='applyrecord',
            name='eState',
            field=models.CharField(choices=[('审核中', '审核中'), ('审核已通过', '审核已通过'), ('审核未通过', '审核未通过'), ('审核过期', '审核过期')], default='审核中', max_length=6, verbose_name='状态'),
        ),
    ]
