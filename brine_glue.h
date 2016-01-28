
struct account;
struct irc;
struct bee;
void register_protocol(struct prpl *plugin);
gboolean root_command_add(const char *name, int params, void (*f)(struct irc *, char **args), int flags);
void imcb_connected(struct im_connection *ic);
struct im_connection *imcb_new(struct account *acc);

struct groupchat *bee_chat_by_title(struct bee *bee, struct im_connection *ic, const char *title);

struct groupchat *imcb_chat_new(struct im_connection *ic, const char *handle);
void imcb_chat_add_buddy(struct groupchat *c, const char *handle);
void imcb_chat_name_hint(struct groupchat *c, const char *name);
void imcb_chat_msg(struct groupchat *c, const char *who, char *msg, guint32 flags, time_t sent_at);
void imcb_chat_free(struct groupchat *c);
#define imcb_buddy_msg(ic, source, msg, flags, sent_at) log_message(LOGLVL_INFO, "MESSAGE <%s> %s", source, msg)

void imcb_log(struct im_connection *ic, char *fmt, ...);
void imcb_error(struct im_connection *ic, char *fmt, ...);


void imcb_add_buddy(struct im_connection *ic, const char *handle, const char *group);
void imcb_remove_buddy(struct im_connection *ic, const char *handle, char *group);
void imcb_rename_buddy(struct im_connection *ic, const char *handle, const char *realname);
void imcb_buddy_nick_hint(struct im_connection *ic, const char *handle, const char *nick);
struct bee_user *imcb_buddy_by_handle(struct im_connection *ic, const char *handle);
