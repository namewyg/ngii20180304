#include <SPI.h>
#include <SD.h>
File myFile;  // 创建File实例化对象
char next;  int a=0; 
String data=""; String Data[150]; 
int Flag = 0; //标识符，串口有数据来时才驱动机械手

//定义电机控制用常量
const int motor1dirPin =2 ;     // 第一关节电机方向
const int motor1stepPin = 3;    // 第一关节电机步进控制引脚
const int motor2dirPin = 4;     // 第二关节电机方向控制引脚
const int motor2stepPin = 5;    // 第二关节电机步进控制引脚
const int motor3dirPin = 6;     // 第三关节电机方向控制引脚
const int motor3stepPin = 7;    // 第三关节电机步进控制引脚
const int motor4dirPin = 8;     // 第四关节电机方向控制引脚
const int motor4stepPin = 9;     // 第四关节电机方向控制引脚
const int motor5dirPin = 10;    // 第五关节电机方向控制引脚
const int motor5stepPin =11;    // 第五关节电机步进控制引脚 
const int motor6dirPin =12;    // 第六关节电机方向控制引脚
const int motor6stepPin = 13;   // 第六关节电机步进控制引脚  


int n;
///////////各电机delta T 时间内的脉冲数Step与时间nT的拟合曲线（四次多项式）
double A4_1;   double A3_1;   double A2_1;   double A1_1;   double A0_1;  
int Step1_n = A4_1*pow(10,-10)*5538.7553*pow(n,4)+A3_1*pow(10,-8)*5538.7553*pow(n,3)+A2_1*pow(10,-6)*5538.7553*pow(n,2)+A1_1*pow(10,-4)*5538.7553*pow(n,1)+A0_1*pow(10,-2)*5538.7553*pow(n,0) ;

double A4_2;   double A3_2;   double A2_2;   double A1_2;   double A0_2;
int Step2_n = A4_2*pow(10,-10)*12414.4517*pow(n,4)+A3_2*pow(10,-8)*12414.4517*pow(n,3)+A2_2*pow(10,-6)*12414.4517*pow(n,2)+A1_2*pow(10,-4)*12414.4517*pow(n,1)+A0_2*pow(10,-2)*12414.4517*pow(n,0) ;

double A4_3;   double A3_3;   double A2_3;   double A1_3;   double A0_3;
int Step3_n = A4_3*pow(10,-10)*11936.9728*pow(n,4)+A3_3*pow(10,-8)*11936.9728*pow(n,3)+A2_3*pow(10,-6)*11936.9728*pow(n,2)+A1_3*pow(10,-4)*11936.9728*pow(n,1)+A0_3*pow(10,-2)*11936.9728*pow(n,0) ;

double A5_4;   double A4_4;   double A3_4;   double A2_4;   double A1_4;   double A0_4;
int Step4_n = A4_4*pow(10,-10)*2638.2301*pow(n,4)+A3_4*pow(10,-8)*2638.2301*pow(n,3)+A2_4*pow(10,-6)*2638.2301*pow(n,2)+A1_4*pow(10,-4)*2638.2301*pow(n,1)+A0_4*pow(10,-2)*2638.2301*pow(n,0) ;

double A4_5;   double A3_5;   double A2_5;   double A1_5;   double A0_5;
int Step5_n = A3_5*pow(10,-8)*2646.5974*pow(n,3)+A2_5*pow(10,-6)*2646.5974*pow(n,2)+A1_5*pow(10,-4)*2646.5974*pow(n,1)+A0_5*pow(10,-2)*2646.5974*pow(n,0) ;

double A4_6;   double A3_6;   double A2_6;   double A1_6;   double A0_6;
int Step6_n = A4_6*pow(10,-10)*2639.49*pow(n,4)+A3_6*pow(10,-8)*2639.49*pow(n,3)+A2_6*pow(10,-6)*2639.49*pow(n,2)+A1_6*pow(10,-4)*2639.49*pow(n,1)+A0_6*pow(10,-2)*2639.49*pow(n,0) ;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
    if (!SD.begin())
          {
            return;
          }
    
   
  }
  
void loop() { 
  
      myFile = SD.open("ZQ.txt", FILE_WRITE);
      if (myFile)
      {
         while (Serial.available()>0){
             next = char(Serial.read()); 
             if (next!=-1){       //"-1"代表空字节
                  myFile.print(next);
                  Serial.print(next);
                  delayMicroseconds(1);
             }
         }
        myFile.close();  //若再出现写不进SD卡的情况，原因是少了这条语句，至于为什么我不清楚
  }

        
}

