#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winsock2.h>



#define BUF_SIZE 1024

void            ErrorHandling(char* message);
unsigned WINAPI ThreadFunc(void* arg);

int main(int argc, char* argv[])
{
    HANDLE       hThread;
    DWORD        wr;
    unsigned int threadID;
    int          thread_prama = 5;
    hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, (void*)&thread_prama, 0, &threadID);
    if (hThread == 0) ErrorHandling("thread create error");
    if ((wr = WaitForSingleObject(hThread, 1000)) == WAIT_FAILED) {
        puts("thread wait error");
        return -1;
    }
    printf("wait resault is %s \n", (wr == WAIT_OBJECT_0) ? "signaled" : "time_out");
    puts("end of main");
    return 0;
}

unsigned WINAPI ThreadFunc(void* arg)
{
    int i;
    int cnt = *((int*)arg);
    for (i = 0; i < cnt; i++) {
        puts("thread");
        Sleep(1000);
    }
    return 0;
}

void ErrorHandling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}