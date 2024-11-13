#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "response.h"
#include "server.h"

static int readFile(char *buff, char *path){
    FILE *file = fopen(path, "rb");

    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);

    buff = (char *)realloc(buff, fileSize);
    size_t n = fread(buff, sizeof(char), fileSize, file);
    if(n < fileSize){
        fprintf(stderr, "could not read entire file file.\n");
        return -1;
    }
    return n;
}

int checkPermission(HttpRequest *request){
    // check if req access dir ro file
    // if dir then access its server.index file
    // add constrains for ../.. path

    if(access(request->path, F_OK | R_OK) == 0){
        return 1;
    }
    return 0;
}

HttpResponse *generateResponse(HttpRequest *request){
    char *buff = NULL;
    // check if its static file or need execution
    size_t n = readFile(buff, request->path);
    // check n

    HttpResponse *response = (HttpResponse *) malloc(sizeof(HttpResponse));
    response->contentLength = n;
    response->content = buff;
    return response;
}