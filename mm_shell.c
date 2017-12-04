#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

// parses the line, a string, using given delimeter, also a string
char **parse_string(char *line, char *delimeter){
  char **args = (char**)calloc(10, sizeof(char *));
  int i = 0;
  while(line) args[i++]= strsep(&line, delimeter);
  
  return args;
}

// in case someone wants to kill our shell with signal of number signo
static void sighandler(int signo){
  if (signo == SIGINT){//keyboard interrupt
    printf("\n[pid %d]program exited due to an interrupt signal\n", getpid());
    exit(0);
  }
}

// just prints the current directory like the bash does
void print_prompt(){
  char cwd[1024];
  getcwd(cwd, sizeof(cwd)) ? fprintf(stdout, "m&m shell:%s$ ", cwd) : perror("getcwd() error");
}

// cleanses user input (takes out the new line because fgets reads that in)
char *get_input(){
  //taking user input
  char *input = (char *)calloc(1024, 1);//when in doubt, calloc is always the answer
  fgets(input, 100, stdin); 
  input[strlen(input)-1]=0;//taking out the new line by replacing it with null

  return input;
}

// handles redirections with > or <
void redirect(char *file, int *backup, int old_fd){
  *backup = dup(old_fd);
  int fd = open(file, O_RDWR | O_CREAT, 0644);
  dup2(fd, old_fd);
  close(fd);
}

// handles redirections with >> or <<
void redirectAppend(char *file, int *backup, int old_fd){
  *backup = dup(old_fd);
  int fd = open(file, O_RDWR | O_CREAT | O_APPEND, 0644);
  dup2(fd, old_fd);
  close(fd);
}

// runs command cmd2 using outputs of command cmd1
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

// run a single command
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
    char** arguments = (char**)calloc(10, sizeof(char *));//yes
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
      if(execvp(arguments[0], arguments)<0){
        printf("your command was invalid\n");
      }
    }
  }
  if(backup) dup2(backup, old_fd);
}

// run multiple commands. 
// Semicolons have to be immediately next to commands -- no ls ; pwd
// only ls;pwd
void run_commands(){
  char **commands = parse_string(get_input(), ";");

  int i = 0;
  for( ; commands[i] && i <= 10; i++){
    run_command(commands[i]);
  }
}

// executes our bash
int main(){
  while(1){
    signal(SIGINT, sighandler);//whenever the SIGINT is sent, RUN this function
    
    print_prompt();
    run_commands();
  }

  return 0;
}