#include <AccelStepper.h>  
#include <stdio.h>
#include <SPI.h>
#include <SD.h>
#include <string.h>

 //定义电机控制用常量
const int motor1dirPin = 2;     // 第一关节电机方向
const int motor1stepPin = 3;    // 第一关节电机步进控制引脚
const int motor2dirPin = 4;     // 第二关节电机方向控制引脚
const int motor2stepPin = 5;    // 第二关节电机步进控制引脚   
const int motor3dirPin = 6;     // 第三关节电机方向控制引脚
const int motor3stepPin = 7;    // 第三关节电机步进控制引脚
const int motor4dirPin = 8;     // 第四关节电机方向控制引脚
const int motor4stepPin = 9;    // 第四关节电机步进控制引脚
const int motor5dirPin = 10;    // 第五关节电机方向控制引脚
const int motor5stepPin =11;    // 第五关节电机步进控制引脚  
const int motor6dirPin = 12;    // 第六关节电机方向控制引脚
const int motor6stepPin = 13;   // 第六关节电机步进控制引脚       
 
AccelStepper stepper1(1,motor1stepPin,motor1dirPin);//建立第一关节步进电机对象1
AccelStepper stepper2(1,motor2stepPin,motor2dirPin);//建立第二关节步进电机对象2
AccelStepper stepper3(1,motor3stepPin,motor3dirPin);//建立第三关节步进电机对象3
AccelStepper stepper4(1,motor4stepPin,motor4dirPin);//建立第四关节步进电机对象4
AccelStepper stepper5(1,motor5stepPin,motor5dirPin);//建立第五关节步进电机对象5
AccelStepper stepper6(1,motor6stepPin,motor6dirPin);//建立第六关节步进电机对象6

