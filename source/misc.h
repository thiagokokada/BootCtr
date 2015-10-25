#pragma once

#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "filesystem.h"

void init_services();
void exit_services();
int file_exists(const char *fname);
const char *get_filename_ext(const char *filename);
void reboot();
void print_error(const char *msg, ...);
