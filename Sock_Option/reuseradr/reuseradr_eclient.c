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
    char        message[BUF_SIZE];
    char        rev[BUF_SIZE];
    int         strLen  = 0;
    int         readLen = 0, readCnt;
    if (argc != 3) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        ErrorHandling("WSAStartup() error !");
    }

    hSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (hSocket == INVALID_SOCKET) ErrorHandling("socket() error");

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family      = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port        = htons(atoi(argv[2]));

    if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
        ErrorHandling("connect() error");
    else
        printf("Connected......");
    while (1) {
        fputs("Input message Q to quit):", stdout);
        fgets(message, BUF_SIZE, stdin);

        if (!strcmp(message, "q/n") || !strcmp(message, "Q\n")) break;

        strLen  = send(hSocket, message, strlen(message), 0);
        readLen = 0;
        memset(rev, 0, BUF_SIZE);
        while (readLen < strLen) {
            readCnt = recv(hSocket, &rev[readLen], 1, 0);
            if (readLen == -1) ErrorHandling("read() error");
            readLen += readCnt;
            printf("readLen: %d  strLen: %d\n", readLen, strLen);
        }

        rev[strLen] = '\0';
        printf("Message from server: %s\n", rev);
    }

    closesocket(hSocket);
    printf("closed  Sock \n");
    WSACleanup();
    return 0;
}

void ErrorHandling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}