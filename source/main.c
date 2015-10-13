#include <stdlib.h>
#include <string.h>

#include <3ds.h>

#include "filesystem.h"
#include "boot.h"
#include "config.h"

#define DEFAULT_BOOT "/boot_default.3dsx"
#define DEFAULT_DELAY 10000000
#define INI_FILE "/boot_config.ini"

// ugly global variable, however I don't know a better way to pass the pressed
// key to handler function
char *KEY_PRESSED;

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

    // offset potential issues caused by homebrew that just ran
    aptOpenSession();
    APT_SetAppCpuTimeLimit(NULL, 0);
    aptCloseSession();

    hidScanInput();

    u32 key = hidKeysDown();
    switch (key) {
        case KEY_A:
            KEY_PRESSED = "KEY_A";
            break;
        case KEY_B:
            KEY_PRESSED = "KEY_B";
            break;
        case KEY_SELECT:
            KEY_PRESSED = "KEY_SELECT";
            break;
        case KEY_START:
            KEY_PRESSED = "KEY_START";
            break;
        case KEY_DRIGHT:
            KEY_PRESSED = "KEY_DRIGHT";
            break;
        case KEY_DLEFT:
            KEY_PRESSED = "KEY_DLEFT";
            break;
        case KEY_DUP:
            KEY_PRESSED = "KEY_DUP";
            break;
        case KEY_DDOWN:
            KEY_PRESSED = "KEY_DDOWN";
            break;
        case KEY_R:
            KEY_PRESSED = "KEY_R";
            break;
        case KEY_L:
            KEY_PRESSED = "KEY_L";
            break;
        case KEY_X:
            KEY_PRESSED = "KEY_X";
            break;
        case KEY_Y:
            KEY_PRESSED = "KEY_Y";
            break;
        default:
            KEY_PRESSED = "DEFAULT";
            break;
    }
    
    configuration config = { .path = DEFAULT_BOOT, .delay = DEFAULT_DELAY };
    ini_parse(INI_FILE, handler, &config);

    // cleanup whatever we have to cleanup
    hidExit();
    exitFilesystem();
    closeSDArchive();
    aptExit();
    srvExit();

    // wait some time to improve boot chance in CFWs
    svcSleepThread(config.delay);

    // run application
    return bootApp(config.path);
}
