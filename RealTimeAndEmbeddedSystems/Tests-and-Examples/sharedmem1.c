#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include<unistd.h>

/*
creates shared memory at address of an array
*/

/*
Conclusions system refused to map the memory to the address of the array
*/

void main() {
    int data[] = {1, 2, 3, 4, 5, 6};
    int id;
    void* base;
    if ((id = shmget(10, 24, 0666|IPC_CREAT)) == -1) {
        /* error */
        printf("Error get.\n\n");
    } else {
        if ((base = shmat(id, data /*base of array*/, 0))==-1) {
            /* error */
            printf("Error map.\n\n");
        } else {
            while(1);
        }
    }
}
