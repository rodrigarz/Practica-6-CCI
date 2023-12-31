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


//Funcion para convertir a mayusculas la cadena eco
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


///char mensaje_recibe[MAXSIZEDATA];
char mensaje[MAXSIZEDATA];

main()
{
    Mensaje mensaje_recibe;
    Mensaje mensaje_devuelve;
    int id;

    //Conectamos a la cola con la KEY 1
    if ((id = msgget(MKEY1, 0)) < 0)
    {
        perror("No se puede acceder a la cola");
        exit(EXIT_FAILURE);
    }

    //Recibimos el primer mensaje de la cola
    int res = msgrcv(id, &mensaje_recibe, sizeof(mensaje_recibe), 0, 0);
    if (res < 0)
    {
        perror("No se puede recibir de la cola");
        exit(EXIT_FAILURE);
    }


    //En funcion del comando indicado realizamos servicio eco, dir, cd o get
    if (mensaje_recibe.comando == 0)
    {
        char* temp;
        temp = convertirAMayusculas(mensaje_recibe.data);

        strcpy(mensaje_devuelve.data, temp);
    }
    else if (mensaje_recibe.comando == 1)
    {
        //Funciones para hacer dir y devolver el resultado
        char *directorio = mensaje_recibe.data;
        char comando_ls[MAXSIZEDATA];
        snprintf(comando_ls, sizeof(comando_ls), "ls %s", directorio);

        FILE* fp = popen(comando_ls, "r");
        if (fp == NULL)
        {
            perror("Error al hacer dir");
            exit(EXIT_FAILURE);
        }
        size_t leido = fread(mensaje_devuelve.data, 1, MAXSIZEDATA, fp);

        pclose(fp);
	}
    else if (mensaje_recibe.comando == 2)
    {
        //Funcion para cambiar de directorio con cd
        if (chdir(mensaje_recibe.data) != 0)
        {
            perror("Error al cambiar el directorio");
            exit(EXIT_FAILURE);
       }
    }
    else if (mensaje_recibe.comando == 3)
    {
        //Funciones para leer el contenido de un fichero
        FILE* archivo = fopen(mensaje_recibe.data, "r");
        if (archivo == NULL)
        {
            perror("Error al abir el archivo");
            exit(EXIT_FAILURE);
        }
        size_t leido = fread(mensaje_devuelve.data, 1, MAXSIZEDATA, archivo);
        fclose(archivo);
        if (leido < 0)
        {
            perror("No se pudo leer el archivo");
            exit(EXIT_FAILURE);
        }
    }


    //Armamos el mensaje de vuelta y lo enviamos a la cola
    mensaje_devuelve.tipo = 2;
    mensaje_devuelve.origen = getpid();

    int snd = msgsnd(id, &mensaje_devuelve, sizeof(mensaje_devuelve), 0);
    if (snd < 0)
    {
        perror("Error al enviar a la cola");
        exit(EXIT_FAILURE);
    }

    exit(0);

}