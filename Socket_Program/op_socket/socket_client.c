#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#define BUF_SIZE 1024
#define ALIG 4
void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
    WSADATA     wsaData;
    SOCKET      hSocket;
    SOCKADDR_IN servAddr;
    int         count;
    char        opmsg[BUF_SIZE];
    int         resault, i;
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
        printf("Connected......\n");

    fputs("Operand count:", stdout);
    scanf("%d", &count);
    opmsg[0] = (char)count;
    for (i = 0; i < count; i++) {
        printf("Operand  %d:", i);
        scanf("%d", (int*)&opmsg[i * ALIG + 1]);
    }
    fgetc(stdin);
    fputs("Operator:", stdout);
    scanf("%c", &opmsg[count * ALIG + 1]);
    send(hSocket, opmsg, count * ALIG + 2, 0);
    recv(hSocket, (char*)&resault, BUF_SIZE - 1, 0);

    printf("Operation result: %d\n", resault);

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