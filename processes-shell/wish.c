
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>

char* path = "/bin";

int cmd_cd(char** args) {
    if (args[1] == NULL) {
        fprintf(stderr, "lsh: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("lsh");
        }
    }
    return 1;
}

int cmd_path(char** args) {
    path = args[1];
}

int cmd_exit() {
    return 0;
}

char *builtin_str[] = {
  "cd",
  "path",
  "exit"
};

int (*builtin_cmd[]) (char **) = {
  &cmd_cd,
  &cmd_path,
  &cmd_exit
};

char* get_line() {
  int bufsize = 1024;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    // Read a character
    c = getchar();

    // If we hit EOF, replace it with a null character and return.
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize) {
      bufsize += 1024;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
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

int execute(char** args) {
    char exec_path[256];
    strcpy(exec_path, path);
    strcat(exec_path, "/");
    strcat(exec_path, args[0]);

    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Child process
        if (execv(exec_path, args) < 0) {
            perror("wish");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error forking
        perror("wish");
    } else {
        // Parent process
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int command(char ** args) {
    if (args[0] == NULL) {
        return 1;
    }
    
    for (int i = 0; i < 3; i++) {
        if (strcmp(builtin_str[i], args[0]) == 0) {
            return (*builtin_cmd[i])(args);
        }
    }
        
    return execute(args);
}

int main(int argc, char *argv[]) {
    char *line;
    char **args;
    int status = 1;

    do {
        printf("wish> ");
        line = get_line();
        args = str_split(line, ' ');
        
        status = command(args);

        free(line);
        free(args);
    } while (status);

   exit(0);
}