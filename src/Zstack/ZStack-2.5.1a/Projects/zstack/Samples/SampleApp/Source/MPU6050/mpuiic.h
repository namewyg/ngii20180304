#ifndef __MPUIIC_H
#define __MPUIIC_H

#include"iocc2530.h"
#include"OnBoard.h"
#include "delay.h"

//IO��������
#define MPU_SCL_OUT()  {P0DIR |= 0x40;}//P0.6����Ϊ���
#define MPU_SDA_IN()  {P0DIR &= ~0x80;}//P0.7����Ϊ����
#define MPU_SDA_OUT() {P0DIR |= 0x80;}//P0.7����Ϊ���

//IO��������
#define MPU_IIC_SCL P0_6                //SCL
#define MPU_IIC_SDA P0_7                //SDA	 
#define MPU_READ_SDA   (P0_7)

//IIC���в�������
void MPU_IIC_Delay(void);				//MPU IIC��ʱ����
void MPU_IIC_Init(void);                //��ʼ��IIC��IO��				 
void MPU_IIC_Start(void);				//����IIC��ʼ�ź�
void MPU_IIC_Stop(void);	  			//����IICֹͣ�ź�
void MPU_IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 MPU_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 MPU_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void MPU_IIC_Ack(void);					//IIC����ACK�ź�
void MPU_IIC_NAck(void);				//IIC������ACK�ź�

void IMPU_IC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 MPU_IIC_Read_One_Byte(u8 daddr,u8 addr);	  



#endif

