
int ssl_pending(void *conn);
b_input_condition ssl_getdirection(void *conn);
void ssl_disconnect(void *conn_);
void *ssl_connect(char *host, int port, gboolean verify, ssl_input_function func, gpointer data);
char *ssl_verify_strerror(int code);
int ssl_read(void *conn, char *buf, int len);
int ssl_write(void *conn, const char *buf, int len);
int ssl_getfd(void *conn);
