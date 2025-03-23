#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "env_loader.h"

void load_env(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Cannot open file .env");
        return;
    }

    char line[MAX_ENV_LINE];
    while (fgets(line, sizeof(line), file)) {
        // skip empty lines and comments
        if (line[0] == '#' || line[0] == '\n') {
            continue;
        }

        line[strcspn(line, "\n")] = 0;

        char *key = strtok(line, "=");
        char *value = strtok(NULL, "=");

        if (key && value) {
            setenv(key, value, 1);
        }
    }

    fclose(file);
}