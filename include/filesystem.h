#pragma once

#include <3ds.h>

extern FS_Archive sdmcArchive;

//system stuff
void initFilesystem(void);
void exitFilesystem(void);

void openSDArchive(void);
void closeSDArchive(void);
