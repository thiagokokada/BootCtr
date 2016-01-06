#include <stdio.h>
#include <string.h>
#include <3ds.h>

#include "filesystem.h"

FS_Archive sdmcArchive;

void initFilesystem(void)
{
    fsInit();
    sdmcInit();
}

void exitFilesystem(void)
{
    sdmcExit();
    fsExit();
}

void openSDArchive(void)
{
    sdmcArchive = (FS_Archive){0x00000009, (FS_Path){PATH_EMPTY, 1, (u8*)""}, 0};
    FSUSER_OpenArchive(&sdmcArchive);
}

void closeSDArchive(void)
{
    FSUSER_CloseArchive(&sdmcArchive);
}
