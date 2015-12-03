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

gboolean root_command_add(const char *command, int params, void (*func)(irc_t *, char **args), int flags);
G_MODULE_EXPORT gint b_input_add(int fd, b_input_condition cond, b_event_handler func, gpointer data);
G_MODULE_EXPORT gint b_timeout_add(gint timeout, b_event_handler func, gpointer data);
G_MODULE_EXPORT void b_event_remove(gint id);
gint b_input_add(gint source, b_input_condition condition, b_event_handler function, gpointer data);
gint b_timeout_add(gint timeout, b_event_handler func, gpointer data);
void b_event_remove(gint tag);
gint b_input_add(gint fd, b_input_condition condition, b_event_handler function, gpointer data);
gint b_timeout_add(gint timeout, b_event_handler function, gpointer data);
void b_event_remove(gint id);
struct http_request *http_dorequest(char *host, int port, int ssl, char *request, http_input_function func, gpointer data);
void http_close(struct http_request *req);
struct http_request *http_dorequest(char *host, int port, int ssl, char *request, http_input_function func, gpointer data);
void http_close(struct http_request *req);
json_value * json_parse_ex(json_settings * settings, const json_char * json, size_t length, char * error);
void json_value_free(json_value * value);
json_value * json_parse_ex(json_settings * settings, const json_char * json, size_t length, char * error);
void json_value_free(json_value *);
void http_decode(char *s);
void http_encode(char *s);
void random_bytes(unsigned char *buf, int count);
int bool2int(char *value);
G_MODULE_EXPORT void http_decode(char *s);
G_MODULE_EXPORT void http_encode(char *s);
G_MODULE_EXPORT void random_bytes(unsigned char *buf, int count);
G_MODULE_EXPORT int bool2int(char *value);
void sha1_init(sha1_state_t *ctx);
void sha1_append(sha1_state_t *ctx, const guint8 * message_array, guint len);
char *sha1_random_uuid(sha1_state_t * context);
void sha1_init(sha1_state_t *);
void sha1_append(sha1_state_t *, const guint8 *, unsigned int);
char *sha1_random_uuid(sha1_state_t *);
G_MODULE_EXPORT void *ssl_connect(char *host, int port, gboolean verify, ssl_input_function func, gpointer data);
G_MODULE_EXPORT int ssl_write(void *conn, const char *buf, int len);
G_MODULE_EXPORT void ssl_disconnect(void *conn_);
G_MODULE_EXPORT int ssl_getfd(void *conn);
void *ssl_connect(char *host, int port, gboolean verify, ssl_input_function func, gpointer data)
int ssl_write(void *conn, const char *buf, int len);
void ssl_disconnect(void *conn_);
int ssl_getfd(void *conn);
void *ssl_connect(char *host, int port, gboolean verify,
int ssl_write(void *conn, const char *buf, int len);
void ssl_disconnect(void *conn_);
int ssl_getfd(void *conn);
void *ssl_connect(char *host, int port, gboolean verify, ssl_input_function func, gpointer data);
int ssl_write(void *conn, const char *buf, int len);
void ssl_disconnect(void *conn_);
int ssl_getfd(void *conn);
int url_set(url_t *url, const char *set_url);
int url_set(url_t *url, const char *set_url);
void log_message(int level, const char *message, ...);
void log_message(int level, const char *message, ...) G_GNUC_PRINTF(2, 3);
void op_log_message(void *opdata, const char *message);
void account_on(bee_t *bee, account_t *a);
void account_off(bee_t *bee, account_t *a);
void account_on(bee_t *bee, account_t *a);
void account_off(bee_t *bee, account_t *a);
bee_user_t *bee_user_new(bee_t *bee, struct im_connection *ic, const char *handle, bee_user_flags_t flags);
bee_user_t *bee_user_by_handle(bee_t *bee, struct im_connection *ic, const char *handle);
G_MODULE_EXPORT void imcb_buddy_status(struct im_connection *ic, const char *handle, int flags, const char *state,
G_MODULE_EXPORT void imcb_buddy_msg(struct im_connection *ic, const char *handle, const char *msg, guint32 flags,
G_MODULE_EXPORT struct groupchat *imcb_chat_new(struct im_connection *ic, const char *handle);
G_MODULE_EXPORT void imcb_chat_name_hint(struct groupchat *c, const char *name);
G_MODULE_EXPORT void imcb_chat_free(struct groupchat *c);
G_MODULE_EXPORT void imcb_chat_msg(struct groupchat *c, const char *who, char *msg, guint32 flags, time_t sent_at);
G_MODULE_EXPORT void imcb_chat_topic(struct groupchat *c, char *who, char *topic, time_t set_at);
G_MODULE_EXPORT void imcb_chat_add_buddy(struct groupchat *c, const char *handle);
G_MODULE_EXPORT void imcb_chat_remove_buddy(struct groupchat *c, const char *handle, const char *reason);
G_MODULE_EXPORT struct groupchat *bee_chat_by_title(bee_t *bee, struct im_connection *ic, const char *title);
struct groupchat *imcb_chat_new(struct im_connection *ic, const char *handle);
void imcb_chat_name_hint(struct groupchat *c, const char *name);
void imcb_chat_free(struct groupchat *c);
void imcb_chat_msg(struct groupchat *c, const char *who, char *msg, guint32 flags, time_t sent_at);
void imcb_chat_topic(struct groupchat *c, char *who, char *topic, time_t set_at);
void imcb_chat_add_buddy(struct groupchat *c, const char *handle);
void imcb_chat_remove_buddy(struct groupchat *c, const char *handle, const char *reason);
struct groupchat *bee_chat_by_title(bee_t *bee, struct im_connection *ic, const char *title);
bee_user_t *bee_user_new(bee_t *bee, struct im_connection *ic, const char *handle, bee_user_flags_t flags);
bee_user_t *bee_user_by_handle(bee_t *bee, struct im_connection *ic, const char *handle);
void imcb_buddy_status(struct im_connection *ic, const char *handle, int flags, const char *state, const char *message);
void imcb_buddy_msg(struct im_connection *ic, const char *handle, const char *msg, guint32 flags, time_t sent_at);
void imcb_buddy_typing(struct im_connection *ic, const char *handle, guint32 flags);
void register_protocol(struct prpl *p);
void imcb_log(struct im_connection *ic, char *format, ...);
void imcb_error(struct im_connection *ic, char *format, ...);
void imcb_connected(struct im_connection *ic);
void imc_logout(struct im_connection *ic, int allow_reconnect);
void imcb_ask(struct im_connection *ic, char *msg, void *data,
void imcb_add_buddy(struct im_connection *ic, const char *handle, const char *group);
void imcb_rename_buddy(struct im_connection *ic, const char *handle, const char *fullname);
void imcb_remove_buddy(struct im_connection *ic, const char *handle, char *group);
void imcb_buddy_nick_hint(struct im_connection *ic, const char *handle, const char *nick);
void imcb_ask_auth(struct im_connection *ic, const char *handle, const char *realname);
struct bee_user *imcb_buddy_by_handle(struct im_connection *ic, const char *handle);
G_MODULE_EXPORT void register_protocol(struct prpl *);
G_MODULE_EXPORT void imcb_connected(struct im_connection *ic);
G_MODULE_EXPORT void imc_logout(struct im_connection *ic, int allow_reconnect);
G_MODULE_EXPORT void imcb_log(struct im_connection *ic, char *format, ...) G_GNUC_PRINTF(2, 3);
G_MODULE_EXPORT void imcb_error(struct im_connection *ic, char *format, ...) G_GNUC_PRINTF(2, 3);
G_MODULE_EXPORT void imcb_ask(struct im_connection *ic, char *msg, void *data, query_callback doit,
G_MODULE_EXPORT void imcb_ask_auth(struct im_connection *ic, const char *handle, const char *realname);
G_MODULE_EXPORT void imcb_add_buddy(struct im_connection *ic, const char *handle, const char *group);
G_MODULE_EXPORT void imcb_remove_buddy(struct im_connection *ic, const char *handle, char *group);
G_MODULE_EXPORT void imcb_rename_buddy(struct im_connection *ic, const char *handle, const char *realname);
G_MODULE_EXPORT void imcb_buddy_nick_hint(struct im_connection *ic, const char *handle, const char *nick);
G_MODULE_EXPORT void imcb_buddy_typing(struct im_connection *ic, const char *handle, guint32 flags);
G_MODULE_EXPORT struct bee_user *imcb_buddy_by_handle(struct im_connection *ic, const char *handle);
#define debug(text ...) imcb_log(ic, text);
gboolean root_command_add(const char *command, int params, void (*func)(irc_t *, char **args), int flags);
set_t *set_find(set_t **head, const char *key);
char *set_getstr(set_t **head, const char *key);
int set_getint(set_t **head, const char *key);
int set_getbool(set_t **head, const char *key);
int set_setstr(set_t **head, const char *key, char *value);
int set_setint(set_t **head, const char *key, int value);
set_t *set_find(set_t **head, const char *key);
G_MODULE_EXPORT char *set_getstr(set_t **head, const char *key);
G_MODULE_EXPORT int set_getint(set_t **head, const char *key);
G_MODULE_EXPORT int set_getbool(set_t **head, const char *key);
int set_setstr(set_t **head, const char *key, char *value);
int set_setint(set_t **head, const char *key, int value);
