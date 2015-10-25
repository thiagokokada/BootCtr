Homebrew Launcher emergency boot
================================

By mashers at GBATemp, modified by m45t3r
-----------------------------------------

Based on 3DS Homebrew Launcher code by [Smealum][1]. The original version is
from [mashers][2], I simply removed some unused code used by Homebrew Launcher
that isn't necessary in a simple bootloader.

Installation:
-------------

* Rename the ``boot.3dsx`` file already on your SD card to ``boot_default.3dsx``.
* Place some other launcher or FTP client in the root of your SD card and call
it ``boot_1.3dsx``.
* Rename ``HBL-Emergency.3dsx`` to ``boot.3dsx``, copy this file and
``boot_config.ini`` to the root of your SD card.
* Change ``boot_config.ini`` according to your necessities.

Usage
-----

* Boot up your homebrew hax as usual.
* If you do nothing, then ``boot_default.3dsx`` will be launched (your normal
* launcher).
* If something goes wrong with your launcher, hold down the right shoulder (R)
button while launching your homebrew hax. This will cause ``boot_1.3dsx`` to be
booted instead.
* You may change the behavior using the included ``boot_config.ini`` file,
allowing boot of up-to 12+1 homebrews (12 using keys plus 1 default). In N3DS,
boot up-to 18+1.

Compiling
---------

First, clone this repository including the submodules:

    $ git clone --recursive https://github.com/m45t3r/HBL-Emergency.git

Compile a new binary using the included ``Makefile`` and [devkitPro][3].
If devkitPro is correctly installed all you need to do is:

    $ make

And copy/rename the new ``HBL-Emergency.3dsx`` file to your SD card. Do not
forget to copy ``boot_config.ini`` too.

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
