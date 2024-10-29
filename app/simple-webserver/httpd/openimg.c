#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    FILE *file;

    char path[100];
    strcpy(path, ".");

    file = fopen(strcat(path, "/img/a.jpeg"), "rb");
    if(file == NULL){
        printf("could not open file\n");
        return 1;
    }

    fseek(file, 0L, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    printf("file_size: %d\n", file_size);

    size_t content_size = (sizeof(char) * 512) + file_size;
    char *buffer = (char *)malloc(content_size);
    memset(buffer, 0, content_size);

    char temp[] = "HTTP/1.0 200 OK\n"
        "Server: httpd.c\n"
        "Cache-Control: xxxx\n"
        "Content-Language: en\n"
        "Expires: -1\n"
        "X-Frame-Options: SAMEORIGIN\n"
        "Content-Type: image/jpeg\n"
        "Content-Length: %d\n";

    printf("size temp: %d\n", strlen(temp));

    snprintf(buffer, content_size, 
        temp,
        file_size
    );
    printf("buff size: %d\n", strlen(buffer));

    // strcpy(buffer, temp);
    // strcat(buffer, );
    size_t read_size = fread(buffer + strlen(buffer), sizeof(char), file_size, file);
    if(read_size < file_size){
        printf("could not read entire file file\n");
        return 1;
    }


    printf("success\n");
    printf("%d\n", strlen(buffer));
    printf("%s\n", buffer);

    return 0;
}