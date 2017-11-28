#include <stdio.h>
#include <unistd.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

int main(){
   FILE* fp;
   fp = popen("ls -l","r");
//    char path[10];
//    while (fgets(path, 10, fp) != NULL){}
//    printf("path: %s\n", path);
}