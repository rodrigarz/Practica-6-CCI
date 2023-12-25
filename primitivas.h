#ifndef PRIMITIVAS_H
#define PRIMITIVAS_H
#include "mensaje.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>



int peticion(int sockfd, struct sockaddr_in* servaddr, struct Mensaje* mensaje_prim)
{
	int len = sendto(sockfd, &mensaje_prim, sizeof(mensaje_prim), 0, (struct sockaddr*)servaddr, sizeof(struct sockaddr_in));
	return len;
}

int indicacion(int sockfd,  struct sockaddr_in* servaddr, struct Mensaje* mensaje_prim)
{
	socklen_t len = sizeof(servaddr);
	int n = recvfrom(sockfd, &mensaje_prim, sizeof(mensaje_prim), MSG_WAITALL, (struct sockaddr*)servaddr, &len);
	return n;
}

#endif
