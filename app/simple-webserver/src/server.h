#ifndef simple_server_h
#define simple_server_h

typedef struct {
    int s;
    char *listen;
    int port;
    char *rootDocument;
    char *index;
} Server;

extern Server server;

int initServer(char *configPath);
int startServer();
void stopServer();

#endif