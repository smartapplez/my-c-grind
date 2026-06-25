#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
  pid_t pid;

  pid = fork();
  if (pid == 0) {
    char *HOME = getenv("HOME");
    char path[4096];
    snprintf(path, sizeof(path), "%s/%s", HOME, "Documents");
    char *args[5] = {"ls", "-l", "-a", path, NULL};
    execvp(args[0], args);
    perror("execvp error:");
    exit(EXIT_FAILURE);
  } else {
    int status, child_return_val;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status) && (child_return_val = WEXITSTATUS(status)) != 0) {
      fprintf(stderr, "Child process returned an error code\n");
      return child_return_val;
    } else if (child_return_val == 0) {
      puts("Child Process returned normally with status 0");
    } else {
      puts("Child processes returned to parent Abnormally");
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}
