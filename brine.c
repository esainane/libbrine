
#include <glib.h>
#include <gmodule.h>
#include "bitlbee.h"

#include "brine.h"

#define PLUGINDIR "/usr/local/lib/bitlbee"

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
static plugins_num = 0;
static plugins_max = 0;

#define TARGET "skype"

static struct brine *brine_callbacks;

void register_protocol(struct prpl *plugin)
{
    log_message(LOGLVL_INFO, "Received plugin for `%s'.", plugin->name);
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



void imcb_log(struct im_connection *ic, char *fmt, ...)
{
	va_list params;
	char *text;

	text = g_strdup_vprintf(fmt, params);
	if (brine_callbacks && brine_callbacks->net_msgrecv) {
		brine_callbacks->net_msgrecv(0, 0, text, 300);
	} else {
  	log_message(LOGLVL_INFO, "** %s > %s", ic->acc->prpl->name, text);
	}
	g_free(text);
}

void imcb_error(struct im_connection *ic, char *fmt, ...)
{
	va_list params;
	char *text;

	text = g_strdup_vprintf(fmt, params);
	if (brine_callbacks && brine_callbacks->net_msgrecv) {
		brine_callbacks->net_msgrecv(0, 0, text, 400);
	} else {
  	log_message(LOGLVL_ERROR, "** %s > %s", ic->acc->prpl->name, text);
	}
	g_free(text);
}



gboolean root_command_add(const char *name, int params, void (*f)(irc_t *, char **args), int flags) {
    log_message(LOGLVL_INFO, "Plugin wants to register root command `%s'.", name);
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

void imcb_chat_add_buddy(struct groupchat *c, const char *handle) {
	log_message(LOGLVL_INFO, "Want to add (%s) to groupchat (%p)", handle, c);
}
void imcb_chat_free(struct groupchat *c) {
	log_message(LOGLVL_INFO, "Want to free groupchat (%p)", c);
}
void imcb_chat_msg(struct groupchat *c, const char *who, char *msg, guint32 flags, time_t sent_at) {
		log_message(LOGLVL_INFO, "Want to send message (%s) from (%s) to groupchat (%p) with flags (%u)", msg, who, c);
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

#define STR(x) #x
#define XSTR(x) STR(x)

void brine_init(struct brine *b) {
	brine_callbacks = b;
  // log_link(LOGLVL_ERROR, LOGOUTPUT_CONSOLE);
  // log_link(LOGLVL_WARNING, LOGOUTPUT_CONSOLE);
  // log_link(LOGLVL_INFO, LOGOUTPUT_CONSOLE);
  load_plugins();
  log_message(LOGLVL_INFO, "Setting up event handler...");
  b_main_init();
}

void brine_bootstrap(const char *protocol, char *username, char *password, void (* config_callback)(struct set **)) {
	struct prpl *target = find_protocol(protocol);
	if (!target) {
		log_message(LOGLVL_ERROR, "No plugin found for (%s), cannot bootstrap!", protocol);
	}
  account_t account = {
    .prpl = target,
    .user = username,
    .pass = password,
    .tag = "STM",
    .reconnect = 0,
    .set = 0,
    .bee = 0,
    .ic = 0,
    .next = 0
  };

  log_message(LOGLVL_INFO, "Initializing %s...\n", target->name);
  target->init(&account);
	if (config_callback) {
		config_callback(&account.set);
	}
  set_setstr(&account.set, "skypeconsole_receive", "true");
  set_setstr(&account.set, "read_groups", "true");
  log_message(LOGLVL_INFO, "Attempting to log in to %s...\n", target->name);
  target->login(&account);
	brine_eventloop();
}

void brine_eventloop(void) {
  log_message(LOGLVL_INFO, "Starting event loop!");
  b_main_run();
}
