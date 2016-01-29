#include <3ds.h>
#include <stdlib.h>
#include <string.h>

#include "filesystem.h"
#include "loader.h"
#include "config.h"
#include "misc.h"

#define DEFAULT_PATH NULL
#define DEFAULT_DELAY 2000 /* ms */
#define DEFAULT_PAYLOAD -1 /* <0 - auto, 0 - disable, >0 - enabled */
#define DEFAULT_OFFSET 0x12000
#define DEFAULT_SECTION "GLOBAL"
#define DEFAULT_SPLASH true
#define DEFAULT_SPLASH_IMAGE NULL
#define INI_FILE "/boot_config.ini"

// handled in main
// doing it in main is preferred because execution ends in launching another 3dsx
void __appInit()
{
    srvInit();
    aptInit();
    gfxInitDefault();
    initFilesystem();
    openSDArchive();
    hidInit();
}

// same
void __appExit()
{
    hidExit();
    gfxExit();
    closeSDArchive();
    exitFilesystem();
    aptExit();
    srvExit();
}

int main()
{
    // offset potential issues caused by homebrew that just ran
    aptOpenSession();
    APT_SetAppCpuTimeLimit(0);
    aptCloseSession();

    // set application default preferences
    application app = {
        .config = {
            .section = DEFAULT_SECTION,
            .path = DEFAULT_PATH,
            .delay = DEFAULT_DELAY,
            .payload = DEFAULT_PAYLOAD,
            .offset = DEFAULT_OFFSET,
            .splash = DEFAULT_SPLASH,
            .splash_image = DEFAULT_SPLASH_IMAGE,
        }
    };

    // override default options with [GLOBAL] section
    // don't need to check error for now
    ini_parse(INI_FILE, handler, &app.config);

    // get pressed user key, convert to string to pass to ini_parse
    hidScanInput();
    u32 key = hidKeysDown();
    switch (key) {
        // using X-macros to generate each switch-case rules
        // https://en.wikibooks.org/wiki/C_Programming/Preprocessor#X-Macros
        #define KEY(k) \
        case KEY_##k: \
            app.config.section = "KEY_"#k; \
            break;
        #include "keys.def"
        default:
            app.config.section = "DEFAULT";
            break;
    }

    // load config from section [KEY_*], check error for issues
    int config_err = ini_parse(INI_FILE, handler, &app.config);
    switch (config_err) {
        case 0:
            // section not found, try to load [DEFAULT] section
            if (!app.config.path) {
                app.config.section = "DEFAULT";
                // don't need to check error again
                ini_parse(INI_FILE, handler, &app.config);
                if (!app.config.path)
                    panic("Section [DEFAULT] not found or \"path\" not set.");
            } else if (app.config.path[0] != '/') {
                panic("In section [%s], path %s is invalid.",
                        app.config.section, app.config.path);
            } else if (!file_exists(app.config.path)) {
                panic("In section [%s], file %s not found.",
                        app.config.section, app.config.path);
            }
            break;
        case -1:
            panic("Config file %s not found.", INI_FILE);
            break;
        case -2:
            // should not happen, however better be safe than sorry
            panic("Config file %s too big.", INI_FILE);
            break;
        default:
            panic("Error found in config file %s on line %d.",
                    INI_FILE, config_err);
            break;
    }

    if (app.config.splash) {
        // print entry information in bottom screen
        consoleInit(GFX_BOTTOM, NULL);
        printf("Booting entry [%s]:\n"
               "\t* path = %s\n"
               "\t* delay = %llu\n"
               "\t* payload = %d\n"
               "\t* offset = 0x%lx\n",
               app.config.section, app.config.path, app.config.delay,
               app.config.payload, app.config.offset);

        // try to load user set splash image
        if (app.config.splash_image) {
            // load image in memory, doing proper error checking
            FILE *splash_file = fopen(app.config.splash_image, "rb");
            if (!splash_file) {
                printf("\nCouldn't open splash image %s.\n",
                        app.config.splash_image);
                goto force_ascii_splash;
            }
            fseek(splash_file, 0, SEEK_END);
            u32 splash_size = ftell(splash_file);
            fseek(splash_file, 0, SEEK_SET);
            u8 *splash_buf = malloc(splash_size);
            if (!splash_buf) {
                printf("\nSplash image %s is too big.\n",
                        app.config.splash_image);
                goto force_ascii_splash;
            }
            size_t splash_read = fread(splash_buf, 1, splash_size, splash_file);
            if (splash_read != splash_size) {
                printf("\nError while reading splash image %s.\n",
                        app.config.splash_image);
                goto force_ascii_splash;
            }
            // disable double buffering, to avoid flickering
            gfxSetDoubleBuffering(GFX_TOP, false);
            // copy splash image to framebuffer
            memcpy(gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL),
                    splash_buf, splash_size);
        } else {
// in case of error while loading splash image, force ASCII splash screen
force_ascii_splash:
            // print BootCtr logo
            // http://patorjk.com/software/taag/#p=display&f=Bigfig&t=BootCtr
            consoleInit(GFX_TOP, NULL);
            printf(" _           __\n"
                   "|_) _  _ _|_/  _|_ __\n"
                   "|_)(_)(_) |_\\__ |_ |\n"
                   "%s\n"
                   "\n"
                   "is loading...", VERSION_STRING);
        }
    }

    return load(app);
}
