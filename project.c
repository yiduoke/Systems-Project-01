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
// (done)Note: exit and cd cannot be run through a forked child process, 
// (done)you will have to implement these commands on your own.
// (done)check out the chdir() function
// done with 1st feature!!!

// Read and separate multiple commands on one line with ; 

char ** parse_args(char *line, char* delimeter ){
  char ** output = (char**)calloc(15, sizeof(char *));
  int i=0;
  while (line){
    char * str = strsep(&line, delimeter);
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

void run_command(char * command){
  char** arguments = (char**)calloc(10, sizeof(char *));//yes
  arguments = parse_args(command, " ");//parsing user input into commands and flags
  //courtesy of https://stackoverflow.com/questions/298510/how-to-get-the-current-directory-in-a-c-program
  chdir(arguments[1]);//cd doesn't work in child processes
  //so I gotta do it in parent process

  if (!strncmp("exit",arguments[0],4)){//exit doesn't work in child processes
    exit(1);//so I gotta do it in parent process
  }

  execvp(arguments[0], arguments);
}

int main(){
  signal(SIGINT, sighandler);//whenever the SIGNIT gets sent, RUN this function
  signal(SIGUSR1, sighandler);
  
  while(1){
    char* input = (char*)calloc(30,10);//when in doubt, calloc is always the answer
    //taking user input
    // printf("should be the current directory lol: ");
    //prints current working directory
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL){
      fprintf(stdout, "m&m shell: %s", cwd);
      printf("$ ");
    }
    else{
      perror("getcwd() error");
    }
    
    fgets(input, 100, stdin); 
    input[strlen(input)-1]=0;//taking out the new line by replacing it with null
    
    char** commands = (char**)calloc(10, sizeof(char *));//yes
    commands = parse_args(input, ";");//parsing user input into commands and flags

    int i = 0;
    while(commands[i] && i <= 10){
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
	run_command(commands[i]);
	return getpid();
      }
      i++;
    }
  }
  return 0;
}
