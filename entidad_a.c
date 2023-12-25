#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mensaje.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "msgq.h"

//#define SERV_UDP_PORT 8524
//Direccion saturno (donde esta el cliente)
//#define SERV_HOST_ADDR "157.88.207.244"
//Direccion carpanta (donde esta el servidor)
#define SERV_HOST_ADDR "157.88.200.41"


Mensaje mensaje_recibe;
Mensaje mensaje;
char mensaje_para_enviar[MAXSIZEDATA];
char mensaje_para_devolver[MAXSIZEDATA];

int crearCola()
{
	int id;
	if ((id = msgget(MKEY2, PERMS | IPC_CREAT)) < 0)
	{
		perror("Error al crear la cola");
		exit(-1);
	}
	return id;
}


main()
{
	int id_cola;
	mensaje.tipo = 1;
	
	//Llenamos la estructura con 0
	struct sockaddr_in servaddr = { 0 };

	//Creamos socket udp
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == -1)
	{
		perror("failed to create socket");
		exit(EXIT_FAILURE);
	}

	id_cola = crearCola();
	if (id_cola == -1) {
		perror("Error al recibir id cola");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	//Datos del servidor
	

	mensaje.tipo = 1;

	int rcv = msgrcv(id_cola, &mensaje, sizeof(mensaje), 0, 0);
	if (rcv < 0)
	{
		perror("Error al recibir de la cola");
	}

	int serv_udp_port = mensaje.puerto;

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(serv_udp_port);
	servaddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);

	//strcpy(mensaje.data, mensaje_para_enviar);
	mensaje.patron = 204; //11001100
	mensaje.longitud = strlen(mensaje.data);

	//Enviamos mensaje a servidor
	int len = sendto(sockfd, (struct Mensaje*)&mensaje, sizeof(mensaje),
		0, (const struct sockaddr*)&servaddr, sizeof(servaddr));
	if (len == -1)
	{
		perror("failed to send");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	int n = recvfrom(sockfd, (struct Mensaje*)&mensaje_recibe, sizeof(mensaje_recibe), MSG_WAITALL, (struct sockaddr*)&servaddr, &len);
	if (n == -1)
	{
		perror("Error al recibir");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	printf("Mensaje recibido del servidor\n");

	strcpy(mensaje_para_devolver, mensaje_recibe.data);

	int snd = msgsnd(id_cola, &mensaje_recibe, sizeof(mensaje_recibe), 0);
	if (snd < 0)
	{
		perror("Error al enviar a la cola");
	}
	
	msgctl(id_cola, IPC_RMID, NULL);

	//Cerramos socket
	close(sockfd);
    exit(0);
}