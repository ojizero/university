#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<unistd.h>


/*
Conclusions ::
Once message queue is closed -> noone can send or recieve by the instance of closing it.
*/


struct data
{
    long type;
    char* dt;//[20];
    int pid;
} s, r;

void main()
{
    key_t key = ftok("tmp", 11);

    int id = msgget(key, IPC_CREAT | 666);
    // FIXME issue with permissions of the process itself -> couldn't creatre queue
    // FIXED by executing as root

    if (id != -1)
    {
        int pid = fork();
        if(pid == 0) // child
        {
            s.type = 1;
            s.dt = "Child";
            s.pid = getpid();

            printf("child -> %s\n", s.dt);
            int sc = msgsnd(id, &s, sizeof(struct data)-sizeof(long), 0);
            printf("child sending %d\n", sc);
			// msgctl(id, IPC_RMID, NULL);
            sleep(2);

            int rc = msgrcv(id, &r, sizeof(s)-sizeof(long), s.pid, 0);
            printf("child recieving %d\n", rc);

            printf("child -> %s\n", r.dt);
        }else if (pid == -1)
        {
            printf("Error\n"); // error
        }
        else
        {
            sleep(2);
            int v = msgrcv(id, &r, sizeof(r)-sizeof(long), 1, 0);
            printf("parent recieved %d\n", v);
            printf("parent -> %s\n", r.dt);

            int type = r.pid;
            s.type = type;
            s.dt = "Jajaja";

            // sleep(2);
            // parent couldn't send or recieve after child closed the queue
            v = msgsnd(id, &s, sizeof(s)-sizeof(long), 0);
            // msgctl(id, IPC_RMID, NULL); // child did not recieve after this line
            printf("parent send %d\n", v);

        }
    }else
    {
        printf("Failed to create queue!!\n");
    }
}
