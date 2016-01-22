BootCtr - A simple boot manager for 3DS
=======================================

Based on 3DS Homebrew Launcher code by [Smealum][hbl]. The original version is
from [mashers][hbe], This version removed some unused code used by Homebrew
Launcher that isn't necessary in a simple bootloader, and added some
interesting new features.

Any question can be done in the [official topic][ofc] in GBAtemp forum.

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

* (Optional, but recommended) Install [HomeMenuHax][hmh] and set up autoboot.
* Modify, according to your needs, the included ``boot_config.ini`` file.
* Copy both ``boot.3dsx`` and ``boot_config.ini`` to the root of your SD card.

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

Compile a new binary using the included ``Makefile`` and [devkitPro][dkp].
If devkitPro is correctly installed all you need to do is:

    $ make

And copy the new ``boot.3dsx`` file to your SD card. Do not forget to copy
``boot_config.ini`` too.

Alternatively you can run the command below too:

    $ make release

To generate a .zip file including all source code and auxiliary files, ready
for distribution.

License
-------

The original source code ([Homebrew Launcher][hbl]) does not have a proper
license, and since this code is derived from it I can't simple change the
license for something that I would like. The ``README.md`` from HBL says:

> "Feel free to use code from hbmenu for your own projects, so long as you
give credit to its original authors."

In the section below we give proper credits to everyone that helped creating
this project. Of course, all of my modifications is distributed in a similar
license, so go ahead and fork and do something interesting.

Credits
-------

* @smealum and everyone that worked in the [Homebrew Launcher][hbl]
* @mashers, for the original source code of [HBL-Emergency][hbe]
* @Cpasjuste, for some ideas from his [CtrBootManager][cbm]
* @benhoyt, for [inih][inh]
* @mid-kid and others, for [CakeBrah][ckb]

[hbl]: https://github.com/smealum/3ds_hb_menu
[hbe]: https://gbatemp.net/threads/release-homebrew-emergency-launcher.399394/
[dkp]: http://devkitpro.org/
[cbm]: https://github.com/Cpasjuste/CtrBootManager
[inh]: https://github.com/benhoyt/inih
[ckb]: https://github.com/mid-kid/CakeBrah
[hmh]: https://github.com/yellows8/3ds_homemenuhax
[ofc]: https://gbatemp.net/threads/re-release-bootctr-a-simple-boot-manager-for-3ds.401630/
