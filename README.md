BootCtr - A simple boot manager for 3DS
=======================================

Based on 3DS Homebrew Launcher code by [Smealum][1]. The original version is
from [mashers][2], This version removed some unused code used by Homebrew
Launcher that isn't necessary in a simple bootloader, and added some
interesting new features.

Features
--------

* Supports both ``.3dsx`` files and binary payloads (for example,
``/rxTools/sys/code.bin``, ``/Cakes.dat``, etc.).
* Supports advanced features in ``.3dsx`` files, like network support.
* Boot delay, to increase the boot rate chance of CFWs like rxTools and
CakesFW.
* Allows up-to **12+1** (12 using keys plus 1 default) homebrews in O3DS,
up-to **18+1** homebrews in N3DS.

Installation
------------

* (Optional, but recommended) Install [HomeMenuHax][7] and set up autoboot.
* Modify, according to your needs, the included ``boot_config.ini`` file.
* Rename ``BootCtr.3dsx`` to ``boot.3dsx``, copy both ``boot.3dsx`` and
``boot_config.ini`` to the root of your SD card.

Usage
-----

* Boot up your homebrew hax as usual.
* If you do nothing, then ``boot_default.3dsx`` (or anything that you setup in
[DEFAULT] section) will be launched.
* If you want to load an alternative launcher, hold down a button (configured
according your ``boot_config.ini``) to boot it.

Compiling
---------

First, clone this repository including the submodules:

    $ git clone --recursive https://github.com/m45t3r/BootCtr.git

Compile a new binary using the included ``Makefile`` and [devkitPro][3].
If devkitPro is correctly installed all you need to do is:

    $ make

And copy/rename the new ``BootCtr.3dsx`` file to your SD card. Do not forget
to copy ``boot_config.ini`` too.

License
-------

The original source code ([Homebrew Launcher][1]) does not have a proper
license, and since this code is derived from it I can't simple change the
license for something that I would like. The ``README.md`` from HBL says:

> "Feel free to use code from hbmenu for your own projects, so long as you
give credit to its original authors."

In the section below we give proper credits to everyone that helped creating
this project. Of course, all of my modifications is distributed in a similar
license, so go ahead and fork and do something interesting.

Credits
-------

* @smealum and everyone that worked in the [Homebrew Launcher][1]
* @mashers, for the original source code of [HBL-Emergency][2]
* @Cpasjuste, for some ideas from his [CtrBootManager][4]
* @benhoyt, for [inih][5]
* @mid-kid and others, for [CakeBrah][6]

[1]: https://github.com/smealum/3ds_hb_menu
[2]: https://gbatemp.net/threads/release-homebrew-emergency-launcher.399394/
[3]: http://devkitpro.org/
[4]: https://github.com/Cpasjuste/CtrBootManager
[5]: https://github.com/benhoyt/inih
[6]: https://github.com/mid-kid/CakeBrah
[7]: https://github.com/yellows8/3ds_homemenuhax
