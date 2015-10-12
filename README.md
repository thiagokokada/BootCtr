Homebrew Launcher emergency boot
================================

By mashers at GBATemp, modified by m45t3r
-----------------------------------------

Based on 3DS Homebrew Launcher code by [Smealum][1]. The original version is
from [mashers][2], I simply removed some unused code used by Homebrew Launcher
that isn't necessary in a simple bootloader.

Installation:
-------------

* Rename the boot.3dsx file already on your SD card to boot_1.3dsx.
* Place some other launcher or FTP client in the root of your SD card and call
it boot_2.3dsx.
* Rename HBL-Emergency.3dsx to boot.3dsx, copy the file from this archive to the
root of your SD card.

Usage
-----

* Boot up your homebrew hax as usual.
* If you do nothing, then boot_1.3dsx will be launched (your normal launcher).
* If something goes wrong with your launcher, hold down the right shoulder (R)
button while launching your homebrew hax. This will cause boot_2.3dsx to be
booted instead.

Modifications and compiling
---------------------------

To add another entry, simple edit ``source/main.c`` file and add another entry
in the ``switch..case`` part. For example, if you want to add a third entry
using the left shoulder (L) you would add the following lines:

```C
switch (key) {
/* ... */
case KEY_L:
   execPath = "/boot_3.3dsx";
   break;
/* ... */
}
```

After that, compile a new binary using the included ``Makefile`` and
[devkitPro][3]. If devkitPro is correctly installed all you need to do is:

    $ make

And copy/rename the new ``HBL-Emergency.3dsx`` file to your SD card.

[1]: https://github.com/smealum/3ds_hb_menu
[2]: https://gbatemp.net/threads/release-homebrew-emergency-launcher.399394/
[3]: http://devkitpro.org/
