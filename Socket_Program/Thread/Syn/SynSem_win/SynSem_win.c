#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>




#define BUF_SIZE 1024
#define NUM_THREAD 50
void            ErrorHandling(char* message);
unsigned WINAPI thread_accu(void* arg);
unsigned WINAPI thread_read(void* arg);

long long num = 0;
HANDLE    sem_read, sem_accu;

int main(int argc, char* argv[])
{
    HANDLE hThread_accu, hThread_read;
    int    thr_parame = 5;
    sem_read          = CreateSemaphore(NULL, 1, 1, NULL);
    sem_accu          = CreateSemaphore(NULL, 0, 1, NULL);

    hThread_accu = (HANDLE)_beginthreadex(NULL, 0, thread_accu, (void*)&thr_parame, 0, NULL);
    if (hThread_accu == 0) ErrorHandling("thread create error");
    hThread_read = (HANDLE)_beginthreadex(NULL, 0, thread_read, (void*)&thr_parame, 0, NULL);
    if (hThread_read == 0) ErrorHandling("thread create error");

    WaitForSingleObject(hThread_accu, INFINITE);
    WaitForSingleObject(hThread_read, INFINITE);
    CloseHandle(sem_read);
    CloseHandle(sem_accu);
    puts("end of main");
    return 0;
}

unsigned WINAPI thread_accu(void* arg)
{
    int i, sum = 0;
    int cnt = *((int*)arg);
    WaitForSingleObject(sem_accu,
                        INFINITE);   // 利用Wait获取mutex状态，然后自动变为non-signaled状态
    for (i = 0; i < cnt; i++) {

        sum += num;
    }
    ReleaseSemaphore(sem_read, 1, NULL);   // 释放mutex，恢复signaled状态

    printf("sum val is: %d\n", sum);
    return 0;
}

unsigned WINAPI thread_read(void* arg)
{
    int i;
    int cnt = *((int*)arg);
    WaitForSingleObject(sem_read, INFINITE);
    for (i = 0; i < cnt; i++) {

        scanf("%d", &num);
    }
    ReleaseSemaphore(sem_accu, 1, NULL);
    return 0;
}

void ErrorHandling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}