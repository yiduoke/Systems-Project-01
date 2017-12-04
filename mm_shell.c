#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

/*
args:
- char *line: a command-line command
- char * delimeter: the delimeter by which to separate arguments

returns:
- number of tokens in the line

use:
- counts how many tokens are in a line
*/

int count_tokens(char *line, char *delimeter){
  int i;
  char *tmp = line;

  for(i = 1; tmp = strchr(++tmp, delimeter[0]); i++){}
  
  return i;
}

/*
args:
- char *line: a command-line command
- char *delimeter: the delimeter by which to separate arguments

returns:
- pointer to array of pointers to each parsed argument

use:
- parses the line, a string, using given delimeter, also a string
*/
char **parse_string(char *line, char *delimeter){
  char **args = (char**)calloc(count_tokens(line, delimeter)+1, sizeof(char *));
  int i = 0;
  while(line) args[i++]= strsep(&line, delimeter);
  
  return args;
}

/*
args:
- int signo: number of the signal

returns:
- none

use:
- handles the case in which someone wants to kill our shell with an interrupt signal
*/
static void sighandler(int signo){
  if (signo == SIGINT){//keyboard interrupt
    printf("\n[pid %d]program exited due to an interrupt signal\n", getpid());
    exit(0);
  }
}


/*
args:
- none

returns:
- none

use:
- just prints the current directory's path like the bash does
*/
void print_prompt(){
  char cwd[1024];
  getcwd(cwd, sizeof(cwd)) ? fprintf(stdout, "m&m shell:%s$ ", cwd) : perror("getcwd() error");
}

/*
args:
- none

returns:
- pointer to string of cleaned input 

use:
- cleanses user input (takes out the new line because fgets reads that in)
*/
char *get_input(){
  //taking user input
  char *input = (char *)calloc(1, 1024);//when in doubt, calloc is always the answer
  fgets(input, 1000, stdin); 
  input[strlen(input)-1]=0;//taking out the new line by replacing it with null

  return input;
}

/*
args:
- char *file: path for file to redirect into/from
- int *backup: the delimeter by which to separate arguments
- int old_fd: the delimeter by which to separate arguments

returns:
- none

use:
- handles redirections with > or <
*/
void redirect(char *file, int *backup, int old_fd){
  *backup = dup(old_fd);
  int fd = open(file, O_RDWR | O_CREAT, 0644);
  dup2(fd, old_fd);
  close(fd);
}


/*
args:
- char *cmd1: command to take input from
- char *cmd2: command to pipe input into

returns:
- none

use:
- runs command cmd2 using outputs of command cmd1
*/
void pipe_commands(char* cmd1, char* cmd2){
  FILE* fp;
  fp = popen(cmd1,"r");
  FILE* new_fp = popen(cmd2, "w");
  char path[1024];
  while (fgets(path, 1024, fp)){
    fprintf(new_fp, "%s", path);
  }
  pclose(fp);
  pclose(new_fp);
}

/*
args:
- char *command: command to be run

returns:
- none

use:
- runs a single command
*/
void run_command(char *command){
  int backup, old_fd;
  
  if(strchr(command, '>')){
    char **args = parse_string(command, ">");
    old_fd = STDOUT_FILENO;
    redirect(args[1], &backup, old_fd);
  }
  if(strchr(command, '<')){
    char **args = parse_string(command, "<");
    old_fd = STDIN_FILENO;
    redirect(args[1], &backup, old_fd);
  }
  if(strchr(command, '|')){
    char **args = parse_string(command, "|");
    pipe_commands(args[0], args[1]);
    // exit(0);
  }
  else{
    //printf("tokens: %d\n", count_tokens(command, ' '));
    char** arguments = (char**)calloc(count_tokens(command, " "), sizeof(char *));//yes
    arguments = parse_string(command, " ");//parsing user input into commands and flags
    
    if (!strncmp("exit", arguments[0], 4)) exit(1); //exit doesn't work in child processes so I gotta do it in parent process
    //courtesy of https://stackoverflow.com/questions/298510/how-to-get-the-current-directory-in-a-c-program
    if (!strncmp("cd", arguments[0], 4)) chdir(arguments[1]); //cd doesn't work in child processes so I gotta do it in parent process
    
    //making the kid do the work
    int fork_pid = fork();
    //fork error detection
    if(fork_pid == -1){
      printf("fork failed: %s\n", strerror(errno));
      exit(-1);
    }
    
    if (fork_pid){//if it's the parent
      int status;
      int child_pid = wait(&status);
    }
    else{
      execvp(arguments[0], arguments);
    }
  }
  if(backup) dup2(backup, old_fd);
}

/*
args:
- none

returns:
- none

use:
- gets user inputs, cleans them into recognizable commands, and runs them
- Semicolons have to be immediately next to commands -- no ls ; pwd
- only ls;pwd for now
- handles up to 10 commands on one line for now
*/
void run_commands(){
  char *input = get_input();
  int num_tokens = count_tokens(input, ";");
  char **commands = parse_string(input, ";");

  int i;
  for(i = 0; commands[i] && i < num_tokens; i++){
    run_command(commands[i]);
  }
}

/*
args:
- none

returns:
- the int 0

use:
- executes our shell
*/
int main(){
  while(1){
    signal(SIGINT, sighandler);//whenever the SIGINT is sent, RUN this function
    
    print_prompt();
    run_commands();
  }

  return 0;
}
