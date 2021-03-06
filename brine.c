
#include <glib.h>
#include <gmodule.h>
#include "bitlbee.h"

#include "brine.h"

#include "irc_downstream.h"

#define PLUGINDIR "/usr/local/lib/bitlbee"

static struct prpl *current_loading_plugin = 0;

gboolean load_plugin(char *path)
{
	void (*init_function) (void);

  log_message(LOGLVL_INFO, "Attempting to load plugin `%s'...", path);

	GModule *mod = g_module_open(path, G_MODULE_BIND_LAZY);

	if (!mod) {
		log_message(LOGLVL_ERROR, "Can't find `%s', not loading (%s)\n", path, g_module_error());
		return FALSE;
	}

	if (!g_module_symbol(mod, "init_plugin", (gpointer *) &init_function)) {
		log_message(LOGLVL_WARNING, "Can't find function `init_plugin' in `%s'\n", path);
		return FALSE;
	}

	init_function();

	if (current_loading_plugin->cmd_num) {
		commands_finalize(current_loading_plugin->cmds, current_loading_plugin->cmd_num);
	}
	current_loading_plugin = 0;

	return TRUE;
}

void load_plugins(void)
{
	GDir *dir;
	GError *error = NULL;

	dir = g_dir_open(PLUGINDIR, 0, &error);

	if (dir) {
		const gchar *entry;
		char *path;

		while ((entry = g_dir_read_name(dir))) {
			path = g_build_filename(PLUGINDIR, entry, NULL);
			if (!path) {
				log_message(LOGLVL_ERROR, "Can't build path for %s\n", entry);
				continue;
			}

			if (load_plugin(path)) {
        log_message(LOGLVL_INFO, "Plugin loaded\n");
      }

			g_free(path);
		}

		g_dir_close(dir);
	}
}

static struct prpl **plugins = 0;
static int plugins_num = 0;
static int plugins_max = 0;

static struct brine *brine_callbacks;

void register_protocol(struct prpl *plugin)
{
    log_message(LOGLVL_INFO, "Received plugin for `%s'.", plugin->name);
		current_loading_plugin = plugin;
		if (plugins_num == plugins_max) {
			plugins = realloc(plugins, (plugins_max += 5) * (sizeof(struct prpl *)));
		}
    plugins[plugins_num++] = plugin;
}

static struct prpl *find_protocol(const char *name)
{
	int i;
	for (i = plugins_num; i--;) {
		if (!strcmp(name, plugins[i]->name)) {
			return plugins[i];
		}
	}
	return NULL;
}

int brine_pluginexists(const char *name)
{
	return find_protocol(name) != NULL;
}

void imcb_log(struct im_connection *ic, char *fmt, ...)
{
	va_list params;
	char *text;

	va_start(params, fmt);
	text = g_strdup_vprintf(fmt, params);
	va_end(params);

	if (brine_callbacks && brine_callbacks->net_msgrecv) {
		brine_callbacks->net_msgrecv(ic->acc, text, 300);
	} else {
		log_message(LOGLVL_INFO, "** %s > %s", ic->acc->prpl->name, text);
	}
	g_free(text);
}

void imcb_lognum(struct im_connection *ic, int num, char *fmt, ...)
{
	va_list params;
	char *text;

	va_start(params, fmt);
	text = g_strdup_vprintf(fmt, params);
	va_end(params);

	if (brine_callbacks && brine_callbacks->net_msgrecv) {
		brine_callbacks->net_msgrecv(ic->acc, text, num);
	} else {
		log_message(LOGLVL_INFO, "** %s > %s", ic->acc->prpl->name, text);
	}
	g_free(text);
}

void imcb_error(struct im_connection *ic, char *fmt, ...)
{
	va_list params;
	char *text;

	va_start(params, fmt);
	text = g_strdup_vprintf(fmt, params);
	va_end(params);

	if (brine_callbacks && brine_callbacks->net_msgrecv) {
		brine_callbacks->net_msgrecv(ic->acc, text, 400);
	} else {
		log_message(LOGLVL_ERROR, "** %s > %s", ic->acc->prpl->name, text);
	}
	g_free(text);
}

gboolean root_command_add(const char *name, int params, void (*f)(irc_t *, char **args), int flags) {
	log_message(LOGLVL_INFO, "Plugin '%s' wants to register root command `%s'.", current_loading_plugin->name, name);
	// plugin_command_add(current_loading_plugin, name, params, f);
	return 0;
}

