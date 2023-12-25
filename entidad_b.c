#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mensaje.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "msgq.h"

#define SERV_UDP_PORT 8524
//Direccion saturno (donde esta el cliente)
#define SERV_HOST_ADDR "157.88.207.244"
//Direccion carpanta (donde esta el servidor)
//#define SERV_HOST_ADDR "157.88.200.41"


Mensaje mensaje_recibe;
Mensaje mensaje;



int crearCola()
{
    int id;
    if ((id = msgget(MKEY1, PERMS | IPC_CREAT)) < 0)
    {
        perror("Error al crear la cola");
        exit(-1);
    }
    return id;
}

main()
{
    char* mensaje_manda;
    struct sockaddr_in servaddr = { 0 };
    struct sockaddr_in cliaddr = { 0 };
    int id_cola;

    //Creamos el socket UDP
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
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
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(SERV_UDP_PORT);

    //Indicamos que el servidor esucha al puerto indicado
    int rc = bind(sockfd, (const struct sockaddr*)&servaddr,
        sizeof(servaddr));

    if (rc == -1) {
        perror("failed to bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    socklen_t len = sizeof(cliaddr);

    //Recibimos del cliente
    int n = recvfrom(sockfd, (struct Mensaje*)&mensaje_recibe, sizeof(mensaje_recibe), MSG_WAITALL,
        (struct sockaddr*)&cliaddr, &len);

    printf("Received message from client\n");
    printf("Tipo: %d\n", mensaje_recibe.comando);

    char mensaje_recibe_char[MAXSIZEDATA];
    strcpy(mensaje_recibe_char, mensaje_recibe.data);
    
    int snd = msgsnd(id_cola, &mensaje_recibe, sizeof(mensaje_recibe), 0);
    if (snd < 0)
    {
        perror("Error al enviar a la cola");
    }

    char mensaje_enviar_char[MAXSIZEDATA];

    int rcv = msgrcv(id_cola, &mensaje, sizeof(mensaje), 0, 0);
    if (rcv < 0)
    {
        perror("Error al leer de la cola");
    }
 
    mensaje.tipo = 1;
    sendto(sockfd, (struct Mensaje*)&mensaje, sizeof(mensaje), 0,
        (const struct sockaddr*)&cliaddr, sizeof(cliaddr));

    msgctl(id_cola, IPC_RMID, NULL);

    close(sockfd);
    exit(0);
}

