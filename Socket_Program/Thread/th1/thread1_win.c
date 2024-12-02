#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>




#define BUF_SIZE 1024
#define NUM_THREAD 50
void            ErrorHandling(char* message);
unsigned WINAPI thread_inc(void* arg);
unsigned WINAPI thread_dec(void* arg);

long long        num;
CRITICAL_SECTION cs;
int              main(int argc, char* argv[])
{
    HANDLE hThread[NUM_THREAD];

    InitializeCriticalSection(&cs);

    for (int i = 0; i < NUM_THREAD; i++) {
        if (i % 2)
            hThread[i] = (HANDLE)_beginthreadex(NULL, 0, thread_inc, NULL, 0, NULL);
        else
            hThread[i] = (HANDLE)_beginthreadex(NULL, 0, thread_dec, NULL, 0, NULL);

        if (hThread[i] == 0) ErrorHandling("thread create error");
    }

    WaitForMultipleObjects(NUM_THREAD, hThread, TRUE, INFINITE);
    printf("The num resault is: %lld\n",num);
    puts("end of main");
    return 0;
}

unsigned WINAPI thread_inc(void* arg)
{
    int i;
    EnterCriticalSection(&cs);
    for (i = 0; i < 5000; i++) ++num;
    LeaveCriticalSection(&cs);
    return 0;
}

unsigned WINAPI thread_dec(void* arg)
{
    int i;
    EnterCriticalSection(&cs);
    for (i = 0; i < 5000; i++) --num;
    LeaveCriticalSection(&cs);
    return 0;
}

void ErrorHandling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}