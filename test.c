
#include "stdlib.h"
#include "stdio.h"

#include "narg.h"


#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <syslog.h>
#include <glib.h>
#include <gmodule.h>

#define MOCK(x, args...) x(args);
#define TESTVA(x, args...) test(NARG_NARG(args));
#define MOCKVA(x, args...) x(args) G_GNUC_PRINTF(NARG_NARG(args) - 1, NARG_NARG(args));
#include "test.h"

int main(void) {
  TESTVA(void test, 1,2,...);
  test(NARG_NARG(1,...));
  test(NARG_NARG(...));
  test(NARG_NARG());
  // test2(4);
  return 0;
}

void test(int y) {
  printf("%d\n", y);
}

void test2(int a, const char *fmt, ...) {
  printf("%d\n", a);
}
