#include <pthread.h>
#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h>

// definition constant
#define D_LIMIT_NUM (9)

// macro function
#define M_GET_RANDOM ( (rand() % (D_LIMIT_NUM+1)) + 1)

// global static variable

static int container = 0;
static pthread_mutex_t lock;
static pthread_cond_t condProduce;


// function definition

void* consumer(void* argv){
    for(int i=0;i<10;++i){
        pthread_mutex_lock(&lock);

        pthread_cond_wait(&condProduce, &lock);     // acctually pthread_mutex_unlock(&lock) -> wait signal condProduce -> pthread_mutex_lock(&lock)
        printf("get %d\n",container);
        container = 0;
    
        pthread_mutex_unlock(&lock);
    }


    pthread_exit(NULL);
}

void* producer(void* argv){

    for(int i=0;i<10;++i){

        while( container > 0 )                       // wait until container is empty
        {
            sleep(1);
        }
        pthread_mutex_lock(&lock);

        container = M_GET_RANDOM;
        printf("set %d\n",container);
        pthread_cond_signal(&condProduce);          // acctually send signal condProduce
        pthread_mutex_unlock(&lock);
    }

    pthread_exit(NULL);
}

void main() {
    pthread_t tID1;
    pthread_t tID2;


    pthread_create(&tID1, NULL, consumer, NULL);
    pthread_create(&tID2, NULL, producer, NULL);


    pthread_join(tID1, NULL);
    pthread_join(tID2, NULL);


    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&condProduce);
}