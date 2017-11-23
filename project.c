#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

// Read a line at a time, parse the line to separate the command from its arguments. 
// It should then fork and exec the command. 
// The parent process should wait until the exec'd program exits and 
// then it should read the next command.
// Note: exit and cd cannot be run through a forked child process, 
// you will have to implement these commands on your own.
// check out the chdir() function

int main(int argc, char **argv){
    execvp(argv[0], argv+1);
}