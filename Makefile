
CFLAGS = -O

all: entidad_a entidad_b

entidad_a: entidad_a.o
	cc $(CFLAGS) -o entidad_a entidad_a.o

entidad_b: entidad_b.o
	cc $(CFLAGS) -o entidad_b entidad_b.o

clean: 
	rm -f *.o entidad_a entidad_b