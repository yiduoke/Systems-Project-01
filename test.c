#include <stdio.h>
#include <unistd.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

void duplicate(char file[60]){
    int stdout_bk = dup(fileno(stdout));//is fd for stdout backup
    int fd = open(file, O_CREAT | O_RDWR | O_TRUNC, 0644); 
    dup2(fd, fileno(stdout));
}

int main(){
   duplicate("newfile");

   printf("LAST THING IN STDOUT\n");
   return 0;
}