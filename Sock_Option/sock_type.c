#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#define BUF_SIZE 30
void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
    WSADATA wsaData;
    SOCKET  tcpSock, udpSock;
    int     sock_type, socket_len;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        ErrorHandling("WSAStartup() error !");
    }
    tcpSock = socket(PF_INET, SOCK_STREAM, 0);
    udpSock = socket(PF_INET, SOCK_DGRAM, 0);
    printf("Stream is %d\n", SOCK_STREAM);
    printf("Dgram is %d\n", SOCK_DGRAM);

    socket_len = sizeof(sock_type);
    if (getsockopt(tcpSock, SOL_SOCKET, SO_TYPE, (char*)&sock_type, &socket_len))
        ErrorHandling("getsocketopt(tcp) error");

    printf("Socket type one : %d\n", sock_type);

    if (getsockopt(udpSock, SOL_SOCKET, SO_TYPE, (char*)&sock_type, &socket_len))
        ErrorHandling("getsocketopt(udp) error");

    printf("Socket type two : %d\n", sock_type);
    closesocket(tcpSock);
    closesocket(udpSock);
    WSACleanup();
    return 0;
}

void ErrorHandling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}