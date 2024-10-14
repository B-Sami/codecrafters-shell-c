#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char *const list_commands_allow[] = {
  "echo",
  "exit",
  "type"
};

int num_commands = sizeof(list_commands_allow) / sizeof(list_commands_allow[0]);

char* get_word_at_index(char* input, int index) {
    char* word = (char*)malloc(100);
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

void type(char* second_word) {
    if (is_command_allowed(second_word)) {
      printf("%s is a shell builtin\n", second_word);
    } else {
      printf("%s: not found\n", second_word);
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
    char input[100];
    fgets(input, 100, stdin);
    input[strlen(input) - 1] = '\0';

    char* first_word = get_word_at_index(input, 0);
    int const is_command_allow = is_command_allowed(first_word);
    if (is_command_allow) {
      run_command(input);
    } else {
      printf("%s: command not found\n", input);
    }
  }
  return 0;
}
