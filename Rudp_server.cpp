#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

int main(){
    struct sockaddr_in s;
    int sock;
    char buff[182];
    //IPv4----PF_INET   IPv6----PF_INET6
    if(sock = socket(AF_INET6, SOCK_DGRAM, 0) == -1){
        //申请套接字失败
        perror("error");
    }
    memset(&s, 0, sizeof(s));
    s.sin_family = AF_INET6;
    s.sin_port = htons(3047);
    s.sin_addr.s_addr = INADDR_ANY;

    if(bind(sock, (struct sockaddr *)&s, sizeof(s)) == -1){
        perror("error");
    }

    int sock_len = sizeof(s);
    recvfrom(sock, buff, sizeof(buff)-1, 0, (struct sockaddr*)&s, sock_len);
    printf("message:%s", buff);
    return 0;
}
