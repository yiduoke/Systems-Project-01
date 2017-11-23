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
    char ** output = (char**)calloc(10, sizeof("ugaediwbdhadkhwd"));
    int i=0;
    while (line){
        char * str = strsep(&line," ");
        output[i]= str;
        i++;
    }
    return output;
}

// int main(){
    // char line[30] = "ls -a -l";
    // char ** commands = parse_args(line);
    // execvp(commands[0], commands);
// }

// int main(int argc, char **argv){
//     int ancestor = getpid();
//     int child = fork();
//     if (getpid() == ancestor){//ancestor
//         int status;
//         int childPID = wait(&status);
//         if (status){
//           printf("child is done");
//         }
//     }
//     if (!child){//don't let this fool you! this means it IS a child
//         execvp(argv[1], argv+1);
//     }
// }

int main(){
    char* commands;
    printf("Should be the current directory lol: ");
    fgets(commands, 30, stdin); 
    commands[strlen(commands)-1]=0;

    char ** parsed = parse_args(commands);
    execvp(parsed[0], parsed);
    return 0;
}