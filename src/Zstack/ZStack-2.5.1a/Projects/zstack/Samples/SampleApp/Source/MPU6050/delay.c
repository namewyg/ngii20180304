#include "OnBoard.h"
#include "delay.h"

//��ʱ����
void Delay_1us() //1 us��ʱ
{
    MicroWait(1); 
}

void Delay_10us() //10 us��ʱ
{
  MicroWait(10);  
}

void delay_us(u16 Time)//n ms��ʱ
{
    unsigned char i;
    while(Time--)
    {
        Delay_1us();
    }
}

void delay_ms(u16 Time)//n ms��ʱ
{
    unsigned char i;
    while(Time--)
    {
        for(i=0;i<100;i++)
            Delay_10us();
    }
}

void delay_1ms()//n ms��ʱ
{
  unsigned char i;

  for(i=0;i<100;i++)
    Delay_10us();
}

