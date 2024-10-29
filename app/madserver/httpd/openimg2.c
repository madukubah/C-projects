#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

void printHex(char *buff, int size){
    char *p;
    int i;
    for(p = buff, i = 0; i < size; i++){
        printf("0x%02x ", *p++);
    }
    printf("\n");
    fflush(stdout);
}

int main(){
    int fd;
    char buff[512];
    char *fBuff;
    int n;

    fd = open("./img/a.jpeg", O_RDONLY);
    if(fd < 0){
        printf("error open");
    }
    // printf("%s\n", buff);
    // printHex(buff, 512);

    fBuff = (char *)malloc(512);
    int x = 0;
    while(1){
        memset(buff, 0, 512);
        n = read(fd, buff, 512);

        if(n == -1){
            free(fBuff);
            break;
        }

        if(!n){
            break;
        }
        fBuff = (char *)realloc(fBuff, x + n);
        memcpy(fBuff + x, buff, n);
        x += n;
        
        // memcpy(fBuff + x, buff, n);
        // x += n;
        // if( n < 512){
        //     break;
        // }else{
        //     fBuff = (char *)realloc(fBuff, x);
        // }
        // break;
    }

    printf("size: %d\n", x);
    // printHex(fBuff, x);
    close(fd);

    fd = open("./img/b.jpeg", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    printf("\ndest_fd: %d\n", fd);
    int wn = write(fd, fBuff, x);
    wn = write(fd, fBuff, x);
    printf("\n%d\n", wn);
    printf("\n%d\n", x);
    close(fd);


    // memset(buff, 0, 512);
    // n = read(fd, buff, 512);

    // memcpy(fBuff, buff, n);
    // printf("size: %d\n", n);
    // printHex(buff, 512);


    
    return 0;
}