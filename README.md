# speedwm
![image](/docs/preview.png)

## Table of contents

0. Table of contents
1. What is speedwm?
2. Installation
3. Layouts
4. Default keybinds
5. Dependencies
6. Important
7. Configuration and .Xresources
8. Signals
9. Status bar
10. Additional note on autostart
11. Configuring the bar
12. Keybinds
13. Credits

## 1. What is speedwm?

speedwm is a window manager forked from dwm or dynamic window manager. It manages the user's open windows and tiles them according to a set layout (dynamic).

Just like dwm, speedwm also tries to be minimal but also has functionality and aesthetics as a goal. Tiling window managers (unlike floating window managers that you may be used to) tile windows based on a set layout making them easy to get productive on. They also encourage the user to use their keyboard instead of the mouse so that the user doesn't have to move his hands much but there are also mouse binds and more can be added by the user if desired.

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

## 3. Layouts

speedwm comes with the following layouts:

  - Tile
  - Monocle
  - Grid
  - Deck
  - Centered Master
  - Tatami
  - Spiral
  - Dwindle
  - Bottom Stack
  - Horizontal Bottom Stack
  - Horizonal Grid
  - Dynamic Grid
  - Custom
  - Empty

They can be switched between using a little menu (See Keybinds for more information) or by right clicking the Layout indicator.
The more commonly used layouts can be switched between using a quick keybind.

## 4. Default keybinds

This is a full list of keybinds.
Please let me know if any keybinds are missing as these have been manually added.

### Keybinds for regular applications

- Super+Shift+Enter         - Opens the defined terminal (default is st)
- Super+Shift+Colon         - Opens an spmenu prompt
- Super+Shift+w             - Opens the defined web browser

### These keybinds are for navigating speedwm

