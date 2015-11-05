#include "loader.h"

void boot_fix(int delay)
{
    for (;aptMainLoop() && delay > 0; --delay) {
        //gfxClearColor((u8[]){0x00, 0x00, 0x00});
	gfxFlushBuffers();
	gfxSwapBuffers();
	gspWaitForVBlank();
    }
}

int load_3dsx(application app)
{
    scanExecutable2(&app.em, app.config.path);
    exit_services(false);
    return bootApp(app.config.path, &app.em);
}

int load_payload(application app)
{
#define BUF 512
    char error_msg[BUF];

    if (brahma_init()) {
        int result = load_arm9_payload(app.config.path,
                app.config.offset, PAYLOAD_MAX_SIZE);
        if (result != 1) {
            snprintf(error_msg, BUF, "Couldn't load %s payload", app.config.path);
            goto error;
        }
        exit_services(true);
        firm_reboot();
        brahma_exit();
    } else {
        snprintf(error_msg, BUF, "Couldn't init Brahma loader");
        goto error;
    }
    return 0;

error:
    exit_services(false);
    print_error(error_msg);
    return -1;
#undef BUF
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
            print_error("Invalid file: %s\n", app.config.path);
            return 1;
        }
    } else if (app.config.payload > 0) {
        return load_payload(app);
    } else {
        return load_3dsx(app);
    }
}
