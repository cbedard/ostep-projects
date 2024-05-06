
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *getLine() {
    char *line = NULL;
    size_t len = 0;
    ssize_t lineSize = 0;
    
    lineSize = getline(&line, &len, stdin);
    
    return line;
    //free(line);
}

int main(int argc, char *argv[]) {
    char *line;
    char **args;
    int status = 1;

    do {
        printf("wish> ");
        line = getLine();
        //args = lsh_split_line(line);
        //status = lsh_execute(args);

        printf("You entered: \"%s\"", line); 
        free(line);
        //free(args);
    } while (status);

   exit(0);
}