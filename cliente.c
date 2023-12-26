#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mensaje.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "msgq.h"
#include <unistd.h>

//char mensaje_recibe[MAXSIZEDATA];


int main(int argc, char *argv[])
{
    Mensaje mensaje_recibe;
    int id;
    char* terna = argv[1];
    char s;
    int puerto, pas, pid;

    //Separamos la terna que identifica el destino en las variables necesarias
    if (sscanf(terna, " %d:%d:%d %c", &puerto, &pas, &pid, &s) > 4)
    {
        printf("Error al leer los argumentos");
        return -1;
    }

    //Conectamos a la cola con la Key 2
    if ((id = msgget(MKEY2, 0)) < 0)
    {
        perror("No se puede acceder a la cola");
    }

    char inputString[MAXSIZEDATA];

    //En funcion del servicio solicitado mostramos y recogemos distinta informacion por pantalla
    if (pas == 20)
    {
        printf("Introduce el comando eco: \n");
        scanf("%8191[^\n]", inputString);
        strncpy(mensaje_recibe.data, inputString, strlen(inputString));
        mensaje_recibe.comando = 0;
    }

    if (pas == 42)
    {
        char intruccion[100];
        char comando[4];
        char argumento[96];
        printf("Introduce el comando TF: \n");
        scanf("%99[^\n]", intruccion);
        if (sscanf(intruccion, "%s %[^\n]", comando, argumento) != 2) {
            printf("Formato incorrecto de instrucción.\n");
            return 1;
        }

        if (strcmp(comando, "cd") == 0)
        {
            mensaje_recibe.comando = 2;
        }
        else if (strcmp(comando, "dir") == 0)
        {
            mensaje_recibe.comando = 1;
        }
        else if (strcmp(comando, "get") == 0)
        {
            mensaje_recibe.comando = 3;
        }
        else if (strcmp(comando, "quit") == 0)
        {
            mensaje_recibe.comando = 4;
        }
        else
        {
            printf("Error al leer el comando");
            return -1;
        }

        strcpy(mensaje_recibe.data, argumento);

    }

    //Rellenamos los campos restantes
        mensaje_recibe.destino = pid;
        mensaje_recibe.puerto = puerto;
        mensaje_recibe.tipo = 1; //marcamos que es peticion de servicio
        mensaje_recibe.origen = getpid();

    //Enviamos a la cola el mensaje
    int snd = msgsnd(id, &mensaje_recibe, sizeof(mensaje_recibe), 0);
    if (snd < 0)
    {
        perror("Error al enviar a la cola");
        exit(EXIT_FAILURE);
    }

    //Recibimos de la cola el mensaje, y mostramos la informacion por pantalla
    Mensaje mensaje_final;
    int rcv = msgrcv(id, &mensaje_final, sizeof(mensaje_final), 0, 0);
    if (rcv < 0)
    {
        perror("Error al leer de la cola");
        exit(EXIT_FAILURE);
    }

    printf("Mensaje recibido tipo: %d\n", mensaje_final.tipo);

    printf("Respuesta recibida: %s\n", mensaje_final.data);

    return 0;
}