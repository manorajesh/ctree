#include <dirent.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wchar.h>

#define COLOR_RESET "\033[0m"
#define COLOR_DIR "\033[1;34m"
#define COLOR_PIPE "\033[1;30m"
#define COLOR_BRANCH "\033[1;30m"

const char *color_reset = COLOR_RESET;
const char *color_dir = COLOR_DIR;
const char *color_pipe = COLOR_PIPE;
const char *color_branch = COLOR_BRANCH;

void print_tree(const char *base_path, int depth) {
  struct dirent *dp;
  DIR *dir = opendir(base_path);

  if (!dir) {
    perror("Unable to open directory");
    return;
  }

  while ((dp = readdir(dir)) != NULL) {
    if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
      for (int i = 0; i < depth; i++) {
        printf("%s│   %s", color_pipe, color_reset);
      }
      printf("%s├ %s", color_branch, color_reset);

      char path[1000];
      snprintf(path, sizeof(path), "%s/%s", base_path, dp->d_name);

      struct stat statbuf;
      if (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
        printf("%s%s%s\n", color_dir, dp->d_name, color_reset);
        print_tree(path, depth + 1);
      } else {
        printf("%s\n", dp->d_name);
      }
    }
  }

  closedir(dir);
}

void print_help(const char *prog_name) {
  printf("Usage: %s [OPTIONS] PATH\n", prog_name);
  printf("Print the directory tree starting from PATH.\n\n");
  printf("Options:\n");
  printf("  -h        Show this help message and exit\n");
  printf("  -V        Show version information and exit\n");
  printf("  -n        Disable colored output\n");
}

void print_version() { printf("ctree version 1.0\n"); }

int main(int argc, char *argv[]) {
  setlocale(LC_ALL, "");

  int opt;
  const char *path = ".";
  int no_color = 0;

  while ((opt = getopt(argc, argv, "hVn")) != -1) {
    switch (opt) {
    case 'h':
      print_help(argv[0]);
      exit(EXIT_SUCCESS);
    case 'V':
      print_version();
      exit(EXIT_SUCCESS);
    case 'n':
      no_color = 1;
      break;
    default:
      print_help(argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  if (optind < argc) {
    path = argv[optind];
  }

  if (no_color) {
    color_reset = "";
    color_dir = "";
    color_pipe = "";
    color_branch = "";
  }

  print_tree(path, 0);
  return 0;
}