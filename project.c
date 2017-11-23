#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

// (done)Read a line at a time, parse the line to separate the command from its arguments. 
// (done)It should then fork and exec the command. 
// (done)The parent process should wait until the exec'd program exits and 
// then it should read the next command.
// Note: exit and cd cannot be run through a forked child process, 
// you will have to implement these commands on your own.
// check out the chdir() function

char ** parse_args( char * line ){
    char ** output = (char**)calloc(15, sizeof("ugaediwbdhadkhwd"));
    int i=0;
    while (line){
        char * str = strsep(&line," ");
        output[i]= str;
        i++;
    }
    return output;
}

int main(){
    char* commands = (char*)calloc(30,10);//when in doubt, calloc is always the answer
    //taking user input
    printf("should be the current directory lol: ");
    fgets(commands, 30, stdin); 
    commands[strlen(commands)-1]=0;//taking out the new line by replacing it with null

    char** parsed = (char**)calloc(3, 10);//yes
    parsed = parse_args(commands);//parsing user input into commands and flags

    //making the kid do the work
    int mom = getpid();
    int child = fork();
    if (getpid() == mom){//if it's the parent
        int status;
        int childPID = wait(&status);
        if (status){
          printf("my son %d is done\n", WEXITSTATUS(status));// doesn't work except for
          // the things that don't for this like exit and cd (works when it fails lol)
          // is this supposed to happen
        }
    }
    else{//don't let the ! mislead you; this means it's a child
        execvp(parsed[0], parsed);
        return getpid();
    }
    return 0;
}