#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mensaje.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "msgq.h"

char mensaje_recibe[MAXSIZEDATA];

main()
{
    int id;

    if ((id = msgget(MKEY2, 0)) < 0)
    {
        perror("No se puede acceder a la cola");
    }

    char inputString[MAXSIZEDATA];

    printf("Introduce el comando eco: \n");
    scanf("%49[^\n]", inputString);
    strncpy(mensaje_recibe, inputString, strlen(inputString));

    int snd = msgsnd(id, mensaje_recibe, strlen(mensaje_recibe), 0);
    if (snd < 0)
    {
        perror("Error al enviar a la cola");
    }

    char mensaje_final[MAXSIZEDATA];
    int rcv = msgrcv(id, mensaje_final, MAXSIZEDATA, 0, 0);
    if (rcv < 0)
    {
        perror("Error al leer de la cola");
    }

    printf("Respuesta recibida: %s\n", mensaje_final);

    return 0;
}