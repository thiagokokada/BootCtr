#include <3ds.h>
#include <stdlib.h>
#include <string.h>

#include "filesystem.h"
#include "loader.h"
#include "config.h"
#include "misc.h"

#define DEFAULT_BOOT "/boot_default.3dsx"
#define DEFAULT_DELAY 150 /* ms */
#define DEFAULT_PAYLOAD AUTO
#define DEFAULT_OFFSET 0x12000
#define INI_FILE "/boot_config.ini"

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
    init_services();

    // offset potential issues caused by homebrew that just ran
    aptOpenSession();
    APT_SetAppCpuTimeLimit(NULL, 0);
    aptCloseSession();

    application app = {
        .config.key = "DEFAULT",
        .config.path = DEFAULT_BOOT,
        .config.delay = DEFAULT_DELAY,
        .config.payload = DEFAULT_PAYLOAD,
        .config.offset = DEFAULT_OFFSET
    };

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
            if (app.config.path[0] != '/') {
                print_error("%s is a invalid path (missing '/')",
                        app.config.path);
            }
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

    print_debug("\n"
                "key: %s\n"
                "path: %s\n"
                "delay: %d\n"
                "payload: %d\n"
                "offset: %x\n",
                app.config.key, app.config.path, app.config.delay,
                app.config.payload, app.config.offset);

    return load(app);
}
