#include "bitlbee.h"

#define MOCK(x, args...) x(args) { \
  printf(#x " is not yet implemented!\n");\
}
#define MOCKVA MOCK

#include "mock.h"


void closesocket(int fd)
{
	close(fd);
}
