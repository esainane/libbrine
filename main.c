
#include "brine.h"
#include "log.h"

#define STR(x) #x
#define XSTR(x) STR(x)


/*
 * Example libbrine application
 */

static void config(struct set **set) {
  set_setstr(set, "skypeconsole_receive", "true");
  set_setstr(set, "read_groups", "true");
}

int main(void) {
  struct brine b = {
    .net_msgrecv = 0,
    .net_msgsend = 0,
    .user_msgrecv = 0,
    .user_msgsend = 0,
    .chan_msgrecv = 0,
    .chan_msgsend = 0
  };
  brine_init(&b);
#if !defined(USERNAME) || !defined(PASSWORD)
  // log_message(LOGLVL_INFO, "Done (missing username or password for testing)\n");
  brine_eventloop();
#else
  brine_bootstrap("skype", XSTR(USERNAME), XSTR(PASSWORD), &config);
#endif
}
