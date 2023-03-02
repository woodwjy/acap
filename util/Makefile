CC = gcc
AR = ar

CFLAGS = -g -Wall

src=$(wildcard *.c)
objs=$(patsubst %.c,%.o,$(src))
target=libutil.a

$(target):$(objs)
	$(AR) rcs $@ $^

%.o:%.c
	$(CC) -c $<

.PHONY:clean
clean:
	-rm -f $(target) $(objs)