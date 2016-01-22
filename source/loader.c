#include "loader.h"

void boot_fix(int delay)
{
    // voodoo to improve rxTools boot rate
    gfxFlushBuffers();
    gfxSwapBuffers();
    gspWaitForVBlank();
    // actually delay boot for some ms
    svcSleepThread(delay * MS_TO_NS);
}

int load_3dsx(application app)
{
    return bootApp(app.config.path);
}

int load_payload(application app)
{
    enum { buf = 512 };
    char error_msg[buf];

    if (brahma_init()) {
        int result = load_arm9_payload_offset(app.config.path,
                app.config.offset, MAX_PAYLOAD_SIZE);
        if (result != 1) {
            snprintf(error_msg, buf, "Couldn't load %s payload", app.config.path);
            goto error;
        }
        firm_reboot();
        brahma_exit();
    } else {
        snprintf(error_msg, buf, "Couldn't init Brahma loader");
        goto error;
    }
    return 0;

error:
    panic(error_msg);
    return -1;
}

int load(application app)
{
    boot_fix(app.config.delay);

    if (app.config.payload < 0) {
        char *ext = get_filename_ext(app.config.path);
        strtolower(ext);

        if(!strncmp(ext, "bin", 3) || !strncmp(ext, "dat", 3)) {
            return load_payload(app);
        } else if(!strncmp(ext, "3dsx", 4)) {
            return load_3dsx(app);
        } else {
            panic("Invalid file: %s\n", app.config.path);
            return 1;
        }
    } else if (app.config.payload > 0) {
        return load_payload(app);
    } else {
        return load_3dsx(app);
    }
}
