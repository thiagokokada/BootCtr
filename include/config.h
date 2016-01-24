#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "ini.h"

typedef struct {
    char *section;
    char *path;
    unsigned long long delay;
    int payload;
    unsigned long offset;
    bool cfw_fix;
    bool debug;
} configuration;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
int handler(void *user, const char *section, const char *name, const char *value);
