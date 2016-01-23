#pragma once

#include <3ds.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "filesystem.h"

//#define DEBUG

int file_exists(const char *fname);
char *get_filename_ext(const char *filename);
void strtolower(char *str);
void reboot(void);
void wait_key(const u32 key);
void debug(const char *msg, ...) __attribute__((format(printf, 1, 2)));
void panic(const char *msg, ...) __attribute__((format(printf, 1, 2)));
