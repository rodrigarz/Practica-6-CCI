#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mensaje.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERV_UDP_PORT 8524
//Direccion saturno (donde esta el cliente)
//#define SERV_HOST_ADDR "157.88.207.244"
//Direccion carpanta (donde esta el servidor)
#define SERV_HOST_ADDR "157.88.200.41"


Mensaje mensaje_recibe;
Mensaje mensaje;



main()
{
	mensaje.tipo = 1;
	char inputString[50];

	printf("Introduce el comando eco: \n");
	scanf("%49[^\n]", inputString);
	strncpy(mensaje.data, inputString, sizeof(mensaje.data));
	//Llenamos la estructura con 0
	struct sockaddr_in servaddr = { 0 };

	//Creamos socket udp
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == -1)
	{
		perror("failed to create socket");
		exit(EXIT_FAILURE);
	}

	//Datos del servidor
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_UDP_PORT);
	servaddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);

	mensaje.tipo = 1;

	//Enviamos mensaje a servidor
	int len = sendto(sockfd, (const char*)&mensaje, sizeof(mensaje),
		0, (const struct sockaddr*)&servaddr, sizeof(servaddr));
	if (len == -1)
	{
		perror("failed to send");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	int n = recvfrom(sockfd, (const char*)&mensaje_recibe, sizeof(mensaje_recibe), MSG_WAITALL, (struct sockaddr*)&servaddr, &len);
	if (n == -1)
	{
		perror("Error al recibir");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	printf("Mensaje recibido del servidor\n");
	printf("Tipo mensaje: %d\n", mensaje_recibe.tipo);
	printf("Contenido: %s\n", mensaje_recibe.data);

	//Cerramos socket
	close(sockfd);
    exit(0);
}