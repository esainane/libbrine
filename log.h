
typedef enum {
	LOGLVL_INFO,
	LOGLVL_WARNING,
	LOGLVL_ERROR,
#ifdef DEBUG
	LOGLVL_DEBUG,
#endif
} loglvl_t;

void log_message(int level, const char *message, ...);
