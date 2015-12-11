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

#define PACKAGE "BitlBeeMock"
#define BITLBEE_VERSION "3.4.1"
#define VERSION BITLBEE_VERSION
#define BITLBEE_VER(a, b, c) (((a) << 16) + ((b) << 8) + (c))
#define BITLBEE_VERSION_CODE BITLBEE_VER(3, 4, 1)


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

#define OPT_LOGGED_IN   0x00000001
#define OPT_LOGGING_OUT 0x00000002
#define OPT_AWAY        0x00000004
#define OPT_MOBILE      0x00000008
#define OPT_DOES_HTML   0x00000010
#define OPT_LOCALBUDDY  0x00000020 /* For nicks local to one groupchat */
#define OPT_SLOW_LOGIN  0x00000040 /* I.e. Twitter Oauth @ login time */
#define OPT_TYPING      0x00000100 /* Some pieces of code make assumptions */
#define OPT_THINKING    0x00000200 /* about these values... Stupid me! */
#define OPT_NOOTR       0x00001000 /* protocol not suitable for OTR */
#define OPT_PONGS       0x00010000 /* Service sends us keep-alives */
#define OPT_PONGED      0x00020000 /* Received a keep-alive during last interval */
#define OPT_SELFMESSAGE 0x00080000 /* A message sent by self from another location */


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

typedef void (*query_callback) (void *data);

#include "events.h"

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

#define CTYPES "&#"

typedef enum {
	IRC_CHANNEL_JOINED = 1, /* The user is currently in the channel. */
	IRC_CHANNEL_TEMP = 2,   /* Erase the channel when the user leaves,
	                           and don't save it. */

	/* Hack: Set this flag right before jumping into IM when we expect
	   a call to imcb_chat_new(). */
	IRC_CHANNEL_CHAT_PICKME = 0x10000,
} irc_channel_flags_t;

typedef struct irc irc_t;
typedef struct url url_t;

typedef GChecksum *sha1_state_t;
typedef gboolean (*ssl_input_function)(gpointer, int, void*, b_input_condition);

#include "http_client.h"
#include "set.h"
#include "sha1.h"

#include "data.h"

#include "brine_glue.h"

#include "narg.h"

#define MOCK(x, args...) x(args);
#define MOCKVA(x, args...) x(args) G_GNUC_PRINTF(NARG_NARG(args) - 1, NARG_NARG(args));

#include "mock.h"

#undef MOCK
#undef MOCKVA


#include "log.h"

#endif
