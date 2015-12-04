typedef struct bee {
  struct set *set;
  struct bee_user *users;
	struct account *accounts; /* TODO(wilmer): Use GSList here too? */
	void *ui_data;
} bee_t;

typedef struct account {
	struct prpl *prpl;
  char *tag;
	char *user;
	char *pass;
  set_t *set;
  struct bee *bee ;
  struct im_connection *ic;
	struct account *next;
} account_t;

struct im_connection {
	account_t *acc;
  uint32_t flags;
	void *proto_data;
	bee_t *bee;
};

typedef struct bee_user {
  struct im_connection *ic;
	char *handle;
  void *data;
} bee_user_t;

typedef struct irc {
	GSList *channels;
	struct bee *b;
} irc_t;

struct groupchat {
	struct im_connection *ic;
	GList *in_room;
	char *title;
	void *ui_data;
};

typedef struct set {
  set_flags_t flags;
} set_t;

struct prpl {
        int options;
        /* You should set this to the name of your protocol.
         * - The user sees this name ie. when imcb_log() is used. */
        const char *name;
        void *data;
        /* Maximum Message Size of this protocol.
         * - Introduced for OTR, in order to fragment large protocol messages.
         * - 0 means "unlimited". */
        unsigned int mms;

        /* Added this one to be able to add per-account settings, don't think
         * it should be used for anything else. You are supposed to use the
         * set_add() function to add new settings. */
        void (* init)           (account_t *);
        /* The typical usage of the login() function:
         * - Create an im_connection using imcb_new() from the account_t parameter.
         * - Initialize your myproto_data struct - you should store all your protocol-specific data there.
         * - Save your custom structure to im_connection->proto_data.
         * - Use proxy_connect() to connect to the server.
         */
        void (* login)          (account_t *);
        /* Implementing this function is optional. */
        void (* keepalive)      (struct im_connection *);
        /* In this function you should:
         * - Tell the server about you are logging out.
         * - g_free() your myproto_data struct as BitlBee does not know how to
         *   properly do so.
         */
        void (* logout)         (struct im_connection *);

        /* This function is called when the user wants to send a message to a handle.
         * - 'to' is a handle, not a nick
         * - 'flags' may be ignored
         */
        int (* buddy_msg)      (struct im_connection *, char *to, char *message, int flags);
        /* This function is called then the user uses the /away IRC command.
         * - 'state' contains the away reason.
         * - 'message' may be ignored if your protocol does not support it.
         */
        void (* set_away)       (struct im_connection *, char *state, char *message);
        /* Implementing this function is optional. */
        int (* send_typing)    (struct im_connection *, char *who, int flags);

        /* 'name' is a handle to add/remove. For now BitlBee doesn't really
         * handle groups, just set it to NULL, so you can ignore that
         * parameter. */
        void (* add_buddy)      (struct im_connection *, char *name, char *group);
        void (* remove_buddy)   (struct im_connection *, char *name, char *group);

        /* Block list stuff. Implementing these are optional. */
        void (* add_permit)     (struct im_connection *, char *who);
        void (* add_deny)       (struct im_connection *, char *who);
        void (* rem_permit)     (struct im_connection *, char *who);
        void (* rem_deny)       (struct im_connection *, char *who);

        /* Request profile info. Free-formatted stuff, the IM module gives back
           this info via imcb_log(). Implementing these are optional. */
        void (* get_info)       (struct im_connection *, char *who);

        /* Group chat stuff. */
        /* This is called when the user uses the /invite IRC command.
         * - 'who' may be ignored
         * - 'message' is a handle to invite
         */
        void (* chat_invite)    (struct groupchat *, char *who, char *message);
        /* This is called when the user uses the /kick IRC command.
         * - 'who' is a handle to kick
         * - 'message' is a kick message or NULL
         */
        void (* chat_kick)      (struct groupchat *, char *who, const char *message);
        /* This is called when the user uses the /part IRC command in a group
         * chat. You just should tell the user about it, nothing more. */
        void (* chat_leave)     (struct groupchat *);
        /* This is called when the user sends a message to the groupchat.
         * 'flags' may be ignored. */
        void (* chat_msg)       (struct groupchat *, char *message, int flags);
        /* This is called when the user uses the /join #nick IRC command.
         * - 'who' is the handle of the nick
         */
        struct groupchat *
        (* chat_with)      (struct im_connection *, char *who);
        /* This is used when the user uses the /join #channel IRC command.  If
         * your protocol does not support publicly named group chats, then do
         * not implement this. */
        struct groupchat *
        (* chat_join)      (struct im_connection *, const char *room,
                            const char *nick, const char *password, set_t **sets);
        /* Change the topic, if supported. Note that BitlBee expects the IM
           server to confirm the topic change with a regular topic change
           event. If it doesn't do that, you have to fake it to make it
           visible to the user. */
        void (* chat_topic)     (struct groupchat *, char *topic);

        /* If your protocol module needs any special info for joining chatrooms
           other than a roomname + nickname, add them here. */
        void (* chat_add_settings)      (account_t *acc, set_t **head);
        void (* chat_free_settings)     (account_t *acc, set_t **head);
                /* You can tell what away states your protocol supports, so that
         * BitlBee will try to map the IRC away reasons to them. If your
         * protocol doesn't have any, just return one generic "Away". */
        GList *(* away_states)(struct im_connection *ic);

        /* Mainly for AOL, since they think "Bung hole" == "Bu ngho le". *sigh*
         * - Most protocols will just want to set this to g_strcasecmp().*/
        int (* handle_cmp) (const char *who1, const char *who2);

        /* Implement these callbacks if you want to use imcb_ask_auth() */
        void (* auth_allow)     (struct im_connection *, const char *who);
        void (* auth_deny)      (struct im_connection *, const char *who);

        /* Incoming transfer request */
        void (* transfer_request) (struct im_connection *, file_transfer_t *ft, char *handle);

        void (* buddy_data_add) (struct bee_user *bu);
        void (* buddy_data_free) (struct bee_user *bu);

        GList *(* buddy_action_list) (struct bee_user *bu);
        void *(* buddy_action) (struct bee_user *bu, const char *action, char * const args[], void *data);

        /* If null, equivalent to handle_cmp( ic->acc->user, who ) */
        gboolean (* handle_is_self) (struct im_connection *, const char *who);

        /* Some placeholders so eventually older plugins may cooperate with newer BitlBees. */
        void *resv1;
        void *resv2;
        void *resv3;
        void *resv4;
        void *resv5;
};

typedef struct url {
  int proto;
  int port;
  char host[MAX_STRING + 1];
  char file[MAX_STRING + 1];
  char user[MAX_STRING + 1];
  char pass[MAX_STRING + 1];
} url_t;

struct http_request {

};
