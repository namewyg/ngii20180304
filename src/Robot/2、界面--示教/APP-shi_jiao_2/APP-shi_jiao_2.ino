#include <AccelStepper.h>  
#include <stdio.h>
#include <SPI.h>
#include <SD.h>
#include <string.h>

File myFile;  // 创建File实例化对象

int line_count = 0;
String order = "" ; char next;  int cishu = 1;
char order_flag;
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


double joint1_last=0;  double joint2_last=0;  double joint3_last=0;  
double joint4_last=0;  double joint5_last=0;  double joint6_last=0;

double joint1_new=0;   double joint2_new=0;   double joint3_new=0;
double joint4_new=0;   double joint5_new=0;   double joint6_new=0;

double delta_joint1;   double delta_joint2;   double delta_joint3;
double delta_joint4;   double delta_joint5;   double delta_joint6;

double joint1_F=0;   double joint2_F=0;   double joint3_F=0;
double joint4_F=0;   double joint5_F=0;   double joint6_F=0;

void setup() {
  
   Serial.begin(115200);
   while (!Serial)
        {   }  ; //等待串口打开，注：空语句，空循环，小技巧，学习！
  
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

    if (!SD.begin())
     {
        return;
     }

}


void loop() {

  while (Serial.available()>0){
     order_flag = char(Serial.read());
     cishu = 1;   //非常重要
     int Flag = 1;
     
     if (order_flag == 'Q'){  //启动，代表实物启动
            stepper2.moveTo(26000);
                 
            stepper3.moveTo(26000);
            
            stepper5.moveTo(-500); 
            
            stepper3.moveTo(45000);
        
           while ((stepper2.distanceToGo() != 0 ) ||  (stepper3.distanceToGo() != 0) || (stepper5.distanceToGo() != 0 ) ) {     
                stepper2.run();
                stepper3.run();
                stepper5.run();   
               }
            ///////各关节已运动至零位，将此位置设置为关节坐标系的零点////////////////      
           stepper1.setCurrentPosition(0);   //第一关节准备就绪
           stepper2.setCurrentPosition(0);   //第二关节准备就绪
           stepper3.setCurrentPosition(0);   //第三关节准备就绪
           stepper4.setCurrentPosition(0);   //第四关节准备就绪
           stepper5.setCurrentPosition(0);   //第五关节准备就绪；
           stepper6.setCurrentPosition(0);   //第六关节准备就绪；
      }
//////////////////////////////////////////////////////////////////////////////////////////////     
    
     if ( order_flag == 'F'){  //复位，代表实物复位
           //////回零位，,注：moveTo是绝对坐标系/////////////////////////////////////////////////////
            //先根据SD卡最后一行数据，计算当前个关节角度（以脉冲数表示），然后方向反过来(其中三、五、六关节仿真与实物相反，故不变)，直接用moveTo实现
            int Step1_F = (joint1_F*180/3.14)/300*29000;
            int Step2_F = (joint2_F*180/3.14)/120*25700;
            int Step3_F = (joint3_F*180/3.14)/120*26000;
            int Step4_F = (joint4_F*180/3.14)/360*16576;
            int Step5_F = (joint5_F*180/3.14)/210*9700;
            int Step6_F = (joint6_F*180/3.14)/360*16576;

            Serial.println(Step1_F);
            Serial.println(Step2_F);
            Serial.println(Step3_F);
            Serial.println(Step4_F);
            Serial.println(Step5_F);
            Serial.println(Step6_F);

            
            stepper1.move(-Step1_F);
            stepper2.move(-Step2_F);
            stepper3.move(Step3_F);
            stepper4.move(-Step4_F);
            stepper5.move(Step5_F);
            stepper6.move(Step6_F);

            
            while ( (stepper1.distanceToGo() != 0 ) || (stepper2.distanceToGo() != 0 ) ||  (stepper3.distanceToGo() != 0) || (stepper4.distanceToGo() != 0 ) ||(stepper5.distanceToGo() != 0 ) || (stepper6.distanceToGo() != 0 )  ) {     
                  stepper1.run();
                  stepper2.run();
                  stepper3.run();
                  stepper4.run();
                  stepper5.run(); 
                  stepper6.run();  
            }
          Serial.println("复位");
 
      } 
///////////////////////////////////////////////////////////////////////////////////////////////////////
            
     if ( order_flag == 'S' ){  
          //Start,代表实物动作指令,因为不管是直线、圆弧，还是示教，
          //区别只是SD卡的数据，而arduino控制电机的方法都一样，故将他们合并为一个命令“S”
          String Data[6]; //数组A用于分批次读取SD卡
      
          if (!SD.begin())   // 检测是否初始化完成
           {
              Serial.println("initialization failed!");
              return;
           }
          Serial.println("initialization done.");  
          
         ///////数组Data读取SD卡
         myFile = SD.open("ZQ2.txt"); 
         if (myFile)
         {
          int b;  String data;  //int Flag = 1; 
           //////////数组A读SD卡部分/////////
           while ( myFile.available() ) {  //直接用这个作为大的循环，而非for（固定循环次数）
                                         //或者直接用个大的数组，把SD卡前全部读进来，但不现实，因为数据量会变得很大，而且也违背了你的分批读取原则，故不予采纳 
             char next=myFile.read();
             if (next!=-1){       //"-1"代表文末的空字节
                if(!isWhitespace(next) and next!='\n' ){
                   data+=next; 
                 }
                if (isWhitespace(next)){   //判断是否为空格符，经测试if(next==' ')行不通
                  Data[b]=data; 
                  data="";
                  b=b+1;  
                }
                
               if (next=='\n'){  //到这说明已读完一行，则直接先去驱动电机，然后再读下一行
                  Data[b]=data;
                  b = 0;  data="";  //为下一行做准备
                  line_count = line_count + 1;  //行数+1
                  
                  Serial.println("输出数组Data：");  //只是一行
                  for (int i=0;i<6;i++){
                    Serial.print(Data[i]);
                    Serial.print(" ");
                    }
                  Serial.println("");


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
                  //每次循环都归零
                 int count1=0;   int count2=0;   int count3=0;  int count4=0;   int count5=0;   int count6=0;             
                //这部分用于驱动六个电机，那一大段程序，直接结合以前各文件，略作修改即可,以角度的方式进行驱动
                joint1_new = atof(Data[0].c_str());  //将String转换为double
                joint2_new = atof(Data[1].c_str());  //将String转换为double
                joint3_new = atof(Data[2].c_str());  //将String转换为double
                joint4_new = atof(Data[3].c_str());  //将String转换为double
                joint5_new = atof(Data[4].c_str());  //将String转换为double
                joint6_new = atof(Data[5].c_str());  //将String转换为double
    
                /////第一行（轨迹起点）用AccelStepper库进行处理，使机械手先运动至轨迹起点，与其他行的循环发脉冲不同
                if (Flag == 1 or Flag == 2 ){  //代表第一行，附:这里不知为何，SD卡第一行读不到，时间紧急，出此下策，以第二行代替第一行
                    joint1_last = joint1_new  ;
                    joint2_last = joint2_new  ;
                    joint3_last = joint3_new  ;
                    joint4_last = joint4_new  ;
                    joint5_last = joint5_new  ;
                    joint6_last = joint6_new  ;
                    Flag = Flag + 1 ;
    
                    int Step1_begin = (joint1_new*180/3.14)/300*29000;
                    int Step2_begin = (joint2_new*180/3.14)/120*25700;
                    int Step3_begin = (joint3_new*180/3.14)/120*26000;
                    int Step4_begin = (joint4_new*180/3.14)/360*16576;
                    int Step5_begin = (joint5_new*180/3.14)/210*9700;
                    int Step6_begin = (joint6_new*180/3.14)/360*16576;
    
                    //////先从零位动作至轨迹起点/////////////////////////////////////////////////////
                    stepper1.moveTo(Step1_begin);
                    stepper2.moveTo(Step2_begin);
                    stepper3.moveTo(-Step3_begin);
                    stepper4.moveTo(Step4_begin);
                    stepper5.moveTo(-Step5_begin);
                    stepper6.moveTo(Step6_begin);
                    while ( (stepper1.distanceToGo() != 0 ) || (stepper2.distanceToGo() != 0 ) ||  (stepper3.distanceToGo() != 0) || (stepper4.distanceToGo() != 0 ) ||(stepper5.distanceToGo() != 0 ) || (stepper6.distanceToGo() != 0 )  ) {     
                          stepper1.run();
                          stepper2.run();
                          stepper3.run();
                          stepper4.run();
                          stepper5.run(); 
                          stepper6.run();  
                         }
                    ////停顿1.5秒
                    delay(1500);
                }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
                //其他行的处理方式：
                ////下一步，求差值
                  delta_joint1 = joint1_new - joint1_last ;
                  delta_joint2 = joint2_new - joint2_last ;
                  delta_joint3 = joint3_new - joint3_last ;
                  delta_joint4 = joint4_new - joint4_last ;
                  delta_joint5 = joint5_new - joint5_last ;
                  delta_joint6 = joint6_new - joint6_last ;
                //将角度差值转换为脉冲数，以备驱动电机
                /////注意，MATLAB逆解后的角度是按180°=pi=3.14来算的，故要转换
                 int Step1 = (delta_joint1*180/3.14)/300*29000;
                 int Step2 = (delta_joint2*180/3.14)/120*25700; //
                 int Step3 = (delta_joint3*180/3.14)/120*26000;
                 int Step4 = (delta_joint4*180/3.14)/360*16576;
                 int Step5 = (delta_joint5*180/3.14)/210*9700;
                 int Step6 = (delta_joint6*180/3.14)/360*16576;
               ///////////////////////////////////////////////////////////////////////////////////////////
                  //定义电机转向，并发脉冲驱动电机
                ///////////判断电机转动方向
                  if (Step1>0) {
                    digitalWrite(motor1dirPin,HIGH);  }
                  else {
                    digitalWrite(motor1dirPin,LOW);
                  }
            
                  if (Step2>0) {
                    digitalWrite(motor2dirPin,HIGH);  }
                  else {
                    digitalWrite(motor2dirPin,LOW);
                  }
            
                  if (Step3>0) {
                    digitalWrite(motor3dirPin,LOW);  }
                  else {
                    digitalWrite(motor3dirPin,HIGH);
                  }
            
                  if (Step4>0) {
                    digitalWrite(motor4dirPin,HIGH);  }
                  else {
                    digitalWrite(motor4dirPin,LOW);
                  }
            
                  if (Step5>0) {
                    digitalWrite(motor5dirPin,LOW);  }
                  else {
                    digitalWrite(motor5dirPin,HIGH);
                  }
            
                  if (Step6>0) {
                    digitalWrite(motor6dirPin,HIGH);  }
                  else {
                    digitalWrite(motor6dirPin,LOW);
                  }
            
                  ////////////////发脉冲，驱动各关节
                 while ((count1 != abs(Step1)) || (count2 != abs(Step2)) || (count3 != abs(Step3)) || (count4 != abs(Step4)) || (count5 != abs(Step5)) || (count6 !=abs(Step6)) )  {
    //              while ( (count1 != abs(Step1))  )  {
                      if (count1 != abs(Step1)) {
                          digitalWrite(motor1stepPin,HIGH);
                          delayMicroseconds(1);
                          digitalWrite(motor1stepPin,LOW);
                          delayMicroseconds(300);   //越小，速度越快
                          count1=count1+1;
                       }
            
                      if (count2 != abs(Step2)) {
                          digitalWrite(motor2stepPin,HIGH);
                          delayMicroseconds(1);
                          digitalWrite(motor2stepPin,LOW);
                          delayMicroseconds(100);   //越小，速度越快
                          count2=count2+1;
                       }
            
                      if (count3 != abs(Step3)) {
                          digitalWrite(motor3stepPin,HIGH);
                          delayMicroseconds(1);
                          digitalWrite(motor3stepPin,LOW);
                          delayMicroseconds(100);   //越小，速度越快
                          count3=count3+1;
                       }
            
                      if (count4 != abs(Step4)) {
                          digitalWrite(motor4stepPin,HIGH);
                          delayMicroseconds(1);
                          digitalWrite(motor4stepPin,LOW);
                          delayMicroseconds(100);   //越小，速度越快
                          count4=count4+1;
                       }
            
                     if (count5 != abs(Step5)) {
                          digitalWrite(motor5stepPin,HIGH);
                          delayMicroseconds(1);
                          digitalWrite(motor5stepPin,LOW);
                          delayMicroseconds(100);   //越小，速度越快
                          count5=count5+1;
                       }
            
                      if (count6 != abs(Step6)) {
                          digitalWrite(motor6stepPin,HIGH);
                          delayMicroseconds(1);
                          digitalWrite(motor6stepPin,LOW);
                          delayMicroseconds(100);
                          count6=count6+1;
                      }
                  }
                  ////////至此，已驱动电机走完一行角度数据，准备掉回头继续读下一行
                    joint1_last = joint1_new  ;
                    joint2_last = joint2_new  ;
                    joint3_last = joint3_new  ;
                    joint4_last = joint4_new  ;
                    joint5_last = joint5_new  ;
                    joint6_last = joint6_new  ;
                 
                  }
               }
            }
           /////至此已读完最后一行
          joint1_F = joint1_new  ;
          joint2_F = joint2_new  ;
          joint3_F = joint3_new  ;
          joint4_F = joint4_new  ;
          joint5_F = joint5_new  ;
          joint6_F = joint6_new  ;
         }
         myFile.close(); //执行完后关闭文件
     }

//////////////////////////////////////////////////////////////////////////////////////////////        
    
     if ( (order_flag != 'Q') and (order_flag != 'F') and (order_flag != 'S')and (order_flag != '痧')  ) {
          //如果都不是以上命令符，那就是进行读数据并写入SD卡
          myFile = SD.open("ZQ2.txt", FILE_WRITE);
          if (myFile)
          {
             while (Serial.available()>0){
                 if (cishu == 1){  //非常重要
                      next =  order_flag;
                      cishu = 2;
                 }
                 else {
                      next = char(Serial.read()); 
                 }
                 
                 if (next!=-1){       //"-1"代表空字节
                      myFile.print(next);
                      delayMicroseconds(100);
                 }
             }
            myFile.close();  //若再出现写不进SD卡的情况，原因是少了这条语句，至于为什么我不清楚
          } 
      }
  }


  }


    
