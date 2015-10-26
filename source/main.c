#include <3ds.h>
#include <stdlib.h>
#include <string.h>

#include "libcakebrah/brahma.h"

#include "filesystem.h"
#include "boot.h"
#include "scanner.h"
#include "config.h"
#include "misc.h"

#define DEFAULT_BOOT "/boot_default.3dsx"
#define DEFAULT_DELAY 150 /* ms */
#define DEFAULT_PAYLOAD 0 /* 0 - false; 1 - true */
#define DEFAULT_OFFSET 0x12000
#define INI_FILE "/boot_config.ini"

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
                "delay: %llu\n"
                "payload: %d\n"
                "offset: %x\n",
                app.config.key, app.config.path, app.config.delay,
                app.config.payload, app.config.offset);

    // sleep for some ms, to increase boot rate in CFWs
    // to be reliable, it needs to wait right after the payload
    // is loaded
    boot_fix(app.config.delay);

    // run application
    char err_msg[32] = "Unknown error";
    if (app.config.payload) {
        // Memory for the arm9 payload
        const u32 payload_size = 0x50000;
        void *payload = malloc(payload_size);
        if (!payload) {
            snprintf(err_msg, sizeof(err_msg), "Couldn't allocate payload");
            goto error;
        }

        int rc;
        // Load the arm9 payload into memory
        FILE *file = fopen(app.config.path, "r");
        if (!file) {
            snprintf(err_msg, sizeof(err_msg), "Couldn't open %s", app.config.path);
            goto error;
        }

        if(app.config.offset > 0) {
            rc = fseek(file, app.config.offset, SEEK_SET);
            if (rc != 0) {
                snprintf(err_msg, sizeof(err_msg), "Couldn't seek %s", app.config.path);
                goto error;
            }
        }

        fread(payload, payload_size, 1, file);
        if (ferror(file) != 0) {
            snprintf(err_msg, sizeof(err_msg), "Couldn't read %s", app.config.path);
            goto error;
        }
        fclose(file);

        if (brahma_init()) {
            rc = load_arm9_payload_from_mem(payload, payload_size);
            if (rc != 1) {
                snprintf(err_msg, sizeof(err_msg), "Couldn't load ARM9 payload");
                goto error;
            }
            exit_services();
            firm_reboot();
            brahma_exit();
            return 0;
        } else {
            snprintf(err_msg, sizeof(err_msg), "Couldn't init Brahma");
            goto error;
        }
    } else {
        gfxExit();
        exit_services();
        return bootApp(app.config.path, &app.em);
    }

error:
    print_error("%s", err_msg);
    gfxExit();
    exit_services();
    return 1;
}
