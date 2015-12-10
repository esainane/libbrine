void register_protocol(struct prpl *plugin);
gboolean root_command_add(const char *name, int params, void (*f)(irc_t *, char **args), int flags);
void imcb_connected(struct im_connection *ic);
struct im_connection *imcb_new(account_t *acc);

struct groupchat *bee_chat_by_title(bee_t *bee, struct im_connection *ic, const char *title);

#define imcb_buddy_msg(ic, source, msg, flags, sent_at) log_message(LOGLVL_INFO, "MESSAGE <%s> %s", source, msg)

// FIXME: These should be routed to the network buffer
#define imcb_log(ic, fmt, args...) log_message(LOGLVL_INFO, "** %s > " fmt, ic->acc->prpl->name, ## args)
#define imcb_error(ic, fmt, args...) log_message(LOGLVL_ERROR, "** %s > " fmt, ic->acc->prpl->name, ## args)
