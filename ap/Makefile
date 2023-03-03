LIBVAR   = -lutil
LIBPATH  = -L../util

src=$(wildcard *.c)
objs=$(patsubst %.c,%.o,$(src))
target=ap

$(target):$(objs)
	$(CC) -o $@ $^ $(LIBVAR) $(LIBPATH)

%.o:%.c
	$(CC) $(CFLAGS) -c $<

.PHONY:clean
clean:
	-rm -f $(target) $(objs)