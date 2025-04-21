#include <pthread.h>
#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h>

// definition constant
#define D_LIMIT_NUM (99)

// macro function
#define M_GET_RANDOM ( (rand() % (D_LIMIT_NUM+1)) + 1)

// global static variable
static int num;
static pthread_rwlock_t rwlock;

// function definition
void* read_handler(void* arg1){
    int* ret = (int*)(malloc(sizeof(int)));
    sleep(1);
    pthread_rwlock_rdlock(&rwlock);
    *ret = num;
    printf("thread No.%ld read: %d\n",pthread_self(),num);
    pthread_rwlock_unlock(&rwlock);
    pthread_exit(ret);
}

void* write_handler(void* arg2){
    int* ret = (int*)(malloc(sizeof(int)));

    sleep(1);
    pthread_rwlock_wrlock(&rwlock);
    *ret = ++num;
    printf("thread No.%ld write: %d\n",pthread_self(),num);
    pthread_rwlock_unlock(&rwlock);
    pthread_exit(ret);
}


void main(){
    pthread_t tRead[5];
    pthread_t tWrite[2];
    void* ret = NULL;
    num = 2;
    printf("init number = %d\n",num);


    for(int i=0; i<2;++i){
        pthread_create(&tWrite[i], NULL, write_handler, NULL);
    }
    for(int i=0; i<5;++i){
        pthread_create(&tRead[i], NULL, read_handler, NULL);
    }



    for(int i=0; i<5;++i){
        pthread_join(tRead[i], &ret);
        printf("Read done with number: %d\n",*(int*)ret);
        free(ret);
    }
    for(int i=0; i<2;++i){
        pthread_join(tWrite[i], &ret);
        printf("Write done with number: %d\n",*(int*)ret);
        free(ret);
    }

}

// BT5. Chia Sẻ Tài Nguyên Với Read-Write Lock
// Viết một chương trình mô phỏng tình huống có nhiều thread đọc và ghi dữ liệu:
// 1.	Tạo một biến data chứa một số nguyên.
// 2.	Tạo 5 threads đọc và 2 threads ghi:
// o	Các thread đọc sẽ chỉ đọc data mà không thay đổi.
// o	Các thread ghi sẽ tăng giá trị của data lên 1.
// 3.	Sử dụng read-write lock để đảm bảo nhiều threads có thể đọc cùng lúc, nhưng chỉ một thread được ghi tại bất kỳ thời điểm nào.
// 4.	In ra giá trị của data sau khi tất cả các threads hoàn thành.
// Gợi ý: Sử dụng pthread_rwlock_rdlock và pthread_rwlock_wrlock cho đọc-ghi đồng bộ.


// https://www.ibm.com/docs/en/aix/7.2?topic=programming-using-readwrite-locks
// In these cases, you can allow threads to read concurrently while holding the lock 
// and allow only one thread to hold the lock when data is modified. 
// A multiple-reader single-writer lock (or read/write lock) does this. A read/write lock is acquired either 
// for reading or writing, and then is released. 
// The thread that acquires the read-write lock must be the one that releases it.