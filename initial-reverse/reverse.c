#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//argc == 0, r+w stdin/out
//argc == 1, read from input, write to stdout
//argc == 2, r+w to provided filenames

//checklist
//if argc == 2, input file path != output filepath
//no limits to file length, line length
//if input || output not valid files paths, print 'error: cannot open file 'input.txt'', exit 1
//Malloc fails: If you call malloc() to allocate some memory, and malloc fails, you should print 'malloc failed' and exit 1
//argc > 2,  print 'usage: reverse <input> <output>' and exit 1

/*  How to print error messages: On any error, you should print the error to the screen using fprintf(), and send the error 
    message to stderr (standard error) and not stdout (standard output). This is accomplished in your C code as follows: 
    fprintf(stderr, "whatever the error message is\n"); */

int main(int argc, char *argv[]) {
    FILE *fileIn;
    FILE *fileOut;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    //usage corectness check
    if (argc > 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        return 1;
    }

    if (argc == 3 && strcmp(argv[2], argv[1]) == 0) {
        fprintf(stderr, "reverse: input and output file must differ\n");
        return 1;
    }
    
    //init file streams
    if (argc > 1) {
        fileIn = fopen(argv[1], "r");
        if (fileIn == NULL) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
            return 1;
        }
        if (argc == 1) fileOut = stdout;
    }
    if (argc == 3) {
        fileOut = fopen(argv[2], "w");
        if (fileOut == NULL) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[2]);
            return 1;
        }
    }
    if (argc == 1) {
        fileIn = stdin;
        fileOut = stdout;
    }

    char *arr[100];
    int size = 0;
    //reverse time
    while ((read = getline(&line, &len, fileIn)) != -1) {
        arr[size] = malloc(len + 1);
        strcpy(arr[size], line);
        size = size + 1;
    }

    //print to output file
    for(int i = size-1; i >= 0; i = i-1) {
        fprintf(fileOut, "%s", arr[i]);
    }

    //close and free
    if (fileIn != stdin) fclose(fileIn);
    if (fileOut != stdout) fclose(fileOut);
    if (line) free(line);
    return 0;
}

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}