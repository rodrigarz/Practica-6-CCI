
CFLAGS = -O

all: entidad_a entidad_b

entidad_a: entidad_a.o
	cc $(CFLAGS) -o entidad_a entidad_a.o

entidad_b: entidad_b.o
	cc $(CFLAGS) -o entidad_b entidad_b.o

cliente: cliente.o
	cc $(CFLAGS) -o cliente cliente.o

servidor: servidor.o
	cc $(CFLAGS) -o servidor servidor.o

clean: 
	rm -f *.o entidad_a entidad_b cliente servidor