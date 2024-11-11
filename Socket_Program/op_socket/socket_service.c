#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32")

#define BUF_SIZE 1024
#define ALIG 4

int  clculate(char opcnt, char opertor, int* operand);
void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
    WSADATA wsaData;
    char    opinfo[BUF_SIZE];
    char    opertor, opcnt;
    int*    send_num;
    int     opNum = 0;
    int     recv_cnt;
    int     strLen, recvLen, i;

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
    for (i = 0; i < 5; i++) {
        opcnt     = 0;
        recvLen   = 0;
        hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
        recv(hClntSock, &opcnt, 1,
             0);   // �ȶ�������,�����������鱻��ȡ1�����ܻ���������-1
        if (hClntSock == INVALID_SOCKET)
            ErrorHandling("accept() error");
        else
            printf("Connected client %d \n", i + 1);

        while (recvLen < (ALIG * opcnt + 1)) {
            recv_cnt = recv(hClntSock, &opinfo[recvLen], BUF_SIZE - 1, 0);
            recvLen += recv_cnt;
        }
        opNum = clculate(opcnt, opinfo[recvLen - 1], (int*)&opinfo);
        printf("cnt %d \n", opcnt);
        printf("num1 %d \n", *((int*)&opinfo));
        printf("recvLen %d \n", recvLen);
        printf("operator %c \n", opinfo[recvLen - 1]);
        printf("opNum %d \n", opNum);

        send(hClntSock, (char*)&opNum, sizeof(opNum), 0);

        closesocket(hClntSock);
    }

    closesocket(hServSock);
    WSACleanup();
    return 0;
}

int clculate(char opcnt, char opertor, int* operand)
{
    int opNum = 0, i;
    switch (opertor) {
    case '+':
        opNum = 0;
        for (i = 0; i < opcnt; i++) {
            opNum += operand[i];
        }
        break;
    case '-':
        opNum = 0;
        for (i = 0; i < opcnt; i++) {
            opNum -= operand[i];
        }
        break;
    case '*':
        opNum = 1;
        for (i = 0; i < opcnt; i++) {
            opNum *= operand[i];
        }
        break;
    }

    return opNum;
}

void ErrorHandling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}