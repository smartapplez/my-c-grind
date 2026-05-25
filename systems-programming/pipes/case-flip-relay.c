#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  // pipefd[1] - Write
  // pipefs[0] - Read
  int pipefd[2];
  const int BUF_SIZE = 25;
  char buf[BUF_SIZE];
  ssize_t nbytes;
  int status = pipe(pipefd);
  if (status == -1) {
    perror("ERROR: pipe() failed");
    exit(-1);
  }

  pid_t pid = fork();
  switch (pid) {
  case -1:
    perror("ERROR: fork() failed");
    exit(-1);
    break;
  case 0:             // Child process - writes to pipe
    close(pipefd[1]); // Close the write fildes
    nbytes = read(pipefd[0], buf, BUF_SIZE);
    printf("[CHILD] Received: %s\n", buf);
    // There are no further read, EOF
    close(pipefd[0]);
    exit(EXIT_SUCCESS);
    break;
  default:            // Parent process - writes to pipe
    close(pipefd[0]); // Close the read filedes

    printf("Enter an input: ");
    fgets(buf, BUF_SIZE, stdin);
    printf("[PARENT] Sending: %s\n", buf);
    write(pipefd[1], buf, BUF_SIZE);
    close(pipefd[1]);
    while (1) {
      pid_t pid = waitpid(-1, NULL, 0);
      // When there are no more child process to wait for
      if (pid == -1)
        break;
    }
    exit(EXIT_SUCCESS);
    break;
  }

  return 0;
}
