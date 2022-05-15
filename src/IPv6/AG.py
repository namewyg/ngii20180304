# -*- coding: utf-8 -*-
"""
Created on Tue Sep 17 19:16:12 2019
加速度、角速度
@author: GXU
"""
import time
import serial
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

#数据帧
mpuDf = pd.DataFrame(columns=['Ax', 'Ay', 'Az', 'Gx', 'Gy', 'Gz'])

#串口数据接收
def recv(ser):
    while True: 
        data = ser.readline()
        if data == '':
            continue
        else:
            break
    return data

def datafilter(adc1,adc2):
    adc = (adc1 + adc2) / 2.0
    
    return adc
    


# 串口数据保存
def saveCom(ser):
    global mpuDf 
    i = 0
    while True:
        data =recv(ser)
        if data != b'' :
            data = data.decode('utf-8', "ignore")
            #数据分离
            data = data.split(",")
            print(time.time())
            print(data)
            #data[0] = data[0][2:len(data[0])]
            #创建numpy数组并追加到DataFrame            
            d = np.array([float(data[0]), float(data[1]), float(data[2]), float(data[3]), float(data[4]), float(data[5])])
                
            s = pd.DataFrame(d.reshape(1,6), columns=['Ax', 'Ay', 'Az', 'Gx', 'Gy', 'Gz'])
            
            #print(s)
            mpuDf = mpuDf.append(s,ignore_index=True)
            
            #数据保存为csv文件
            #mpuDf.to_csv('E:\Python_OBJ\GUI\RTS\Mpu6050.csv', sep=',', header=True, index=True)
            
            i = i + 1
            if i ==100:
                #数据保存为csv文件
                mpuDf.to_csv('E:\Python_OBJ\GUI\RTS\Mpu6050.csv', sep=',', header=True, index=True)        
                break


class kalman_filter:
    def __init__(self, Q, R):
        self.Q = Q
        self.R = R
        
        self.P_k_k1 = 1
        self.Kg = 0
        self.P_k1_k1 = 1
        self.x_k_k1 = 0
        self.ADC_OLD_Value = 0
        self.Z_k = 0
        self.kalman_adc_old=0
        
    def kalman(self, ADC_Value):
       
        self.Z_k = ADC_Value
        
        if (abs(self.kalman_adc_old - ADC_Value) >= 60):
            self.x_k1_k1 = ADC_Value * 0.382 + self.kalman_adc_old * 0.618
        else:
            self.x_k1_k1 = self.kalman_adc_old;
    
        self.x_k_k1 = self.x_k1_k1
        self.P_k_k1 = self.P_k1_k1 + self.Q
    
        self.Kg = self.P_k_k1/(self.P_k_k1 + self.R)
    
        kalman_adc = self.x_k_k1 + self.Kg * (self.Z_k - self.kalman_adc_old)
        self.P_k1_k1 = (1 - self.Kg)*self.P_k_k1
        self.P_k_k1 = self.P_k1_k1
    
        self.kalman_adc_old = kalman_adc
        
        return kalman_adc

            
if __name__ == '__main__':
    ser = serial.Serial('COM3',115200, timeout=0.1)  #/dev/ttyUSB0
    if ser.isOpen():
        print("open success")
        saveCom(ser)
        #初始设置
        fig = plt.figure()
        ax1=fig.add_subplot(2,1,1)
        ax2=fig.add_subplot(2,1,2)
        #plt.ion()
        ax1.plot(mpuDf['Ax'], label = 'X-axis acceleration')
        #ax1.plot(mpuDf['Ay'], label = 'Y-axis acceleration')
        #ax1.plot(mpuDf['Az'], label = 'Z-axis acceleration')

        #ax2.plot(mpuDf['Gx'], label = 'X-axis')
        #ax2.plot(mpuDf['Gy'], label = 'Y-axis')
        #ax2.plot(mpuDf['Gz'], label = 'Z-axis')
        plt.legend()
        plt.show()
        #ax1.legend(loc="best",ncol=0)
        
        #mpuDf.plot()
        ser.close()
    else :
        print("open failed")   
  
       
        plt.show()

           

'''
print(Ax)
print(Ay)
print(Az)
print(Gx)
print(Gy)
print(Gz)

#第一个传感器
Ax = data[2:7]
Ay = data[8:12]
Az = data[13:17]
Gx = data[22:27]
Gy = data[28:32]
Gz = data[33:38]

#第二个传感器
Ax = data[2:6]
Ay = data[7:11]
Az = data[12:17]
Gx = data[22:27]
Gy = data[28:33]
Gz = data[34:39]


Ax = data[2:6]
Ay = data[7:11]
Az = data[12:17]
Gx = data[22:27]
Gy = data[28:33]
Gz = data[34:39]

print(Ax)
print(Ay)
print(Az)
print(Gx)
print(Gy)
print(Gz)

print(data[0])
print(data[1])
print(data[2])
print(data[3])
print(data[4])
print(data[5])
print(' ')


#异常处理
try:
    d = np.array([float(Ax), float(Ay), float(Az), float(Gx), float(Gy), float(Gz)])
except:
    if Gy[0] == '-':
        Gy = Gy[0:5]
    else:
        Gy = '-' + Gy[0:4]
        
    if Gz[0] == '-':
        Gz = Gz[0:5]
    else:
        Gz = '-' + Gz[0:4]
        
    if Gz[0] == '.':
        Gz = '-' + '0' + Gz[0:3]
        print(Gz)
        
        
#for i in range(0,20):
    #mpuDf['Ax'][i+1] = (mpuDf['Ax'][i] + mpuDf['Ax'][i + 1])/2.0
    #mpuDf['Ay'][i+1] = (mpuDf['Ax'][i] + mpuDf['Ax'][i + 1])/2.0
    #mpuDf['Az'][i+1] = (mpuDf['Ax'][i] + mpuDf['Ax'][i + 1])/2.0
    #mpuDf['Gx'][i+1] = (mpuDf['Ax'][i] + mpuDf['Ax'][i + 1])/2.0
    #mpuDf['Gy'][i+1] = (mpuDf['Ax'][i] + mpuDf['Ax'][i + 1])/2.0
    #mpuDf['Gz'][i+1] = (mpuDf['Ax'][i] + mpuDf['Ax'][i + 1])/2.0        

'''


