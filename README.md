BootCtr - A simple boot manager for 3DS
=======================================

![BootCtr logo](bootctr.png?raw=true)

Based on 3DS Homebrew Launcher code by [Smealum][hbl]. The original version is
from [mashers][hbe]. This version removed some unused code used by Homebrew
Launcher that isn't necessary in a simple bootloader, and added some
interesting new features.

Any question can be done in the [official topic][ofc] in GBAtemp forum.

Features
--------

* Supports both `.3dsx` files and binary payloads (for example, `/Cakes.dat`,
* `/ReiNand.dat`, `/rxTools/sys/code.bin`, etc.).
* Supports advanced features in `.3dsx` files, like network support.
* Boot delay, to increase the boot rate chance of CFWs like rxTools and
CakesFW.
* Allows up-to **16+1** (16 using keys plus 1 default) homebrews in O3DS,
up-to **22+1** homebrews in N3DS.
* Support for boot screens, including an ASCII art (default) or custom
images in [BGR][bgr] format:

[![Boot screen support!](http://img.youtube.com/vi/_rdHWEJwhLA/0.jpg)](http://www.youtube.com/watch?v=_rdHWEJwhLA)

Installation
------------

* (Optional, but recommended) Install [HomeMenuHax][hmh] and set up autoboot.
* Modify, according to your needs, the included `boot_config.ini` file.
* Copy both `boot.3dsx` and `boot_config.ini` to the root of your SD card.
* For splash screen, look inside `splash` folder for some ready to use `.bin`
files (including source file/preview in `.png` format), or use the included
`img2bgr.sh` script to convert your own images (or convert them using this
[site][i2b]). Dont't forget to edit `boot_config.ini` to include `splash_file`
option in the desired entry.

Usage
-----

* Boot up your homebrew hax as usual.
* If you do nothing, then `boot_default.3dsx` (or anything that you setup in
`[DEFAULT]` section) will be launched.
* If you want to load an alternative launcher, hold down a button (configured
according your `boot_config.ini`) to boot it.

Compiling
---------

First, clone this repository including the submodules:

    $ git clone --recursive https://github.com/m45t3r/BootCtr.git

Compile a new binary using the included `Makefile` and [devkitPro][dkp].
If devkitPro is correctly installed all you need to do is:

    $ make

And copy the new `boot.3dsx` file to your SD card. Do not forget to copy
`boot_config.ini` too.

Alternatively you can run the command below too (needs `zip` and `git`
installed):

    $ make release

To generate a .zip file including all source code and auxiliary files, ready
for distribution.

License
-------

The original source code ([Homebrew Launcher][hbl]) does not have a proper
license, and since this code is derived from it I can't simple change the
license for something that I would like. The `README.md` from HBL says:

> "Feel free to use code from hbmenu for your own projects, so long as you
give credit to its original authors."

In the section below we give proper credits to everyone that helped creating
this project. Of course, all of my modifications is distributed in a similar
license, so go ahead and fork and do something interesting.

CakeBrah does not seem to have a proper license too. However, [brahma][bhm]
uses a license similar to MIT (see `README-brahma` for details), and
CakeBrah is a derived work so it should use a similar license.

[inih][inh] is distributed under New BSD License, while [libkhax][khx] (not
used directly, however it is a dependency of CakeBrah) uses MIT license.

Credits
-------

* @smealum and everyone that worked in the [Homebrew Launcher][hbl]
* @mashers, for the original source code of [HBL-Emergency][hbe]
* @Cpasjuste, for some ideas from his [CtrBootManager][cbm]
* @benhoyt, for [inih][inh]
* @mid-kid and others, for [CakeBrah][ckb]
* @pbanj from GBAtemp for the [logo and splash screen][lgs]

[hbl]: https://github.com/smealum/3ds_hb_menu
[hbe]: https://gbatemp.net/threads/release-homebrew-emergency-launcher.399394/
[dkp]: http://devkitpro.org/
[cbm]: https://github.com/Cpasjuste/CtrBootManager
[inh]: https://github.com/benhoyt/inih
[ckb]: https://github.com/mid-kid/CakeBrah
[hmh]: https://github.com/yellows8/3ds_homemenuhax
[ofc]: https://gbatemp.net/threads/re-release-bootctr-a-simple-boot-manager-for-3ds.401630/
[bhm]: https://github.com/patois/Brahma
[khx]: https://github.com/Myriachan/libkhax
[bgr]: https://xem.github.io/3DShomebrew/tools/image-to-bin.html
[lgs]: https://gbatemp.net/threads/re-release-bootctr-a-simple-boot-manager-for-3ds.401630/page-14#post-6083200
[i2b]: https://xem.github.io/3DShomebrew/tools/image-to-bin.html
