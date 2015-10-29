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
    char error_msg[512];
    int rc;
    s32 fsize;
    s32 psize;

    // Memory for the arm9 payload
    void *payload = malloc(PAYLOAD_SIZE);
    if (!payload) {
        sprintf(error_msg, "Couldn't allocate payload");
        goto error;
    }

    // Open payload file
    FILE *file = fopen(app.config.path, "r");
    if (!file) {
        sprintf(error_msg, "Couldn't open %s", app.config.path);
        goto error;
    }

    // Get file and payload size
    rc = fseek(file, 0L, SEEK_END);
    fsize = ftell(file);
    psize = fsize - app.config.offset;
    if (psize <= 8) {
        sprintf(error_msg, "Payload %s has invalid size", app.config.path);
        goto error;
    }
    if (psize > PAYLOAD_SIZE) psize = PAYLOAD_SIZE;

    // Load the arm9 payload into memory
    rc = fseek(file, app.config.offset, SEEK_SET);
    if (rc != 0) {
        sprintf(error_msg, "Couldn't seek %s", app.config.path);
        goto error;
    }

    fread(payload, psize, 1, file);
    if (ferror(file) != 0) {
        sprintf(error_msg, "Couldn't read %s", app.config.path);
        goto error;
    }
    fclose(file);

    // Put payload in proper place and tries to load it
    if (brahma_init()) {
        rc = load_arm9_payload_from_mem(payload, psize);
        if (rc != 1) {
            sprintf(error_msg, "Couldn't load ARM9 payload");
            goto error;
        }
        free(payload);
        exit_services(true);
        firm_reboot();
        brahma_exit();
    } else {
        sprintf(error_msg, "Couldn't init Brahma");
        goto error;
    }
    return 0;

error:
    free(payload);
    exit_services(false);
    print_error(error_msg);
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
            print_error("Invalid file: %s\n", app.config.path);
            return 1;
        }
    } else if (app.config.payload > 0) {
        return load_payload(app);
    } else {
        return load_3dsx(app);
    }
}
