#include <pthread.h>
#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h>

// definition constant
#define D_LIMIT_NUM (99)
#define D_EVEN_MODE (0)
#define D_ODD_MODE (1)

// macro function
#define M_GET_RANDOM ( (rand() % (D_LIMIT_NUM+1)) + 1)

// global static variable

static int container[D_LIMIT_NUM+1];

void init_container(){
    int oddCnt=0;
    int evenCnt=0;
    for(int i=0;i<=D_LIMIT_NUM;++i)
    {
        container[i] = M_GET_RANDOM;
        printf("No.%d ", i);
        if( container[i]%2 != 0 )
        {
            printf("odd+%d ",++oddCnt);
        } else {
            printf("even+%d ",++evenCnt);
        }
        printf(": %d; ",container[i]);
        printf("\n");
    }
    
}

void* count(void* argv){
    int* sum = (int*)malloc(sizeof(int));
    int* mode = (int*) argv;
    *sum = 0;
    for(int i=0; i<= D_LIMIT_NUM;++i)
    {
        if(container[i]%2 == *mode){
            *sum += container[i]; 
        }
    }
    switch (*mode)
    {
    case D_ODD_MODE:
        printf("sum odd ");
        break;
    case D_EVEN_MODE:
        printf("sum even ");
        break;   
    default:
        break;
    }
    printf("%d\n", *sum);
    pthread_exit(sum);
}

// main function
void main(){
    pthread_t tID1;
    pthread_t tID2;
    void* retCntOdd = NULL;
    void* retCntEven = NULL;
    int arg1 = D_ODD_MODE;
    int arg2 = D_EVEN_MODE;

    init_container();

    pthread_create(&tID1, NULL, count, &arg1);
    pthread_create(&tID2, NULL, count, &arg2);



    pthread_join(tID1, &retCntOdd);
    pthread_join(tID2, &retCntEven);

    
    printf("sum even + odd = %d\n", *(int*)retCntOdd + *(int*)retCntEven);

    free(retCntOdd);
    free(retCntEven);
}

// BT4. Đếm Số Chẵn và Số Lẻ với Threads
// Viết một chương trình để thực hiện các bước sau:
// 1.	Tạo một mảng ngẫu nhiên chứa 100 số nguyên từ 1 đến 100.
// 2.	Tạo hai threads: một thread đếm số chẵn và một thread đếm số lẻ trong mảng.
// 3.	Khi hai threads hoàn tất việc đếm, in ra tổng số số chẵn và tổng số số lẻ.
// 4.	Sử dụng pthread_join để đảm bảo rằng chương trình chỉ kết thúc khi cả hai threads hoàn thành nhiệm vụ.
// Gợi ý: Mỗi thread sẽ xử lý một phần công việc khác nhau, vì vậy không cần mutex trong bài này.
