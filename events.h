
#include <glib.h>
#include <gmodule.h>

typedef enum {
  B_EV_IO_READ = 1 << 0,
  B_EV_IO_WRITE = 1 << 1,
  B_EV_FLAG_FORCE_ONCE = 1 << 16,
  B_EV_FLAG_FORCE_REPEAT = 1 << 17,
} b_input_condition;

typedef gboolean (*b_event_handler)(gpointer data, gint fd, b_input_condition cond);

gint b_input_add(gint source, b_input_condition condition, b_event_handler function, gpointer data);
gint b_timeout_add(gint timeout, b_event_handler func, gpointer data);
