#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winsock2.h>



#define BUF_SIZE 1024
#define NUM 50
void ErrorHandling(char* message);
void send_msg(char* msg, int len, SOCKET* clnt_sock, int num_sock);
void CompressEvents(WSAEVENT* event, int idx, int total);
void CompressSockets(SOCKET* sock, int idx, int total);

int main(int argc, char* argv[])
{
    WSADATA wsaData;
    char    message[BUF_SIZE];
    int     str_len, i;

    SOCKET           hServSock, clnt_sock;
    SOCKET           hSock[NUM];
    SOCKADDR_IN      servAddr, clntAddr;
    WSAEVENT         hEvents[NUM];
    WSAEVENT         newEvent;
    WSANETWORKEVENTS netEvent;
    int              Numofsock = 0;
    int              pos_info, start_index;
    int              szClntAddr;

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

    newEvent = WSACreateEvent();
    if (WSAEventSelect(hServSock, newEvent, FD_ACCEPT)) ErrorHandling("EventSelect error");

    hSock[Numofsock]   = hServSock;
    hEvents[Numofsock] = newEvent;
    Numofsock++;

    szClntAddr = sizeof(clntAddr);

    while (1) {
        pos_info = WSAWaitForMultipleEvents(Numofsock, hEvents, FALSE, WSA_INFINITE, FALSE);
        // if (pos_info == WSA_INVALID_EVENT) continue;
        start_index = pos_info - WSA_WAIT_EVENT_0;
        for (size_t i = start_index; i < Numofsock; i++) {
            int sigEventindex = WSAWaitForMultipleEvents(1, &hEvents[i], TRUE, 0, FALSE);
            if (sigEventindex == WAIT_TIMEOUT || sigEventindex == WSA_WAIT_FAILED)
                continue;
            else {
                sigEventindex = i;
                if (WSAEnumNetworkEvents(hSock[sigEventindex], hEvents[sigEventindex], &netEvent) ==
                    SOCKET_ERROR)
                    ErrorHandling("WSAEnum error");
                if (netEvent.lNetworkEvents & FD_ACCEPT) {
                    if (netEvent.iErrorCode[FD_ACCEPT_BIT] != 0) {
                        puts("Accept error");
                        printf("%d\n", netEvent.iErrorCode[FD_ACCEPT_BIT]);
                        break;
                    }
                    clnt_sock = accept(hSock[sigEventindex], (SOCKADDR*)&clntAddr, &szClntAddr);
                    if (clnt_sock == INVALID_SOCKET) ErrorHandling("accept() error");


                    newEvent = WSACreateEvent();
                    if (WSAEventSelect(clnt_sock, newEvent, FD_READ | FD_CLOSE))
                        ErrorHandling("EventSelect error");
                    hSock[Numofsock]   = clnt_sock;
                    hEvents[Numofsock] = newEvent;
                    Numofsock++;
                    printf("Connected client %d \n", clnt_sock);
                }
                else if (netEvent.lNetworkEvents & FD_READ) {
                    if (netEvent.iErrorCode[FD_READ_BIT] != 0) {
                        puts("Read error");
                        break;
                    }
                    str_len = recv(hSock[sigEventindex], message, BUF_SIZE, 0);
                    send_msg(message, str_len, &hSock[1], Numofsock - 1);
                }
                else if (netEvent.lNetworkEvents & FD_CLOSE) {
                    if (netEvent.iErrorCode[FD_CLOSE_BIT] != 0) {
                        puts("Close error");
                        break;
                    }
                    closesocket(hSock[sigEventindex]);
                    WSACloseEvent(hEvents[sigEventindex]);
                    printf("close client : %d\n", hSock[sigEventindex]);
                    CompressSockets(hSock, sigEventindex, Numofsock);
                    CompressEvents(hEvents, sigEventindex, Numofsock);
                    --Numofsock;
                }
            }
        }
    }

    closesocket(hServSock);
    WSACleanup();
    return 0;
}

void send_msg(char* msg, int len, SOCKET* clnt_sock, int num_sock)
{
    for (int i = 0; i < num_sock; i++) {
        send(clnt_sock[i], msg, len, 0);
    }
}

void CompressSockets(SOCKET* sock, int idx, int total)
{
    for (int i = idx; i < total - 1; i++) {
        sock[i] = sock[i + 1];
    }
}
void CompressEvents(WSAEVENT* event, int idx, int total)
{
    for (int i = idx; i < total - 1; i++) {
        event[i] = event[i + 1];
    }
}
void ErrorHandling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}