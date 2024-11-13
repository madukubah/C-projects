#ifndef simple_request_handler_h
#define simple_request_handler_h

typedef struct {
    char method[16];
    char url[256];
    char httpVersion[8];
    char *path;

} HttpRequest;

char *clientRead(int c);
HttpRequest *allocateRequest(char *method, char *url, char *httpVersion);
HttpRequest *parseHttp(char *str);

#endif
