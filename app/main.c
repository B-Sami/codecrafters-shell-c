#include <stdio.h>
#include <string.h>
#include <stdlib.h>



char* get_first_word(char* input) {
  char* firstWord = (char*)malloc(100);
  int i = 0;
  while (input[i] != ' ' && input[i] != '\0') {
    firstWord[i] = input[i];
    i++;
  }
  firstWord[i] = '\0';
  return firstWord;
}

int is_command_allowed(const char* command, const char *const list_commands_allow[], int numCommands) {
    for (int i = 0; i < numCommands; i++) {
        if (!strcmp(command, list_commands_allow[i])) {
            return 1;
        }
    }
    return 0;
}

void run_command(char* command) {
  if (!strcmp(command, "exit 0"))
    exit(0);
  else if (!strcmp(get_first_word(command), "echo")){
    printf("%s\n", command + 5);
  }
}



int main() {
  const char *const list_commands_allow[] = {
      "echo",
      "exit"
  };

  int numCommands = sizeof(list_commands_allow) / sizeof(list_commands_allow[0]);


  while(1){
    printf("$ ");
    fflush(stdout);
    char input[100];
    fgets(input, 100, stdin);
    input[strlen(input) - 1] = '\0';

    char* firstWord = get_first_word(input);
    int const is_command_allow = is_command_allowed(firstWord, list_commands_allow, numCommands);
    if (is_command_allow) {
      run_command(input);
    } else {
      printf("%s: command not found\n", input);
    }
  }
  return 0;
}
