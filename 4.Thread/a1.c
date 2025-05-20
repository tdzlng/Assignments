#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


// use a flag -pthread to compile program with library pthread.h
void* thread_handler(void* arg);
void* thread_handler(void* arg) {
    int *ret = (int*)malloc(sizeof(int));
    pthread_t self;
    self = pthread_self();
    sleep(2);
    printf("Thread ID %lu: Hello from thread\n",self);

    *ret = 0;

    pthread_exit(ret);
}


void main(){
    pthread_t threadID1;
    pthread_t threadID2;
    void *retVal;

    pthread_create(&threadID2, NULL, thread_handler, NULL);
    printf("Thread %lu is initialized\n",threadID2);    

    pthread_create(&threadID1, NULL, thread_handler, NULL);
    printf("Thread %lu is initialized\n",threadID1);



    pthread_join(threadID1, &retVal);
    printf("thread %lu is return value and ternminate here and continue the next instruction code\n",threadID1);
    if(retVal != NULL){
        free(retVal);
    }
    pthread_join(threadID2, &retVal);
    printf("thread %lu is return value and ternminate here and continue the next instruction code\n",threadID2);
    if(retVal != NULL){
        free(retVal);
    }

}

// khi nao thread ket thuc?, su dung ham pthread