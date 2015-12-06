
struct http_request;
typedef void (*http_input_function)(struct http_request *);
struct http_request *http_dorequest(char *host, int port, int ssl, char *request, http_input_function func,
                                    gpointer data);

void http_close(struct http_request *req);
