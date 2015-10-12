#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <3ds.h>
#include <sys/stat.h>

#include "filesystem.h"
#include "boot.h"
#include "scanner.h"

// handled in main
// doing it in main is preferred because execution ends in launching another 3dsx
void __appInit()
{
}

// same
void __appExit()
{
}

int main()
{
    srvInit();
    aptInit();
    initFilesystem();
    openSDArchive();
    hidInit(NULL);
    irrstInit(NULL);
    acInit();
    ptmInit();
    titlesInit();

    // offset potential issues caused by homebrew that just ran
    aptOpenSession();
    APT_SetAppCpuTimeLimit(NULL, 0);
    aptCloseSession();

    char * execPath;

    hidScanInput();

    if (hidKeysDown()&KEY_R || hidKeysHeld()&KEY_R) {
        execPath = "/boot_2.3dsx";
    }
    else {
        execPath = "/boot_1.3dsx";
    }

    // cleanup whatever we have to cleanup
    titlesExit();
    ptmExit();
    acExit();
    irrstExit();
    hidExit();
    exitFilesystem();
    closeSDArchive();
    aptExit();
    srvExit();

    return bootApp(execPath, NULL);
}
