#ifndef simple_response_h
#define simple_response_h

#include <stdlib.h>
#include "request_handler.h"

typedef struct {
    int type;
    size_t contentLength;
    char *content;
} HttpResponse;

int checkPermission(HttpRequest *request);
HttpResponse *generateResponse(HttpRequest *request);
#endif