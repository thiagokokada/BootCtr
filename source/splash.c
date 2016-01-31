#include "splash.h"

void splash_ascii(void)
{
    // print BootCtr logo
    // http://patorjk.com/software/taag/#p=display&f=Bigfig&t=BootCtr
    consoleInit(GFX_TOP, NULL);
    printf(ASCII_ART_TEMPLATE, VERSION_STRING);
}

void splash_image(char *splash_path)
{
    // load image in memory, doing proper error checking
    FILE *splash_file = fopen(splash_path, "rb");
    if (!splash_file) {
        // show ASCII splash art in case of error
        printf("\nCouldn't open splash image %s.\n", splash_path);
        goto force_splash_ascii;
    }
    fseek(splash_file, 0, SEEK_END);
    u32 splash_size = ftell(splash_file);
    fseek(splash_file, 0, SEEK_SET);
    u8 *splash_buf = malloc(splash_size);
    if (!splash_buf) {
        printf("\nSplash image %s is too big.\n", splash_path);
        goto force_splash_ascii;
    }
    size_t splash_read = fread(splash_buf, 1, splash_size, splash_file);
    if (splash_read != splash_size) {
        printf("\nError while reading splash image %s.\n", splash_path);
        goto force_splash_ascii;
    }
    // disable double buffering, to avoid flickering
    gfxSetDoubleBuffering(GFX_TOP, false);
    // copy splash image to framebuffer
    memcpy(gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL),
            splash_buf, splash_size);
    return;

force_splash_ascii:
    splash_ascii();
}
