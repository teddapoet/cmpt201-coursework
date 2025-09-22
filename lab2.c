// lab2.c
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

static const char *basename_const(const char *path) {
  const char *slash = strrchr(path, '/');
  return (slash && slash[1] != '\0') ? slash + 1 : path;
}

int main(void) {
  char line[1024];

  puts("Enter programs to run.");
  for (;;) {
    fputs("> ", stdout);
    fflush(stdout);

    if (!fgets(line, sizeof(line), stdin)) {
      putchar('\n');
      break;
    }

    line[strcspn(line, "\n")] = '\0';

    if (line[0] == '\0') {
      continue;
    }

    pid_t pid = fork();
    if (pid < 0) {
      perror("fork");
      continue;
    }

    if (pid == 0) {
      const char *argv0 = basename_const(line);
      execl(line, argv0, (char *)NULL);

      fprintf(stderr, "Exec failure\n");

      _exit(127);
    } else {
      int status;
      if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
      }
    }
  }

  return 0;
}
