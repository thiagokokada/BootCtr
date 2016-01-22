#pragma once

#include <3ds.h>

#include "boot.h"
#include "config.h"
#include "misc.h"
#include "brahma.h"

#define MAX_PAYLOAD_SIZE 0x10000
#define MS_TO_NS 1000000ULL

typedef struct {
    configuration config;
} application;

void boot_fix(int delay);
int load_3dsx(application app);
int load_payload(application app);
int load(application app);
