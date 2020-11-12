import socket
import threading


def recv_msg(udp_socket):
    while True:
        recv_data = udp_socket.recvfrom(1024)
        print(recv_data)


def send_msg(udp_socket, dest_ip, dest_port):
     while True:
        send_data = input("输入要发送的数据：")
        udp_socket.sendto(send_data.encode("utf-8"), (dest_ip, dest_port))   


def main():
    udp_socket = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM)
    udp_socket.bind(("", 3047))

    dest_ip = "2001:da8:270:2020:f816:3eff:fe2c:1123"
    dest_port = 3047

    t_recv = threading.Thread(target = recv_msg, args = (udp_socket,))
    t_send = threading.Thread(target = send_msg, args = (udp_socket, dest_ip, dest_port))

    t_recv.start()
    t_send.start()

if __name__ == "__main":
    main()