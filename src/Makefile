all: main.o funcs.o
	gcc main.o funcs.o -o main -lpng

main.o: main.c
	gcc -c main.c -lpng

funcs.o: funcs.c funcs.h libs.h
	gcc -c funcs.c -lpng

clean:
	rm -rf *.o
