
void register_protocol(struct prpl *plugin);
gboolean root_command_add(const char *name, int params, void (*f)(irc_t *, char **args), int flags);
void imcb_connected(struct im_connection *ic);
struct im_connection *imcb_new(account_t *acc);

struct groupchat *bee_chat_by_title(bee_t *bee, struct im_connection *ic, const char *title);

#define imcb_buddy_msg(ic, source, msg, flags, sent_at) log_message(LOGLVL_INFO, "MESSAGE <%s> %s", source, msg)

void imcb_log(struct im_connection *ic, char *fmt, ...);
void imcb_error(struct im_connection *ic, char *fmt, ...);
