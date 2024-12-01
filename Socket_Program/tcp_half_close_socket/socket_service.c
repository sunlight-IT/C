#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32")
#define BUF_SIZE 30
void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
    WSADATA wsaData;
    FILE*   fp;
    char    message[BUF_SIZE];
    int     read_cnt;

    SOCKET      hServSock, hClntSock;
    SOCKADDR_IN servAddr, clntAddr;
    int         szClntAddr;

    if (argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        ErrorHandling("WSAStartup() error !");
    }

    fp = fopen("socket_service.c", "rb");
    if (fp == NULL) ErrorHandling("fopen() error");
    hServSock = socket(PF_INET, SOCK_STREAM, 0);
    if (hServSock == INVALID_SOCKET) ErrorHandling("socket() error");

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family      = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port        = htons(atoi(argv[1]));

    if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
        ErrorHandling("bind() error");
    }

    if (listen(hServSock, 5) == SOCKET_ERROR) ErrorHandling("listen() error");

    szClntAddr = sizeof(clntAddr);
    hClntSock  = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
    if (hClntSock == INVALID_SOCKET) ErrorHandling("accept() error");


    while (1) {
        read_cnt = fread((void*)message, 1, BUF_SIZE, fp);
        if (read_cnt < BUF_SIZE) {
            send(hClntSock, message, read_cnt, 0);
            break;
        }
        send(hClntSock, message, BUF_SIZE, 0);
    }
    shutdown(hClntSock, SD_SEND);
    recv(hClntSock, message, sizeof(message), 0);
    printf("Message from client: %s", message);
    fclose(fp);
    closesocket(hClntSock);
    closesocket(hServSock);
    WSACleanup();
    return 0;
}

void ErrorHandling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}