
#include <stdio.h>
#include <stdlib.h>


#include <winsock2.h>

void ErrorHandling(char* message);


int main(int argc, char* argv[])
{
    WSADATA         wsaData;
    SOCKADDR_IN     addr;
    struct hostent* host;
    int             i;
    if (argc != 2) {
        printf("Usage :%s <addr>\n", argv[0]);
        exit(1);
    }
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) ErrorHandling("WSAStartup() error");

    addr.sin_addr.s_addr = inet_addr(argv[1]);
    host                 = gethostbyaddr((char*)&addr.sin_addr, 4, AF_INET);
    if (!host) ErrorHandling("gethostbyname() error");

    printf("Official name: %s \n", host->h_name);
    for (i = 0; host->h_aliases[i]; i++)   //
        printf("Aliases %d: %s \n", i + 1, host->h_aliases[i]);
    printf("Address type: %s \n",   //
           (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");
    for (i = 0; host->h_addr_list[i]; i++)
        printf("IP Addr %d: %s\n",
               i + 1,   //
               inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
    WSACleanup();
    return 0;
}

void ErrorHandling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}