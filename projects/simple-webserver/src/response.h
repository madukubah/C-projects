#ifndef simple_response_h
#define simple_response_h

#include <stdlib.h>

#include "common.h"
#include "request_handler.h"

#define STATUS_OK 200
#define STATUS_NOT_FOUND 404
#define STATUS_SRV_ERR 500


typedef struct {
    int statusCode;
    ContentType type;
    size_t contentLength;
    char *content;
} HttpResponse;

int checkPermission(HttpRequest *request);
HttpResponse *generateResponse(HttpRequest *request);
void sendhttpHeader(int c, int statucCode, ContentType type);
void sendContent(int c, char *content, size_t contentLength);
#endif