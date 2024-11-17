#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "server.h"
#include "config.h"
#include "request_handler.h"
#include "response.h"
#include "./utils/memory.h"

Server server;

static char *readFile(char *configPath){
    FILE *file = fopen(configPath, "rb");
    if(file == NULL){
        fprintf(stderr, "Error: fopen %s\n.", configPath);
        exit(74);
    }
    fseek(file, 0L, SEEK_END);
    int fileSize = ftell(file);
    rewind(file);

    char *buff = ALLOCATE(char, fileSize + 1);
    int readBytes = fread(buff, sizeof(char), fileSize, file);
    if(readBytes != fileSize){
        fprintf(stderr, "Read file incomplete.");
        exit(74);
    }

    buff[fileSize] = '\0';

    fclose(file);
    return buff;
}

static int acceptConnection(){
    struct sockaddr_in cli;
    socklen_t addrlen;

    addrlen = 0;
    memset(&cli, 0, sizeof(cli));
    int c = accept(server.s, (struct sockaddr *)&cli, &addrlen);
    if(c < 0){
        fprintf(stderr, "accept() error.");
        return 0;
    }
    printf("Client: %d.\n\n", c);
    return c;
}

static void serve(int c){
    char *buff = clientRead(c);
    if(buff == NULL){
        close(c);
        return;
    };

    HttpRequest *httpRequest = parseHttp(buff);
    if(httpRequest == NULL){
        fprintf(stderr, "Failed to parse request.\n");
        FREE(char, buff);
        close(c);
        return;
    }
    printf("method: %s\turl: %s\tversion: %s\tpath: %s\n", httpRequest->method, httpRequest->url, httpRequest->httpVersion, httpRequest->path);

    int perm = checkPermission(httpRequest);
    if(perm){
        HttpResponse *response = generateResponse(httpRequest);
        if(response != NULL){
            sendhttpHeader(c, response->statusCode, response->type);
            sendContent(c, response->content, response->contentLength);
            FREE(char, response->content);
            FREE(HttpResponse, response);
        }
    }else{
        // TODO: return 403 or 404
        sendhttpHeader(c, 404, TXT_PLAIN);
        sendContent(c, "Not Found.", strlen("Not Found."));
    }

    FREE(char, httpRequest->path);
    FREE(HttpRequest, httpRequest);
    FREE(char, buff);
    close(c);
}


int initServer(char *configPath){
    server.s = -1;
    server.listen = NULL;
    server.port = 0;
    server.rootDocument = NULL;

    char *buff = readFile(configPath);
    int res = parseConfig(buff);
    FREE(char, buff);

    if(res == 0){
        return 0;
    }

    return 1;
}

int serverListen(){
    struct sockaddr_in srv;

    server.s = socket(AF_INET, SOCK_STREAM, 0);
    if(server.s < 0){
        fprintf(stderr, "socket() error");
        return 0;
    }

    srv.sin_family = AF_INET;
    srv.sin_addr.s_addr = inet_addr(server.listen);
    srv.sin_port = htons(server.port);

    if(bind(server.s, (struct sockaddr *)&srv, sizeof(srv)) == -1){
        fprintf(stderr, "bind() error");
        return 0;
    }

    if(listen(server.s, 5)){
        fprintf(stderr, "listen() error");
        return 0;
    }

    printf("listening on: %s:%d\n", server.listen, server.port);
    return 1;
}
int startServer(){
    int c;
    if(!serverListen()){
        return 0;
    }

    while(1){
        c = acceptConnection();
        if(!c){
            continue;
        }

        serve(c);

    }
    return 1;
}