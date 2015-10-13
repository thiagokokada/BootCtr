#pragma once

#include <stdlib.h>
#include <string.h>

#include "ini.h"

// used to pass pressed key information from main() to handler()
extern char *KEY_PRESSED;

typedef struct
{
    char *path;
    unsigned long long delay;
} configuration;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
int handler(void *user, const char *section, const char *name, const char *value);
