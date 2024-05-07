
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_line() {
    char *line = NULL;
    size_t len = 0;
    ssize_t lineSize = 0;
    
    lineSize = getline(&line, &len, stdin);
    
    return line;
}

char** str_split(char* a_str, const char a_delim) {
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];  
    delim[0] = a_delim;
    delim[1] = 0;

    // Count how many elements will be extracted.
    while (*tmp) {
        if (a_delim == *tmp) {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    // Add space for trailing token.
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result) {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token) {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

int main(int argc, char *argv[]) {
    char *line;
    char **args;
    int status = 1;

    do {
        printf("wish> ");
        line = get_line();
        args = str_split(line, " ");
        
        //status = execute(args);

        printf("You entered: \"%s\"", line); 
        free(line);
        //free(args);
    } while (status);

   exit(0);
}