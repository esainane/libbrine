
#include "brine.h"
#include "log.h"

#include <glib.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define STR(x) #x
#define XSTR(x) STR(x)


/*
 * Example libbrine application
 */

static void config(struct set **set) {
  set_setstr(set, "skypeconsole_receive", "true");
  set_setstr(set, "read_groups", "true");
}

int main(int argc, char **argv) {
  struct brine b = {
    .net_msgrecv = 0,
    .net_msgsend = 0,
    .user_msgrecv = 0,
    .user_msgsend = 0,
    .chan_msgrecv = 0,
    .chan_msgsend = 0
  };
  int i = 0;

#define ARG(VAR, SHOPT, NAME) \
if (!strcmp(argv[i], SHOPT) || !strcmp(argv[i], "--" NAME)) { \
  POP_ARGV_OR_ERROR(VAR, NAME) \
  continue; \
}
#define POP_ARGV_OR_ERROR(VAR, NAME) \
if (++i == argc) { \
  fprintf(stderr, "Missing parameter for " NAME "!"); \
  exit(1); \
} \
VAR = argv[i];
  const char *username = "testuser";
  const char *password = 0;
  gchar *prompt;
  const char *plugin = "skype";
  while (++i != argc) {
    ARG(username, "-u", "username")
    ARG(plugin, "-s", "service")
    ARG(plugin, "-p", "protocol")
    ARG(plugin, "-p", "plugin")
    fprintf(stderr, "Unrecognised option (%s)\n", argv[i]);
    fprintf(stderr, "Usage: %s -u|--username <username> -s|--service|-p|--protocol|--plugin <service|protocol|plugin>\n", argv[0]);
    exit(1);
  }
  prompt = g_strdup_printf("Password for (%s) on (%s):\n", username, plugin);
  while (!password) {
    password = getpass(prompt);
  }
  g_free(prompt);

  brine_init(&b);
  brine_bootstrap(g_strdup(plugin), g_strdup(username), g_strdup(password), &config);

  return 0;
}
