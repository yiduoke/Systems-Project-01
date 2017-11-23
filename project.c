#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

// (done)Read a line at a time, parse the line to separate the command from its arguments. 
// It should then fork and exec the command. 
// The parent process should wait until the exec'd program exits and 
// then it should read the next command.
// Note: exit and cd cannot be run through a forked child process, 
// you will have to implement these commands on your own.
// check out the chdir() function

int main(int argc, char **argv){
    int ancestor = getpid();
    int child = fork();
    if (getpid() == ancestor){//ancestor
        int status;
        int childPID = wait(&status);
        if (status){
          printf("child is done");
        }
    }
    if (!child){//don't let this fool you! this means it IS a child
        execvp(argv[1], argv+1);
    }
}