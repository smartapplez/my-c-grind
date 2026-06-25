#include <fcntl.h>
#include <getopt.h>
#include <linux/limits.h>
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define ERR_DUP2 3
#define ERR_OPEN 4
#define ERR_EXECVP 21

#define NUM_DEPENDENCIES 2
#define MAX_PATH_LENGTH 4096

enum dependencies { FFMPEG = 0, FZF };
static enum dependencies deps_array[2] = {FFMPEG, FZF};
static char *const dep_str[2] = {"ffmpeg", "fzf"};
static int verbosity = 0;
static inline int *check_bin_dependency(enum dependencies *dep_flag);
static char *const libx264_crf_choices = "";
static char *const libx264_preset_choices = "";
static char *const av1_crf_preset = "22 (Archival)\n24 (Great)\n26 (Good)\n";
static char *const av1_preset = "";
static char *const libx265_crf_choices = "18 (Archival)\n20 (Great)\n22 (Good)";
static char *const libx265_preset_choices = "slow\nmedium\nfast";
