#ifndef simple_request_handler_h
#define simple_request_handler_h

#include "common.h"

typedef struct {
    char method[16];
    char url[256];
    char httpVersion[8];
    char *path;
    ContentType accept;
} HttpRequest;

char *clientRead(int c);
HttpRequest *parseHttp(char *str);

#endif
