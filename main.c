
#include <glib.h>
#include <gmodule.h>
#include "bitlbee.h"

#define PLUGINDIR "/usr/local/lib/bitlbee"

gboolean load_plugin(char *path)
{
	void (*init_function) (void);

  log_message(LOGLVL_INFO, "Attempting to load plugin `%s'...\n", path);

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
				log_message(LOGLVL_WARNING, "Can't build path for %s\n", entry);
				continue;
			}

			load_plugin(path);

			g_free(path);
		}

		g_dir_close(dir);
	}
}

struct prpl *steam;


void register_protocol(struct prpl *plugin) {
    log_message(LOGLVL_INFO, "Received plugin for `%s'.\n", plugin->name);
    if (!strncmp(plugin->name, "steam", sizeof("steam"))) {
      steam = plugin;
    }
}

gboolean root_command_add(const char *name, int params, void (*f)(irc_t *, char **args), int flags) {
    log_message(LOGLVL_INFO, "Plugin wants to register root command `%s'.\n", name);
}

struct im_connection *imcb_new(account_t *acc) {
  struct im_connection *ic;

	ic = g_new0(struct im_connection, 1);

	ic->bee = acc->bee;
	ic->acc = acc;
	acc->ic = ic;

	return(ic);
}

int main(void) {
  // log_link(LOGLVL_ERROR, LOGOUTPUT_CONSOLE);
  // log_link(LOGLVL_WARNING, LOGOUTPUT_CONSOLE);
  // log_link(LOGLVL_INFO, LOGOUTPUT_CONSOLE);
  load_plugins();
#if !defined(USERNAME) || !defined(PASSWORD)
  log_message(LOGLVL_INFO, "Done (no username or password provided for testing)\n");
#else
  account_t account = {
    .prpl = steam,
    .user = #USERNAME,
    .pass = #PASSWORD,
    .tag = "STM",
    .reconnect = 0,
    .set = 0,
    .bee = 0,
    .ic = 0,
    .next = 0
  };

  log_message(LOGLVL_INFO, "Attempting to log in to %s...\n", steam->name);
  steam->login(&account);
#endif

  return 0;
}
