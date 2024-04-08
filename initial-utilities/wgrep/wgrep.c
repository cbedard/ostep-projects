#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    if (argc < 2) {
        printf("%s\n", "wgrep: searchterm [file ...]");
        return 1;
    }
    
    fp = fopen(argv[2], "r");
    if (fp == NULL) {
        printf("%s\n", "wgrep: cannot open file");
        return 1;
    }

    while ((read = getline(&line, &len, fp)) != -1) {
        if (strstr(line, argv[1]) != NULL) {
            printf("%s", line);
        }
    }

    fclose(fp);
    if (line)
        free(line);
    return 0;
}