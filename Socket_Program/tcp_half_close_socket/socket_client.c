#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#define BUF_SIZE 30
void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
    WSADATA     wsaData;
    SOCKET      hSocket;
    SOCKADDR_IN servAddr;
    FILE*       fp;
    char        message[BUF_SIZE];
    int         readCnt = 0;
    if (argc != 3) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        ErrorHandling("WSAStartup() error !");
    }

    fp = fopen("receive.dat", "wb");
    if (fp == NULL) ErrorHandling("fopen() error");
    hSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (hSocket == INVALID_SOCKET) ErrorHandling("socket() error");

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family      = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port        = htons(atoi(argv[2]));

    if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
        ErrorHandling("connect() error");
    while ((readCnt = recv(hSocket, message, BUF_SIZE, 0)) != 0) {
        fwrite((void*)message, 1, readCnt, fp);
    }

    printf("receive file data");
    send(hSocket, "Thank you", 10, 0);

    fclose(fp);
    closesocket(hSocket);
    WSACleanup();
    return 0;
}

void ErrorHandling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}