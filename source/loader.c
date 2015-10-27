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
    // Memory for the arm9 payload
    const u32 payload_size = 0x50000;
    void *payload = malloc(payload_size);
    if (!payload) {
        print_error("Couldn't allocate payload");
        return -1;
    }

    int rc;
    // Load the arm9 payload into memory
    FILE *file = fopen(app.config.path, "r");
    if (!file) {
        print_error("Couldn't open %s", app.config.path);
        return -1;
    }

    if(app.config.offset > 0) {
        rc = fseek(file, app.config.offset, SEEK_SET);
        if (rc != 0) {
            print_error("Couldn't seek %s", app.config.path);
            return -1;
        }
    }

    fread(payload, payload_size, 1, file);
    if (ferror(file) != 0) {
        print_error("Couldn't read %s", app.config.path);
        return -1;
    }
    fclose(file);

    if (brahma_init()) {
        rc = load_arm9_payload_from_mem(payload, payload_size);
        if (rc != 1) {
            print_error("Couldn't load ARM9 payload");
            return -1;
        }
        exit_services(true);
        firm_reboot();
        brahma_exit();
        return 0;
    } else {
        print_error("Couldn't init Brahma");
        return -1;
    }
}

int load(application app)
{
    boot_fix(app.config.delay);

    if (app.config.payload < 0) {
        // this ; is just to fix a quirk from C
        // http://stackoverflow.com/a/18496437
        ; char *ext = get_filename_ext(app.config.path);
        // convert to lower case
        for (int i = 0; ext[i]; ++i) ext[i] = tolower((unsigned char)ext[i]);

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
