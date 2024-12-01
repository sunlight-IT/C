#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#define BUF_SIZE 30
void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
    WSADATA wsaData;
    SOCKET  tcpSock, udpSock;
    int     sock_type, socke_buf_in, socke_buf_out, socket_len, socke_buf_in_len, socke_buf_out_len;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        ErrorHandling("WSAStartup() error !");
    }
    tcpSock = socket(PF_INET, SOCK_STREAM, 0);
    udpSock = socket(PF_INET, SOCK_DGRAM, 0);
    printf("Stream is %d\n", SOCK_STREAM);
    printf("Dgram is %d\n", SOCK_DGRAM);

    socket_len        = sizeof(sock_type);
    socke_buf_in_len  = sizeof(socke_buf_in);
    socke_buf_out_len = sizeof(socke_buf_out);
    if (getsockopt(tcpSock, SOL_SOCKET, SO_TYPE, (char*)&sock_type, &socket_len))
        ErrorHandling("getsocketopt(tcp) error");
    if (getsockopt(tcpSock, SOL_SOCKET, SO_SNDBUF, (char*)&socke_buf_in, &socke_buf_in_len))
        ErrorHandling("getsocketopt(tcp) error");
    if (getsockopt(tcpSock, SOL_SOCKET, SO_RCVBUF, (char*)&socke_buf_out, &socke_buf_out_len))
        ErrorHandling("getsocketopt(tcp) error");
    printf("Socket type one : %d\n", sock_type);
    printf(
        "Socket type one outBuf length is %d ,inBuf length is %d\n", socke_buf_in, socke_buf_out);

    if (getsockopt(udpSock, SOL_SOCKET, SO_TYPE, (char*)&sock_type, &socket_len))
        ErrorHandling("getsocketopt(udp) error");
    if (getsockopt(udpSock, SOL_SOCKET, SO_SNDBUF, (char*)&socke_buf_in, &socke_buf_in_len))
        ErrorHandling("getsocketopt(udp) error");
    if (getsockopt(udpSock, SOL_SOCKET, SO_RCVBUF, (char*)&socke_buf_out, &socke_buf_out_len))
        ErrorHandling("getsocketopt(udp) error");
    printf("Socket type two : %d\n", sock_type);
    printf(
        "Socket type one inBuf length is %d ,outBuf length is %d\n", socke_buf_in, socke_buf_out);
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