#if 0
typedef enum {
	ACC_FLAG_AWAY_MESSAGE = 0x01,   /* Supports away messages instead of just states. */
	ACC_FLAG_STATUS_MESSAGE = 0x02, /* Supports status messages (without being away). */
	ACC_FLAG_HANDLE_DOMAINS = 0x04, /* Contact handles need a domain portion. */
	ACC_FLAG_LOCAL = 0x08,          /* Contact list is local. */
} account_flag_t;
#endif
void imcb_connected(struct im_connection *ic)
{
	/* MSN servers sometimes redirect you to a different server and do
	   the whole login sequence again, so these "late" calls to this
	   function should be handled correctly. (IOW, ignored) */
	if (ic->flags & OPT_LOGGED_IN) {
		return;
	}

#if 0
	if (ic->acc->flags & ACC_FLAG_LOCAL) {
		GHashTableIter nicks;
		gpointer k, v;
		g_hash_table_iter_init(&nicks, ic->acc->nicks);
		while (g_hash_table_iter_next(&nicks, &k, &v)) {
			ic->acc->prpl->add_buddy(ic, (char *) k, NULL);
		}
	}
#endif

	imcb_log(ic, "Logged in");

	ic->flags |= OPT_LOGGED_IN;
#if 0
	start_keepalives(ic, 60000);
	/* Necessary to send initial presence status, even if we're not away. */
	imc_away_send_update(ic);
#endif

#if 0
	/* Apparently we're connected successfully, so reset the
	   exponential backoff timer. */
	ic->acc->auto_reconnect_delay = 0;
#endif

#if 0
	if (ic->bee->ui->imc_connected) {
		ic->bee->ui->imc_connected(ic);
	}
#endif
}

struct im_connection *imcb_new(account_t *acc) {
  struct im_connection *ic;

	ic = g_new0(struct im_connection, 1);

	ic->bee = acc->bee;
	ic->acc = acc;
	acc->ic = ic;

	return(ic);
}

struct groupchat *imcb_chat_new(struct im_connection *ic, const char *handle)
{
	struct groupchat *c = g_new0(struct groupchat, 1);
	bee_t *bee = ic->bee;

	/* This one just creates the conversation structure, user won't see
	   anything yet until s/he is joined to the conversation. (This
	   allows you to add other already present participants first.) */

	ic->groupchats = g_slist_prepend(ic->groupchats, c);
	c->ic = ic;
	c->title = g_strdup(handle);
	c->topic = g_strdup_printf(
	        "BitlBee groupchat: \"%s\". Please keep in mind that root-commands won't work here. Have fun!",
	        c->title);

	if (set_getbool(&ic->bee->set, "debug")) {
		imcb_log(ic, "Creating new conversation: (id=%p,handle=%s)", c, handle);
	}
#if 0
	if (bee->ui->chat_new) {
		bee->ui->chat_new(bee, c);
	}
#endif
	log_message(LOGLVL_INFO, "Want to create new groupchat (%s)", handle);
	brine_callbacks->chan_add(ic->acc, handle);
	return c;
}
void imcb_chat_add_buddy(struct groupchat *c, const char *handle) {
	log_message(LOGLVL_INFO, "Want to add (%s) to groupchat (%p)", handle, c);
	brine_callbacks->chan_join(c->ic->acc, handle, c->title);
}
void imcb_chat_free(struct groupchat *c) {
	log_message(LOGLVL_INFO, "Want to free groupchat (%p)", c);
}
void imcb_chat_msg(struct groupchat *c, const char *who, char *msg, guint32 flags, time_t sent_at) {
	log_message(LOGLVL_INFO, "Want to send message (%s) from (%s) to groupchat (%p) with flags (%u)", msg, who, c);
	brine_callbacks->chan_msgrecv(c->ic->acc, msg, who, c->title);
}
void imcb_chat_name_hint(struct groupchat *c, const char *name) {
	log_message(LOGLVL_INFO, "Want to set chat name hint for (%p) to (%s)", name);
}

struct groupchat *bee_chat_by_title(bee_t *bee, struct im_connection *ic, const char *title)
{
	log_message(LOGLVL_INFO, "Attempting to look up chat by the title (%s)", title);
	struct groupchat *c;
	GSList *l;

	for (l = ic->groupchats; l; l = l->next) {
		c = l->data;
		if (strcmp(c->title, title) == 0) {
			return c;
		}
	}

	return NULL;
}

void imcb_add_buddy(struct im_connection *ic, const char *handle, const char *group) {
	bee_user_t *bu;
	log_message(LOGLVL_INFO, "Want to add buddy (%s)", handle);

	if (imcb_buddy_by_handle(ic, handle)) {
		log_message(LOGLVL_ERROR, "Attempted to create already existing user (%s)!", handle);
		return;
	}

	bu = g_new0(bee_user_t, 1);
	bu->ic = ic;
	bu->handle = g_strdup(handle);
	ic->bee->users = g_slist_prepend(ic->bee->users, bu);
	if (ic->acc->prpl->buddy_data_add) {
		ic->acc->prpl->buddy_data_add(bu);
	}
	imcb_buddy_status(ic, handle, 0, NULL, NULL);
	brine_callbacks->user_add(ic->acc, handle);
}
void imcb_remove_buddy(struct im_connection *ic, const char *handle, char *group) {
	log_message(LOGLVL_INFO, "Want to remove (%s)", handle);
	brine_callbacks->user_remove(ic->acc, handle);
}
void imcb_rename_buddy(struct im_connection *ic, const char *handle, const char *realname) {
	log_message(LOGLVL_INFO, "Want to rename buddy (%s) to (%s)", handle, realname);
	brine_callbacks->user_rename(ic->acc, handle, realname);
}
void imcb_buddy_nick_hint(struct im_connection *ic, const char *handle, const char *nick) {
	log_message(LOGLVL_INFO, "Want to set buddy (%s) nick hint to (%s)", handle, nick);
}
struct bee_user *imcb_buddy_by_handle(struct im_connection *ic, const char *handle) {
	log_message(LOGLVL_INFO, "Attempting to look up buddy by the handle (%s)", handle);
	GSList *l;

