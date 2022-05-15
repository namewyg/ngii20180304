# -*- coding: utf-8 -*-
"""
Created on Sun May 29 21:29:57 2019
读串口数据
@author: GXU
"""

import serial
import time

abc = 0

def recv(ser):
    while True: 
        data = ser.read(16)
        if data == '':
            continue
        else:
            break
    return data

# 串口数据保存
def saveCom(ser):
    #ser = serial.Serial('COM3',115200, timeout=0.1)  #/dev/ttyUSB0
    while True:
        data =recv(ser)
        if data != b'' :
            data = data.decode("utf-8","ignore")
            T = data[3:7]
            #print(time.time())
            print(T)         
            #数据保存
            with open("Temperature.csv", "a", newline = '', encoding="utf-8") as csvf:   
                csvf.writelines(T + '\n')    
                csvf.close()                                    
        #return T

# 串口数据保存
def saveCom1():
    #ser = serial.Serial('COM3',115200, timeout=0.1)  #/dev/ttyUSB0
    
    data =recv(ser)
    if data != b'' :
        data = data.decode("utf-8","ignore")
        T = data[3:7]
        #print(time.time())
        #print(T[3:7])         
        #数据保存
        with open("Temperature.csv", "a", newline = '', encoding="utf-8") as csvf:   
            csvf.writelines(T + '\n')    
            csvf.close()   
            
        return T

    
if __name__ == '__main__':
    ser = serial.Serial('COM3',115200, timeout=0.1)  #/dev/ttyUSB0
    if ser.isOpen():
        print("open success")
        saveCom(ser)
        ser.close()
    else :
        print("open failed")
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
'''
    while True:
        data =recv(ser)
        if data != b'' :
            #print("receive:",data.decode("ascii","ignore"))
            #print("receive:",data.decode("utf-8","ignore"))
            data = data.decode("utf-8","ignore")
            T = data[3:7]
            print(T[3:7])

            #print(len(T))
            #print("receive : ",str(data))
            
            adc = data[0] << 8
            adc = adc + data[len(data)-1]
            #print(adc)
            
            #数据处理
            adc = adc * (3.3/4096)
            with open("car_data.csv", "a", newline = '', encoding="utf-8") as csvf:   
                csvf.writelines(str(adc) + '\n')    
                
                csvf.close()                                    
            if 0.02 < adc < 0.25:
                print(adc)
            

#with open('', 'r') as f:
#    print(f.read())
                      
#ser.write(data) #数据写回     
#ser.close()  #关闭串口
'''
