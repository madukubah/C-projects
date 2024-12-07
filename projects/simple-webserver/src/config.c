#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#include "config.h"
#include "server.h"
#include "./utils/scanner.h"
#include "./utils/memory.h"

int parseConfig(char *str){
    Scanner scanner;
    initScanner(&scanner, str);

    for(;;){
        skipWhiteSpace(&scanner);
        scanner.start = scanner.current;

        if(isAtEnd(&scanner)) break;

        char c = advance(&scanner);

        if(isAlpha(c)){
            switch(c){
                case 'l':
                    while(isAphaNumeric(peek(&scanner)) && !isAtEnd(&scanner)) advance(&scanner);
                    if((scanner.current - scanner.start) == 6){
                        if(memcmp(scanner.start, "listen", 6) == 0){
                            printf("listen ");
                            skipWhiteSpace(&scanner);
                            scanner.start = scanner.current;
                            while(((isAphaNumeric(peek(&scanner)) || peek(&scanner) == '.') && !isAtEnd(&scanner))) advance(&scanner);
                            size_t len = (int)(scanner.current - scanner.start);
                            if(len > 0){
                                server.listen = ALLOCATE(char, len + 1);
                                memcpy(server.listen, scanner.start, len);
                                server.listen[len + 1] = '\0';
                            }else{
                                fprintf(stderr, "Fail to parse listen value.");
                                return 0;
                            }
                            
                            printf("%s \n", server.listen);
                        }
                    }
                    break;
                case 'p':
                    while(isAphaNumeric(peek(&scanner)) && !isAtEnd(&scanner)) advance(&scanner);
                    if((scanner.current - scanner.start) == 4){
                        if(memcmp(scanner.start, "port", 4) == 0){
                            printf("port ");
                            skipWhiteSpace(&scanner);
                            scanner.start = scanner.current;
                            while((isDigit(peek(&scanner)) && !isAtEnd(&scanner))) advance(&scanner);
                            if(scanner.current - scanner.start <= 0){
                                fprintf(stderr, "Fail to parse port value.");
                                return 0;
                            }
                            server.port = atoi(scanner.start);
                            printf("%d \n", server.port);
                        }
                    }
                    break;
                case 'd':
                    while(isAphaNumeric(peek(&scanner)) && !isAtEnd(&scanner)) advance(&scanner);
                    if((scanner.current - scanner.start) == 13){
                        if(memcmp(scanner.start, "document_root", 13) == 0){
                            printf("document_root ");
                            skipWhiteSpace(&scanner);
                            scanner.start = scanner.current;
                            while(((isAphaNumeric(peek(&scanner)) || (peek(&scanner) == '/')) && !isAtEnd(&scanner))) advance(&scanner);
                            size_t len = (int)(scanner.current - scanner.start);
                            if(len > 0){
                                server.rootDocument = ALLOCATE(char, len + 1);
                                memcpy(server.rootDocument, scanner.start, len);
                                server.rootDocument[len + 1] = '\0';
                            }else{
                                fprintf(stderr, "Fail to parse document_root value.");
                                return 0;
                            }
                            
                            printf("%s \n", server.rootDocument);
                        }
                    }
                    break;
                case 'i':
                    while(isAphaNumeric(peek(&scanner)) && !isAtEnd(&scanner)) advance(&scanner);
                    if((scanner.current - scanner.start) == 5){
                        if(memcmp(scanner.start, "index", 5) == 0){
                            printf("index ");
                            skipWhiteSpace(&scanner);
                            scanner.start = scanner.current;
                            while(((isAphaNumeric(peek(&scanner)) || peek(&scanner) == '.') && !isAtEnd(&scanner))) advance(&scanner);
                            size_t len = (int)(scanner.current - scanner.start);
                            if(len > 0){
                                server.index = ALLOCATE(char, len + 1);
                                memcpy(server.index, scanner.start, len);
                                server.index[len + 1] = '\0';
                            }else{
                                fprintf(stderr, "Fail to parse index value.");
                                return 0;
                            }
                            
                            printf("%s \n", server.index);
                        }
                    }
                    break;
            }
        }

    }
    return 1;
}
