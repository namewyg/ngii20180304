from django.db import models

# Create your models here.
DRUG_UNIT_CHOICES = (
                        ('台', '台'),
                        ('套', '套'),
                        ('千克', '千克'),
                    )
DRUG_SAFETY_DEGREE_CHOICES = (
                    ('一般', '一般'),
                    ('危险', '危险'),
                )

class Student(models.Model):
    userName = models.CharField(max_length=11, verbose_name='姓名')
    userNum = models.CharField(max_length=15, verbose_name='学号')
    userPhone = models.CharField(max_length=11, verbose_name='联系方式')
    userPwd = models.CharField(max_length=78, verbose_name='密码')
    def __str__(self):
        return self.userName

class Teacher(models.Model):
    userName = models.CharField(max_length=11, verbose_name='姓名')
    userNum = models.CharField(max_length=15, verbose_name='工号')
    userPhone = models.CharField(max_length=11, verbose_name='联系方式')
    userPwd = models.CharField(max_length=78, verbose_name='密码')
    def __str__(self):
        return self.userName

class Equipment(models.Model):
    TYPE_CHOICES = (
                    ("机械类", "机械类"),
                    ("电气类", "电气类"),
                    )
    eType = models.CharField(max_length=10, verbose_name="设备类型", choices=TYPE_CHOICES, default="请选择设备类型")
    eName = models.CharField(max_length=10, verbose_name="设备名称")
    eModel = models.CharField(max_length=10, verbose_name="设备型号")
    eLocation = models.CharField(max_length=10, verbose_name="设备位置")
    eWarranty = models.DateField(verbose_name="时间")
    eDirector = models.ForeignKey(Teacher, on_delete=models.CASCADE, verbose_name="负责人")
    STATE_CHOICES = (
                    ('可借', '可借'),
                    ('不可借', '不可借'),
                )
    eState = models.CharField(max_length=6, verbose_name="状态", choices=STATE_CHOICES, default="可借")
    def __str__(self): 
            return self.eName

class ApplyRecord(models.Model):
    student = models.ForeignKey(Student, on_delete=models.CASCADE, verbose_name="预约人")
    equipment = models.ForeignKey(Equipment, on_delete=models.CASCADE, verbose_name="预约设备")
    eDate = models.DateField(verbose_name="预约时间")
    eBegin = models.IntegerField(verbose_name="开始时间段")
    eEnd = models.IntegerField(verbose_name="结束时间段")
    STATE_CHOICES = (
                        ('待审核', '待审核'),
                        ('审核通过', '审核通过'),
                        ('审核未通过', '审核未通过'),
                        ('审核过期', '审核过期'),
                    )
    eState = models.CharField(max_length=6, verbose_name="状态", choices=STATE_CHOICES, default="待审核")
    PAYMENT_METHOD_CHOICES = (
                                ('经费卡', '经费卡'),
                                ('自费', '自费'),
                            )
    director = models.CharField(max_length=20, verbose_name="指导教师")
    paymentMethod = models.CharField(max_length=4, verbose_name="支付方式", choices=PAYMENT_METHOD_CHOICES, default="经费卡")
    projectName = models.CharField(max_length=10, verbose_name="项目名称", null=True,blank=True)
    projectCode = models.CharField(max_length=10, verbose_name="项目代码", null=True,blank=True)
    PROJECT_TYPE_CHOICES = (
                            ('科研', '科研'),
                            ('非科研', '非科研'),
                        )
    projectType = models.CharField(max_length=10, verbose_name="项目类型", choices=PROJECT_TYPE_CHOICES, default="科研")
    experimentContent = models.CharField(max_length=1000, verbose_name="实验内容", null=True,blank=True)
    experimentNote = models.CharField(max_length=1000, verbose_name="实验说明", null=True,blank=True)

    def __str__(self):
        return self.equipment.eName

class Drug(models.Model):
    drugName = models.CharField(max_length=20, verbose_name="设备名称")
    drugCAS = models.CharField(max_length=20, verbose_name="设备编号")
    drugUnit = models.CharField(max_length=20, verbose_name="单位", choices=DRUG_UNIT_CHOICES)
    drugAmount = models.FloatField(verbose_name="数量")
    drugSafetyDegree = models.CharField(max_length=20, verbose_name="危险性", choices= DRUG_SAFETY_DEGREE_CHOICES, default="一般药品")

class DrugCheck(models.Model):
    student = models.CharField(max_length=20, verbose_name="登记人员")
    teacher = models.CharField(max_length=20, verbose_name="审核人员",null=True,blank=True)
    FUNCTION_TYPE_CHOICES = (
                        ('设备入库', '设备入库'),
                        ('设备使用', '设备使用'),
                    )
    function = models.CharField(max_length=20, verbose_name="功能", choices=FUNCTION_TYPE_CHOICES, default="设备使用")
    drugName = models.CharField(max_length=20, verbose_name="设备名称")
    drugCAS = models.CharField(max_length=20, verbose_name="设备编号")
    drugUnit = models.CharField(max_length=20, verbose_name="计量单位", choices=DRUG_UNIT_CHOICES)
    drugAmount = models.FloatField(verbose_name="数量")
    drugSafetyDegree = models.CharField(max_length=20, verbose_name="危险性", choices= DRUG_SAFETY_DEGREE_CHOICES, default="一般药品")
    date = models.DateField(verbose_name="登记时间")
    STATE_CHOICES = (
                        ('待审核', '待审核'),
                        ('审核通过', '审核通过'),
                        ('审核未通过', '审核未通过'),
                        ('审核过期', '审核过期'),
                    )
    state = models.CharField(max_length=6, verbose_name="状态", choices=STATE_CHOICES, default="待审核")
    note = models.CharField(max_length=1000, verbose_name="备注",null=True,blank=True)

