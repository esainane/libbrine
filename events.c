#include "events.h"

typedef struct _GaimIOClosure {
	b_event_handler function;
	gpointer data;
	guint flags;
} GaimIOClosure;

gint b_input_add(gint source, b_input_condition condition, b_event_handler function, gpointer data)
{
	GaimIOClosure *closure = g_new0(GaimIOClosure, 1);
	GIOChannel *channel;
	GIOCondition cond = 0;
	int st;

	closure->function = function;
	closure->data = data;
	closure->flags = condition;

	if (condition & B_EV_IO_READ) {
		cond |= GAIM_READ_COND;
	}
	if (condition & B_EV_IO_WRITE) {
		cond |= GAIM_WRITE_COND;
	}

	channel = g_io_channel_unix_new(source);
	st = g_io_add_watch_full(channel, G_PRIORITY_DEFAULT, cond,
	                         gaim_io_invoke, closure, gaim_io_destroy);

	event_debug("b_input_add( %d, %d, %p, %p ) = %d (%p)\n", source, condition, function, data, st, closure);

	g_io_channel_unref(channel);
	return st;
}

gint b_timeout_add(gint timeout, b_event_handler func, gpointer data)
{
	/* GSourceFunc and the BitlBee event handler function aren't
	   really the same, but they're "compatible". ;-) It will do
	   for now, BitlBee only looks at the "data" argument. */
	gint st = g_timeout_add(timeout, (GSourceFunc) func, data);

	event_debug("b_timeout_add( %d, %p, %p ) = %d\n", timeout, func, data, st);

	return st;
}
