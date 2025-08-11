#include<stdio.h>
#include<stdbool.h>

char *squeeze(char *s1, char *s2){
    int i = 0, j = 0;
    for(; s1[i] != '\0'; i++){
        //printf("%c \n", s1[i]);
        bool f = false;
        for(int k = 0; s2[k] != '\0'; k++){
            if(s1[i] == s2[k]){
                f = true;
                //printf("found \n");
                break;
            }
        }
        if(!f){
            s1[j++] = s1[i];
        }
    }
    s1[j] = '\0';
    return s1;
}

int main(int argc, const char *argv[]){
    printf("alohaa \n");    
    char s1[] = "ababababab";
    char s2[] = "a";

    printf("s1: %s \n", s1);    
    printf("s2: %s \n", s2);    
    printf("squeeze : %s \n", squeeze(s1, s2));
    return 0;
}
