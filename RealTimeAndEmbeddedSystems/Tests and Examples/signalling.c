/*
This is an example of capturing a system signal,
counts number of times CTRL+C (SIGINT) is pressed,
terminates on new line
*/

#include<stdio.h>
#include<signal.h>

int count = 0;

void count_ctrlc(int signum)
// parameter given by OS, is signal number, can be used when one handler is responsible for many signals
{
    count++;
}

void main()
{
    /*
    Note: changes to SIGKILL and SIGSTOP are ignored by OS
    */

    int old_handler = signal(SIGINT, count_ctrlc);
    int c;

    while ((c = getchar()) != '\n') {
        ;
    }

    printf("You pressed CTRL+C : %d times\n", count);
}
