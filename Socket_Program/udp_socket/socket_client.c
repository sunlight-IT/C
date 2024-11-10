#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>



#define MSG_LEN 100
void ErrorHandling(char *message);

int main(int argc, char *argv[]) {
  WSADATA wsaData;
  SOCKET hSocket;
  SOCKADDR_IN servAddr, fromAddr;
  char message[MSG_LEN];
  int strLen = 0, add_sz = 0;
  if (argc != 3) {
    printf("Usage : %s <port>\n", argv[0]);
    exit(1);
  }
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    ErrorHandling("WSAStartup() error !");
  }

  hSocket = socket(PF_INET, SOCK_STREAM, 0);
  if (hSocket == INVALID_SOCKET)
    ErrorHandling("socket() error");

  memset(&servAddr, 0, sizeof(servAddr));
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = inet_addr(argv[1]);
  servAddr.sin_port = htons(atoi(argv[2]));


  while (1) {
    fputs("Input message Q to quit):", stdout);
    fgets(message, sizeof(message), stdin);
    if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))
    break;

    add_sz = sizeof(fromAddr);
    sendto(hSocket, message, strlen(message), 0,(struct sockaddr*)&servAddr,sizeof(servAddr));
    strLen = recvfrom(hSocket, message, MSG_LEN, 0,(struct sockaddr*)&fromAddr,&add_sz);
    message[strLen] = 0;
    printf("Message from server: %s", message);
  }

  closesocket(hSocket);
  WSACleanup();
  return 0;
}

void ErrorHandling(char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}