#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    int pid1 = 0, pid2 = 0;
    pid1 = fork();
    if(pid1 < 0) {return 1;}
    
    pid2 = fork();
    if(pid2 < 0) {return 1;}

    // The parent and the first child both have children, clean them up.
    if(pid2 > 0) {
        while( wait(NULL) > 0 ) {}
    }

    fprintf(stdout, "hello world from PID %d!\n", getpid());
    return 0;
}