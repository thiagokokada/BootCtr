#pragma once

#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASCII_ART_TEMPLATE (" _           __\n" \
                            "|_) _  _ _|_/  _|_ __\n" \
                            "|_)(_)(_) |_\\__ |_ |\n" \
                            "%s\n" \
                            "\n" \
                            "is loading...")

int splash_ascii(void);
int splash_image(char *splash_path);
