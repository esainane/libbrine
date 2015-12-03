#ifndef BITLBEE_H
#define BITLBEE_H

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

struct http_request;
struct im_connection;
struct prpl;
struct set;
typedef struct set set_t;
typedef struct bee bee_t;
typedef enum {
        BEE_USER_ONLINE = 1,    /* Compatibility with old OPT_LOGGED_IN flag */
        BEE_USER_AWAY = 4,      /* Compatibility with old OPT_AWAY flag */
        BEE_USER_MOBILE = 8,    /* Compatibility with old OPT_MOBILE flag */
        BEE_USER_LOCAL = 256,   /* Locally-added contacts (not in real contact list) */
        BEE_USER_SPECIAL = 512, /* Denotes a user as being special */
        BEE_USER_NOOTR = 4096,  /* Per-user version of OPT_NOOTR */
} bee_user_flags_t;
typedef struct bee_user bee_user_t;
typedef struct account account_t;

typedef enum {
  B_EV_IO_READ = 1 << 0,
  B_EV_IO_WRITE = 1 << 1,
  B_EV_FLAG_FORCE_ONCE = 1 << 16,
  B_EV_FLAG_FORCE_REPEAT = 1 << 17,
} b_input_condition;

typedef char *(*set_eval) (struct set *set, char *value);
typedef gboolean (*b_event_handler)(gpointer data, gint fd, b_input_condition cond);
typedef void (*http_input_function)(struct http_request *);
typedef void (*query_callback) (void *data);

typedef enum {
        USTATUS_OFFLINE = 0,
        USTATUS_AUTHORIZED = 1, /* Gave the correct server password (PASS). */
        USTATUS_LOGGED_IN = 2,  /* USER+NICK(+PASS) finished. */
        USTATUS_IDENTIFIED = 4, /* To NickServ (root). */
        USTATUS_SHUTDOWN = 8,   /* Now used to indicate we're shutting down.
                                   Currently just blocks irc_vawrite(). */
        USTATUS_CAP_PENDING = 16,
        USTATUS_SASL_PLAIN_PENDING = 32,

        /* Not really status stuff, but other kinds of flags: For slightly
           better password security, since the only way to send passwords
           to the IRC server securely (i.e. not echoing to screen or written
           to logfiles) is the /OPER command, try to use that command for
           stuff that matters. */
        OPER_HACK_IDENTIFY = 0x100,
        OPER_HACK_IDENTIFY_NOLOAD = 0x01100,
        OPER_HACK_IDENTIFY_FORCE  = 0x02100,
        OPER_HACK_REGISTER = 0x200,
        OPER_HACK_ACCOUNT_PASSWORD = 0x400,
        OPER_HACK_ANY = 0x3700, /* To check for them all at once. */

        IRC_UTF8_NICKS = 0x10000, /* Disable ASCII restrictions on buddy nicks. */
} irc_status_t;
typedef struct irc irc_t;
typedef struct url url_t;

#ifndef json_char
#define json_char char
#endif
#ifndef json_int_t
   #ifndef _MSC_VER
      #include <inttypes.h>
      #define json_int_t int64_t
   #else
      #define json_int_t __int64
   #endif
#endif

struct json_settings;
typedef enum {
        json_none,
        json_object,
        json_array,
        json_integer,
        json_double,
        json_string,
        json_boolean,
        json_null

} json_type;
typedef struct _json_value json_value;
typedef GChecksum *sha1_state_t;
typedef gboolean (*ssl_input_function)(gpointer, int, void*, b_input_condition);


