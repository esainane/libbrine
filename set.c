
#include "set.h"
#include <stdarg.h>
#include <glib.h>

char *SET_INVALID = "nee";


set_t *set_find(set_t **head, const char *key)
{
	set_t *s = *head;

	while (s) {
		if (g_strcasecmp(s->key, key) == 0) {
			break;
		}
		s = s->next;
	}

	return s;
}

int set_getbool(set_t **head, const char *key)
{
	char *s = set_getstr(head, key);

	if (!s) {
		return 0;
	}

	return bool2int(s);
}

char *set_getstr(set_t **head, const char *key)
{
	set_t *s = set_find(head, key);

	if (!s || (!s->value && !s->def)) {
		return NULL;
	}

	return set_value(s);
}

char *set_eval_int(set_t *set, char *value)
{
	char *s = value;

	/* Allow a minus at the first position. */
	if (*s == '-') {
		s++;
	}

	for (; *s; s++) {
		if (!g_ascii_isdigit(*s)) {
			return SET_INVALID;
		}
	}

	return value;
}

char *set_eval_bool(set_t *set, char *value)
{
	return is_bool(value) ? value : SET_INVALID;
}
