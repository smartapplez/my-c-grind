#include "davinci_video_convert.h"

void *thread_check_bin_dependency(void *arg) {
  enum dependencies *dep_flag = (enum dependencies *)arg;
  int *which_status_val;
  which_status_val = check_bin_dependency(dep_flag);
  return (void *)which_status_val;
}

static inline int *check_bin_dependency(enum dependencies *dep_flag) {
  int *which_status_val = malloc(sizeof(int));
  *which_status_val = -1; // default for which_status_val in case child process
                          // returns Abnormally
  int status;
  pid_t pid;
  int null_dev_fd = 0;
  char *const args[2] = {"which", dep_str[*dep_flag]};

  if (verbosity)
    printf("Starting depedency check on %s\n", args[1]);

  pid = fork();
  if (pid == 0) {
    // child process
    // null_dev_fd file remains open across an execv
    if ((null_dev_fd = open("/dev/null", O_WRONLY)) < 0) {
      perror("open: failed to open /dev/null");
      exit(ERR_OPEN);
    }

    // stdout > /dev/null
    if (dup2(null_dev_fd, STDOUT_FILENO) < 0) {
      perror("dup2: Failed to dupe null_dev_fd into STDOUT");
      exit(ERR_DUP2);
    }
    // stderr > /dev/null
    if (dup2(null_dev_fd, STDERR_FILENO) < 0) {
      perror("dup2: Failed to dupe null_dev_fd into STDERR");
      exit(ERR_DUP2);
    }

    // child: close the duplicate null_dev_fd since we now have STDOUT
    // pointing to /dev/null fd
    close(null_dev_fd);

    if (execvp(args[0], args) < 0)
      exit(ERR_EXECVP);
  } else {
    waitpid(pid, &status, 0);
    if (WIFEXITED(status)) {
      *which_status_val = WEXITSTATUS(status);
    }
  }
  return (int *)which_status_val;
}

static inline void depedency_check() {
  pthread_t threads[2];
  void *which_status_vals[2] = {NULL, NULL};
  int dep_fail_check = 0;

  puts("Checking for dependencies...");

  for (int i = 0; i < 2; i++) {
    if (pthread_create(&threads[i], NULL, thread_check_bin_dependency,
                       &deps_array[i]) == -1) {
      perror("Failed to create thead");
      exit(EXIT_FAILURE);
    }
  }

  for (int i = 0; i < 2; i++) {
    if (pthread_join(threads[i], &which_status_vals[i]) == -1) {
      perror("Failed to join thread");
      exit(EXIT_FAILURE);
    }
    // If return value is zero
    int *return_val = (int *)which_status_vals[i];
    switch (*return_val) {
    case 0:
      printf("Dependency Check %s: PASSED\n", dep_str[i]);
      break;
    case ERR_EXECVP:
      fprintf(stderr, "Dependency Check %s : Error with execvp - which %s\n",
              dep_str[i], dep_str[i]);
      dep_fail_check = 1;
      break;
    case ERR_OPEN:
      fprintf(stderr, "Dependency Check %s: Error with open() - which %s\n",
              dep_str[i], dep_str[i]);
      dep_fail_check = 1;
      break;
    case ERR_DUP2:
      fprintf(stderr, "Dependency Check %s: Error with dup2() - which %s\n",
              dep_str[i], dep_str[i]);
      dep_fail_check = 1;
      break;
    case EXIT_FAILURE:
      fprintf(stderr, "Dependency Check %s : Does not exist\n", dep_str[i]);
      dep_fail_check = 1;
      break;
    default:
      fprintf(stderr, "Dependency Check %s : Does not exist\n", dep_str[i]);
      dep_fail_check = 1;
      break;
    }
    // free the allocated memory from the threads
    free((void *)return_val);
    // Prevent dangling pointers
    which_status_vals[i] = NULL;
    // Do I also have to prevent dangling thread_t info?
  }
  if (dep_fail_check) {
    fprintf(stderr, "One or more deps either does not exist or failed\n");
    exit(EXIT_FAILURE);
  }
  puts("All dependencies are successfully in PATH");
}

int main(int argc, char *argv[]) {
  // 1. Check for dependencies if user wants to
  char *input_directory = ".", *output_directory = "./convert/";
  int input_dir_flag = 0, output_dir_flag = 0, dep_check = 1;

  int opt, option_index = 0;
  static struct option long_options[] = {
      {"verbose", no_argument, 0, 'v'},
      {"file", required_argument, 0, 'f'},
      {"input_directory", required_argument, 0, 'i'},
      {"output_directory", required_argument, 0, 'o'},
      {"no_check_depedency", no_argument, 0, 'n'},
      {"help", no_argument, 0, 'h'},
      {0, 0, 0, 0}};

  while ((opt = getopt_long(argc, argv, "vf:i:o:nh", long_options,
                            &option_index)) != -1) {
    switch (opt) {
    case 'v':
      verbosity = 1;
      break;
    case 'i':
      input_directory = optarg;
      input_dir_flag = 1;
      break;
    case 'o':
      output_directory = optarg;
      output_dir_flag = 1;
      break;
    case 'n':
      dep_check = 0;
      break;
    case 'f':
      fprintf(stderr, "WARNING: Individual file selection not supported!\n");
      break;
    case '?':
      exit(EXIT_FAILURE);
    case 'h':
    default:
      printf("Usage: %s [--verbose] [--input_directory] [path] "
             "[--output_directory] [path] [--no_check_depedency] [--help] \n",
             argv[0]);
      exit(EXIT_SUCCESS);
    }
  }
  if (dep_check) {
    depedency_check();
  }

  // 1. Open directory and compare against the other directory
  // If there are files in the array vector but no input directory, then only do
  // those files
  //
  // If there are files in the array vector AND an input directory, then do both
  // inputs -> output directory
  //
  // If there are no files in the array vector AND no input directory, do the
  // default input directory (current_dir)

  // 2. Check if the output_directory exists:
  // Check to see if input directory = output directory, if it does, abort,
  // otherwise, keep going
  //
  // If no output dirrectory specified and it doesn't
  // exist, attempt to create one. If can't, abort
  //
  // If output directory specified
  // doesn't exist, abort If output directory specified does exist, proceed

  // 3. Choosing output video codec (video enabled/disabled)
  // av1, x264, x265, copy
  char buffer[4096];
  int fd_pipe[2];
  size_t bytes_read;

  // 3a. Choosing CRF number (depends on codec)

  // 3b. Choosing codec preset label (depends on codec)
  //
  // 4. Choosing audio output codec (audio enabled/disabled)
  //
  // 5. Choosing whether we want subtitles copied or taken out

  return EXIT_SUCCESS;
}

// The input, output directory, and file names can be either be parsed or caught
// later on

// 1. Open directory and compare against the other directory
// 2.
// fzf command: printf "Hello World\nHow are you?\n" | fzf --prompt="Select an
// option\n > "
//
// Hardware acceleration choices: https://trac.ffmpeg.org/wiki/HWAccelIntro
