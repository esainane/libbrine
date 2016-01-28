/********************************************************************\
  * BitlBee -- An IRC to other IM-networks gateway                     *
  *                                                                    *
  * Copyright 2002-2013 Wilmer van der Gaast and others                *
  \********************************************************************/

/* Some stuff to register, handle and save user preferences             */

/*
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License with
  the Debian GNU/Linux distribution in /usr/share/common-licenses/GPL;
  if not, write to the Free Software Foundation, Inc., 51 Franklin St.,
  Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "set.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <glib.h>

#include "log.h"

char *SET_INVALID = "nee";

typedef struct set {
	void *data;
	char *key;
  char *value;
	char *def;
  set_flags_t flags;
	set_eval eval;
	struct set *next;
} set_t;

#define set_value(set) ((set)->value) ? ((set)->value) : ((set)->def)

static set_t *set_find(set_t **head, const char *key)
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


static set_t *set_add_(set_t **head, const char *key, const char *def, set_eval eval, void *data)
{
	set_t *s = set_find(head, key);

	/* Possibly the setting already exists. If it doesn't exist yet,
	   we create it. If it does, we'll just change the default. */
	if (!s) {
		if ((s = *head)) {
			/* Sorted insertion. Special-case insertion at the start. */
			if (strcmp(key, s->key) < 0) {
				s = g_new0(set_t, 1);
				s->next = *head;
				*head = s;
			} else {
				while (s->next && strcmp(key, s->next->key) > 0) {
					s = s->next;
				}
				set_t *last_next = s->next;
				s->next = g_new0(set_t, 1);
				s = s->next;
				s->next = last_next;
			}
		} else {
			s = *head = g_new0(set_t, 1);
		}
		s->key = g_strdup(key);
	}

	if (s->def) {
		g_free(s->def);
		s->def = NULL;
	}
	if (def) {
		s->def = g_strdup(def);
	}

	s->eval = eval;
	s->data = data;

	return s;
}

void set_add(set_t **head, const char *key, const char *def, set_eval eval, void *data)
{
  set_add_(head, key, def, eval, data);
}

void set_add_with_flags(set_t **head, const char *key, const char *def, set_eval eval, void *data, set_flags_t flags)
{
  set_add_(head, key, def, eval, data)->flags = flags;
}

char *set_getstr(set_t **head, const char *key)
{
	set_t *s = set_find(head, key);

	if (!s || (!s->value && !s->def)) {
    log_message(LOGLVL_INFO, "set_getstr: %s -> NULL", key);
		return NULL;
	}

  log_message(LOGLVL_INFO, "set_getstr: %s -> \"%s\"", key, set_value(s));
	return set_value(s);
}


int set_getint(set_t **head, const char *key)
{
	char *s = set_getstr(head, key);
	int i = 0;

	if (!s) {
		return 0;
	}

	if (sscanf(s, "%d", &i) != 1) {
		return 0;
	}

	return i;
}


int bool2int(char *value)
{
	int i;

	if ((g_strcasecmp(value,
	                  "true") == 0) || (g_strcasecmp(value, "yes") == 0) || (g_strcasecmp(value, "on") == 0)) {
		return 1;
	}
	if ((g_strcasecmp(value,
	                  "false") == 0) || (g_strcasecmp(value, "no") == 0) || (g_strcasecmp(value, "off") == 0)) {
		return 0;
	}

	if (sscanf(value, "%d", &i) == 1) {
		return i;
	}

	return 0;
}

int set_getbool(set_t **head, const char *key)
{
	char *s = set_getstr(head, key);

	if (!s) {
		return 0;
	}

	return bool2int(s);
}

int set_setstr(set_t **head, const char *key, char *value)
{
  log_message(LOGLVL_INFO, "set_setstr: %s -> \"%s\"", key, value);
	set_t *s = set_find(head, key);
	char *nv = value;

	if (!s) {
		/*
		Used to do this, but it never really made sense.
		s = set_add( head, key, NULL, NULL, NULL );
		*/
		return 0;
	}

	if (value == NULL && (s->flags & SET_NULL_OK) == 0) {
		return 0;
	}

	/* Call the evaluator. For invalid values, evaluators should now
	   return SET_INVALID, but previously this was NULL. Try to handle
	   that too if NULL is not an allowed value for this setting. */
	if (s->eval && ((nv = s->eval(s, value)) == SET_INVALID ||
	                ((s->flags & SET_NULL_OK) == 0 && nv == NULL))) {
		return 0;
	}

	if (s->value) {
		g_free(s->value);
		s->value = NULL;
	}

	/* If there's a default setting and it's equal to what we're trying to
	   set, stick with s->value = NULL. Otherwise, remember the setting. */
	if (!s->def || (strcmp(nv, s->def) != 0)) {
		s->value = g_strdup(nv);
	}

	if (nv != value) {
		g_free(nv);
	}

	return 1;
}

char *set_eval_int(void *data, char *value)
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

static int is_bool(char *value)
{
	if (*value == 0) {
		return 0;
	}

	if ((g_strcasecmp(value,
	                  "true") == 0) || (g_strcasecmp(value, "yes") == 0) || (g_strcasecmp(value, "on") == 0)) {
		return 1;
	}
	if ((g_strcasecmp(value,
	                  "false") == 0) || (g_strcasecmp(value, "no") == 0) || (g_strcasecmp(value, "off") == 0)) {
		return 1;
	}

	while (*value) {
		if (!g_ascii_isdigit(*value)) {
			return 0;
		} else {
			value++;
		}
	}

	return 1;
}

char *set_eval_bool(void *data, char *value)
{
	return is_bool(value) ? value : SET_INVALID;
}
