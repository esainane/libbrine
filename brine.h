/*
 * Brine library callbacks
 */

#ifdef __cplusplus
extern "C" {
#endif

#define USRCMD brine_handle connection
#define MSGCMD USRCMD, const char *message

struct account;
typedef struct account *brine_handle;

struct brine {
  int (* net_setnick)(USRCMD, const char *name);
  // Sends a message to/from a network buffer.
  int (* net_msgrecv)(MSGCMD, int type); // eg. status messages
  // Sends a message to/from a user
  int (* user_msgrecv)(MSGCMD, const char *user); // eg. hi <brineuser>!
  // Sends a message to/from a channel
  int (* chan_msgrecv)(MSGCMD, const char *sender, const char *channel); // eg. hi <brineuser>!

  int (* user_add)(USRCMD, const char *user);
  int (* user_rename)(USRCMD, const char *user, const char *newuser);
  int (* user_remove)(USRCMD, const char *user);

  int (* chan_add)(USRCMD, const char *channel);
  int (* chan_join)(USRCMD, const char *user, const char *channel);
};

struct set;

/*
 * Brine library API
 */

void brine_init(struct brine *);

// Invokes brine_eventloop, will never return!
void brine_bootstrap(const char *protocol, char *username, char *password, void (* config_callback)(struct set **));

brine_handle brine_conn_init(const char *protocol, const char *username, const char *password, void (* config_callback)(struct set **));
brine_handle brine_conn_login(brine_handle handle);

brine_handle brine_connect(const char *protocol, const char *username, const char *password, void (*config_callback)(struct set **));

void brine_disconnect(brine_handle connection);

// Will never return!
void brine_eventloop(void);

int brine_pluginexists(const char *);

/* Parse an IRC formatted message */
void brine_putline(brine_handle connection, char *line);

/* WORKAROUND */
struct set;
int set_setstr(struct set **head, const char *key, char *value);

#ifdef __cplusplus
} /* extern "C" */
#endif
