#include "bitlbee.h"

#define MOCK(x, args...) x(args) { \
  fprintf(stderr, #x " is not yet implemented!\n");\
}
#define MOCKVA MOCK

#include "mock.h"
