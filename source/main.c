#include <stdlib.h>
#include <string.h>

#include <3ds.h>

#include "filesystem.h"
#include "boot.h"
#include "config.h"

#define DEFAULT_BOOT "/boot_default.3dsx"
#define DEFAULT_DELAY 100 /* ms */
#define INI_FILE "/boot_config.ini"
#define MS_TO_NS 1000000ULL

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
    amInit();

    // offset potential issues caused by homebrew that just ran
    aptOpenSession();
    APT_SetAppCpuTimeLimit(NULL, 0);
    aptCloseSession();

    // initialize default configuration
    configuration config = {
        .key = "DEFAULT",
        .path = DEFAULT_BOOT,
        .delay = DEFAULT_DELAY
    };

    // https://github.com/smealum/ctrulib/blob/master/libctru/include/3ds/services/hid.h
    hidScanInput();
    u32 key = hidKeysDown();
    switch (key) {
        case KEY_A:
            config.key = "KEY_A";
            break;
        case KEY_B:
            config.key = "KEY_B";
            break;
        case KEY_SELECT:
            config.key = "KEY_SELECT";
            break;
        case KEY_START:
            config.key = "KEY_START";
            break;
        case KEY_RIGHT:
            config.key = "KEY_RIGHT";
            break;
        case KEY_LEFT:
            config.key = "KEY_LEFT";
            break;
        case KEY_UP:
            config.key = "KEY_UP";
            break;
        case KEY_DOWN:
            config.key = "KEY_DOWN";
            break;
        case KEY_R:
            config.key = "KEY_R";
            break;
        case KEY_L:
            config.key = "KEY_L";
            break;
        case KEY_X:
            config.key = "KEY_X";
            break;
        case KEY_Y:
            config.key = "KEY_Y";
            break;
        case KEY_ZL:
            config.key = "KEY_ZL";
            break;
        case KEY_ZR:
            config.key = "KEY_ZR";
            break;
        default:
            config.key = "DEFAULT";
            break;
    }

    // the handler only change the "config" variable if it finds a
    // valid configuration
    ini_parse(INI_FILE, handler, &config);

    // cleanup whatever we have to cleanup
    amExit();
    ptmExit();
    acExit();
    irrstExit();
    hidExit();
    closeSDArchive();
    exitFilesystem();
    aptExit();
    srvExit();

    // wait some time to improve boot chance in CFWs
    // we convert to microseconds here, since nanoseconds is to fast
    // to be useful
    svcSleepThread(config.delay * MS_TO_NS);

    // run application
    return bootApp(config.path);
}
