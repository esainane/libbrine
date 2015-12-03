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

bee_user_t *bee_user_by_handle(bee_t *bee, struct im_connection *ic, const char *handle);
bee_user_t *bee_user_new(bee_t *bee, struct im_connection *ic, const char *handle, bee_user_flags_t flags);
char *set_getstr(set_t **head, const char *key);
char *sha1_random_uuid(sha1_state_t *);
char *sha1_random_uuid(sha1_state_t * context);
#define debug(text ...) imcb_log(ic, text);
gboolean root_command_add(const char *command, int params, void (*func)(irc_t *, char **args), int flags);
gint b_input_add(gint fd, b_input_condition condition, b_event_handler function, gpointer data);
gint b_input_add(gint source, b_input_condition condition, b_event_handler function, gpointer data);
gint b_input_add(int fd, b_input_condition cond, b_event_handler func, gpointer data);
gint b_timeout_add(gint timeout, b_event_handler func, gpointer data);
gint b_timeout_add(gint timeout, b_event_handler function, gpointer data);
int bool2int(char *value);
int set_getbool(set_t **head, const char *key);
int set_getint(set_t **head, const char *key);
int set_setint(set_t **head, const char *key, int value);
int set_setstr(set_t **head, const char *key, char *value);
int ssl_getfd(void *conn);
int ssl_write(void *conn, const char *buf, int len);
int url_set(url_t *url, const char *set_url);
json_value * json_parse_ex(json_settings * settings, const json_char * json, size_t length, char * error);
set_t *set_find(set_t **head, const char *key);
struct bee_user *imcb_buddy_by_handle(struct im_connection *ic, const char *handle);
struct groupchat *bee_chat_by_title(bee_t *bee, struct im_connection *ic, const char *title);
struct groupchat *imcb_chat_new(struct im_connection *ic, const char *handle);
struct http_request *http_dorequest(char *host, int port, int ssl, char *request, http_input_function func, gpointer data);
void account_off(bee_t *bee, account_t *a);
void account_on(bee_t *bee, account_t *a);
void b_event_remove(gint id);
void b_event_remove(gint tag);
void http_close(struct http_request *req);
void http_decode(char *s);
void http_encode(char *s);
void imcb_add_buddy(struct im_connection *ic, const char *handle, const char *group);
void imcb_ask_auth(struct im_connection *ic, const char *handle, const char *realname);
void imcb_ask(struct im_connection *ic, char *msg, void *data,
void imcb_ask(struct im_connection *ic, char *msg, void *data, query_callback doit,
void imcb_buddy_msg(struct im_connection *ic, const char *handle, const char *msg, guint32 flags,
void imcb_buddy_msg(struct im_connection *ic, const char *handle, const char *msg, guint32 flags, time_t sent_at);
void imcb_buddy_nick_hint(struct im_connection *ic, const char *handle, const char *nick);
void imcb_buddy_status(struct im_connection *ic, const char *handle, int flags, const char *state,
void imcb_buddy_status(struct im_connection *ic, const char *handle, int flags, const char *state, const char *message);
void imcb_buddy_typing(struct im_connection *ic, const char *handle, guint32 flags);
void imcb_chat_add_buddy(struct groupchat *c, const char *handle);
void imcb_chat_free(struct groupchat *c);
void imcb_chat_msg(struct groupchat *c, const char *who, char *msg, guint32 flags, time_t sent_at);
void imcb_chat_name_hint(struct groupchat *c, const char *name);
void imcb_chat_remove_buddy(struct groupchat *c, const char *handle, const char *reason);
void imcb_chat_topic(struct groupchat *c, char *who, char *topic, time_t set_at);
void imcb_connected(struct im_connection *ic);
void imcb_error(struct im_connection *ic, char *format, ...);
void imcb_error(struct im_connection *ic, char *format, ...) G_GNUC_PRINTF(2, 3);
void imcb_log(struct im_connection *ic, char *format, ...);
void imcb_log(struct im_connection *ic, char *format, ...) G_GNUC_PRINTF(2, 3);
void imcb_remove_buddy(struct im_connection *ic, const char *handle, char *group);
void imcb_rename_buddy(struct im_connection *ic, const char *handle, const char *fullname);
void imcb_rename_buddy(struct im_connection *ic, const char *handle, const char *realname);
void imc_logout(struct im_connection *ic, int allow_reconnect);
void json_value_free(json_value *);
void json_value_free(json_value * value);
void log_message(int level, const char *message, ...);
void log_message(int level, const char *message, ...) G_GNUC_PRINTF(2, 3);
void op_log_message(void *opdata, const char *message);
void random_bytes(unsigned char *buf, int count);
void register_protocol(struct prpl *);
void register_protocol(struct prpl *p);
void sha1_append(sha1_state_t *, const guint8 *, unsigned int);
void sha1_append(sha1_state_t *ctx, const guint8 * message_array, guint len);
void sha1_init(sha1_state_t *);
void sha1_init(sha1_state_t *ctx);
void *ssl_connect(char *host, int port, gboolean verify,
void *ssl_connect(char *host, int port, gboolean verify, ssl_input_function func, gpointer data)
void *ssl_connect(char *host, int port, gboolean verify, ssl_input_function func, gpointer data);
void ssl_disconnect(void *conn_);
