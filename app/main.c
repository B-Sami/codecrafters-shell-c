#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
  while(1){
    printf("$ ");
    fflush(stdout);

    char input[100];
    fgets(input, 100, stdin);
    input[strlen(input) - 1] = '\0';
    if (!strcmp(input, "exit 0"))
      exit(0);
    printf("%s: command not found\n", input);
  }
  return 0;
}
