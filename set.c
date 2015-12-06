

char *SET_INVALID = "nee";

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
