LIBVAR   =
LIBPATH  = 

src=$(wildcard *.c)
objs=$(patsubst %.c,%.o,$(src))

all : server client

server: server.o packet.o yyjson.o
	$(CC) -o $@ $^ $(LIBVAR) $(LIBPATH)

client: client.o packet.o yyjson.o
	$(CC) -o $@ $^ $(LIBVAR) $(LIBPATH)	


%.o:%.c
	$(CC) $(CFLAGS) -c $<

.PHONY:clean
clean:
	-rm -f listener server client test $(objs)