- Super+f                   - Fullscreen the selected window
- Super+b                   - Show/hide the speedwm bar
- Super+s                   - Show/hide the systray
- Super+j/k                 - Move focus between visible windows
- Super+m                   - Moves focus to the Master window (or back if Master is already focused)
- Super+a/d                 - Increase/decrease size of each window
- Super+w/e                 - Resize the window keeping the aspect ratio of it.
- Super+o                   - Hide a window
- Super+Minus               - Show the scratchpad
- Super+Equal               - Remove the scratchpad
- Super+Enter               - Switch order of windows
- Super+Tab                 - Switch between windows on the same tag. (Microsoft Windows-like Alt+Tab)
- Super+v                   - Center the focused window.
- Super+0                   - Reset mfact
- Super+r                   - Reset number of masters
- Super+y                   - Make the current selected window sticky
- Super+d                   - Focus the previous monitor.
- Super+/                   - Focus the next monitor.
- Super+.                   - Increase bar padding by 1
- Super+,                   - Decrease bar padding by 1
- Super+\                   - Toggle mark on a client.
- Super+]                   - Swap focused client with the marked client.
- Super+[                   - Swap focus with the marked client.
- Super+1                   - Move to tag 1
- Super+2                   - Move to tag 2
- Super+3                   - Move to tag 3
- Super+4                   - Move to tag 4
- Super+5                   - Move to tag 5
- Super+6                   - Move to tag 6
- Super+7                   - Move to tag 7
- Super+8                   - Move to tag 8
- Super+9                   - Move to tag 9
- Super+Shift+1             - Preview tag 1
- Super+Shift+2             - Preview tag 2
- Super+Shift+3             - Preview tag 3
- Super+Shift+4             - Preview tag 4
- Super+Shift+5             - Preview tag 5
- Super+Shift+6             - Preview tag 6
- Super+Shift+7             - Preview tag 7
- Super+Shift+8             - Preview tag 8
- Super+Shift+9             - Preview tag 9
- Super+Shift+q             - Close the current window
- Super+Shift+\             - Kill all clients except focused.
- Super+Shift+.             - Increase bar padding and gaps by 1
- Super+Shift+,             - Decrease bar padding and gaps by 1
- Super+Shift+/             - Reset bar padding and gaps
- Super+Shift+h/j/k/l       - Rotates a stack.
- Super+Shift+Arrow         - Resizes a window in floating mode
- Super+Shift+Equal         - Toggle scratchpads
- Super+Shift+Minus         - Hide the scratchpad
- Super+Shift+Space         - Toggle floating windows
- Super+Control+1           - Combine the current tag with tag 1
- Super+Control+2           - Combine the current tag with tag 2
- Super+Control+3           - Combine the current tag with tag 3
- Super+Control+4           - Combine the current tag with tag 4
- Super+Control+5           - Combine the current tag with tag 5
- Super+Control+6           - Combine the current tag with tag 6
- Super+Control+7           - Combine the current tag with tag 7
- Super+Control+8           - Combine the current tag with tag 8
- Super+Control+9           - Combine the current tag with tag 9
- Super+Control+0           - Tag all tags at once.
- Super+Control+Enter       - Mirror the layout
- Super+Control+i           - Increase stackcount by 1
- Super+Control+u           - Decrease stack count by 1
- Super+Control+z/x         - Increase/decrease gaps between windows by 5
- Super+Control+j/k         - Move focus between hidden windows (Can then 'Show')
- Super+Control+Arrow       - Moves a window to any corner of your screen (Arrow key)
- Super+Control+Tab         - Open a spmenu prompt asking the user what layout to switch to
- Super+Control+o           - Show a hidden focused window
- Super+Control+a/d         - Move to the next/previous tag
- Super+Control+Shift+o     - Show all hidden windows
- Super+Control+Shift+p     - Hide all windows
- Super+Control+Shift+z/c   - Move to the next/previous tag skipping any without windows open
- Super+Control+Shift+a/d   - Move between available layouts
- Super+Control+Shift+1     - Move the focused window to tag 1
- Super+Control+Shift+2     - Move the focused window to tag 2
- Super+Control+Shift+3     - Move the focused window to tag 3
- Super+Control+Shift+4     - Move the focused window to tag 4
- Super+Control+Shift+5     - Move the focused window to tag 5
- Super+Control+Shift+6     - Move the focused window to tag 6
- Super+Control+Shift+7     - Move the focused window to tag 7
- Super+Control+Shift+8     - Move the focused window to tag 8
- Super+Control+Shift+9     - Move the focused window to tag 9
- Super+Control+Shift+Arrow - Resize the window to the screen size.
- Super+Control+Shift+j/k   - Change window size vertically (cfact)
- Super+Control+Shift+Equal - Increase bar height by 1
- Super+Control+Shift+Minus - Decrease bar height by 1

### Chained keybinds

- Super+t & r               - Reorganize tags and move windows
- Super+t & t               - Toggle tag area in the bar
- Super+t & y               - Toggle empty tag area in the bar
- Super+t & w               - Toggle title area in the bar
- Super+t & u               - Toggle unselected title area in the bar
- Super+t & s               - Toggle status area in the bar
- Super+t & l               - Toggle layout area in the bar
- Super+t & o               - Toggle inactive fade
- Super+t & r               - Reset all bar modules
- Super+t & s               - Make the current selected window sticky
- Super+p & j/k             - Increase/decrease bar padding by 1
- Super+p & u/d             - Increase/decrease bar padding by 5
- Super+p & r               - Reset bar padding to default
- Super+p & t               - Toggle bar padding on/off
- Super+, & r               - Reset powerline options
- Super+l & q               - Rotate forward in the layout axis
- Super+l & w               - Rotate forward in the master axis
- Super+l & e               - Rotate forward in the stack axis
- Super+l & r               - Rotate forward in the secondary stack axis
- Super+l & q               - Rotate backwards in the layout axis
- Super+l & w               - Rotate backwards in the master axis
- Super+l & e               - Rotate backwards in the stack axis
- Super+l & r               - Rotate backwards in the secondary stack axis
- Super+l & 1               - Set layout to layout 0
- Super+l & 2               - Set layout to layout 1
- Super+l & 3               - Set layout to layout 2
- Super+l & 4               - Set layout to layout 3
- Super+l & 5               - Set layout to layout 4
- Super+l & 6               - Set layout to layout 5
- Super+l & 7               - Set layout to layout 6
- Super+l & 8               - Set layout to layout 7
- Super+l & 9               - Set layout to layout 8
- Super+l & 0               - Set layout to layout 9
- Super+Shift+g & i         - Decrease inner gaps by 1
- Super+Shift+g & o         - Decrease outer gaps by 1

### These will only work if your keyboard has special multimedia buttons.

- Browser button            - Opens your defined web browser

### These binds can be activated using your mouse

- Tag <num> (Left click)                  - Switch to tag <num>
- Tag (Scrolling up/down)                 - Switch to the next/previous tag
- Layout indicator (Left click)           - Switch to the next layout
- Layout indicator (Right click)          - Switch to the previous layout
- Layout indicator (Middle click)         - Open an spmenu list of all layouts (requires speedwm-extras)
- Layout indicator (Scrolling up/down)    - Switch to the next/previous layout
- Window title (Left click)               - Show/hide the window
- Window title (Scrolling up/down)        - Move windows around in the stack.
- Focused window (Super+Alt+Left click)   - Move the focused window around
- Focused window (Super+Alt+Middle click) - Make the focused window floating
- Focused window title (Middle click)     - Rotate stack
- Dragging (Super+Right click)            - Increase/decrease size of each window (mfact)
- Dragging (SuperControl+Right click)     - Increase/decrease cfact

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

## 6. Important

If you're used to dwm, speedwm might be a little unfamiliar to you at first. This is because speedwm doesn't use config.h (or config.def.h).

Instead, config.h is split into different parts to make it easier to edit. Instead of editing config.h you'll want to edit:

- autostart.h for starting stuff right before speedwm (For example xclip, pywal, etc.)
- bar.h for configuring bar rules.
- options.h for changing colors and applications to use with keybinds.
- signal.h for adding fake signals
- colors.h for changing alpha options and color options, most users won't need to edit it.
- xresources.h for adding/removing .Xresources options
- rules.h for adding/removing rules
- keybinds.h for adding/removing keybinds.
- mouse.h for adding/removing mouse binds.
- status.h for adding/removing status modules and aadding/removing statuscmd clicks.
- ipc.h for adding/removing IPC commands. (If support is compiled in)
- query.h for adding/removing queries.
- toggle.h for adding/removing features from getting compiled in.

After you've edited one of the files, you need to run 'make clean install' to reinstall speedwm.
Remember that you can change colors through your .Xresources file (see .Xresources and Pywal) meaning you do not need to recompile speedwm.

You can edit the status bar simply by editing its modules (modules_...) and running 'make clean install'.
You can also configure the modules by editing '~/.config/speedwm/statusrc' which is configured entirely in shell script syntax.

## 7. Configuration and .Xresources

speedwm has .Xresources support thanks to the .Xresources patch. It also has pywal support (tool which grabs colors based on your wallpaper).

To configure speedwm, you may /usr/share/speedwm/example.Xresources to either ~/.speedwmrc or ~/.config/speedwm/speedwmrc. Alternatively, you can also copy the values to your .Xresources file.

.speedwmrc or speedwm/speedwmrc will be loaded when speedwm restarts. If you want to load a .Xresources file you'll need to add that to autostart.sh.

Colors do not reload automatically though, you must reload them manually. Use a signal for this (See list of signals above) or simply 'libspeedwm --perform core_wm_reload'. This won't restart speedwm, but it will reload colors.

To use .Xresources, make sure 'xrdb' is installed. If a .xinitrc is used, add 'xrdb /path/to/.Xresources/file' before 'speedwm'. If a .Xresources file is not used, add it to ~/.config/speedwm/autostart.sh instead.

If you don't want to define the options manually, there is an example .Xresources file containing speedwm default settings in docs/example.Xresources. You can copy this somewhere or you can simply '< docs/example.Xresources >> ~/.Xresources' to append the speedwm options to your .Xresources file.

The magic of .Xresources is that it is a universal configuration file. While you *can* use the col.value values, you can also use traditional colors 0 through 15 as well. These colors take priority over regular speedwm colors. This is so that speedwm is compatible with Pywal and more general/mainstream .Xresources configurations.

Below is a list of all .Xresources values you can define.

- speedwm.bar.alpha:                     1 
- speedwm.bar.height:                    3 
- speedwm.bar.position:                  1 
- speedwm.bar.paddingoh:                 0 
- speedwm.bar.paddingov:                 0 
- speedwm.bar.paddingih:                 0 
- speedwm.bar.paddingiv:                 0 
- speedwm.bar.hide:                      0 
- speedwm.bar.hide.tags:                 0 
- speedwm.bar.hide.emptytags:            1 
- speedwm.bar.hide.floating:             0 
- speedwm.bar.hide.layout:               0 
- speedwm.bar.hide.sticky:               0 
- speedwm.bar.hide.status:               0 
- speedwm.bar.hide.systray:              1 
- speedwm.bar.hide.unselected.title:     0 
- speedwm.bar.hide.title:                0 
- speedwm.bar.hide.icon:                 0 
- speedwm.bar.hide.clientindicator:      0 
- speedwm.inset.x:                       0 
- speedwm.inset.y:                       0 
- speedwm.inset.w:                       0 
- speedwm.inset.h:                       0 
- speedwm.bar.titleposition:             1 
- speedwm.border.size:                   1 
- speedwm.client.map:                    1 
- speedwm.client.allowurgent:            1 
- speedwm.client.automove:               1 
- speedwm.client.autofocus:              1 
- speedwm.client.autoresize:             1 
- speedwm.client.decorhints:             1 
- speedwm.client.hide.border:            0 
- speedwm.client.hide.unselected.border: 1 
- speedwm.client.fade.inactive:          1 
- speedwm.client.fade.windows:           1 
- speedwm.client.floatscratchpad:        0 
- speedwm.client.savefloat:              1 
- speedwm.client.swallow:                1 
- speedwm.client.swallowfloating:        1 
- speedwm.client.wmclass:                1 
- speedwm.fonts.font:                    NotoSans Regular 9
- speedwm.col.layout:                    #99b3ff
- speedwm.col.layouttext:                #000000
- speedwm.col.status0:                   #131210
- speedwm.col.status1:                   #bf616a
- speedwm.col.status2:                   #A16F9D
- speedwm.col.status3:                   #68ABAA
- speedwm.col.status4:                   #A89F93
- speedwm.col.status5:                   #D3A99B
- speedwm.col.status6:                   #AFC9AC
- speedwm.col.status7:                   #eae1cb
- speedwm.col.status8:                   #a39d8e
- speedwm.col.status9:                   #6D5E8E
- speedwm.col.status10:                  #a16f9d
- speedwm.col.status11:                  #d3a99b
- speedwm.col.status12:                  #afc9ac
- speedwm.col.status13:                  #eae1cb
- speedwm.col.status14:                  #6d5e8e
- speedwm.col.status15:                  #ffffff
- speedwm.col.powerline0:                #131210
- speedwm.col.powerline1:                #bf616a
- speedwm.col.powerline2:                #A16F9D
- speedwm.col.powerline3:                #68ABAA
- speedwm.col.powerline4:                #A89F93
- speedwm.col.powerline5:                #D3A99B
- speedwm.col.powerline6:                #AFC9AC
- speedwm.col.powerline7:                #eae1cb
- speedwm.col.powerline0_text:           #eeeeee
- speedwm.col.powerline1_text:           #131210
- speedwm.col.powerline2_text:           #131210
- speedwm.col.powerline3_text:           #131210
- speedwm.col.powerline4_text:           #131210
- speedwm.col.powerline5_text:           #131210
- speedwm.col.powerline6_text:           #131210
- speedwm.col.powerline7_text:           #131210
- speedwm.col.systray:                   #222222
- speedwm.col.tag1:                      #99b3ff
- speedwm.col.tag2:                      #99b3ff
- speedwm.col.tag3:                      #99b3ff
- speedwm.col.tag4:                      #99b3ff
- speedwm.col.tag5:                      #99b3ff
- speedwm.col.tag6:                      #99b3ff
- speedwm.col.tag7:                      #99b3ff
- speedwm.col.tag8:                      #99b3ff
- speedwm.col.tag9:                      #99b3ff
- speedwm.col.tag1.text:                 #eeeeee
- speedwm.col.tag2.text:                 #eeeeee
- speedwm.col.tag3.text:                 #eeeeee
- speedwm.col.tag4.text:                 #eeeeee
- speedwm.col.tag5.text:                 #eeeeee
- speedwm.col.tag6.text:                 #eeeeee
- speedwm.col.tag7.text:                 #eeeeee
- speedwm.col.tag8.text:                 #eeeeee
- speedwm.col.tag9.text:                 #eeeeee
- speedwm.col.tagurgent:                 #f0e68c
- speedwm.col.background:                #222222
- speedwm.col.textnorm:                  #bbbbbb
- speedwm.col.textsel:                   #222222
- speedwm.col.titlenorm:                 #222222
- speedwm.col.titlesel:                  #99b3ff
- speedwm.col.titlehid:                  #222222
- speedwm.col.windowbordernorm:          #000000
- speedwm.col.windowbordersel:           #eeeeee
- speedwm.col.windowborderurg:           #f0e68c
- speedwm.text.tag1.empty:               
- speedwm.text.tag2.empty:               
- speedwm.text.tag3.empty:               
- speedwm.text.tag4.empty:               
- speedwm.text.tag5.empty:               
- speedwm.text.tag6.empty:               
- speedwm.text.tag7.empty:               
- speedwm.text.tag8.empty:               
- speedwm.text.tag9.empty:               
- speedwm.text.tag1.used:                
- speedwm.text.tag2.used:                
- speedwm.text.tag3.used:                
- speedwm.text.tag4.used:                
- speedwm.text.tag5.used:                
- speedwm.text.tag6.used:                
- speedwm.text.tag7.used:                
- speedwm.text.tag8.used:                
- speedwm.text.tag9.used:                
- speedwm.text.layout1:                  
- speedwm.text.layout2:                  
- speedwm.text.layout3:                  
- speedwm.text.layout4:                  
- speedwm.text.layout5:                  
- speedwm.text.layout6:                  
- speedwm.text.layout7:                  
- speedwm.text.layout8:                  
- speedwm.text.layout9:                  
- speedwm.text.layout10:                 
- speedwm.text.layout11:                 
- speedwm.text.layout12:                 
- speedwm.text.layout13:                 
- speedwm.text.layout14:                 
- speedwm.text.layout15:                 
- speedwm.color.hiddentitle:             1 
- speedwm.color.layout:                  0 
- speedwm.color.selectedtitle:           0 
- speedwm.bar.powerline.tag.shape:       0 
- speedwm.fullscreen.hidebar:            1 
- speedwm.fullscreen.lockfullscreen:     1 
- speedwm.fullscreen.movefullscreenmon:  0 
- speedwm.gaps.enable:                   1 
- speedwm.gaps.sizeih:                   10 
- speedwm.gaps.sizeiv:                   10 
- speedwm.gaps.sizeoh:                   10 
- speedwm.gaps.sizeov:                   10 
- speedwm.gaps.smartgaps:                0 
- speedwm.gaps.smartgapsize:             0 
- speedwm.icon.size:                     15 
- speedwm.icon.spacing:                  5 
- speedwm.layout.deck.count:             0 
- speedwm.layout.deck.format:            [%d]
- speedwm.layout.monocle.clientcount:    0 
- speedwm.layout.monocle.count:          0 
- speedwm.layout.monocle.format:         [%d/%d]
- speedwm.mfact:                         0.50 
- speedwm.mfact.lowest:                  0.05 
- speedwm.mouse.clicktofocus:            0 
- speedwm.mouse.mfact: 		           1 
- speedwm.mouse.cfact:                   1 
- speedwm.rule.refresh:                  0 
- speedwm.stack.attachdirection:         3 
- speedwm.stack.centerfloating:          1 
- speedwm.stack.i3mcount:                0 
- speedwm.stack.mastercount:             1 
- speedwm.stack.snap:                    20 
- speedwm.status.separator:              ;
- speedwm.status.defaultstatus:
- speedwm.switcher.maxheight:            200 
- speedwm.switcher.maxwidth:             600 
- speedwm.switcher.menupositionv:        1 
- speedwm.switcher.menupositionh:        1 
- speedwm.systray.padding:               2 
- speedwm.systray.pinning:               0 
- speedwm.systray.position:              0 
- speedwm.tag.pertag:                    1 
- speedwm.tag.preview:                   1 
- speedwm.tag.preview.bar:               1 
- speedwm.tag.preview.paddingh:          0 
- speedwm.tag.preview.paddingv:          0 
- speedwm.tag.preview.scale:             4 
- speedwm.tag.resetgaps:                 0 
- speedwm.tag.resetlayout:               0 
- speedwm.tag.resetmfact:                0 
- speedwm.tag.start:                     1 
- speedwm.tag.underline:                 0 
- speedwm.tag.underlineall:              0 
- speedwm.tag.underlinepad:              5 
- speedwm.tag.underlinestroke:           2 
- speedwm.tag.underlinevoffset:          0 
- speedwm.tag.urgentwindows:             1 
- speedwm.tiling.resizehints:            0 
- speedwm.run.shell:                     /bin/sh
- speedwm.status.hideemptymodule:        1
- speedwm.status.leftpadding:
- speedwm.status.rightpadding:
- speedwm.status.separator:

## 8. Signals

Thanks to the 'fsignal' patch available on suckless.org's website, we can easily write shell scripts to interact with dwm and therefore speedwm.
I made some changes to this patch, because it has some.. questionable behaviour in my opinion.

To use signals, you can use libspeedwm. Previously, speedwm-utils (part of speedwm-extras) would be used but that now depends on libspeedwm anyway. Using libspeedwm directly is the easiest option.

If you do not have speedwm-extras or libspeedwm, you can use the speedwm binary itself. The syntax is speedwm -s "#cmd:<signum>"
This option is not as solid though as signums can and will likely be moved around breaking your scripts. Therefore I highly recommend you use libspeedwm when writing scripts.

Below is a list of all signums and what they do.

- 1  - Switch to the Tiling layout
- 2  - Switch to the Floating layout
- 3  - Switch to the Monocle layout
- 4  - Switch to the Grid layout
- 5  - Switch to the Deck layout
- 6  - Switch to the Centered Master layout
- 7  - Switch to the Tatami layout
- 8  - Switch to the Fibonacci Spiral layout
- 9  - Switch to the Fibonacci Dwindle layout
- 10 - Switch to the Bottom Stack Vertical layout
- 11 - Switch to the Bottom Stack Horizontal layout
- 12 - Switch to the Horizontal Grid layout
- 13 - Switch to the Dynamic Grid layout
- 14 - Switch to the Custom layout
- 15 - Custom layout options
- 16 - Switch to the next layout
- 17 - Switch to the previous layout
- 18 - Increase mfact by 0.05
- 19 - Decrease mfact by 0.05
- 20 - Toggle sticky
- 21 - Toggle the bar
- 22 - Toggle fullscreen
- 23 - Toggle floating
- 24 - Swap the current window with the next
- 25 - Reorganize tags (Reorder them)
- 26 - Shutdown speedwm
- 27 - Restart speedwm
- 28 - Show the focused window
- 29 - Hide the focused window
- 30 - Kill the focused window
- 31 - Rotate the stack up
- 32 - Rotate the stack down
- 33 - Increase number of windows in the master stack
- 34 - Decrease number of windows in the master stack
- 35 - Focus the Master window
- 36 - Switch focus between windows +1
- 37 - Switch focus between windows -1
- 38 - Switch focus between hidden windows +1
- 39 - Switch focus between hidden windows -1
- 40 - Toggle opacity for windows
- 41 - Increase cfact by 0.05
- 42 - Decrease cfact by 0.05
- 43 - Switch to the previous tag
- 44 - Switch to the next tag
- 45 - Reset gaps to the default size
- 46 - Toggle gaps
- 47 - Increase gaps by 1
- 48 - Decrease gaps by 1
- 49 - Increase inner gaps by 1
- 50 - Decrease inner gaps by 1
- 51 - Increase outer gaps by 1
- 52 - Decrease outer gaps by 1
- 53 - Kill all windows except focused
- 54 - Focus the next monitor
- 55 - Focus the previous monitor
- 56 - Show the scratchpad
- 57 - Hide the scratchpad
- 58 - Remove the scratchpad
- 59 - Reset layout/mfact
- 60 - Reset mastercount
- 61 - Toggle systray
- 62 - Hide all windows
- 63 - Show all windows
- 64 - Reset mfact
- 65 - Reload .Xresources on the fly
- 66 - Switch to the previous tag, skipping empty tags
- 67 - Switch to the next tag, skipping empty tags
- 68 - Toggle the tag area in the bar
- 69 - Toggle the empty tags in the bar
- 70 - Unused
- 71 - Unused
- 72 - Unused
- 73 - Unused
- 74 - Unused
- 75 - Unused
- 76 - Unused
- 77 - Toggle the title area in the bar
- 78 - Toggle the unselected title area in the bar
- 79 - Toggle the layout area in the bar
- 80 - Toggle the status area in the bar
- 81 - Toggle the floating indicator area in the bar
- 82 - Toggle the sticky indicator area in the bar
- 83 - Toggle the icon in the window title
- 84 - Unused
- 85 - Unused
- 86 - Unused
- 87 - Increase bar height by 1
- 88 - Decrease bar height by 1
- 89 - Reset bar height
- 90 - Increase vertical barpadding by 1
- 91 - Decrease vertical barpadding by 1
- 92 - Increase horizontal barpadding by 1
- 93 - Decrease horizontal barpadding by 1
- 94 - Increase vertical and horizontal barpadding by 1
- 95 - Decrease vertical and horizontal barpadding by 1
- 96 - Toggle vertical barpadding
- 97 - Toggle horizontal barpadding
- 98 - Toggle vertical and horizontal barpadding
- 99 - Reset vertical barpadding
- 100 - Reset horizontal barpadding
- 101 - Reset vertical and horizontal barpadding
- 102 - Increase stack count by 1
- 103 - Decrease stack count by 1
- 104 - Rotate forward in the layout axis
- 105 - Rotate forward in the master axis
- 106 - Rotate forward in the stack axis
- 107 - Rotate forward in the secondary stack axis
- 108 - Rotate backwards in the layout axis
- 109 - Rotate backwards in the master axis
- 110 - Rotate backwards in the stack axis
- 111 - Rotate backwards in the secondary stack axis
- 112 - Mirror the layout
- 113 - Enter an empty layout where all windows are hidden
- 114 - Increase barpadding and gaps by 1
- 115 - Decrease barpadding and gaps by 1
- 116 - Toggle mark on a window.
- 117 - Swap focus with the marked window.
- 118 - Swap the focused window with the marked window.
- 119 - Center the focused window
- 120 - Toggle border for the focused window
- 121 - Reset bar padding and gaps

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
## 10. Additional note on autostart

If you wish to add autostart entries without recompiling, consider using $HOME/.config/speedwm/autostart.sh. This is a path added to autostart.h and you can fill it with anything you want.
Make sure your user has permission to execute the script.

Note that this script or any other commands in autostart.h will **not** run when speedwm is restarted, only when speedwm is first started.

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

## 12. Keybinds

Like the bar array mentioned previously, there is a `Key keys` array in `keybinds.h` which contains all keybinds speedwm will recognize. While this isn't the only way to add keybinds in speedwm, it does not require any additional software to be installed.

### Event

The event value allow you to specify when a keybind is executed.

- KeyPress:               Activate immediately on key press.
- KeyRelease:             Activate immediately on key release.

### Modifier

There are a lot of different modifiers, this list is only going to list the ones declared in speedwm.

- CONTROL                 Left Control (Ctrl) key.
- SHIFT                   Left Shift key.
- ALT                     Left Alt key.
- SUPER                   Left Super (Windows) key.
- SUPERR                  Right Super (Windows) key.

In keybinds.h, MODIFIER1 and MODIFIER2 are also defined. MODIFIER1 is defined in order to make modifying keybinds easier. MODIFIER1 is by default defined as SUPER or Mod4Mask. For instance, if you want to use MODIFIER1 as your modifier, `MODIFIER1` would be the right value here.

Do note that you can may use multiple modifiers. Do this by adding two or more modifiers separated by a pipe (MODIFIER1|SHIFT)

### Chain key

This value *is* mandatory, but it does not have to be used. speedwm has supported chained keybinds since 0.4. Chained keybinds allow more keybinds as like the name implies, the user has to press multiple keys to activate it. You can think of it as in Vim where there's different modes.

The first key that needs to be pressed to activate a chain needs to be the key specified here. In order to see a list of possible keys, see `/usr/include/X11/keysymdef.h`. If, let's say we want to use simply the letter `a` as the key to start a chain, the value here would be `XK_a`.

If we want a normal key, one that is **not** chained, we can accomplish that by simply setting the Chain key to `-1`.

### Key

The next value, similar to the Chain key also needs to be a keysym. If the key is a chained key, this is the second key that needs to be pressed. If it isn't a chained key, this is the key that needs to be pressed while the Modifier is pressed.

In order to see a list of possible keys, see `/usr/include/X11/keysymdef.h`. Note that this path may differ depending on your operating system.

There are a lot more keys technically defined, see `/usr/include/X11/XF86keysym.h` for a list of media keys.

### Function

Many functions expect an argument to be passed to them. However this value does require any function argument to be specified because it simply passes the next value to the function and calls it.

There is no list of functions, but `speedwm.c` has a lot of declared functions which you may use in keybinds provided the function expects `const Arg *arg`.

### Function arguments

This is the actual argument passed to the function we're calling when the keybind is activated.

Passing a float (such as mfact) through would mean I should override `.f`, passing an integer (such as barposition) through would mean I should override `.i`. There's also `.v` for `void` and `.ui` for tags. This is not used unless you want to mess with tag keybinds.

When you don't care about the value given to the function, you may simply enter a `0`.

### Example keybind

With all that said, you probably get the idea of how it works. But you still don't know how to put all this knowledge together.

1. `{ KeyPress,`
2. `MODIFIER1|SHIFT,`
3. `-1,`
4. `XK_w,`
5. `spawn,`
6. `cmd( "firefox" ) },`

Combined into one line: `{ KeyPress, MODIFIER1|SHIFT, -1, XK_w, spawn, cmd( "firefox" ) },`

### Example chained keybind

1. `{ KeyPress,`
2. `MODIFIER1|SHIFT,`
3. `XK_a,`
4. `XK_w,`
5. `togglebar,`
6. `{0} },`

Combined into one line: `{ KeyPress, MODIFIER1|SHIFT, XK_a, XK_w, togglebar, {0} },`

This should give you a brief idea of how keybinds work in speedwm. Most of this carries over to `dwm` as well, although it does not have some of this stuff.

## Credits

I far from wrote this entire project myself. Below are people who made this project what it is through submitting patches to suckless or otherwise contributing code in some way in alphabetical order.

- Adam Yuan
- Alex Cole
- Anukul Adhikari
- Ayoub Khater
- bakkeby
- bit6tream
- cd
- Chris Down
- Chris Noxz
- Daniel Bylinka
- Dhaval Patel
- Eon S. Jeon
- explosion-mental
- Fabian Blatz
- Finn Rayment
- Georgios Oxinos
- Ivan J.
- Jan Christoph Ebersbach
- Jared
- kleinbottle4
- Luigi Foscari
- Luke Smith
- Marius Iacob
- Markus Teich
- Mihir Lad
- MLquest8
- Ondřej Grover
- ornx
- Patrick Steinhardt
- phi
- prx
- Rob King
- Ryan Roden-Corrent
- sipi
- Sönke Lambert
- speedie
- Stefan Mark
- Stefan Matz
- suckless.org
- Timmy Keller
- Viliam Kováč

See Codeberg contributions for more information.
