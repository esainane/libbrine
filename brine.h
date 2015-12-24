/*
 * Brine library callbacks
 */

#ifdef __cplusplus
extern "C" {
#endif

#define USRCMD const void *connection
#define MSGCMD USRCMD, const char *message

struct brine {
  // Sends a message to/from a network buffer.
  int (* net_msgrecv)(MSGCMD, int type); // eg. status messages
  // Sends a message to/from a user
  int (* user_msgrecv)(MSGCMD, const char *user); // eg. hi <brineuser>!
  // Sends a message to/from a channel
  int (* chan_msgrecv)(MSGCMD, const char *channel); // eg. hi <brineuser>!
};

struct set;

/*
 * Brine library API
 */

void brine_init(struct brine *);

// Invokes brine_eventloop, will never return!
void brine_bootstrap(const char *protocol, char *username, char *password, void (* config_callback)(struct set **));

void *brine_connect(const char *protocol, const char *username, const char *password, void (*config_callback)(struct set **));

void brine_disconnect(void *connection);

// Will never return!
void brine_eventloop(void);

int brine_pluginexists(const char *);

#ifdef __cplusplus
} /* extern "C" */
#endif
