#include "misc.h"

void init_services(void)
{
    aptInit();
    gfxInitDefault();
    initFilesystem();
    openSDArchive();
    hidInit();
    acInit();
    ptmuInit();
}

void exit_services(void)
{
    ptmuExit();
    acExit();
    hidExit();
    gfxExit();
    closeSDArchive();
    exitFilesystem();
    aptExit();
}

int file_exists(const char *fname)
{
    return access(fname, F_OK) == 0;
}

// http://stackoverflow.com/a/5309508
char *get_filename_ext(const char *filename)
{
    char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) return "";
    return dot + 1;
}

void strtolower(char *str)
{
    for (int i = 0; str[i]; ++i)
	str[i] = tolower((unsigned char)str[i]);
}

void reboot(void)
{
    aptOpenSession();
    APT_HardwareResetAsync();
    aptCloseSession();
    exit_services();
    exit(EXIT_FAILURE);
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
#ifndef DEBUG
    (void)msg;
#else
    consoleInit(GFX_TOP, NULL);

    printf("DEBUG: ");

    va_list args;
    va_start(args, msg);
    vprintf(msg, args);
    va_end(args);

    printf("\nPress START to continue...\n");

    wait_key(KEY_START);
#endif
}

void print_error(const char *msg, ...)
{
    consoleInit(GFX_TOP, NULL);

    printf("ERROR: ");

    va_list args;
    va_start(args, msg);
    vprintf(msg, args);
    va_end(args);

    printf("\nPress START to reboot...\n");

    wait_key(KEY_START);

    reboot();
}
