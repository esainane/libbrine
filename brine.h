/*
 * Brine library callbacks
 */

#define USRCMD const void *brineuser
#define NETCMD USRCMD, const void *network
#define MSGCMD NETCMD, const char *message

struct brine {
  // Sends a message to/from a network buffer.
  int (* net_msgrecv)(MSGCMD, int type); // eg. status messages
  int (* net_msgsend)(MSGCMD); // eg. raw commands
  // Sends a message to/from a user
  int (* user_msgrecv)(MSGCMD, const char *user); // eg. hi <brineuser>!
  int (* user_msgsend)(MSGCMD, const char *user); // eg. hi bob!
  // Sends a message to/from a channel
  int (* chan_msgrecv)(MSGCMD, const char *channel); // eg. hi <brineuser>!
  int (* chan_msgsend)(MSGCMD, const char *channel); // eg. hi #example!
};

struct set;

/*
 * Brine library API
 */

void brine_init(struct brine *);

// Invokes brine_eventloop, will never return!
void brine_bootstrap(const char *protocol, char *username, char *password, void (* config_callback)(struct set **));

// Will never return!
void brine_eventloop(void);
