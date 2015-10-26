#pragma once

#include <3ds.h>
#include <ctype.h>

#include "boot.h"
#include "scanner.h"
#include "config.h"
#include "misc.h"
#include "libcakebrah/brahma.h"

typedef struct {
    configuration config;
    executableMetadata_s em;
} application;

void boot_fix(int delay);
int load_3dsx(application app);
int load_payload(application app);
int load(application app);
