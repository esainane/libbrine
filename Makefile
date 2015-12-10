
# Yet another quick hack :^o)

all: libbrine

test: libbrine
	gcc -g -O2 -fno-strict-aliasing -fPIE -L. -lbrine main.c -o test

libbrine: *.o
	gcc -g -O2 -fno-strict-aliasing -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -lm -lssl -lcrypto -lglib-2.0 -lgmodule-2.0  -fPIC *.o brine.c -o libbrine.so -pie -shared

%.o: %.c %.h
	gcc -g -O2 -fno-strict-aliasing -fPIC -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -lglib-2.0 -c *.c 2>&1; rm brine.o
