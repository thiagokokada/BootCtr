#include "misc.h"

void init_services()
{
    srvInit();
    aptInit();
    initFilesystem();
    openSDArchive();
    gfxInitDefault();
    hidInit(NULL);
    irrstInit(NULL);
    acInit();
    ptmInit();
    amInit();
}

void exit_services()
{
    amExit();
    ptmExit();
    acExit();
    irrstExit();
    hidExit();
    // gfxExit(); // hangs brahma/libkhax
    closeSDArchive();
    exitFilesystem();
    aptExit();
    srvExit();
}

int file_exists(const char *fname)
{
    FILE *file = fopen(fname, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

// http://stackoverflow.com/a/5309508
char *get_filename_ext(const char *filename)
{
    char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) return "";
    return dot + 1;
}

void reboot()
{
	aptInit();
	aptOpenSession();
	APT_HardwareResetAsync(NULL);
	aptCloseSession();
	aptExit();
	exit(EXIT_SUCCESS);
}

void wait_key(const u32 key)
{
	while (aptMainLoop()) {
		hidScanInput();
		if (hidKeysDown() & key) break;

		gfxFlushBuffers();
		gfxSwapBuffers();

		gspWaitForVBlank();
	}
}

void print_debug(const char *msg, ...)
{
#ifdef DEBUG
	va_list args;
	char fmt[512];

	va_start(args, msg);
	vsprintf(fmt, msg, args);
	va_end(args);

	consoleInit(GFX_TOP, NULL);
	printf("DEBUG: %s\n", fmt);
	printf("Press START to continue...\n");

	wait_key(KEY_START);
#endif
}

void print_error(const char *msg, ...)
{
	va_list args;
	char fmt[512];

	va_start(args, msg);
	vsprintf(fmt, msg, args);
	va_end(args);

	consoleInit(GFX_TOP, NULL);
	printf("ERROR: %s\n", fmt);
	printf("Press START to reboot...\n");

	wait_key(KEY_START);
}
