

#include "log.h"

#include "stdio.h"
#include <glib.h>

static void log_console(int level, const char *message)
{
	if (level == LOGLVL_ERROR) {
		fprintf(stderr, "Error: %s\n", message);
	}
	if (level == LOGLVL_WARNING) {
		fprintf(stderr, "Warning: %s\n", message);
	}
	if (level == LOGLVL_INFO) {
		fprintf(stdout, "Informational: %s\n", message);
	}
#ifdef DEBUG
	if (level == LOGLVL_DEBUG) {
		fprintf(stdout, "Debug: %s\n", message);
	}
#endif
// 	/* Always log stuff in syslogs too. */
// 	log_syslog(level, message);
	return;
}

void log_message(int level, const char *message, ...)
{

	va_list ap;
	char *msgstring;

	va_start(ap, message);
	msgstring = g_strdup_vprintf(message, ap);
	va_end(ap);

	if (level == LOGLVL_INFO) {
	  log_console(level, msgstring);
	}
	if (level == LOGLVL_WARNING) {
		log_console(level, msgstring);
	}
	if (level == LOGLVL_ERROR) {
		log_console(level, msgstring);
	}
#ifdef DEBUG
	if (level == LOGLVL_DEBUG) {
		log_console(level, msgstring);
	}
#endif

	g_free(msgstring);

	return;
}
