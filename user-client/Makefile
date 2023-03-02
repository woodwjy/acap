user: main.o
	$(CC)  main.o -o ../user
main.o: main.c
	$(CC) $(CFLAGS)  -c main.c -o main.o
clean:
	rm -rf ../user *.o