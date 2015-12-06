void ssl_disconnect(void *conn_);
void *ssl_connect(char *host, int port, gboolean verify, ssl_input_function func, gpointer data);
char *ssl_verify_strerror(int code);
