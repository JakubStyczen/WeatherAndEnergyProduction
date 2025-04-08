#include "../include/env_loader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/log.h"

void load_env(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    log_error("Cannot open file .env");
    return;
  }

  char line[MAX_ENV_LINE];
  while (fgets(line, sizeof(line), file)) {
    if (line[0] == '#' || line[0] == '\n') {
      continue;
    }

    line[strcspn(line, "\n")] = 0;

    char *equals_sign = strchr(line, '=');
    if (!equals_sign || equals_sign == line) {
      continue;
    }

    *equals_sign = '\0';
    char *key = line;
    char *value = equals_sign + 1;

    if (key && value) {
      setenv(key, value, 1);
    }
  }

  fclose(file);
}