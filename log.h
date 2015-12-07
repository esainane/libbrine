
typedef enum {
	LOGLVL_INFO,
	LOGLVL_WARNING,
	LOGLVL_ERROR,
#ifdef DEBUG
	LOGLVL_DEBUG,
#endif
} loglvl_t;

// FIXME: These should be routed to the network buffer
#define imcb_log(X, fmt, args...) log_message(LOGLVL_INFO, "User < " fmt, ## args)
#define imcb_error(X, fmt, args...) log_message(LOGLVL_ERROR, "User < " fmt, ## args)

void log_message(int level, const char *message, ...);
