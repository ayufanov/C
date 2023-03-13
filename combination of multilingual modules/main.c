#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#define LEN 5

extern int32_t* f(int32_t arr1[LEN], int len);

int main(void){
    srand(123);
    int32_t A[LEN];
    for (int i = 0; i < LEN; i++){
        A[i] = rand() % 1000;
    }
    for (int i = 0; i < LEN; i++){
        printf("%"PRId32, A[i]);
        printf("\n");
    }
     printf("результат = ");
     printf("%d\n", f(A, LEN)); 

    return 0;
}
