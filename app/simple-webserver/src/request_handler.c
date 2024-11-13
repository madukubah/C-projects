#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "request_handler.h"
#include "server.h"
#include "./utils/scanner.h"

char *clientRead(int c){
    char *buff = (char *)malloc(512);
    
    int n = 0, cn = 0;
    while(1){
        cn = read(c, buff + n, 512);
        if(cn < 0){
            fprintf(stderr, "read() error.\n");
            return NULL;
        }
        if(cn == 0){
            fprintf(stderr, "nothing to read().\n");
            free(buff);
            return NULL;
        }
        if(cn == 512){
            n += 512;
            buff = (char *)realloc(buff, n + 512);
        }else{
            buff[n + cn - 1] = '\0';
            break;
        }
    }

    return buff;
}

HttpRequest *allocateRequest(char *method, char *url, char *httpVersion){
    HttpRequest *request = (HttpRequest *)malloc(sizeof(HttpRequest));
    memset(request->method, 0, 16);
    memset(request->url, 0, 256);
    memset(request->httpVersion, 0, 8);
    request->path = NULL;

    strncpy(request->method, method, 16);
    strncpy(request->url, url, 256);
    strncpy(request->httpVersion, httpVersion, 8);

    request->path = (char *)malloc(strlen(server.rootDocument) + strlen(request->url) + 1);
    strcat(request->path, server.rootDocument);
    strcat(request->path, request->url);

    return request;

}

HttpRequest *parseHttp(char *str){
    Scanner scanner;
    initScanner(&scanner, str);

    // read request line
    char *line = readLine(&scanner);
    if(strlen(line) <= 0){
        fprintf(stderr, "there is no request line.\n");
        return NULL;
    }

    char *p = line;
    // http method
    char *method = line;
    while(*p != ' ' && *p != '\0') p++;
    if(*p != ' '){
        fprintf(stderr, "parseHttp().No space found after method.\n");
        return NULL;
    }
    *p = '\0';
    p++;

    // url
    char *url = p;
    while(*p != ' ' && *p != '\0') p++;
    if(*p != ' '){
        fprintf(stderr, "parseHttp().No space found after url.\n");
        return NULL;
    }
    *p = '\0';
    p++;

    // http version: read until end of line
    char *httpVersion = p;

    // TODO: parsing request options

    HttpRequest *req = allocateRequest(method, url, httpVersion);
    return req;
}