bee_user_t *bee_user_by_handle(bee_t *bee, struct im_connection *ic, const char *handle);
bee_user_t *bee_user_new(bee_t *bee, struct im_connection *ic, const char *handle, bee_user_flags_t flags);
char *set_eval_account(set_t *set, char *value);
char *set_getstr(set_t **head, const char *key);
char *sha1_random_uuid(sha1_state_t * context);
#define debug(text ...) imcb_log(ic, text);
gboolean root_command_add(const char *command, int params, void (*func)(irc_t *, char **args), int flags);
gboolean ssl_sockerr_again(void *ssl);
gint b_input_add(gint source, b_input_condition condition, b_event_handler function, gpointer data);
gint b_timeout_add(gint timeout, b_event_handler function, gpointer data);
int bool2int(char *value);
int is_bool(char *value);
int set_getbool(set_t **head, const char *key);
int set_getint(set_t **head, const char *key);
int set_reset(set_t **head, const char *key);
int set_setint(set_t **head, const char *key, int value);
int set_setstr(set_t **head, const char *key, char *value);
int ssl_getfd(void *conn);
int ssl_read(void *conn, char *buf, int len);
int ssl_write(void *conn, const char *buf, int len);
int url_set(url_t *url, const char *set_url);
json_value * json_parse_ex(json_settings * settings, const json_char * json, size_t length, char * error);
set_t *set_add(set_t **head, const char *key, const char *def, set_eval eval, void *data);
set_t *set_find(set_t **head, const char *key);
struct bee_user *imcb_buddy_by_handle(struct im_connection *ic, const char *handle);
struct groupchat *bee_chat_by_title(bee_t *bee, struct im_connection *ic, const char *title);
struct groupchat *imcb_chat_new(struct im_connection *ic, const char *handle);
struct http_request *http_dorequest(char *host, int port, int ssl, char *request, http_input_function func, gpointer data);
struct im_connection *imcb_new(account_t *acc);
void account_off(bee_t *bee, account_t *a);
void account_on(bee_t *bee, account_t *a);
void b_event_remove(gint id);
void http_close(struct http_request *req);
void http_decode(char *s);
void http_encode(char *s);
void imcb_add_buddy(struct im_connection *ic, const char *handle, const char *group);
void imcb_ask_auth(struct im_connection *ic, const char *handle, const char *realname);
void imcb_ask(struct im_connection *ic, char *msg, void *data, query_callback doit, query_callback dont);
void imcb_buddy_msg(struct im_connection *ic, const char *handle, const char *msg, guint32 flags, time_t sent_at);
void imcb_buddy_nick_hint(struct im_connection *ic, const char *handle, const char *nick);
void imcb_buddy_status(struct im_connection *ic, const char *handle, int flags, const char *state, const char *message);
void imcb_buddy_typing(struct im_connection *ic, const char *handle, guint32 flags);
void imcb_chat_add_buddy(struct groupchat *c, const char *handle);
void imcb_chat_free(struct groupchat *c);
void imcb_chat_msg(struct groupchat *c, const char *who, char *msg, guint32 flags, time_t sent_at);
void imcb_chat_name_hint(struct groupchat *c, const char *name);
void imcb_chat_remove_buddy(struct groupchat *c, const char *handle, const char *reason);
void imcb_chat_topic(struct groupchat *c, char *who, char *topic, time_t set_at);
void imcb_connected(struct im_connection *ic);
void imcb_error(struct im_connection *ic, char *format, ...) G_GNUC_PRINTF(2, 3);
void imcb_log(struct im_connection *ic, char *format, ...) G_GNUC_PRINTF(2, 3);
void imcb_remove_buddy(struct im_connection *ic, const char *handle, char *group);
void imcb_rename_buddy(struct im_connection *ic, const char *handle, const char *realname);
void imc_logout(struct im_connection *ic, int allow_reconnect);
void json_value_free(json_value * value);
void log_message(int level, const char *message, ...) G_GNUC_PRINTF(2, 3);
void op_log_message(void *opdata, const char *message);
void random_bytes(unsigned char *buf, int count);
void register_protocol(struct prpl *p);
void sha1_append(sha1_state_t *ctx, const guint8 * message_array, guint len);
void sha1_init(sha1_state_t *ctx);
void *ssl_connect(char *host, int port, gboolean verify, ssl_input_function func, gpointer data);
void ssl_disconnect(void *conn_);

#endif
