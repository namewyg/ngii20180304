#include "OnBoard.h"
#include "delay.h"

//延时函数
void Delay_1us() //1 us延时
{
    MicroWait(1); 
}

void Delay_10us() //10 us延时
{
  MicroWait(10);  
}

void delay_us(u16 Time)//n ms延时
{
    unsigned char i;
    while(Time--)
    {
        Delay_1us();
    }
}

void delay_ms(u16 Time)//n ms延时
{
    unsigned char i;
    while(Time--)
    {
        for(i=0;i<100;i++)
            Delay_10us();
    }
}

void delay_1ms()//n ms延时
{
  unsigned char i;

  for(i=0;i<100;i++)
    Delay_10us();
}

