#pragma once

#include <3ds.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int file_exists(const char *fname);
void reboot();
void print_error(const char *msg, ...);
