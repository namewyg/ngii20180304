# -*- coding: utf-8 -*-
import socket
import csv
import pymysql

#_ipv6_addr = ('2001:da8:270:2021::14b', 9000)
_ipv6_addr = ("fe80::11d9:b4a0:6ab5:3203%20", 9000)

_conn = pymysql.connect(
    host ='localhost',
    user = 'root',
    password = 'mysql',
    db = 'lems',
    charset = 'utf8'
)
cur = _conn.cursor()

def InsertMySQL(tableName, sqlStr):
    try:
        insertOneRow = "insert into " + tableName + " values" + sqlStr + ";"
        cur.execute(insertOneRow)
    except Exception as e:
        print("Insert Error", e)
        return False
    else:
        #cur.commit()
        if(cur.rowcount > 0):
            return True

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
        #StrMpu = '('+str(self.wx)+','+str(self.wy)+','+str(self.wz)+','+str(self.velx)+','+str(self.velx)+','+str(self.velx)+')'
        StrMpu = '('+str(self.wx)+','+str(self.wy)+','+str(self.wz)+','+str(self.velx)+','+str(self.velx)+')'
        return StrMpu

class LemsClient():
    def __init__(self, ipport):
        self.IPv6_addr = ipport
        self.port = ipport[1]
        self.tcpClient = socket.socket(socket.AF_INET6, socket.SOCK_STREAM)
        self.tcpClient.connect(self.IPv6_addr)

    def TcpSend(self, data):
        self.tcpClient.send(data.encode('utf8'))
        recv = self.tcpClient.recv(1024)
        return recv

    def Close(self):
        self.tcpClient.close()

if __name__ == '__main__':
    SendMpu = Mpu6050()
    tcpclient = LemsClient(_ipv6_addr)
    while True:
        try:
            recv = tcpclient.TcpSend("OK")
            if not recv:
                break
            print(recv.decode('utf-8'))
            #InsertMySQL('user_drug', recv.decode('utf-8'))
            #_conn.commit()
        except Exception as e:
            print("RECV ERROR", e)
            break

    # 释放资源
    tcpclient.Close()
    cur.close()
    _conn.close()