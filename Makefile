
# Yet another quick hack :^o)

all: beemock

beemock: ofiles
	gcc -O2 -fno-strict-aliasing -fPIE -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -lm -lssl -lcrypto -lglib-2.0 -lgmodule-2.0 *.o main.c -o beemock -pie -Wl,--export-dynamic

ofiles:
	gcc -O2 -fno-strict-aliasing -fPIE -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -lglib-2.0 -c *.c 2>&1; rm main.o
