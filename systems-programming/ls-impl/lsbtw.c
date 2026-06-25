#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

int show_all = 0;
int long_format = 0;

// See man 2 stat, stat(3type), inode(7)
void mode_string(mode_t mode, char *str) {
  // Figure out the type of file:
  if (S_ISDIR(mode))
    str[0] = 'd';
  else if (S_ISCHR(mode))
    str[0] = 'c';
  else if (S_ISBLK(mode))
    str[0] = 'b';
  else if (S_ISFIFO(mode))
    str[0] = 'p';
  else if (S_ISLNK(mode))
    str[0] = 'l';
  else if (S_ISSOCK(mode))
    str[0] = 's';
  else
    str[0] = '-';

  str[1] = (mode & S_IRUSR) ? 'r' : '-';
  str[2] = (mode & S_IWUSR) ? 'w' : '-';
  str[3] = (mode & S_IXUSR) ? 'x' : '-';
  str[4] = (mode & S_IRGRP) ? 'r' : '-';
  str[5] = (mode & S_IWGRP) ? 'w' : '-';
  str[6] = (mode & S_IXGRP) ? 'x' : '-';
  str[7] = (mode & S_IROTH) ? 'r' : '-';
  str[8] = (mode & S_IWOTH) ? 'w' : '-';
  str[9] = (mode & S_IXOTH) ? 'x' : '-';

  str[10] = '\0';
}

void print_long_form(const char *dir, const char *name) {
  // Using snprintf to integrate dir and name into a fullpath without going out
  // of bounds
  char fullpath[4096]; // 4096 is the max characters a full path can take
  snprintf(fullpath, sizeof(fullpath), "%s/%s", dir, name);

  // lstat on fullpath, st to receive the buffer, returns the status of the fxn
  // stat follows the link while lstat is the link itself
  struct stat st;
  if (lstat(fullpath, &st) < 0) {
    perror("name");
    return;
  }

  char modes[11];
  mode_string(st.st_mode, modes);
  struct passwd *pw = getpwuid(st.st_uid);
  struct group *grp = getgrgid(st.st_gid);

  const char *username = (pw) ? pw->pw_name : "?";
  const char *groupname = (grp) ? grp->gr_name : "?";

  // Using timebuf as a fill-in-the-blank, same thing as lstat
  char timebuf[64];
  struct tm *tm = localtime(&st.st_mtim.tv_sec);
  if (strftime(timebuf, sizeof(timebuf), "%b %e %H:%M", tm) == 0) {
    fprintf(stderr, "strftime: empty format time\n");
  }
  printf("%s %lu %s %s %ld %s %s\n", modes, (unsigned long)st.st_nlink,
         username, groupname, (long)st.st_size, timebuf, name);
}

int main(int argc, char *argv[]) {

  int opt;

  while ((opt = getopt(argc, argv, "al")) != -1) {
    switch (opt) {
    case 'a':
      show_all = 1;
      break;
    case 'l':
      long_format = 1;
      break;
    default:
      fprintf(stderr, "usage %s [-al] [path]\n", argv[0]);
      return EXIT_FAILURE;
    }
  }

  // optind is external from unistd
  // We are checking if the path exists if there is another argument besides the
  // flags
  const char *path = (optind < argc) ? argv[optind] : ".";
  printf("path: %s\n", path);

  DIR *dir = opendir(path);
  if (!dir) {
    perror("opendir");
    return EXIT_FAILURE;
  }

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    if (!show_all && entry->d_name[0] == '.')
      continue;

    if (long_format)
      print_long_form(path, entry->d_name);
    else
      printf("%s\n", entry->d_name);
  }

  closedir(dir);

  return EXIT_SUCCESS;
}
