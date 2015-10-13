#include <stdlib.h>
#include <string.h>

#include <3ds.h>

#include "filesystem.h"
#include "boot.h"
#include "ini.h"

#define DEFAULT_BOOT "/boot_default.3dsx"
#define DEFAULT_DELAY 10000000
#define INI_FILE "/boot_config.ini"

char *KEY_PRESSED;

typedef struct
{
    char *path;
    unsigned long long delay;
} configuration;

static int handler(void *user, const char *section, const char *name, const char *value)
{
    configuration *pconfig = (configuration *) user;
    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
    if (MATCH(KEY_PRESSED, "path")) {
        pconfig->path = strdup(value);
    } else if (MATCH(KEY_PRESSED, "delay")) {
        pconfig->delay = atoi(value);
    } else {
        return 0;
    }
    return 1;
}

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
    gfxInitDefault();
    
    consoleInit(GFX_TOP, NULL);

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
    if (ini_parse(INI_FILE, handler, &config) < 0) {
        printf("Can't load '%s'\n, using default...\n", INI_FILE);
    }
    
    printf("%s detected!\n", KEY_PRESSED);
    printf("Booting %s in %llu ms...\n", config.path, config.delay);

    // cleanup whatever we have to cleanup
    gfxExit();
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
