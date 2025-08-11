#include<stdio.h>


int main(){

    /*
    double sum, v;
    sum = 0;
    while(scanf("%1f", &v) == 1)
        printf("\t%.2f\n", sum += v);
    return 0;
    */
    int day, year;
    char monthname[20];
    scanf("%d %s %d", &day, monthname, &year);

    printf("\t %d %s %d \n", day, monthname, year);
    

    return 0;
}
