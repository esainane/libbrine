

extern char *SET_INVALID;

typedef enum {
	SET_NOSAVE = 0x0001,   /* Don't save this setting (i.e. stored elsewhere). */
	SET_NULL_OK = 0x0100,  /* set->value == NULL is allowed. */
	SET_HIDDEN = 0x0200,   /* Don't show up in setting lists. Mostly for internal storage. */
	SET_PASSWORD = 0x0400, /* Value shows up in settings list as "********". */
	SET_HIDDEN_DEFAULT = 0x0800, /* Hide unless changed from default. */
} set_flags_t;

typedef struct set {
	void *data;
	char *key;
  char *value;
	char *def;
  set_flags_t flags;
	struct set *next;
} set_t;

#define set_value(set) ((set)->value) ? ((set)->value) : ((set)->def)

set_t *set_find(set_t **head, const char *key);
int set_getbool(set_t **head, const char *key);
char *set_getstr(set_t **head, const char *key);

/* Two very useful generic evaluators. */
char *set_eval_int(set_t *set, char *value);
char *set_eval_bool(set_t *set, char *value);
