
#ifndef IRC_DOWNSTREAM_H
#define IRC_DOWNSTREAM_H

#include "data.h"

/* Parse an IRC formatted message from one of our downstream users */
void irc_downstream_putline(struct im_connection *, char *line);

struct irc;
struct prpl;
void plugin_command_add(struct prpl *plugin, const char *name, int params, void (*f)(struct im_connection *, char **args));

void commands_finalize(void *base, size_t ncommands);

#endif /* IRC_DOWNSTREAM_H */
