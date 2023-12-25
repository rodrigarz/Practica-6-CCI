#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mensaje.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "msgq.h"

char* convertirAMayusculas(const char* cadena) {
    char* resultado = strdup(cadena); // Duplicamos la cadena original para no modificar la original
    if (resultado == NULL) {
        perror("failed to duplicate string");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    while (resultado[i]) {
        resultado[i] = toupper(resultado[i]);
        i++;
    }

    return resultado;
}

char mensaje_recibe[MAXSIZEDATA];
char mensaje[MAXSIZEDATA];

main()
{
    int id;

    if ((id = msgget(MKEY1, 0)) < 0)
    {
        perror("No se puede acceder a la cola");
    }

    int res = msgrcv(id, mensaje_recibe, MAXSIZEDATA, 0, 0);
    if (res < 0)
    {
        perror("No se puede recibir de la cola");
    }

    char* temp;
    temp = convertirAMayusculas(mensaje_recibe);

    strcpy(mensaje, temp);

    int snd = msgsnd(id, mensaje, MAXSIZEDATA, 0);
    if (snd < 0)
    {
        perror("Error al enviar a la cola");
    }

    exit(0);

}