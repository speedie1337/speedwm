## 11. Configuring the bar

As of 1.8, speedwm has a module system. It is based on the `barmodules` patch for dwm and allows extensive control over the way the speedwm bar functions. This control has its own header, `bar.h`.

`bar.h` contains a somewhat detailed list of all possible options here, but more importantly it contains a `barrules` array which allows extensive control over where each bar module is placed and how it functions. In theory, this means you could put 22 instances of the same, boring tags on one bar, although why would one do that?

Each module can be aligned to any part of the bar (See 'Alignment' for possible values). If, let's say multiple modules both align to the right next to the center split (middle), the first module takes priority.

### Module list

Below is a list of all modules bundled with speedwm. The source code for these modules are all in `bar/` and declared in `bar/items.c` and `bar/items.h`.

- ltsymbol:               Standard, basic layout icon.
- tags:                   Basic tags, without powerlines.
- tags_pwl:               Tags with powerlines.
- systray:                Basic X11 system tray.
- status_basic:           Basic status bar.
- status_basic_es:        Basic status bar.
- status:                 Clickable status bar with color support through status2d.
- status_es:              Clickable status bar with color support through status2d (Extra status).
- status_pwl:             Non-clickable status bar with powerlines. It supports colors by cycling through colorschemes.
- status_pwl_es:          Non-clickable status bar with powerlines. It supports colors by cycling through colorschemes (Extra status).
- title:                  Title, shows all windows, including hidden windows.
- title_basic:            Basic title, shows focused window.

### Monitor

The monitor value allows you to specify which monitor the module should be placed on. In addition to this, you can also choose to only draw the module on the focused monitor.

-1:                       Show the module on all monitors.
0:                        Show on the main monitor (monitor 0).
1:                        Show on monitor #1 (This can be any monitor you want).

### Bar

This value allows you to specify which bar the module is placed on. speedwm supports two (0 and 1) bars. 0 is the main bar, which is by default placed at the top. 1 is the second bar which is only visible if modules actively use it. If the main bar is placed on the top, the second bar is placed on the bottom and vice versa.

0:                        Place the module on the main bar
1:                        Place the module on the extra bar

### Alignment

This value allows you to specify an alignment for the module in question. As previously mentioned, the first module takes priority if multiple modules have the same alignment.
The 'center split' refers to the middle of the bar, and that's where any free space/remainder of the screen ends up for other modules to use if desired.

- bar_align_left:         Force the module to be placed on the left side of the bar if possible.
- bar_align_right:        Force the module to be placed on the right side of the bar if possible.
- bar_align_center:       Force the module to be placed in the center of the bar if possible.
- bar_align_left_left:    Force the module to be placed on the left side of the bar next to the center split.
- bar_align_left_right:   Force the module to be placed on the right side of the bar next to the center split.
- bar_align_left_center:  Force the module to be placed on the center in the middle of the remaining space left of the center split on the left..
- bar_align_right_left:   Force the module to be placed on the left side of the bar next to the center split.
- bar_align_right_right:  Force the module to be placed on the right side of the bar next to the center split.
- bar_align_right_center: Force the module to be placed on the center in the middle of the remaining space left of the center split on the right.
- bar_align_none:         No specific alignment. This will give the module the remaining space.

### Width

'Width' refers to the function to call which returns the width of a module. The syntax below applies to all default modules.

Syntax:  width_<module>
Example: width_tags_pwl

### Draw

'Draw' refers to the function to call which draws the module on the bar. The syntax below applies to all default modules.

Syntax:  draw_<module>
Example: draw_tags_pwl

### Click

'Click' refers to the function to call which checks if you clicked on said module. The syntax below applies to all default modules.

Syntax:  click_<module>
Example: click_tags_pwl

### Module name

The module name really doesn't mean anything at all.

### Example module

With all that said, you should now be able to add a module. In case you weren't able to follow along, here is an example of how you can add powerline tags on the focused monitor.

1. `{ 'A',`
2. `0,`
3. `bar_align_left,`
4. `width_tags_pwl,`
5. `draw_tags_pwl,`
6. `click_tags_pwl,`
7. `"my cool powerline tags" },`

Combined into one line: `{ 'A', 0, bar_align_left, width-tags_pwl, draw_tags_pwl, click_tags_pwl, "my cool powerline tags" },`

Remember that you can have put any module wherever you want. Nothing is stopping you from having your tags on the right. This is what makes bar rules so powerful.

