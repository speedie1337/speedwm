## 2. Installation

In order to install this build of speedwm, all dependencies must be installed.
You can see 'Dependencies' for a list of all dependencies required and optionally recommended to use speedwm.

  - git clone https://git.speedie.gq/speedwm
  - cd speedwm
  - make clean install
    - If any warnings/errors show up, fix it by installing the missing dependency.
  - If a .xinitrc is used, add `speedwm` to the end of the file. If you're using .xinit you can also just `startx /usr/bin/speedwm`.
    - If you do not have a .xinitrc, you can add autostart commands to the file ~/.config/speedwm/autostart.sh.
  - If a display manager is used, make sure it supports .desktop entries.
  - NOTE: `ly` is known to have issues with dwm and speedwm.

If you see a bar or at least a cursor, your X server is likely working.
If you do **not** see a cursor or a bar, and no keys seem to do anything, your X server is most likely not configured properly.
If you get sent back to the TTY, your X server is most likely not configured properly. In this case, see your distro's wiki page on how to set up X11.

Note that speedwm is not and will **never** be compatible with Wayland. I have no interest in ever supporting or developing for it. Please don't create any issues regarding Wayland support.

If you're having any issues on operating systems with the BSD kernel, or something like NixOS, please file a bug report here on Codeberg, or by emailing me.

If you have any questions, see the official [speedwm wiki](https://speedwm.speedie.gq).

