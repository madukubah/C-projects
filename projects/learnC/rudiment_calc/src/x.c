#include<stdio.h>

int main(){
    
    int day, year, month;
    char monthname[20];
    
    char line[100];
    while(getline(line, sizeof(line)) > 0){
        if(sscanf(line, "%d %s %d", &day, monthname, &year) == 3){
            printf("valid: %s\n", line);
        }
        else if (sscanf(line, "%d/%d/%d", &day, &month, &year) == 3){
            printf("valid: %s\n", line);
        }
        else 
            printf("invalid: %s\n", line);
    }
    return 0;
}
