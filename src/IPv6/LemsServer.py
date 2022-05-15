# -*- coding: utf-8 -*-
import socket
import csv
import pymysql

#_ipv6_addr = ('2001:250:3401:40a0:b97e:3992:57d3:68a9', 9000)
_ipv6_addr = ("fe80::11d9:b4a0:6ab5:3203%20", 9000)


class Mpu6050:
    def __init__(self):
        self.id = 0
        self.wx = 0.0
        self.wy = 0.0
        self.wz = 0.0   #角速度
        self.velx = 0.0
        self.vely = 0.0
        self.velz = 0.0 #角加速度

    def List2Mpu(self, LM):
        self.id = LM[0]
        self.wx = LM[1]
        self.wy = LM[2]
        self.wz = LM[3]
        self.velx = LM[4]
        self.vely = LM[5]
        self.velz = LM[6]

    def ToFormat(self):
        FormatMpu = [self.wx, self.wy, self.wz, self.velx, self.vely, self.velz]
        return FormatMpu

    def ToStr(self):
        StrMpu = '('+str(self.wx)+','+str(self.wy)+','+str(self.wz)+','+str(self.velx)+','+str(self.vely)+','+str(self.velz)+')'
        #StrMpu = '('+str(self.wx)+','+str(self.wy)+','+str(self.wz)+','+str(self.velx)+','+str(self.velx)+')'
        return StrMpu

class LemsServer():
    def __init__(self, ipport):
        self.IPv6_addr = ipport
        self.port = ipport[1]
        self.tcpServer = socket.socket(socket.AF_INET6, socket.SOCK_STREAM)
        self.tcpServer.bind(self.IPv6_addr)
        self.tcpServer.listen(5)

    def TcpRecv(self, mpu_csv):
        conn, addr = self.tcpServer.accept()
        recvdata = conn.recv(1024)
        recvdata = recvdata.decode('utf-8')
        if not recvdata:
            print("NULL")
        else:
            SendMpu = Mpu6050()
            for row in mpu_csv:
                SendMpu.List2Mpu(row)
                mpuStr = SendMpu.ToStr()
                print(mpuStr)
                conn.send(mpuStr.encode('utf-8'))
        conn.close()

    def Close(self):
        self.tcpServer.close()

if __name__ == '__main__':
    path = "E:\Study_Doc\Communicate\IPv6\Mpu6050.csv"
    mpu_csv = csv.reader(open(path))
    tcpserver = LemsServer(_ipv6_addr)
    tcpserver.TcpRecv(mpu_csv)

    # 释放资
    tcpserver.Close()