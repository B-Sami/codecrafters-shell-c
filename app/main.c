#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define SIZE_COMMAND 100

const char *const list_commands_allow[] = {
  "echo",
  "exit",
  "type"
};

int num_commands = sizeof(list_commands_allow) / sizeof(list_commands_allow[0]);

int is_executable(const char *path) { return access(path, X_OK) == 0; }


char* get_word_at_index(char* input, int index) {
    char* word = (char*)malloc(SIZE_COMMAND);
    if (word == NULL) {
        return NULL;
    }
    
    int i = 0, count = 0, wordPos = 0;
    int inWord = 0;
    
    while (input[i] != '\0') {
        if (input[i] != ' ') {
            if (count == index) {
                word[wordPos++] = input[i];
                inWord = 1;
            }
        } else {
            if (inWord) {
                break;
            }
            count++;
        }
        i++;
    }

    word[wordPos] = '\0';
    if (wordPos == 0) {
        free(word);
        return NULL;
    }

    return word;
}

int is_command_allowed(const char* command) {
    for (int i = 0; i < num_commands; i++) {
        if (!strcmp(command, list_commands_allow[i])) {
            return 1;
        }
    }
    return 0;
}

char *find_in_path(const char *command) {
  char *path_env = getenv("PATH");
  if (path_env == NULL) {
    return NULL;
  }
  char *path_copy = strdup(path_env);
  char *dir = strtok(path_copy, ":");
  static char full_path[1024];
  while (dir != NULL) {
    snprintf(full_path, sizeof(full_path), "%s/%s", dir, command);
    if (is_executable(full_path)) {
      free(path_copy);
      return full_path;
    }
    dir = strtok(NULL, ":");
  }
  free(path_copy);
  return NULL;
}

void type(char* second_word) {
    if (is_command_allowed(second_word)) {
      printf("%s is a shell builtin\n", second_word);
    } else {
      char *path = find_in_path(second_word);
      if (path) {
        printf("%s is %s\n", second_word, path);
      } else {
        printf("%s: not found\n", second_word);
      }
    }
}

void run_external_command(char *input) {
    char *args[SIZE_COMMAND];
    int i = 0;
    char *token = strtok(input, " ");
    while (token != NULL && i < SIZE_COMMAND - 1) {
      args[i++] = token;
      token = strtok(NULL, " ");
    }
    args[i] = NULL;

    char *path = find_in_path(args[0]);
    if (path) {
      pid_t pid = fork();
      if (pid == 0) {
        execv(path, args);
        perror("execv");
        exit(EXIT_FAILURE);
      } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
      } else {
        perror("fork");
      }
    } else {
      printf("%s: command not found\n", args[0]);
    }
}



void run_command(char* command) {
  if (!strcmp(command, "exit 0"))
    exit(0);
  else if (!strcmp(get_word_at_index(command, 0), "echo")){
    printf("%s\n", command + 5);
  }
  else if (!strcmp(get_word_at_index(command, 0), "type")){
    type(get_word_at_index(command, 1));
  }
}

int main() {
  while(1){
    printf("$ ");
    fflush(stdout);
    char input[SIZE_COMMAND];
    fgets(input, SIZE_COMMAND, stdin);
    input[strlen(input) - 1] = '\0';

    char* first_word = get_word_at_index(input, 0);
    int const is_command_allow = is_command_allowed(first_word);
    if (is_command_allow) {
      run_command(input);
    } else {
      run_external_command(input);
    }
  }
  return 0;
}
