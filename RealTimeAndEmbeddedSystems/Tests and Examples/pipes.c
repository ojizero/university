#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include <unistd.h>

/*
~ One sender many recievers ->
~ One reciever many senders ->
                            -> Data is recieved at random order by the recievers
*/


void main(int arg_c, int arg_v)
{
    int f_desc[2];
    int count = 0;
    char x[50];

    int s = pipe(f_desc);

    if(s != -1)
    {
        int pid;

        if( (pid = fork()) == -1 )
        {
            exit(pid); // exit on error
        }

        if(pid!=0) // in parent create another child
        {
            count++;
            if( (pid = fork()) == -1 )
            {
                exit(pid); // exit on error
            }
        }

        if (pid!=0) // in parent send/recieve
        {

            // file descriptor is opened once creater // open(f_desc[1]);
            while (1)
            {
                int bytes_read = read(f_desc[0], x, 50);
                printf("%s\n", x);
                // write(f_desc[1], "Parent message ", 50);
            }
        }
        else if (pid==0) // in children recieve/send
        {
            // printf("%d\n", count); // print child number
            char s[50];
            sprintf(s, "Messsage from child : %d", count);

            while(1)
            {
                write(f_desc[1], s, 50);
                // int bytes_read = read(f_desc[0], x, 50);
                // printf("%d -> %s\n", count, x);
            }
        }
    }

}
