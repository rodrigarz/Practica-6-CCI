#define MAXSIZEDATA (150)

#define MENSAJESIZE (sizeof(Mensaje)- MAXSIZEDATA)

typedef struct {
    int patron;
    int destino;
    int origen;
    double longitud;
    int puerto;
    int comando; // 1 para dir, 2 para cd, 3 para get, 4 para quit, 0 para eco
    int tipo;
    char data[MAXSIZEDATA];
} Mensaje;