#include<stdio.h>

unsigned getbits(unsigned x, int p, int n){
    return (x >> (p+1-n)) & ~(~0 << n);
}

int main(){
    printf("bitwise \n");
    printf("result: %d \n", getbits(14, 2, 1));
    printf("result: %d \n", getbits(14, 2, 2));
    printf("result: %d \n", getbits(14, 2, 3));
    return 0;
}
