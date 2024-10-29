/* httpd.c */

// #include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define LISTEN_ADDR "0.0.0.0"

#define LOCATION "/etc/app/simple-webserver/httpd"

/* structures */

struct sHttpRequest {
    char method[8];
    char url[128];
};

typedef struct sHttpRequest httpreq;

/* global */
char *error;

int srv_init(int portno){
    int s;
    struct sockaddr_in srv;

    s = socket(AF_INET, SOCK_STREAM, 0);

    if(s < 0){
        error = "socket() error.";
        return 0;
    }

    srv.sin_family = AF_INET;
    srv.sin_addr.s_addr = inet_addr(LISTEN_ADDR);
    srv.sin_port = htons(portno);

    if(bind(s, (struct sockaddr *)&srv, sizeof(srv))){
        close(s);
        error = "bind() error.";
        return 0;
    }

    if(listen(s, 5)){
        close(s);
        error = "listen() error.";
        return 0;
    }

    return s;
}

/* return 0 in error, or return new client socket fd */
int cli_accept(int s){
    int c;
    socklen_t addrlen;
    struct sockaddr_in cli;

    addrlen = 0;
    memset(&cli, 0, sizeof(cli));
    c = accept(s, (struct sockaddr *)&cli, &addrlen);
    if(c < 0){
        error = "accept() error.";
        return 0;
    }
    return c;
}

httpreq *parse_http(char *str){
    if (str == NULL) {
        error = "Error: Input string is NULL.\n";
        return NULL;
    }

    httpreq *req = (httpreq *)malloc(sizeof(httpreq));
    if (req == NULL) {
        error = "Error: Memory allocation failed.";
        return 0;
    }

    // Optional: Zero out the memory if needed
    // memset(req, 0, sizeof(httpreq));

    char *p = str;

    /* METHOD */
    while (*p && *p != ' ')p++;

    if (*p == ' ') {
        *p = '\0';  // Replace space with null terminator to terminate the method
    } else {
        error = "parse_http(): No space found after method.";
        free(req);
        return 0;
    }
    strncpy(req->method, str, 7);

    p++;
    str = p;

    /* URL */
    while (*p && *p != ' ')p++;

    if (*p == ' ') {
        *p = '\0';  // Replace space with null terminator to terminate the method
    } else {
        error = "parse_http(): No space found after method.";
        free(req);
        return 0;
    }
    strncpy(req->url, str, 127);

    return req;
}

char *cli_read(int c){
    static char buffer[1024];
    if(read(c, buffer, 1023) < 0){
        error = "read() error.";
        return 0;
    }
    return buffer;
}

void http_headers(int c, int status_code){
    char buff[512];
    memset(buff, 0, 512);
    int n;

    snprintf(buff, 511, 
        "HTTP/1.0 %d OK\n"
        "Server: httpd.c\n"
        "Cache-Control: xxxx\n"
        "Content-Language: en\n"
        "Expires: -1\n"
        "X-Frame-Options: SAMEORIGIN\n"
        ,
        status_code
    );
    n = strlen(buff);
    write(c, buff, n);
    return;
}

void http_response(int c, char *content_type, char *response){
    char buff[512];
    memset(buff, 0, 512);
    int n;

    snprintf(buff, 511, 
        "Content-Type: %s\n"
        "Content-Length: %d\n"
        "\n%s\n",
        content_type, (int)strlen(response), response
    );
    n = strlen(buff);
    write(c, buff, n);
    return;
}

int sendfile(int c, char *path){
    printf("path %s\n", path);

    char base_path[100];
    strcpy(base_path, LOCATION);
    strcat(base_path, path);
    if(access(base_path, F_OK) != 0){
        http_headers(c, 404);
        http_response(c, "text/plain", "File not found.");
    }

    FILE *file = fopen(base_path, "rb");
    if(file == NULL){
        http_headers(c, 500);
        http_response(c, "text/plain", "HTTP server error.");
        return 0;
    }

    fseek(file, 0L, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    size_t response_size = (sizeof(char) * 512) + file_size;
    char *buffer = (char *)malloc(response_size);
    if(buffer == NULL){
        http_headers(c, 500);
        http_response(c, "text/plain", "HTTP server error.");
        return 0;
    }
    memset(buffer, 0, response_size);

    snprintf(buffer, response_size, 
        "Content-Type: image/jpeg\n"
        "Content-Length: %zu\n\n",
        file_size
    );

    printf("buff size:%zu\n", strlen(buffer));

    size_t read_size = fread(buffer + strlen(buffer), sizeof(char), file_size, file);
    if(read_size != file_size){
        http_headers(c, 500);
        http_response(c, "text/plain", "could not read entire file file");
        free(buffer);
        fclose(file);
        return 0;
    }

    http_headers(c, 200);    
    write(c, buffer, response_size);
    free(buffer);
    fclose(file);
    return 1;
}

void cli_conn(int s, int c){
    httpreq *req;
    char *p;
    char *res;

    p = cli_read(c);
    if(!p){
        fprintf(stderr, "%s\n", error);
        close(c);
        return;
    }
    req = parse_http(p);
    if(!req){
        fprintf(stderr, "%s\n", error);
        close(c);
        return;
    }
    printf("Method: %s\nURL: %s\n", req->method, req->url);
    /* TODO: improve security for checking ""../.."" etc */
    if((!strcmp(req->method, "GET")) && (!strncmp(req->url, "/img/", 5))){
        printf("Get img: %s\n", req->url);
        if(strstr(req->url, "..")){
            http_headers(c, 300);
            http_response(c, "text/plain", "Access denied.");
        }else{
            sendfile(c, req->url);
        }
    }
    else if((!strcmp(req->method, "GET")) && (!strcmp(req->url, "/app/webpage"))){
        printf("Get webpage\n");
        res = "<html>"
                "<h2>Hello World</h2>"
                "<img src=\"/img/a.jpeg\" />"
                "</html>";
        http_headers(c, 200);
        http_response(c, "text/html", res);
    }else{
        res = "File not found.";
        http_headers(c, 404);
        http_response(c, "text/plain", res);

    }
    free(req);
    close(c);
    return;
}

int main(int argc, char *argv[]){

    int s, c;
    char* port;

    if(argc < 2){
        fprintf(stderr, "Usage: %s <listening port>\n", argv[0]);
        return -1;
    }

    port = argv[1];

    s = srv_init(atoi(port));
    if(!s){
        fprintf(stderr, "%s\n", error);
        return -1;
    }
    printf("listening on %s:%s\n", LISTEN_ADDR, port);
    while(1){
        c = cli_accept(s);
        if(!c){
            fprintf(stderr, "%s\n", error);
            continue;
        }
        printf("Incoming connection\n");
        /*
            for the main process: return the new process id
            for the new process: return 0
        */
        if(!fork()){
            cli_conn(s, c);
        }
        // pid_t pid = fork();
        // if(pid < 0){
        //     printf("Fork failed!\n");
        // }else if(pid == 0){
        //     printf("Hello from the child process! PID: %d\n", getpid());
        //     cli_conn(s, c);
        //     exit(0);
        // }else{
        //     printf("Hello from the parent process! PID: %d, Child PID: %d\n", getpid(), pid);
        //     close(c);
        // }
    }

    return -1;
}