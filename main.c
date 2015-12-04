
#include <glib.h>
#include <gmodule.h>
#include "bitlbee.h"

#define PLUGINDIR "/usr/local/lib/bitlbee"

gboolean load_plugin(char *path)
{
	void (*init_function) (void);

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

int main(void) {
  log_link(LOGLVL_ERROR, LOGOUTPUT_CONSOLE);
  log_link(LOGLVL_WARNING, LOGOUTPUT_CONSOLE);
  log_link(LOGLVL_INFO, LOGOUTPUT_CONSOLE);
  load_plugins();

  return 0;
}
