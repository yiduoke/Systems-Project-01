#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

// (done)Read a line at a time, parse the line to separate the command from its arguments. 
// (done)It should then fork and exec the command. 
// (done)The parent process should wait until the exec'd program exits and 
// (done)then it should read the next command.
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

static void sighandler(int signo){
    if (signo == SIGINT){//keyboard interrupt
        exit(0);
    }
    else if (signo == SIGUSR1){//keyboard interrupt
        printf("hello, my mom is %d\n", getppid());
    }
}

int main(){
    signal(SIGINT, sighandler);//whenever the SIGNIT gets sent, RUN this function
    signal(SIGUSR1, sighandler);
    while(1){
        char* commands = (char*)calloc(30,10);//when in doubt, calloc is always the answer
        //taking user input
        // printf("should be the current directory lol: ");
        //prints current working directory
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL){
            fprintf(stdout, "%s", cwd);
            printf("$ ");
        }
        else{
            perror("getcwd() error");
        }

        fgets(commands, 30, stdin); 
        commands[strlen(commands)-1]=0;//taking out the new line by replacing it with null
    
        char** parsed = (char**)calloc(3, 10);//yes
        parsed = parse_args(commands);//parsing user input into commands and flags
        //cd doesn't work in child processes
        //courtesy of https://stackoverflow.com/questions/298510/how-to-get-the-current-directory-in-a-c-program
        chdir(parsed[1]);
    
        //making the kid do the work
        int mom = getpid();
        int child = fork();
        if (getpid() == mom){//if it's the parent
    
            int status;
            int childPID = wait(&status);
            // if (status){
            //   printf("prolly typed exit, which doesn't work yet lol");
            // }
        }
        else{
            execvp(parsed[0], parsed);
            return getpid();
        }
    }
    return 0;
}