void setup() {
  
  pinMode(motor1dirPin,OUTPUT);     // Arduino控制步进引脚为输出模式
  pinMode(motor1stepPin,OUTPUT);    // Arduino控制方向引脚为输出模式
  pinMode(motor2dirPin,OUTPUT);     // Arduino控制步进引脚为输出模式
  pinMode(motor2stepPin,OUTPUT);      // Arduino控制方向引脚为输出模式
  pinMode(motor3dirPin,OUTPUT);     // Arduino控制步进引脚为输出模式
  pinMode(motor3stepPin,OUTPUT);  // Arduino控制方向引脚为输出模式
  pinMode(motor4dirPin,OUTPUT);     // Arduino控制步进引脚为输出模式
  pinMode(motor4stepPin,OUTPUT);      // Arduino控制方向引脚为输出模式  
  pinMode(motor5dirPin,OUTPUT);
  pinMode(motor5stepPin,OUTPUT);
  pinMode(motor6dirPin,OUTPUT);
  pinMode(motor6stepPin,OUTPUT);
  
  stepper1.setMaxSpeed(20000);
  stepper1.setSpeed(16000);
  stepper1.setAcceleration(12000);
  
  stepper2.setMaxSpeed(20000);
  stepper2.setSpeed(16000);
  stepper2.setAcceleration(12000);
  
  stepper3.setMaxSpeed(20000);
  stepper3.setSpeed(16000);
  stepper3.setAcceleration(12000);
  
  stepper4.setMaxSpeed(20000);
  stepper4.setSpeed(16000);
  stepper4.setAcceleration(12000);
  
  stepper5.setMaxSpeed(20000);
  stepper5.setSpeed(16000);
  stepper5.setAcceleration(12000);
  
  stepper6.setMaxSpeed(20000);
  stepper6.setSpeed(16000);
  stepper6.setAcceleration(12000);
  
//////////////////////////////////////////////////////////////////////////////////////////
/////////第一步 先从未通电的自然状态运动至零位/////////////////////////////////////////////////////

    stepper1.moveTo(2600);
 stepper4.moveTo(2600);

    stepper2.moveTo(26000);
    stepper5.moveTo(2600);

     stepper3.moveTo(2600);
     stepper6.moveTo(26000.);

   while ((stepper2.distanceToGo() != 0 ) ||  (stepper3.distanceToGo() != 0) || (stepper5.distanceToGo() != 0 )||  (stepper4.distanceToGo() != 0) || (stepper6.distanceToGo() != 0 ) ) {     
             
           stepper1.run();
          stepper2.run();
          stepper3.run();
            stepper4.run();
         stepper5.run();
          stepper6.run();      
       }

       
    ///////各关节已运动至零位，将此位置设置为关节坐标系的零点////////////////      
   stepper1.setCurrentPosition(0);   //第一关节准备就绪
   stepper2.setCurrentPosition(0);   //第二关节准备就绪
   stepper3.setCurrentPosition(0);   //第三关节准备就绪
   stepper4.setCurrentPosition(0);   //第四关节准备就绪
   stepper5.setCurrentPosition(0);   //第五关节准备就绪；
   stepper6.setCurrentPosition(0);   //第六关节准备就绪；

///////停顿1秒
   delay(1000);

/////////////////////////////////////////////////////////
///////读SD卡中的关节角
//////暂定只建一个数组A
    String A[6]; //数组A用于读取SD卡轨迹起点的坐标，即第一行，arduino中数组是从0开始算,6代表6个数组元素
   
    Serial.begin(115200);
    while (!Serial)
    {   }  ; //等待串口打开，注：空语句，空循环，小技巧，学习！
    Serial.print("Initializing SD card...");
    if (!SD.begin())   // 检测是否初始化完成
     {
        Serial.println("initialization failed!");
        return;
     }
    Serial.println("SD initialization done.");  

   File myFile;  // 创建File实例化对象
   myFile = SD.open("myFile.txt",FILE_READ); 
   if (myFile)
   {
   Serial.println("SD myFile already have opened"); 
   int a=0 ;   
   String data;  int p=0;  

     //////////数组A读SD卡部分/////////
     while (a<6)  //数组从0开始算
        {  
         char next=myFile.read();
         
         if (next!=-1){       //"-1"代表文末的空字节
            if (isWhitespace(next) || next=='\n' ){   //判断是否为空格符或换行符，经测试if(next==' ')行不通
              A[a]=data; 
              data=""; 
              a=a+1;  
            }
            else {
              data+=next;  //这是关键，因为read（）读出来的是一个字符，即数字1或者符号.
            }  
        }
    }

///////读取轨迹起点关节坐标完毕，关闭SD卡文件
    myFile.close();
   }
//////////////////////////////////////////////////////////////////
    Serial.println("///////////输出数组A:"); 
    for (int a=0; a<6; a++){
        Serial.print(A[a]);  
        Serial.print(" "); 
    }  
      Serial.println(""); 

//////////将数组A（起点）的关节角转化为驱动步进电机的脉冲数
     double joint1=atof(A[0].c_str());  //将String转换为double
     double joint2=atof(A[1].c_str());  //atof()返回float型，即小数点后六位  
     double joint3=atof(A[2].c_str());
     double joint4=atof(A[3].c_str());
     double joint5=atof(A[4].c_str());
     double joint6=atof(A[5].c_str());
        
////////注意，MATLAB逆解后的角度是按180°=pi=3.14来算的，故要转换
     int step1_first = (joint1*180/3.14)/300*29000;
     int step2_first = (joint2*180/3.14)/120*26000;
     int step3_first = (joint3*180/3.14)/120*25000;
     int step4_first = (joint4*180/3.14)/360*16576;
     int step5_first = (joint5*180/3.14)/210*9700;
     int step6_first = (joint6*180/3.14)/360*16576;
  
//驱动电机开始转动，其中moveTo()和runToNewPosition()都是绝对坐标,runToNewPosition()会暂停后面程序的运行
//注：经观察2/3/5关节，MATLAB里的角度方向是顺为负，逆为正，则推断都是如此，则3和4关节与我所定义（根据程序脉冲正负标定）相反，故要加个负号

////////////转换完成，启动机械手，由零位运行至轨迹起点/////////////////
      stepper1.moveTo(step1_first );
      stepper2.moveTo(step2_first );
      stepper3.moveTo(-step3_first );
      stepper4.moveTo(-step4_first );
      stepper5.moveTo(step5_first );
      stepper6.moveTo(step6_first );

      while (stepper1.distanceToGo()  != 0 or stepper2.distanceToGo()  != 0 or stepper3.distanceToGo()  != 0 or stepper4.distanceToGo()  != 0 or stepper5.distanceToGo()  != 0 or stepper6.distanceToGo()  != 0 ) {   
            stepper1.run();  
            stepper2.run();
            stepper3.run();
            stepper4.run();

            stepper5.run();
            stepper6.run();             
       } 
//////////////////////////////////////////////////////////////////////////////////////////
//////////至此已运动至轨迹起点位置，停顿0.2秒/////////////////////////////////////////////////////
        delay(2000);

//////下一步，转换个关节速度（脉冲数）与时间nt表达式，运行轨迹
///////////各电机delta T 时间内的脉冲数Step与时间nT的拟合曲线（四次多项式）
  // delta T=0.01s,10ms，总运行时间10s

  int n;
  
  double A4_1=0.0003528;   double A3_1=-0.007056;   double A2_1=0.03528;   double A1_1=0;   double A0_1=0;
  int Step1_n=A4_1*pow(10,-10)*5538.7553*pow(n,4)+A3_1*pow(10,-8)*5538.7553*pow(n,3)+A2_1*pow(10,-6)*5538.7553*pow(n,2)+A1_1*pow(10,-4)*5538.7553*pow(n,1)+A0_1*pow(10,-2)*5538.7553*pow(n,0);
  
  double A4_2=0.000090984;   double A3_2=-0.0018197;   double A2_2=0.0090984;   double A1_2=0;   double A0_2=0;
  int Step2_n=A4_2*pow(10,-10)*12414.4517*pow(n,4)+A3_2*pow(10,-8)*12414.4517*pow(n,3)+A2_2*pow(10,-6)*12414.4517*pow(n,2)+A1_2*pow(10,-4)*12414.4517*pow(n,1)+A0_2*pow(10,-2)*12414.4517*pow(n,0);
  
  double A4_3=0.00010472;   double A3_3=-0.0020945;   double A2_3=0.010472;   double A1_3=0;   double A0_3=0;
  int Step3_n=A4_3*pow(10,-10)*11936.9728*pow(n,4)+A3_3*pow(10,-8)*11936.9728*pow(n,3)+A2_3*pow(10,-6)*11936.9728*pow(n,2)+A1_3*pow(10,-4)*11936.9728*pow(n,1)+A0_3*pow(10,-2)*11936.9728*pow(n,0);
  
  double A4_4=0;   double A3_4=0;   double A2_4=0;   double A1_4=0;   double A0_4=0;
  int Step4_n=A4_4*pow(10,-10)*2638.2301*pow(n,4)+A3_4*pow(10,-8)*2638.2301*pow(n,3)+A2_4*pow(10,-6)*2638.2301*pow(n,2)+A1_4*pow(10,-4)*2638.2301*pow(n,1)+A0_4*pow(10,-2)*2638.2301*pow(n,0);
  
  double A4_5=-0.00019571;   double A3_5=0.0039142;   double A2_5=-0.019571;   double A1_5=0;   double A0_5=0;
  int Step5_n=A4_5*pow(10,-10)*2646.5974*pow(n,4)+A3_5*pow(10,-8)*2646.5974*pow(n,3)+A2_5*pow(10,-6)*2646.5974*pow(n,2)+A1_5*pow(10,-4)*2646.5974*pow(n,1)+A0_5*pow(10,-2)*2646.5974*pow(n,0);
  
  double A4_6=-0.0003528;   double A3_6=0.007056;   double A2_6=-0.03528;   double A1_6=0;   double A0_6=0;
  int Step6_n=A4_6*pow(10,-10)*2639.49*pow(n,4)+A3_6*pow(10,-8)*2639.49*pow(n,3)+A2_6*pow(10,-6)*2639.49*pow(n,2)+A1_6*pow(10,-4)*2639.49*pow(n,1)+A0_6*pow(10,-2)*2639.49*pow(n,0);

  for (int n=0;n<1001; n++) {
    
    int count1=0;   int count2=0;   int count3=0;  int count4=0;   int count5=0;   int count6=0;
    
    Step1_n=A4_1*pow(10,-10)*5538.7553*pow(n,4)+A3_1*pow(10,-8)*5538.7553*pow(n,3)+A2_1*pow(10,-6)*5538.7553*pow(n,2)+A1_1*pow(10,-4)*5538.7553*pow(n,1)+A0_1*pow(10,-2)*5538.7553*pow(n,0);
    //Serial.println(Step1_n);

    Step2_n=A4_2*pow(10,-10)*12414.4517*pow(n,4)+A3_2*pow(10,-8)*12414.4517*pow(n,3)+A2_2*pow(10,-6)*12414.4517*pow(n,2)+A1_2*pow(10,-4)*12414.4517*pow(n,1)+A0_2*pow(10,-2)*12414.4517*pow(n,0);
   // Serial.println(Step2_n);

    Step3_n=A4_3*pow(10,-10)*11936.9728*pow(n,4)+A3_3*pow(10,-8)*11936.9728*pow(n,3)+A2_3*pow(10,-6)*11936.9728*pow(n,2)+A1_3*pow(10,-4)*11936.9728*pow(n,1)+A0_3*pow(10,-2)*11936.9728*pow(n,0);
  //  Serial.println(Step3_n);

    Step4_n=A4_4*pow(10,-10)*2638.2301*pow(n,4)+A3_4*pow(10,-8)*2638.2301*pow(n,3)+A2_4*pow(10,-6)*2638.2301*pow(n,2)+A1_4*pow(10,-4)*2638.2301*pow(n,1)+A0_4*pow(10,-2)*2638.2301*pow(n,0);
   // Serial.println(Step4_n);

    Step5_n=A4_5*pow(10,-10)*2646.5974*pow(n,4)+A3_5*pow(10,-8)*2646.5974*pow(n,3)+A2_5*pow(10,-6)*2646.5974*pow(n,2)+A1_5*pow(10,-4)*2646.5974*pow(n,1)+A0_5*pow(10,-2)*2646.5974*pow(n,0);
   // Serial.println(Step5_n);
    
    Step6_n=A4_6*pow(10,-10)*2639.49*pow(n,4)+A3_6*pow(10,-8)*2639.49*pow(n,3)+A2_6*pow(10,-6)*2639.49*pow(n,2)+A1_6*pow(10,-4)*2639.49*pow(n,1)+A0_6*pow(10,-2)*2639.49*pow(n,0);
    Serial.println(Step6_n);

    ///////////判断电机转动方向
    
      if (Step1_n>0) {
        digitalWrite(motor1dirPin,HIGH);  }
      else {
        digitalWrite(motor1dirPin,LOW);
      }

      if (Step2_n>0) {
        digitalWrite(motor2dirPin,HIGH);  }
      else {
        digitalWrite(motor2dirPin,LOW);
      }

      if (Step3_n>0) {
        digitalWrite(motor3dirPin,LOW);  }
      else {
        digitalWrite(motor3dirPin,HIGH);
      }

      if (Step4_n>0) {
        digitalWrite(motor4dirPin,LOW);  }
      else {
        digitalWrite(motor4dirPin,HIGH);
      }

      if (Step5_n>0) {
        digitalWrite(motor5dirPin,LOW);  }
      else {
        digitalWrite(motor5dirPin,HIGH);
      }

      if (Step6_n>0) {
        digitalWrite(motor6dirPin,LOW);  }
      else {
        digitalWrite(motor6dirPin,HIGH);
      }

      ////////////////发脉冲，驱动各关节
     while ((count1 != abs(Step1_n)) || (count2 != abs(Step2_n)) || (count3 != abs(Step3_n)) || (count4 != abs(Step4_n)) || (count5 != abs(Step5_n)) || (count6 !=abs(Step6_n)) )  {
     // while ((count1 != abs(Step1_n))||(count2 != abs(Step2_n)) || (count4 != abs(Step4_n)) || (count6 != abs(Step6_n)) )  {

          if (count1 != abs(Step1_n)) {
              digitalWrite(motor1stepPin,HIGH);
              //delayMicroseconds(int(50000/abs(Step1_n)));//不均分。关节2延时2，动作平滑
              digitalWrite(motor1stepPin,LOW);
              delayMicroseconds(50);   //越小，速度越快
            // delayMicroseconds(int(50000/abs(Step1_n)));
              count1=count1+1;
           }

          if (count2 != abs(Step2_n)) {
              digitalWrite(motor2stepPin,HIGH);
             // delayMicroseconds(int(50000/abs(Step2_n)));//不均分。关节2延时2，动作平滑
              digitalWrite(motor2stepPin,LOW);
             // delayMicroseconds(int(50000/abs(Step2_n)));
              count2=count2+1;
           }

          if (count3 != abs(Step3_n)) {
              digitalWrite(motor3stepPin,HIGH);
             // delayMicroseconds(int(50000/abs(Step3_n)));//不均分。关节2延时2，动作平滑
              digitalWrite(motor3stepPin,LOW);
             // delayMicroseconds(int(50000/abs(Step3_n)));
              count3=count3+1;
           }

          if (count4 != abs(Step4_n)) {
              digitalWrite(motor4stepPin,HIGH);
             // delayMicroseconds(int(50000/abs(Step4_n)));//不均分。关节2延时2，动作平滑
              digitalWrite(motor4stepPin,LOW);
              // delayMicroseconds(3000);
             // delayMicroseconds(int(50000/abs(Step4_n)));
              count4=count4+1;
           }

         if (count5 != abs(Step5_n)) {
              digitalWrite(motor5stepPin,HIGH);
             // delayMicroseconds(int(50000/abs(Step5_n)));//不均分。关节2延时2，动作平滑
              digitalWrite(motor5stepPin,LOW);
             // delayMicroseconds(int(50000/abs(Step5_n)));
              count5=count5+1;
           }

          if (count6 != abs(Step6_n)) {
              digitalWrite(motor6stepPin,HIGH);
              //delayMicroseconds(int(50000/abs(Step6_n)));//不均分
              digitalWrite(motor6stepPin,LOW);
             // delayMicroseconds(int(50000/abs(Step6_n)));
              count6=count6+1;
          }
      }
    }
 }
  

void loop() {   }
