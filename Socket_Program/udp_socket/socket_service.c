#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32")

#define MSG_LEN 100
void ErrorHandling(char *message);

int main(int argc, char *argv[]) {
  WSADATA wsaData;
  char message[MSG_LEN];
  int strLen;

  SOCKET hServSock;
  SOCKADDR_IN servAddr, clntAddr;
  int szClntAddr;

  if (argc != 2) {
    printf("Usage : %s <port>\n", argv[0]);
    exit(1);
  }
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    ErrorHandling("WSAStartup() error !");
  }

  hServSock = socket(PF_INET, SOCK_STREAM, 0);
  if (hServSock == INVALID_SOCKET)
    ErrorHandling("socket() error");

  memset(&servAddr, 0, sizeof(servAddr));
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servAddr.sin_port = htons(atoi(argv[1]));

while(1){
  szClntAddr = sizeof(clntAddr);
  strLen = recvfrom(hServSock, message,MSG_LEN,0,(struct sockaddr*)&clntAddr,&szClntAddr);
  sendto(hServSock, message,MSG_LEN,0,(struct sockaddr*)&clntAddr,szClntAddr);
}
  

  closesocket(hServSock);
  WSACleanup();
  return 0;
}

void ErrorHandling(char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}