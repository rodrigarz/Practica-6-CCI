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
#define SERV_HOST_ADDR "157.88.207.244"
//Direccion carpanta (donde esta el servidor)
//#define SERV_HOST_ADDR "157.88.200.41"


Mensaje mensaje_recibe;
Mensaje mensaje;

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

main()
{
    char* mensaje_manda;
    struct sockaddr_in servaddr = { 0 };
    struct sockaddr_in cliaddr = { 0 };

    //Creamos el socket UDP
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("failed to create socket");
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
    int n = recvfrom(sockfd, (char*)&mensaje_recibe, sizeof(mensaje_recibe) - 1, MSG_WAITALL,
        (struct sockaddr*)&cliaddr, &len);

    printf("Received message from client:\n");
    printf("Tipo: %d\n", mensaje_recibe.tipo);

    //Asignamos tipo 1 a solicitud de eco
    if (mensaje_recibe.tipo == 1)
    {
        mensaje_manda = convertirAMayusculas(mensaje_recibe.data);
        int i = 0;
        for (i; i <= strlen(mensaje_manda); i++)
        {
            mensaje.data[i] = mensaje_manda[i];
        }
        mensaje.tipo = 0;
    }

 

    // Aquí es donde enviarías la respuesta al cliente
    // Puedes utilizar la información almacenada en cliaddr
    // para obtener la dirección IP y el puerto del cliente
    // y luego enviar la respuesta utilizando sendto.

    sendto(sockfd, (char*)&mensaje, sizeof(mensaje), 0,
        (const struct sockaddr*)&cliaddr, sizeof(cliaddr));

    close(sockfd);
    exit(0);
}

