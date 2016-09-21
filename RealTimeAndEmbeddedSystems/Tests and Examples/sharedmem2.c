#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>

/*
uses shared memory from #1 and sees the data in it
*/

void main()
{
    int id;
    void* base;

    if ((id = shmget(10, 24, 0666|IPC_CREAT)) == -1) {
        /* error */
        printf("Error get.\n\n");
    } else {
        if ((base = shmat(id, NULL, 0))==-1) {
            /* error */
            printf("Error map.\n\n");
        } else {
            int* x = (int*)base;
            for (size_t i = 0; i < 6; i++) {
                /* code */
                printf("%d\n", x[i]);
            }

            shmdt(base);
        }
    }
}
