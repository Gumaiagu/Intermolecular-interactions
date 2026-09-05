#include <stdio.h>  // Basic file managing and debug
#include <stdlib.h> // free(3) and exit(3) functions

#include <string.h> // String managing tools
#include <ctype.h>  // Managing types

#include "local/values.h"   // My header


void strip_left(char **__restrict string) {
  while (1) {
    if ((*(string)[0] == '\0') | !isblank(*(string)[0])) break;
    string++;
  }
}


void check_value(const char *__restrict name, const char *__restrict line) {
  if (strcmp(name, "WW") == 0) {
    WW = atoi(line);
  }
  else if (strcmp(name, "WH") == 0) {
    WH = atoi(line);
  }
  else if (strcmp(name, "GRAVITY") == 0) {
    G = atof(line);
  }
  else if (strcmp(name, "NUMBER_OF_BALLS") == 0) {
    NUMBER_OF_BALLS = atoi(line);
  }
  else if (strcmp(name, "REPULTION") == 0) {
    REPULTION = atof(line);
  }
  else if (strcmp(name, "MAX_START_SPEED") == 0) {
    MAX_START_SPEED = atof(line);
  }
}


void read_config() {
  FILE* config = fopen("config", "r");
  if (config == NULL) {
    perror("Could not read file");
    exit(1);
  }
  size_t len;
  char* line = NULL;
  char* new = NULL;
  int read;

  read = getline(&new, &len, config);

  while (read > 0) {
    line = new;
    char* name = strsep(&line, ":");
    if (name == NULL) {
      read = getline(&new, &len, config);
      continue;
    }
    strip_left(&line);

    check_value(name, line);

    read = getline(&new, &len, config);
  }
  free(new);
  fclose(config);
}
