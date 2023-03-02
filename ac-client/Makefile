CC = gcc
CFLAGS = -g -Wall
LIBVAR   = -lutil
LIBPATH  = -L../util

ap: main.o func.o
	$(CC)  main.o func.o -o ../ap $(LIBVAR) $(LIBPATH)
main.o: main.c
	$(CC) $(CFLAGS)  -c main.c -o main.o
func.o: func.c
	$(CC) $(CFLAGS) -c func.c -o func.o
clean:
	rm -rf ../ap *.o