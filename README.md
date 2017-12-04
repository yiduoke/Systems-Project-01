m&m shell
by Md Abedin and Yiduo Ke

Features:
* reads user inputs, parses it into commands and arguments, then forks and executes them

Bugs:
* ctrl D doesn't exit 
* only works with single spaces 
* only handles one >, <, and |, with no space before or after them
* files with input commands must end with exit
	
Files & Function Headers:

mm_shell.c
	
Our first, Our last, Our everything.
	
	
	/*======== int count_tokens() ==========
	args:
	- char *line: a command-line command
	- char *delimeter: the delimeter by which to separate arguments

	returns:
	- number of tokens in the line

  	use:
	- counts how many tokens are in a line
	====================*/
	
	/*======== char ** parse_string() ==========
	args:
	- char *line: a command-line command
	- char *delimeter: the delimeter by which to separate arguments

	returns:
	- pointer to array of pointers to each parsed argument

  	use:
	- parses the line, a string, using given delimeter, also a string
	====================*/

	/*======== static void sighandler() ==========
	args:
	- int signo: number of the signal

	returns:
	- none

	use:
	- handles the case in which someone wants to kill our shell with an interrupt signal
	====================*/
	
	/*======== void print_prompt() ==========
	args:
	- none

	returns:
	- none

	use:
	- just prints the current directory's path like the bash does
	====================*/

	/*======== char *get_input() ==========
	args:
	- none

	returns:
	- pointer to string of cleaned input 

	use:
	cleanses user input (takes out the new line because fgets reads that in)
	====================*/

	/*======== void redirect() ==========
	args:
	- char *file: path for file to redirect into/from
	- int *backup: the delimeter by which to separate arguments
	- int old_fd: the delimeter by which to separate arguments

	returns:
	- none

	use:
	- handles redirections with > or <
	====================*/

	/*======== void pipe_commands() ==========
	args:
	- char *cmd1: command to take input from
	- char *cmd2: command to pipe input into

	returns:
	- none

	use:
	- runs command cmd2 using outputs of command cmd1
	====================*/

	/*======== void run_command() ==========
	args:
	- char *command: command to be run

	returns:
	- none

	use:
	- runs a single command
	====================*/

	/*======== void run_commands() ==========
	args:
	- none

	returns:
	- none

	use:
	- gets user inputs, cleans them into recognizable commands, and runs them
	- Semicolons have to be immediately next to commands -- no ls ; pwd
	- only ls;pwd for now
	====================*/

	/*======== int main() ==========
	args:
	- none

	returns:
	- the int 0

	use:
	- executes our shell
	====================*/
