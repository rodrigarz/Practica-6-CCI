#define MAXSIZEDATA (50)

#define MENSAJESIZE (sizeof(Mensaje)- MAXSIZEDATA)

typedef struct {
    int patron;
    double destino;
    double origen;
    double longitud;
    int num_frag;
    int orden_frag;
    int tipo;
    char data[MAXSIZEDATA];
} Mensaje;