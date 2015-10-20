#include <stdlib.h>
#include <string.h>

#include <3ds.h>

#include "filesystem.h"
#include "boot.h"
#include "scanner.h"
#include "config.h"
#include "misc.h"

#define DEFAULT_BOOT "/boot_default.3dsx"
#define DEFAULT_DELAY 100 /* ms */
#define INI_FILE "/boot_config.ini"
#define MS_TO_NS 1000000ULL

typedef struct {
    configuration config;
    executableMetadata_s em;
} application;

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

    application app = {
        .config.key = "DEFAULT",
        .config.path = DEFAULT_BOOT,
        .config.delay = DEFAULT_DELAY
    };

    // load default user configuration, overriding the app defaults
    // the handler only change the "config" variable if it finds a
    // valid configuration, so even without error checking it should be safe
    ini_parse(INI_FILE, handler, &app.config);

    hidScanInput();
    u32 key = hidKeysDown();
    char *aux;
    switch (key) {
        // using X-macros to generate each switch-case rules
        // https://en.wikibooks.org/wiki/C_Programming/Preprocessor#X-Macros
        #define KEY(k) \
        case KEY_##k: \
            aux = "KEY_"#k; \
            break;
        #include "keys.def"
        default:
            aux = "DEFAULT";
            break;
    }
    app.config.key = aux;

    int error = ini_parse(INI_FILE, handler, &app.config);
    switch (error) {
        case 0:
            if (!file_exists(app.config.path)) {
                print_error("File %s not found", app.config.path);
            }
            break;
        case -1:
            if (!file_exists(DEFAULT_BOOT)) {
                print_error("File %s and config file %s not found",
                        DEFAULT_BOOT, INI_FILE);
            }
            break;
        case -2:
            // never happens since we are using stack instead of memory
            // allocations in inih
            break;
        default:
            if (!file_exists(DEFAULT_BOOT)) {
                print_error("File %s not found and error found in config "
                        "file %s on line %d", DEFAULT_BOOT, INI_FILE, error);
            }
            break;
    }

    scanExecutable2(&app.em, app.config.path);

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
    // we convert to microseconds here, since nanoseconds is too fast
    // to be useful
    svcSleepThread(app.config.delay * MS_TO_NS);

    // run application
    return bootApp(app.config.path, &app.em);
}
