#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>

void getPID(pid_t id)
{
    printf("PID: %d\n", id);
}
int a =3;
void main() {
    pid_t id = fork();
    pid_t thisID = getpid();

    if(id == 0) {
        printf("child proces has its own child's ");
        getPID(thisID);
    } else if (id > 0) {
        printf("parent proces has child ");
        getPID(id);
    }

} 

// Khởi tạo và Theo dõi Process
// 1.	Mô tả: Viết một chương trình C tạo một process con bằng cách sử dụng fork(). Sau khi tạo process, tiến trình con sẽ in ra PID của nó và tiến trình cha in ra PID của tiến trình con.
