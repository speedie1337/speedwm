## 5. Dependencies

### Required

These are absolutely necessary, speedwm will NOT compile without them
- libxft
  - Used for rendering text
- pango
  - Used to markup text
- libXinerama
  - Used for multi-monitor support.
  - Can be disabled through editing toggle.mk if you're not interested in multiple monitors.
- imlib2
  - Used for tag previews, window icons.
  - Can be disabled through editing toggle.mk and toggle.h if you don't want these features.
- yajl
  - Used to interact with speedwm through a sock.
  - Required for the IPC patch. If the IPC patch is disabled through toggle.mk and toggle.h, you do not need this.
- tcc
  - Very minimal C compiler that speedwm uses to greatly speed up compile times. If you do not want this dependency, edit host.mk and set CC to 'cc' (or what C99 compiler you prefer).

### Features

These are necessary for certain features and keybinds.
If you want to use an alternative, change it in options.h and/or keybinds.h and mouse.h.

- [spmenu](https://speedie.gq/projects/spmenu.php)
  - As of speedwm 1.9, speedwm now expects spmenu (fork of dmenu) to be on the system instead of dmenu. While you can revert this change I cannot provide support due to missing arguments.
- xrdb
  - xrdb is used for external speedwm configuration (ie. after compiling). It is **not** mandatory if you don't need this.

### Software

This build of speedwm comes with keybinds for software.
You can add, change and remove keybinds by editing `keybinds.h` and `mouse.h` and running `make clean install`.

- speedwm-extras
  - Metapackage containing extra scripts that speedwm makes use of. (https://codeberg.org/speedie/speedwm-extras)
- libspeedwm
  - Dependency for speedwm-extras allowing you to perform actions externally.
- st - Default terminal
- Chromium - Default web browser

And everything under `Features`.

