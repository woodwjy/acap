
CFLAGS   = -g -I../ext/
LDFLAGS  = -ldl
LDFLAGS += -lcjson
LDFLAGS += -lmosquitto
# LDFLAGS += -L../ext/
LDFLAGS += -L/home/wood/work/acap/ext

src=$(wildcard *.c)
objs=$(patsubst %.c,%.o,$(src))

all: libapte_system.so libapte_wifi.so ap

libapte_wifi.so : wifi/wifi.c apte_list.c
	$(CC) $(CFLAGS) -c -fpic wifi/wifi.c -o wifi.o 
	$(CC) $(CFLAGS) -c -fpic apte_list.c -o apte_list.o 
	$(CC) -shared -lc -o libapte_wifi.so wifi.o apte_list.o $(LDFLAGS)

libapte_system.so : system/system.c apte_list.c
	$(CC) $(CFLAGS) -c -fpic system/system.c -o system.o 
	$(CC) $(CFLAGS) -c -fpic apte_list.c -o apte_list.o 
	$(CC) -shared -lc -o libapte_system.so system.o apte_list.o $(LDFLAGS)

ap:$(objs)
	$(CC) -o $@ $^  $(LDFLAGS)

%.o:%.c
	$(CC) $(CFLAGS) -c $<

.PHONY:clean
clean:
	-rm -f $(objs) *.so  *.o ap