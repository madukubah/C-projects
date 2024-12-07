#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#include "response.h"
#include "server.h"
#include "./utils/memory.h"

static int readFile(char **buff, char *path){
    FILE *file = fopen(path, "rb");

    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);

    char *_buff = ALLOCATE(char, fileSize + 1);
    size_t n = fread(_buff, sizeof(char), fileSize, file);
    if(n < fileSize){
        fprintf(stderr, "could not read entire file.\n");
        FREE(char, _buff);
        return -1;
    }
    _buff[n] = '\0';

    *buff = _buff;
    fclose(file);
    return n+1;
}

void sendhttpHeader(int c, int statucCode, ContentType type){
    // header line
    char *m;
    switch (statucCode)
    {
        case STATUS_OK:
            m = "HTTP/1.0 200 OK\n" ;
            write(c, m, strlen(m));
            break;
        case STATUS_NOT_FOUND:
            m = "HTTP/1.0 404 NOT FOUND\n" ;
            write(c, m, strlen(m));
            break;
        case STATUS_SRV_ERR:
            m = "HTTP/1.0 500 SERVER ERROR\n" ;
            write(c, m, strlen(m));
            break;
        default:
            m = "HTTP/1.0 200 OK\n" ;
            write(c, m, strlen(m));
            break;
    }
    
    // content type
    switch(type){
        case TXT_PLAIN:
            m = "Content-Type: text/plain\n";
            write(c, m, strlen(m));
            break;
        case TXT_HTML:
            m = "Content-Type: text/html\n";
            write(c, m, strlen(m));
            break;
        default:
            m = "Content-Type: text/plain\n";
            write(c, m, strlen(m));
    }

}

void sendContent(int c, char *content, size_t contentLength){
    // content length
    char _size[128];
    memset(_size, 0, 128);

    snprintf(_size, 127, "Content-Length: %zu\n\n", contentLength);
    printf("%s \n", _size);
    write(c, _size, strlen(_size));

    printf("%s \n", content);
    write(c, content, contentLength);
}

int checkPermission(HttpRequest *request){
    // add constrains for ../.. path
    if(access(request->path, F_OK | R_OK) != 0){
        return 0;
    }

    return 1;
}

HttpResponse *generateResponse(HttpRequest *request){
    // TODO : check if its static file or need execution
    // for now only static html
    // TODO: check file extention
    HttpResponse *response = ALLOCATE(HttpResponse, 1);
    char *buff;

    size_t n = readFile(&buff, request->path);
    if((int)n == -1){
        char *content = ALLOCATE(char, 50);
        strncpy(content, "HTTP server error.\0", 20); // TODO: need to improve
        response->statusCode = STATUS_SRV_ERR;
        response->type = TXT_PLAIN;
        response->content = content;
        response->contentLength = strlen(content);
        return response;
    }
    response->statusCode = STATUS_OK;
    response->type = request->accept;
    response->contentLength = n;
    response->content = buff;
    return response;
}