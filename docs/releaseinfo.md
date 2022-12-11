### **speedwm 1.8**

Massive update this time, which is also a downgrade in a few ways.. ..for the time being. I plan on fixing these shortcomings in 1.9.

Reason for the massive version bump is simple; massive changes deserve a massive bump. (104+ commits since 1.5.1, 2723+ additions, 2301+ deletions)
I'm sure I'm going to miss a few things in this changelog, so see commit history for more information.

### Changelog
- Implement modified barmodules (don't confuse this with bartoggle or the status modules) which allows you to move each module of the bar around. This is also going to solve **all** mouse/bar related problems with ease. [Source](https://github.com/bakkeby/patches)
- Implement powerlines for the status bar.
- Rewrite bartoggle to work well with barmodules.
- Implement new centerwindow patch, which centers the focused window with a keybind.
- Clean up warnings GCC will spit out. There are still a few but we mainly use tcc so it doesn't really matter.
- Change default browser to Chromium.
- Add credits (see man page/readme).
- Disable and remove mouse previews, they're honestly more annoying and buggy than useful. Of course you can still preview tags using your keyboard.
- Update preview image
- Add option to center the title.
- Add window mapping/unmapping effects for users with a compositor. Picom can be used to change/manipulate these.
- Update inaccurate speedwm-help information
- Remove huge amount of comments that are unnecessary, why not redirect to the man page instead?
- Update status modules, you can now have different icons depending on volume/battery level.
- Remove old toggle that does absolutely nothing.
- Various other bugfixes (see commit history)

### Installation
Nothing has changed with the installation since last release, simply download and unpack the tarball, and `make clean install` as root.

- If you are on Gentoo, you can install both x11-wm/speedwm, x11-wm/libspeedwm and x11-wm/speedwm-extras by adding my overlay.
- I plan on writing an Arch AUR package eventually, but please feel free to do my job ;)
