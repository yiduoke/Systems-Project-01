#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

char ** parse_commands( char * line ){
    char ** output = (char**)calloc(40, sizeof("ugaediwbdhadkhwd"));
    int i=0;
    while (line){
        char * str = strsep(&line," ; ");
        output[i]= str;
        i++;
    }
    return output;
}

char ** parse_args( char * line ){
    char ** output = (char**)calloc(40, sizeof("ugaediwbdhadkhwd"));
    int i=0;
    while (line){
        char * str = strsep(&line," ");
        output[i]= str;
        i++;
    }
    return output;
}

int main(){
    char* commands = (char*)calloc(40,10);//when in doubt, calloc is always the answer
    printf("put yo shit here: ");
    fgets(commands, 30, stdin); 
    commands[strlen(commands)-1]=0;//taking out the new line by replacing it with null
    char** parsed = (char**)calloc(40, 10);//yes
    parsed = parse_commands(commands);
    int i=0;
    while (parsed[i]){
        printf("beginning|%s|end\n", parsed[i]);
        char** further = (char**)calloc(40, 10);//yes
        further = parse_args(parsed[i]);
        int j=0;
        while (further[j]){
            printf("further %d: %s\n", j, further[j]);
            j++;
        }
        i++;
    }

    printf("------");

}