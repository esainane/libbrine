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
#include <stdint.h>
#include <ctype.h>
#include <errno.h>
#include <syslog.h>
#include <glib.h>
#include <gmodule.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BITLBEE_VERSION "3.4.1"


#define event_debug(x ...)


#define sock_make_nonblocking(fd) fcntl(fd, F_SETFL, O_NONBLOCK)
#define sock_make_blocking(fd) fcntl(fd, F_SETFL, 0)
#define sockerr_again() (errno == EINPROGRESS || errno == EINTR || errno == EAGAIN)

#define MAX_STRING 511

#define PROTO_HTTP      2
#define PROTO_HTTPS     5

#define SSL_OK            0
#define SSL_NOHANDSHAKE   1
#define SSL_AGAIN         2

extern int ssl_errno;


typedef enum {
	LOGLVL_INFO,
	LOGLVL_WARNING,
	LOGLVL_ERROR,
#ifdef DEBUG
	LOGLVL_DEBUG,
#endif
} loglvl_t;
typedef enum {
	LOGOUTPUT_NULL,
	LOGOUTPUT_IRC,
	LOGOUTPUT_SYSLOG,
	LOGOUTPUT_CONSOLE,
} logoutput_t;
struct prpl;
typedef enum {
        BEE_USER_ONLINE = 1,    /* Compatibility with old OPT_LOGGED_IN flag */
        BEE_USER_AWAY = 4,      /* Compatibility with old OPT_AWAY flag */
        BEE_USER_MOBILE = 8,    /* Compatibility with old OPT_MOBILE flag */
        BEE_USER_LOCAL = 256,   /* Locally-added contacts (not in real contact list) */
        BEE_USER_SPECIAL = 512, /* Denotes a user as being special */
        BEE_USER_NOOTR = 4096,  /* Per-user version of OPT_NOOTR */
} bee_user_flags_t;
typedef struct bee_user bee_user_t;
typedef struct account account_qt;
struct set;
struct http_request;

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

typedef GChecksum *sha1_state_t;
typedef gboolean (*ssl_input_function)(gpointer, int, void*, b_input_condition);

#include "data.h"

#include "narg.h"

#define MOCK(x, args...) x(args);
#define MOCKVA(x, args...) x(args) G_GNUC_PRINTF(NARG_NARG(args) - 1, NARG_NARG(args));

#include "mock.h"

#undef MOCK
#undef MOCKVA

#endif
