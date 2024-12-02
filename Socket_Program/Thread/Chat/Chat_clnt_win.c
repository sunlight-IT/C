#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winsock2.h>

#define BUF_SIZE 1024
#define NAME_SIZE 40

void            ErrorHandling(char* message);
unsigned WINAPI thread_read(void* arg);
unsigned WINAPI thread_write(void* arg);

static char name[NAME_SIZE];
static char msg[BUF_SIZE];
int         main(int argc, char* argv[])
{
    WSADATA wsaData;

    HANDLE      hThread_read, hThread_write;
    SOCKET      hSocket;
    SOCKADDR_IN servAddr;


    if (argc != 4) {
        printf("Usage : %s <IP> <port> <name>\n", argv[0]);
        exit(1);
    }
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        ErrorHandling("WSAStartup() error !");
    }

    sprintf(name, "[%s]", argv[3]);
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
    hThread_write = (HANDLE)_beginthreadex(NULL, 0, thread_write, (void*)&hSocket, 0, NULL);
    if (hThread_write == 0) ErrorHandling("thread create error");
    hThread_read = (HANDLE)_beginthreadex(NULL, 0, thread_read, (void*)&hSocket, 0, NULL);
    if (hThread_read == 0) ErrorHandling("thread create error");

    WaitForSingleObject(hThread_write, WSA_INFINITE);
    WaitForSingleObject(hThread_read, WSA_INFINITE);
    closesocket(hSocket);
    puts("end of main");
    return 0;
}

unsigned WINAPI thread_read(void* arg)
{
    int    i, readLen;
    SOCKET sock = *((int*)arg);
    char   name_msg[BUF_SIZE + NAME_SIZE];
    while ((readLen = recv(sock, name_msg, BUF_SIZE + NAME_SIZE, 0)) != 0) {
        if (readLen == -1) ErrorHandling("read error");
        name_msg[readLen] = 0;
        printf("%s\n", name_msg);
    }

    return 0;
}

unsigned WINAPI thread_write(void* arg)
{
    int    i;
    SOCKET sock = *((int*)arg);
    char   name_msg[BUF_SIZE + NAME_SIZE];
    while (1) {
        fgets(msg, BUF_SIZE, stdin);
        if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n")) {
            puts("close");
            shutdown(sock, SD_SEND);
            break;
        }
        sprintf(name_msg, "%s %s", name, msg);
        send(sock, name_msg, strlen(name_msg), 0);
        memset(name_msg, 0, sizeof(name_msg));
    }
    return 0;
}

void ErrorHandling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}