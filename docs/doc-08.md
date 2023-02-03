## 9. Status bar

speedwm has a status bar module which is enabled by default. It's the (by default) right part of the bar. It supports:

- Pango markup
- Colored glyphs

The regular (non-powerline) bar also supports:

- status2d markup
  - This allows you to color the status bar text at any time.
- statuscmd markup
  - This allows the status bar to have clickable modules.

as well as regular plain text and colored emojis or glyphs. To override this status, you can use the 'speedwm -s "status text"' command. If you prefer, you can also use `xsetroot -name` which does the same thing.

Bundled with speedwm is a fork of dwmblocks. dwmblocks is a dwm status bar that handles this all for you through a block system. This fork has been integrated into the Makefile and is (by default) installed when speedwm is compiled. The status bar can be configured in the status.c and status.h.

By default the status bar runs modules that are also bundled with speedwm (see modules/ directory). To configure these modules, you can edit ~/.config/speedwm/statusrc which should be created when a module runs.

The bundled status bar is autostarted by speedwm if it is installed. If you want to use your own status bar, comment out 'USESTATUS' in toggle.mk and remove /usr/bin/status if speedwm has been installed previously. Then simply start the status bar through autostart.h, ~/.config/speedwm/autostart.sh, .xinitrc or some other means of running a program.
