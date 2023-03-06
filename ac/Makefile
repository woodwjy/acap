LIBVAR   = -lutil -lpthread -lsqlite3 -lmosquitto
LIBPATH  = -L../util -L../ext

src=$(wildcard *.c)
objs=$(patsubst %.c,%.o,$(src))
target=ac

$(target):$(objs)
	$(CC) -o $@ $^ $(LIBVAR) $(LIBPATH)

%.o:%.c
	$(CC) $(CFLAGS) -c $<

.PHONY:clean
clean:
	-rm -f $(target) $(objs)