#ifndef DATA_H
#define DATA_H

#include <glib.h>
#include <stdint.h>

struct bee;
struct set;
typedef struct account {
	struct prpl *prpl;
	char *user;
	char *pass;

  char *tag;

	int reconnect;

  struct set *set;

  struct bee *bee ;
  struct im_connection *ic;
	struct account *next;
} account_t;

struct im_connection {
	account_t *acc;
  uint32_t flags;
	void *proto_data;
	GSList *deny;
	struct bee *bee;
	GSList *groupchats;
};

#endif /* DATA_H */
