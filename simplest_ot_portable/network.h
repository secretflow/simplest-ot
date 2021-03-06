#ifndef NETWORK_H
#define NETWORK_H

#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define DEFPORT 5001
#define BUFSIZE 65536

union Sockaddr {
  struct sockaddr addr;
  struct sockaddr_in addr_in;
};

int server_listen(const int portno);
int server_accept(int sockfd);
void client_connect(int* sock, const char* host, const int portno);
void writing(int sockfd, void* buffer, const unsigned len);
void reading(int sockfd, void* buffer, const unsigned len);

#endif  // ifndef NETWORK_H
