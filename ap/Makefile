
CFLAGS   = -g -I../ext/
LDFLAGS  = -lapte_util
LDFLAGS += -lcjson
LDFLAGS += -ldl
LDFLAGS += -lmosquitto
# LDFLAGS += -L../ext/
LDFLAGS += -L/home/wood/work/acap/ap
LDFLAGS += -L/home/wood/work/acap/ext


src=$(wildcard *.c)
objs=$(patsubst %.c,%.o,$(src))

all: libapte_util.so libapte_system.so libapte_wifi.so libapte_net.so libapte_template.so ap

libapte_util.so : util/util.c
	$(CC)  $(CFLAGS) -o util/util.o  -c -fpic util/util.c
	$(CC) -shared -lc -o libapte_util.so util/util.o

libapte_wifi.so : wifi/wifi.c libapte_util.so
	$(CC) $(CFLAGS) -c -fpic wifi/wifi.c -o wifi.o 
	$(CC) -shared -lc -o libapte_wifi.so wifi.o  $(LDFLAGS)

libapte_system.so : system/system.c libapte_util.so
	$(CC) $(CFLAGS) -c -fpic system/system.c -o system.o 
	$(CC) -shared -lc -o libapte_system.so system.o  $(LDFLAGS)


libapte_template.so : template/template.c libapte_util.so
	$(CC) $(CFLAGS) -c -fpic template/template.c -o template.o 
	$(CC) -shared -lc -o libapte_template.so template.o  $(LDFLAGS)	

libapte_net.so : net/wan.c libapte_util.so
	$(CC) $(CFLAGS) -c -fpic net/wan.c -o wan.o 
	$(CC) -shared -lc -o libapte_net.so wan.o  $(LDFLAGS)		

ap:$(objs)
	$(CC) -o $@ $^  $(LDFLAGS)

%.o:%.c
	$(CC) $(CFLAGS) -c $<

.PHONY:clean
clean:
	-rm -f $(objs) *.a *.so  *.o ap util/util.o