
# Building
SOURCES=$(wildcard *.c)
OBJS := $(SOURCES:.c=.o)
LIB_NAME=brine
LIB_TARGET=lib$(LIB_NAME).so
TEST_TARGET=test

# Installing
PREFIX=/usr/local
PC_DIR=$(PREFIX)/lib/pkgconfig
LIB_DIR=$(PREFIX)/lib/lib$(LIB_NAME)
PLUGIN_DIR=$(LIB_DIR)/plugins
INCLUDE_DIR=$(PREFIX)/include/lib$(LIB_NAME)
API_INCLUDE_DIR=$(INCLUDE_DIR)/api

INSTALL=install -p

CC=$(shell which clang || which gcc)
CFLAGS=-g -O2 -fno-strict-aliasing -Wno-deprecated-declarations
LIBFLAGS=-fPIC $(shell pkg-config --cflags openssl glib-2.0 gmodule-2.0)
LFLAGS=-lm $(shell pkg-config --libs openssl glib-2.0 gmodule-2.0)

all: $(LIB_TARGET) $(TEST_TARGET)

$(TEST_TARGET): $(LIB_TARGET)
	$(CC) $(CFLAGS) $(LIBFLAGS) $(LFLAGS) -L. -l$(LIB_NAME) test.c -o test

$(LIB_TARGET): $(OBJS) *.c *.h
	$(CC) $(CFLAGS) $(LIBFLAGS) $(LFLAGS) $(OBJS) -Wl,-soname,$(LIB_TARGET) -shared -o $(LIB_TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $(LIBFLAGS) -MMD -MP -MT $@ -c $<

clean:
	rm -f *.o  *.d $(LIB_TARGET) $(TEST_TARGET)

# Presumption - if a .o is already built, there is a suitably up to date .d file with it.
# Still may need to run make twice in some degenerate cases where you move headers around
-include $(OBJS:.o=.d)

install:
	mkdir -p $(API_INCLUDE_DIR) $(INCLUDE_DIR) $(LIB_DIR) $(PC_DIR)
	$(INSTALL) -m 0644 *.h $(API_INCLUDE_DIR)
	$(INSTALL) -m 0644 brine.h $(INCLUDE_DIR)
	$(INSTALL) -m 0644 libbrine.so $(LIB_DIR)
	$(INSTALL) -m 0644 libbrine.pc $(PC_DIR)
	$(INSTALL) -m 0644 libbrine-api.pc $(PC_DIR)

run: all
	LD_LIBRARY_PATH=. ./$(TEST_TARGET)
