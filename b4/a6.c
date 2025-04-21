#include <pthread.h>
#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h>

// define constant number
#define D_NUM (1000000)
#define D_LIMIT_NUM (1000)

// macro function
#define M_GET_RANDOM ( (rand() % (D_LIMIT_NUM+1)) )

// global static variable
static int sum = 0;
static pthread_mutex_t lock;
static int arr[D_NUM];



// definition function
void init_array(int* arr, int n){
    int sumArr = 0;
    for(int i=0;i<n;++i){
        arr[i] = M_GET_RANDOM;
        // printf("%d ",arr[i]);  this for logging data
        sumArr += arr[i];
    }
    printf("init done with the sum is %d\n",sumArr);
}

void* sumCal(void* argv){
    int *mode = NULL;
    int check;
    if( NULL != argv ){
        mode = (int*)argv;
        for(int i=0; i<D_NUM;++i){
            check = i%4;
            if(check == *mode){
                pthread_mutex_lock(&lock);
                sum += arr[i];
                // printf("%ld No.%d %d += %d mode: %d\n",pthread_self(),i,sum,arr[i],*mode); // this for logging data
                pthread_mutex_unlock(&lock);
            } 
        }
    }
    pthread_exit(NULL);
}

void main(){
    pthread_t tID[4];
    init_array(arr, D_NUM);
    int argv[4]={0,1,2,3};
    int i;
    for(i = 0;i<4;++i){
        pthread_create(&tID[i], NULL, sumCal, (void*)(argv+i));
    }


    for(i = 0;i<4;++i){
        pthread_join(tID[i], NULL);
    }
    printf("sum: %d\n",sum);

    pthread_mutex_destroy(&lock);
    
}