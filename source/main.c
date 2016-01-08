#include <3ds.h>
#include <stdlib.h>
#include <string.h>

#include "filesystem.h"
#include "loader.h"
#include "config.h"
#include "misc.h"

#define DEFAULT_BOOT "/boot_default.3dsx"
#define DEFAULT_DELAY 100 /* ms */
#define DEFAULT_PAYLOAD -1 /* <0 - auto, 0 - disable, >0 - enabled */
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

    osSetSpeedupEnable(true);

    // offset potential issues caused by homebrew that just ran
    aptOpenSession();
    APT_SetAppCpuTimeLimit(0);
    aptCloseSession();

    // set application default preferences
    application app = {
        .config = {
            .key = "DEFAULT",
            .path = DEFAULT_BOOT,
            .delay = DEFAULT_DELAY,
            .payload = DEFAULT_PAYLOAD,
            .offset = DEFAULT_OFFSET
        }
    };

    // load default user configuration, overriding the app defaults
    int config_err = ini_parse(INI_FILE, handler, &app.config);

    // get pressed user key, convert to string to pass to ini_parse
    hidScanInput();
    u32 key = hidKeysDown();
    char *key_str;
    switch (key) {
        // using X-macros to generate each switch-case rules
        // https://en.wikibooks.org/wiki/C_Programming/Preprocessor#X-Macros
        #define KEY(k) \
        case KEY_##k: \
            key_str = "KEY_"#k; \
            break;
        #include "keys.def"
        default:
            key_str = "DEFAULT";
            break;
    }
    app.config.key = key_str;

    // only call ini_parse again if user pressed a key to load the
    // corresponding key preferences
    if (key_str[0] != 'D') {
        config_err = ini_parse(INI_FILE, handler, &app.config);
    }

    switch (config_err) {
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
                print_error("File %s not found and error found in config file "
                        "%s on line %d", DEFAULT_BOOT, INI_FILE, config_err);
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
