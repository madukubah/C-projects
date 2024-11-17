#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#include "request_handler.h"
#include "server.h"
#include "./utils/scanner.h"
#include "./utils/memory.h"

static char* constructPath(char *url){
    // if url not contain filename extention *.x then
    // add default index to path
    char *path;
    bool isFIle = false;
    int len;
    for(int i=strlen(url); i > 0; i--){
        if(url[i] == '.'){
            isFIle = true;
        }
    }
    if(isFIle){
        len = strlen(server.rootDocument) + strlen(url) + 1;
        path = ALLOCATE(char, len);
        memcpy(path, server.rootDocument, strlen(server.rootDocument));
        memcpy(path + strlen(server.rootDocument), url, strlen(url));
        
    }else{
        len = strlen(server.rootDocument) + strlen(url) + strlen(server.index) + 1;
        path = ALLOCATE(char, len);
        memcpy(path, server.rootDocument, strlen(server.rootDocument));
        memcpy(path + strlen(server.rootDocument), url, strlen(url));
        memcpy(path + strlen(server.rootDocument) + strlen(url), server.index, strlen(server.index));
        
    }
    path[len - 1] = '\0';
    return path;
}

static char *getFileExt(char *reqPath){
    for(int i=strlen(reqPath); i > 0; i--){
        if(reqPath[i] == '.'){
            return &reqPath[i];
        }
    }
    return NULL;
}

// TODO: add more extention for images and other file ext
// develop regex or string internings
static ContentType getAccept(char *reqPath){
    char *extention = getFileExt(reqPath);
    if(extention == NULL) return TXT_PLAIN;
    // ext: html
    if(strlen(extention) == 5 && memcmp(extention, ".html", 5) == 0){
        return TXT_HTML;
    }

    // ext: jpeg
    if(strlen(extention) == 5 && memcmp(extention, ".jpeg", 5) == 0){
        return IMAGE;
    }

    return TXT_PLAIN;
}

static HttpRequest *allocateRequest(char *method, char *url, char *httpVersion){
    HttpRequest *request = ALLOCATE(HttpRequest, 1);
    memset(request->method, 0, 16);
    memset(request->url, 0, 256);
    memset(request->httpVersion, 0, 8);
    request->path = NULL;

    strncpy(request->method, method, 16);
    strncpy(request->url, url, 256);
    strncpy(request->httpVersion, httpVersion, 8);

    request->path = constructPath(request->url); 
    request->accept = getAccept(request->path); 
    return request;
}

char *clientRead(int c){
    char *buff = ALLOCATE(char, 512);
    
    int n = 0, cn = 0;
    while(1){
        cn = read(c, buff + n, 512);
        if(cn < 0){
            fprintf(stderr, "read() error.\n");
            return NULL;
        }
        if(cn == 0){
            fprintf(stderr, "nothing to read().\n");
            FREE(char, buff);
            return NULL;
        }
        if(cn == 512){
            int _n = n;
            n += 512;
            buff = REALLOCATE(char, buff, _n, n + 512);
        }else{
            buff[n + cn - 1] = '\0';
            break;
        }
    }

    return buff;
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