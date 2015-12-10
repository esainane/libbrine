
# Yet another quick hack :^o)

SOURCES=$(shell ls *.c | grep -v brine.c)
OBJS := $(SOURCES:.c=.o)

all: libbrine.so test

test: libbrine.so
	gcc -g -O2 -fno-strict-aliasing -L. -lbrine main.c -o test

libbrine.so: $(OBJS) *.c *.h
	gcc -g -O2 -fno-strict-aliasing -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -lm -lssl -lcrypto -lglib-2.0 -lgmodule-2.0 -fPIC $(OBJS) brine.c -shared -o libbrine.so

%.o: %.c
	gcc -g -O2 -fno-strict-aliasing -fPIC -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -lglib-2.0 -MMD -MP -MT $@ -c $<

clean:
	rm -f *.o libbrine.so *.d

-include $(OBJS:.o=.d)
