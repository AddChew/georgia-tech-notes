#include <stdio.h>
#include <pthread.h>

#define NUMTHREADS 4

void *hello(void *arg) {
    printf("Hello Thread\n");
}

int main(void) {
    int i;
    pthread_t tid[NUMTHREADS];
    for (i = 0; i < NUMTHREADS; i++) {
        pthread_create(&tid[i], NULL, hello, NULL);
    }
    for (i = 0; i < NUMTHREADS; i++) {
        pthread_join(tid[i], NULL);
    }    
}