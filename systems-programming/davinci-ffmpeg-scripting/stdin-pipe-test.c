#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int pipe_fd[2];
  char buffer[4096];
  pid_t pid;
  size_t bytes_read;

  // 1. Create the pipe
  if (pipe(pipe_fd) == -1) {
    perror("pipe failed:");
    return EXIT_FAILURE;
  }

  // 2. Fork the process
  pid = fork();
  if (pid == 0) {
    // Child Process
    // Closing read pipe_fd
    close(pipe_fd[0]);

    // Redirect stdout to pipe_fd[1]
    // pipe_fd[1] (write) <-- stdout
    if (dup2(pipe_fd[1], STDOUT_FILENO) == -1) {
      perror("Child Process: dup2 error");
      exit(EXIT_FAILURE);
    }

    // Close duplicate write descriptor
    close(pipe_fd[1]);

    char *HOME = getenv("HOME");
    char path[4096];
    // In bash, this is the equivalent of ls "${HOME}/Documents"
    snprintf(path, sizeof(path), "%s/%s", HOME, "Documents");
    char *args[5] = {"ls", "-l", "-a", path, NULL};

    // If execvp returns, then there has been an error
    if (execvp(args[0], args) == -1) {
      perror("execvp error:");
      exit(EXIT_FAILURE);
    }
  } else {
    // Parent Process
    // close the write pipe_fd
    // According to Google: If left open, read() will hang because the pipe
    // never hits EOF.
    close(pipe_fd[1]);

    puts("Reading from child output -------------------------");
    // Read data from the pipe until EOF (when child terminates and closes its
    // stdout)
    while ((bytes_read = read(pipe_fd[0], buffer, sizeof(buffer) - 1)) > 0) {
      buffer[bytes_read] = '\0';
    }
    printf("%s", buffer);

    close(pipe_fd[0]);

    int status;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status)) {
      int child_return_val = WEXITSTATUS(status);
      printf("Child Exited Normally with return code %d\n", child_return_val);
      if (child_return_val != 0) {
        puts("Child Exited with a return code that is not zero");
        return child_return_val;
      }
    } else {
      puts("Child Returned Abnormally...");
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}
