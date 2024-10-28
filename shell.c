#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int is_executable(const char *path) {return access(path, X_OK) == 0;}
char *find_in_path(const char *command) {
  char *path_env = getenv("PATH");
  if(path_env == NULL){
    return NULL;
  }
  char *path_copy = strdup(path_env);
  char *dir = strtok(path_copy, ":");
  static char full_path[1024];

  while (dir != NULL) {
    snprintf(full_path, sizeof(full_path), "%s/%s",dir,command);
    if(is_executable(full_path)){
      free(path_copy);
      return full_path;

    }
    dir = strtok(NULL, ":");
  
  }
  free(path_copy);
  return NULL;
}





int main(){
  char input[100];
  while(1){

    printf("project_shell$ ");
    fflush(stdout);

    if (fgets(input,sizeof(input), stdin)== NULL){
      break;
    }

    input[strcspn(input,"\n")] = 0;

    if (strlen(input) == 0){
      continue;
    }

    if(strcmp(input, "exit 0") == 0){
      return 0;
    }
    
    if(strncmp(input,"echo ", 5) == 0){
      printf("%s\n", input + 5);
      continue;
    }
    if(strcmp(input,"pwd") == 0){
      char cwd[1024];
      getcwd(cwd, sizeof(cwd));
      printf("%s\n", cwd);
  
      continue;
    }

   if(strncmp(input, "type ", 5) == 0) {
      char *command = input + 5;
      if (strcmp(command,"echo") == 0 || strcmp(command, "exit") == 0 || strcmp(command, "type") == 0) {
        printf("%s is a shell builtin\n", command);
      }else {
        char *path = find_in_path(command);
        if(path){
          printf("%s is %s \n", command, path);
        }else {
          printf("%s: not found \n", command);
        }
      }
    continue;
  }
    printf("%s: command not found\n", input);
  }
 return 0;
}
