#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// main returns an integer
int main(int argc, char *argv[]) {
    FILE* ptr;
 
    for(int i = 1; i < argc; i++) {
        // Opening file in reading mode
        ptr = fopen(argv[i], "r");
    
        if (NULL == ptr) {
            printf("wcat: cannot open file");
            return 0;
        }

        char ch = fgetc(ptr);
        while (ch != EOF) {
            printf("%c", ch);
            ch = fgetc(ptr);
        }
    
        // Closing the file
        fclose(ptr);
    }
    
    return 0;
}