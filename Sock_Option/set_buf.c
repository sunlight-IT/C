#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#define BUF_SIZE 30
void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
    WSADATA wsaData;
    SOCKET  Sock;
    int     socke_sen = 1024 * 5, socke_rev = 1024 * 3, socket_buf_len;
    int     sock_sen_len, sock_rev_len;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        ErrorHandling("WSAStartup() error !");
    }
    Sock = socket(PF_INET, SOCK_STREAM, 0);

    socket_buf_len = sizeof(socke_sen);

    if (setsockopt(Sock, SOL_SOCKET, SO_SNDBUF, (char*)&socke_sen, socket_buf_len))
        ErrorHandling("getsocketopt(tcp) error");
    if (getsockopt(Sock, SOL_SOCKET, SO_SNDBUF, (char*)&sock_sen_len, &socket_buf_len))
        ErrorHandling("getsocketopt(tcp) error");
    printf("Socket type one outBuf length is %d\n", sock_sen_len);

    socket_buf_len = sizeof(socke_rev);
    if (setsockopt(Sock, SOL_SOCKET, SO_RCVBUF, (char*)&socke_rev, socket_buf_len))
        ErrorHandling("getsocketopt(udp) error");
    if (getsockopt(Sock, SOL_SOCKET, SO_RCVBUF, (char*)&sock_rev_len, &socket_buf_len))
        ErrorHandling("getsocketopt(tcp) error");
    printf("Socket type one inBuf length is %d \n", sock_rev_len);
    closesocket(Sock);
    WSACleanup();
    return 0;
}

void ErrorHandling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}