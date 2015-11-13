#pragma once

#include <3ds.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>

#include "filesystem.h"

//#define DEBUG

void init_services(void);
void exit_services(void);
int file_exists(const char *fname);
char *get_filename_ext(const char *filename);
void strtolower(char *str);
void reboot(void);
void wait_key(const u32 key);
void print_debug(const char *msg, ...);
void print_error(const char *msg, ...);
