#pragma once

#include <stdlib.h>
#include <string.h>

#include "inih/ini.h"

typedef struct {
    // "key" is the pressed key got in the main() function
    // "path" and "delay" are the results read from handler() function
    char *key;
    char *path;
    unsigned long long delay;
    int payload;
    unsigned long offset;
} configuration;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
int handler(void *user, const char *section, const char *name, const char *value);
