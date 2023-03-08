LIBVAR   =
LIBPATH  = 

src=$(wildcard *.c)
objs=$(patsubst %.c,%.o,$(src))

all : server client test listener

server: server.o packet.o yyjson.o
	$(CC) -o $@ $^ $(LIBVAR) $(LIBPATH)


test: test.o packet.o yyjson.o
	$(CC) -o $@ $^ $(LIBVAR) $(LIBPATH)

client: client.o packet.o yyjson.o
	$(CC) -o $@ $^ $(LIBVAR) $(LIBPATH)	



listener: listener.o packet.o yyjson.o
	$(CC) -o $@ $^ $(LIBVAR) $(LIBPATH)	


%.o:%.c
	$(CC) $(CFLAGS) -c $<

.PHONY:clean
clean:
	-rm -f listener server client test $(objs)