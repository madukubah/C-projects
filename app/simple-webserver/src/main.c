#include <stdio.h>
#include <string.h>

#include "server.h"

#define CONFIG_PATH "/etc/app/simple-webserver/config/httpd.conf"

int main(int argc, const char *argv[] ){
    char configPath[512];
    memset(configPath, 0, 512);
    memcpy(configPath, CONFIG_PATH, strlen(CONFIG_PATH));

    
    if(argc > 1){
        for(int i = 1; i < argc; i++){
            if(memcmp(argv[i], "--conf", 6) == 0){
                if(*(argv[i] + 6) == '='){
                    int len = strlen(argv[i] + 7);
                    memset(configPath, 0, 512);
                    memcpy(configPath, argv[i] + 7, len);
                }else{
                    fprintf(stderr, "--conf does not have a value.\nUsage: --conf=[PATH]\n");
                    return 1;
                }
            }
        }
    }
    printf("%s\n", configPath);
    if(!initServer(configPath)){
        return 1;
    }
    if(!startServer()){
        return 1;
    }
    return 0;
}