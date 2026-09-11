#include <stdio.h>  // Basic file managing and debug
#include <stdlib.h> // free(3) and exit(3) functions

#include <string.h> // String managing tools
#include <ctype.h>  // Managing types

#include <errno.h>

#ifndef __unix__
#include "windows_stuff.c"  // GNU functions, but for windows
#endif

#include "local/values.h"   // My header


void strip_left(char** string) {
  while (1) {
    if (((*string)[0] == '\0') | !isblank((*string)[0])) break;
    (*string)++;
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
  else if (strcmp(name, "CHANGE") == 0) {
    change = atof(line);
  }
}


int read_config() {
  FILE* config = fopen("config", "r");
  ON_ERROR("Could not read config");
  size_t len;
  char* line = NULL;
  char* new = NULL;
  int read;

  read = getline(&new, &len, config);

  while (read > 0) {
    ON_ERROR("Could not read line");
    line = new;
    if ((line[0] == '\n') | (line[0] == '\0') | (line[0] == '#')) {
      read = getline(&new, &len, config);
      continue;
    }

    char* name = strsep(&line, ":");
    if ((name == NULL) | (line == NULL)) {
      read = getline(&new, &len, config);
      continue;
    }
    strip_left(&line);

    check_value(name, line);

    read = getline(&new, &len, config);
  }
  free(new);
  fclose(config);
  ON_ERROR("Could not close config");
  return 0;
}