	for (l = ic->bee->users; l; l = l->next) {
		bee_user_t *bu = l->data;

		if (ic->acc->prpl->handle_cmp(bu->handle, handle) == 0) {
			return bu;
		}
	}

	return NULL;
}

void imcb_selfname(struct im_connection *ic, const char *handle) {
	log_message(LOGLVL_INFO, "Attempting to set own name (%s)", handle);
	brine_callbacks->net_setnick(ic->acc, handle);
}

void brine_putline(brine_handle handle, char *line) {
	irc_downstream_putline(handle->ic, line);
}

void brine_init(struct brine *b) {
	brine_callbacks = b;
	load_plugins();
	log_message(LOGLVL_INFO, "Setting up event handler...");
	b_main_init();
}

brine_handle brine_conn_init(const char *protocol, const char *username, const char *password, void (* config_callback)(struct set **)) {
	/* Isolate things a bit more. We create an entirely new bee for each new brine connection. */
	struct prpl *target = find_protocol(protocol);
	if (!target) {
		log_message(LOGLVL_ERROR, "No plugin found for (%s), cannot bootstrap!", protocol);
		return 0;
	}
	bee_t *bee = g_new0(struct bee, 1);
	account_t *account = g_new0(struct account, 1);
	account->prpl = target;
	account->user = g_strdup(username);
	account->pass = g_strdup(password);
	account->tag = "";
	account->bee = bee;
	bee->accounts = account;

	log_message(LOGLVL_INFO, "Initializing %s...\n", target->name);
	target->init(account);
	if (config_callback) {
		config_callback(&account->set);
	}
	return account;
}

brine_handle brine_conn_login(brine_handle handle) {
	struct account *acc = handle;
	log_message(LOGLVL_INFO, "Attempting to log in to %s...\n", acc->prpl->name);
	acc->prpl->login(acc);
	return acc;
}

brine_handle brine_connect(const char *protocol, const char *username, const char *password, void (* config_callback)(struct set **)) {
	brine_handle handle = brine_conn_init(protocol, username, password, config_callback);
	brine_conn_login(handle);
	return handle;
}

void brine_disconnect(brine_handle handle) {
	struct im_connection *ic = handle->ic;
	GSList *l;
	log_message(LOGLVL_INFO, "Logging out and cleaning up of %s (%s)...\n", ic->acc->user, ic->acc->prpl->name);
	for (l = ic->bee->users; l; ) {
		bee_user_t *bu = l->data;
		l = l->next;
		if (!bu) {
			continue;
		} else if (bu->ic == ic) {
			if (ic->acc->prpl->buddy_data_free) {
				ic->acc->prpl->buddy_data_free(bu);
			}
			ic->bee->users = g_slist_remove(ic->bee->users, bu);
			g_free(bu->handle);
			g_free(bu->status_msg);
			g_free(bu);
		} else {
			log_message(LOGLVL_ERROR, "Had bee user with different connection during cleanup, this should never happen!");
		}
	}
	ic->acc->prpl->logout(ic);
	g_free(ic->acc->user);
	g_free(ic->acc->pass);
	g_free(ic->acc);
	g_free(ic->bee);
	g_free(ic);
}

void brine_bootstrap(const char *protocol, char *username, char *password, void (* config_callback)(struct set **)) {
	struct prpl *target = find_protocol(protocol);
	if (!target) {
		log_message(LOGLVL_ERROR, "No plugin found for (%s), cannot bootstrap!", protocol);
		return;
	}
	bee_t bee = {
		.set = 0,
		.accounts = 0,
		.ui_data = 0
	};
	account_t account = {
		.prpl = target,
		.user = username,
		.pass = password,
		.tag = "STM",
		.reconnect = 0,
		.set = 0,
		.bee = &bee,
		.ic = 0,
		.next = 0
	};

	log_message(LOGLVL_INFO, "Initializing %s...\n", target->name);
	target->init(&account);
	if (config_callback) {
		config_callback(&account.set);
	}
	log_message(LOGLVL_INFO, "Attempting to log in to %s...\n", target->name);
	target->login(&account);
	brine_eventloop();
}

void brine_eventloop(void) {
	log_message(LOGLVL_INFO, "Starting event loop!");
	b_main_run();
}
