src=$(wildcard *.c)
objs=$(patsubst %.c,%.o,$(src))
target=libutil.a

$(target):$(objs)
	$(AR) rcs $@ $^

%.o:%.c
	$(CC) $(CFLAGS) -c $<

.PHONY:clean
clean:
	-rm -f $(target) $(objs)
