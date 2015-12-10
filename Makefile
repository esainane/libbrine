
# Yet another quick hack :^o)

SOURCES=$(wildcard *.c)
OBJS := $(SOURCES:.c=.o)
LIB_NAME=brine
LIB_TARGET=lib$(LIB_NAME).so
TEST_TARGET=test

CC=gcc
CFLAGS=-g -O2 -fno-strict-aliasing
# FIXME: Worst remnant of the quick hack, try pkg-config
LIBFLAGS=-fPIC -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -lm -lssl -lcrypto -lglib-2.0 -lgmodule-2.0

all: $(LIB_TARGET) $(TEST_TARGET)

$(TEST_TARGET): $(LIB_TARGET)
	$(CC) $(CFLAGS) $(LIBFLAGS) -L. -l$(LIB_NAME) test.c -o test

$(LIB_TARGET): $(OBJS) *.c *.h
	$(CC) $(CFLAGS) $(LIBFLAGS) $(OBJS) -Wl,-soname,$(LIB_TARGET) -shared -o $(LIB_TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $(LIBFLAGS) -MMD -MP -MT $@ -c $<

clean:
	rm -f *.o  *.d $(LIB_TARGET) $(TEST_TARGET)

# Presumption - if a .o is already built, there is a suitably up to date .d file with it.
# Still may need to run make twice in some degenerate cases where you move headers around
-include $(OBJS:.o=.d)

run: all
	LD_LIBRARY_PATH=. ./$(TEST_TARGET)
