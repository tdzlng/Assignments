#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// global static variable
static long int counter = 0;
static pthread_mutex_t lock;

// use a flag -pthread to compile program with library pthread.h
void* thread_handler1(void* arg);
void* thread_handler2(void* arg);
void* thread_handler3(void* arg);

void* thread_handler1(void* arg) {
    pthread_t self;
    self = pthread_self();
    sleep(3);
    printf("Thread ID %lu: process here ",self);

    for(long int i=0;i<1000000; ++i)
    {    
    pthread_mutex_lock(&lock);
    ++counter;                              // critical section
    pthread_mutex_unlock(&lock);
    }
    printf("add counter 1000000\n ");

    

    pthread_exit(NULL);
}

void* thread_handler2(void* arg) {
    pthread_t self;
    self = pthread_self();
    sleep(3);
    printf("Thread ID %lu: process here ",self);

    for(long int i=0;i<1; ++i)
    {    
    pthread_mutex_lock(&lock);
    ++counter;                              // critical section
    pthread_mutex_unlock(&lock);
    }
    printf("add counter 1\n ");

    

    pthread_exit(NULL);
}

void* thread_handler3(void* arg) {
    pthread_t self;
    self = pthread_self();
    sleep(3);
    printf("Thread ID %lu: process here ",self);

    for(long int i=0;i<10000; ++i)
    {    
    pthread_mutex_lock(&lock);
    ++counter;                              // critical section
    pthread_mutex_unlock(&lock);
    }
    printf("add counter 10000\n ");

    

    pthread_exit(NULL);
}


void main(){
    pthread_t threadID1;
    pthread_t threadID2;
    pthread_t threadID3;
    void *retVal;

    pthread_create(&threadID2, NULL, thread_handler1, NULL);
    printf("Thread %lu is initialized\n",threadID2);    

    pthread_create(&threadID1, NULL, thread_handler2, NULL);
    printf("Thread %lu is initialized\n",threadID1);

    pthread_create(&threadID3, NULL, thread_handler3, NULL);
    printf("Thread %lu is initialized\n",threadID3);

    while(1){
        sleep(1);
        printf("counter= %ld\n",counter);
    }

    pthread_detach(threadID3);
    pthread_detach(threadID2);
    pthread_detach(threadID1);

}