//                  
//             if (!isWhitespace(next) and  next!= '\n' ){  //判断是否为空格符，经测试if(next==' ')行不通
//                  data += next; 
////                  Serial.print(next);//返回给MATLAB监视
//                  delayMicroseconds(1900);  //这个延时极其重要，若没有，则会远远偏离期望效果
//              }
//              
//             if (isWhitespace(next)){   
//                Data[a]=data; 
//                data="";
//                a=a+1; 
//                next = char(Serial.read());
//                while (isWhitespace(next))
//                   {  next = char(Serial.read()); }   //由于避免元素间多个空格对读数据造成影响，放心，寄存器中的指针一直在移动
//                data += next;   //这句是确保不会遗漏，很重要
////                Serial.print(next);//返回给MATLAB监视
//              } 
//          }   
//     }
//     
//    if (Flag == 1 ) {  //Flag作为标识符，只有当有数据传来时才执行下面的动作
//        for (int i=2; i<3; i++) {
////        A4_1 = 3.528 ;      A3_1 = -70.56 ;      A2_1 = 352.8 ;     A1_1 = 0 ;    A0_1 = 0 ;
////        A4_2 = 0.90984 ;    A3_2 = -18.197 ;     A2_2 = 90.984 ;    A1_2 = 0 ;    A0_2 = 0 ;
////        A4_3 = 1.0472 ;     A3_3 = -20.945 ;     A2_3 = 104.72 ;    A1_3 = 0 ;    A0_3 = 0 ;
////        A4_4 = 0 ;          A3_4 = 0 ;           A2_4 = 0 ;         A1_4 = 0 ;    A0_4 = 0 ;
////        A4_5 = -1.9571;     A3_5 = 39.142;       A2_5 = -195.71;    A1_5 = 0 ;    A0_5 = 0 ;
////          A4_6 = -616.5;      A3_6 = 1316.5;        A2_6 = -891.1;     A1_6 = 197.7 ;    A0_6 = -10.5 ;
//          
////        A4_1 = atof(Data[0+i*30].c_str());      A3_1 = atof(Data[1+i*30].c_str());      A2_1 = atof(Data[2+i*30].c_str());     A1_1 = atof(Data[3+i*30].c_str());      A0_1 = atof(Data[4+i*30].c_str()); 
////        A4_2 = atof(Data[5+i*30].c_str());      A3_2 = atof(Data[6+i*30].c_str());      A2_2 = atof(Data[7+i*30].c_str());     A1_2 = atof(Data[8+i*30].c_str());      A0_2 = atof(Data[9+i*30].c_str()); 
////        A4_3 = atof(Data[10+i*30].c_str());     A3_3 = atof(Data[11+i*30].c_str());     A2_3 = atof(Data[12+i*30].c_str());    A1_3 = atof(Data[13+i*30].c_str());     A0_3 = atof(Data[14+i*30].c_str()); 
////        A4_4 = atof(Data[15+i*30].c_str());     A3_4 = atof(Data[16+i*30].c_str());     A2_4 = atof(Data[17+i*30].c_str());    A1_4 = atof(Data[18+i*30].c_str());     A0_4 = atof(Data[19+i*30].c_str()); 
////        A4_5 = atof(Data[20+i*30].c_str());     A3_5 = atof(Data[21+i*30].c_str());     A2_5 = atof(Data[22+i*30].c_str());    A1_5 = atof(Data[23+i*30].c_str());     A0_5 = atof(Data[24+i*30].c_str()); 
//          A4_6 = atof(Data[25+i*30].c_str());     A3_6 = atof(Data[26+i*30].c_str());     A2_6 = atof(Data[27+i*30].c_str());    A1_6 = atof(Data[28+i*30].c_str());     A0_6 = atof(Data[29+i*30].c_str()); 
//         
////          Serial.print(A4_6); Serial.print("  "); Serial.print(A3_6);Serial.print("  "); Serial.print(A2_6);Serial.print("  ");Serial.print(A1_6);Serial.print("  ");Serial.print(A0_6);
////          for(int j=0;j<150;j++){
////                  Serial.print(Data[j]);
////                  Serial.print("  ");
////          }
//
//
//       for (int n=1;n<101; n++) {
//        
//            int count1=0;   int count2=0;   int count3=0;  int count4=0;   int count5=0;   int count6=0;
//            
////            Step1_n = A4_1*pow(10,-10)*5538.7553*pow(n,4)+A3_1*pow(10,-8)*5538.7553*pow(n,3)+A2_1*pow(10,-6)*5538.7553*pow(n,2)+A1_1*pow(10,-4)*5538.7553*pow(n,1)+A0_1*pow(10,-2)*5538.7553*pow(n,0) ;
////          // Serial.println(Step1_n);
////        
////            Step2_n = A4_2*pow(10,-10)*12414.4517*pow(n,4)+A3_2*pow(10,-8)*12414.4517*pow(n,3)+A2_2*pow(10,-6)*12414.4517*pow(n,2)+A1_2*pow(10,-4)*12414.4517*pow(n,1)+A0_2*pow(10,-2)*12414.4517*pow(n,0) ;
////           // Serial.println(Step2_n);
////        
////            Step3_n = A4_3*pow(10,-10)*11936.9728*pow(n,4)+A3_3*pow(10,-8)*11936.9728*pow(n,3)+A2_3*pow(10,-6)*11936.9728*pow(n,2)+A1_3*pow(10,-4)*11936.9728*pow(n,1)+A0_3*pow(10,-2)*11936.9728*pow(n,0) ;
////          //  Serial.println(Step3_n);
////        
////            Step4_n = A4_4*pow(10,-10)*2638.2301*pow(n,4)+A3_4*pow(10,-8)*2638.2301*pow(n,3)+A2_4*pow(10,-6)*2638.2301*pow(n,2)+A1_4*pow(10,-4)*2638.2301*pow(n,1)+A0_4*pow(10,-2)*2638.2301*pow(n,0) ;
////           // Serial.println(Step4_n);
////        
////            Step5_n = A4_5*pow(10,-10)*2646.5974*pow(n,4)+A3_5*pow(10,-8)*2646.5974*pow(n,3)+A2_5*pow(10,-6)*2646.5974*pow(n,2)+A1_5*pow(10,-4)*2646.5974*pow(n,1)+A0_5*pow(10,-2)*2646.5974*pow(n,0) ;
////           // Serial.println(Step4_n);
//            
//            Step6_n = A4_6*pow(10,-10)*2639.49*pow(n,4)+A3_6*pow(10,-8)*2639.49*pow(n,3)+A2_6*pow(10,-6)*2639.49*pow(n,2)+A1_6*pow(10,-4)*2639.49*pow(n,1)+A0_6*pow(10,-2)*2639.49*pow(n,0) ;
//            //Serial.println(Step6_n);
//        
//            ///////////判断电机转动方向
//            
////              if (Step1_n>0) {
////                digitalWrite(motor1dirPin,HIGH);  }
////              else {
////                digitalWrite(motor1dirPin,LOW);
////              }
////       
////              if (Step2_n>0) {
////                digitalWrite(motor2dirPin,HIGH);  }
////              else {
////                digitalWrite(motor2dirPin,LOW);
////              }
////        
////              if (Step3_n>0) {
////                digitalWrite(motor3dirPin,LOW);  }
////              else {
////                digitalWrite(motor3dirPin,HIGH);
////              }
////        
////              if (Step4_n>0) {
////                digitalWrite(motor4dirPin,LOW);  }
////              else {
////                digitalWrite(motor4dirPin,HIGH);
////              }
////        
////              if (Step5_n>0) {
////                digitalWrite(motor5dirPin,LOW);  }
////              else {
////                digitalWrite(motor5dirPin,HIGH);
////              }
//        
//              if (Step6_n>0) {
//                digitalWrite(motor6dirPin,LOW);  }
//              else {
//                digitalWrite(motor6dirPin,HIGH);
//              }
//        
//              ////////////////发脉冲
////             while ((count1 != abs(Step1_n)) || (count2 != abs(Step2_n)) || (count3 != abs(Step3_n)) || (count4 != abs(Step4_n)) || (count5 != abs(Step5_n)) || (count6 !=abs(Step6_n)) )  {
//             while (  count6 !=abs(Step6_n)  )  {
//        
////                  if (count1 != abs(Step1_n)) {
////                      digitalWrite(motor1stepPin,HIGH);
////                      digitalWrite(motor1stepPin,LOW);
////                      delayMicroseconds(300);
////                      count1=count1+1;
////                   }
////        
////                  if (count2 != abs(Step2_n)) {
////                      digitalWrite(motor2stepPin,HIGH);
////                      digitalWrite(motor2stepPin,LOW);
////                      count2=count2+1;
////                   }
////        
////                  if (count3 != abs(Step3_n)) {
////                      digitalWrite(motor3stepPin,HIGH);
////                      digitalWrite(motor3stepPin,LOW);
////                      count3=count3+1;
////                   }
////        
////                  if (count4 != abs(Step4_n)) {
////                      digitalWrite(motor4stepPin,HIGH);
////                      digitalWrite(motor4stepPin,LOW);
////                      // delayMicroseconds(3000);
////                      count4=count4+1;
////                   }
////        
////                 if (count5 != abs(Step5_n)) {
////                      digitalWrite(motor5stepPin,HIGH);
////                      digitalWrite(motor5stepPin,LOW);
//////                      delayMicroseconds(300);//不均分
////                      count5=count5+1;
////                   }
//        
//                  if (count6 != abs(Step6_n)) {
//                      digitalWrite(motor6stepPin,HIGH);
//                      digitalWrite(motor6stepPin,LOW);
//                      delayMicroseconds(300);//不均分
//                      count6 = count6+1;
//                     }
//           }
//   }
//   }
//    Flag = 0;  //标识符，走完一次后，要直到再次有数据从串口发来时将Flag置1，以此来避开loop循环
//  }
// }  
 
      
