
#ifndef SET_H
#define SET_H

extern char *SET_INVALID;

typedef enum {
	SET_NOSAVE = 0x0001,   /* Don't save this setting (i.e. stored elsewhere). */
	SET_NULL_OK = 0x0100,  /* set->value == NULL is allowed. */
	SET_HIDDEN = 0x0200,   /* Don't show up in setting lists. Mostly for internal storage. */
	SET_PASSWORD = 0x0400, /* Value shows up in settings list as "********". */
	SET_HIDDEN_DEFAULT = 0x0800, /* Hide unless changed from default. */
} set_flags_t;

struct set;
typedef struct set set_t;

typedef char *(*set_eval) (void *data, char *value);

void set_add(set_t **head, const char *key, const char *def, set_eval eval, void *data);
void set_add_with_flags(set_t **head, const char *key, const char *def, set_eval eval, void *data, set_flags_t flags);
char *set_getstr(set_t **head, const char *key);
int set_getint(set_t **head, const char *key);
int set_getbool(set_t **head, const char *key);
int bool2int(char *value);
int set_setstr(set_t **head, const char *key, char *value);

/* Two very useful generic evaluators. */
char *set_eval_int(void *data, char *value);
char *set_eval_bool(void *data, char *value);

#endif /* SET_H */
