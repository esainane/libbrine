void register_protocol(struct prpl *plugin);
gboolean root_command_add(const char *name, int params, void (*f)(irc_t *, char **args), int flags);
struct im_connection *imcb_new(account_t *acc);
