# speedwm
![image](/docs/preview.png)

## What is speedwm?

speedwm is a window manager forked from suckless.org's dwm or dynamic window manager. It manages the user's open windows and tiles them according to a set layout (dynamic). 

Just like dwm, speedwm also tries to be minimal but also has functionality and aesthetics as a goal. It is also much more minimal than other window managers like i3-gaps while offering many more features.

Tiling window managers (unlike floating window managers that you may be used to) tile windows based on a set layout making them easy to get productive on. 
They also encourage the user to use their keyboard instead of the mouse so that the user doesn't have to move their hands much but there are mouse keybinds and more can be added.

## Installation

In order to install this build of speedwm, all dependencies must be installed.
You can see (Dependencies) for a list of all dependencies required to use this fork.

  - git clone https://codeberg.org/speedie/speedwm
  - cd speedwm
  - make clean install
    - If any warnings/errors show up, fix it by installing the missing dependency.
  - If a .xinitrc is used, add 'speedwm' to the end.
    - If you do not have a .xinitrc, you can add autostart commands to the file ~/.config/speedwm/autostart.sh.

## Layouts

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

They can be switched between using a little menu (See Keybinds for more information) or by right clicking the Layout indicator.
The more commonly used layouts can be switched between using a quick keybind.

## Keybinds

This is a full list of keybinds.
Please let me know if any keybinds are missing as these have been manually added.

Keybinds for regular applications

- Super+Shift+Enter         - Opens the defined terminal (default is st)
- Super+Shift+Colon         - Opens a dmenu prompt
- Super+Shift+s             - Opens 'maim' to take a screenshot and copies it to the clipboard using 'xclip' (requires speedwm-extras)
- Super+Shift+f             - Opens the defined file manager
- Super+Shift+w             - Opens the defined web browser
- Super+Shift+o             - Opens the speedwm-dfmpeg dmenu script to record your screen. (requires speedwm-extras)
- Super+Shift+t             - Opens the defined editor in your terminal
- Super+Shift+m             - Kills the defined music player
- Super+Shift+a             - Opens the defined mixer in your terminal
- Super+Shift+m             - Opens the defined music player
- Super+Shift+x             - Opens the defined system process viewer in your terminal
- Super+Shift+c             - Opens the defined chat client
- Super+Shift+d             - Opens iron in the terminal if it is installed
- Super+Shift+u             - Opens the defined RSS reader
- Super+Shift+r             - Opens the defined email client

These keybinds are for navigating speedwm

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
- Super+Colon               - Open a list of desktop entries in dmenu
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
- Super+Shift+.             - Increase bar padding and gaps by 1
- Super+Shift+,             - Decrease bar padding and gaps by 1
- Super+Shift+/             - Reset bar padding and gaps
- Super+Shift+h/j/k/l       - Rotates a stack.
- Super+Shift+Arrow         - Resizes a window in floating mode
- Super+Shift+Escape        - Ask the user if they want to shutdown or reboot or nothing
- Super+Shift+Tab           - Switch between windows and tags in detail.
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
- Super+Control+q           - Mutes your audio
- Super+Control+w           - Increases your volume
- Super+Control+e           - Decreases your volume
- Super+Control+0           - Tag all tags at once.
- Super+Control+Enter       - Mirror the layout
- Super+Control+i           - Increase stackcount by 1
- Super+Control+u           - Decrease stack count by 1
- Super+Control+z/x         - Increase/decrease gaps between windows by 5
- Super+Control+j/k         - Move focus between hidden windows (Can then 'Show')
- Super+Control+Arrow       - Moves a window to any corner of your screen (Arrow key)
- Super+Control+Tab         - Open a dmenu prompt asking the user what layout to switch to
- Super+Control+h           - Open the man page for speedwm in dmenu.
- Super+Control+o           - Show a hidden focused window
- Super+Control+a/d         - Move to the next/previous tag
- Super+Control+Shift+o     - Show all hidden windows
- Super+Control+Shift+p     - Hide all windows
- Super+Control+Shift+Colon - Open a list of extra software in dmenu
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
- Super+Control+Shift+Esc   - Open speedwm-utils (requires speedwm-extras)
- Super+Control+Shift+Arrow - Resize the window to the screen size.
- Super+Control+Shift+s     - Set a wallpaper (requires speedwm-extras)
- Super+Control+Shift+n     - Connect to WLAN using iwd (requires speedwm-extras)
- Super+Control+Shift+b     - Connect to a Bluetooth device using bluez (requires speedwm-extras)
- Super+Control+Shift+q     - Pauses your music
- Super+Control+Shift+w     - Decreases your music volume
- Super+Control+Shift+e     - Increase your music volume
- Super+Control+Shift+r     - Restart speedwm
- Super+Control+Shift+j/k   - Change window size vertically (cfact)
- Super+Control+Shift+Equal - Increase bar height by 1
- Super+Control+Shift+Minus - Decrease bar height by 1

Chained keybinds

- Super+r & s               - Run screenkey if it is installed
- Super+r & d               - Kill screenkey
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
- Super+c & w               - Get the weather (Using wttr.in) using speedwm-core (requires speedwm-extras)
- Super+c & n               - Switch to the next track
- Super+c & p               - Switch to the previous track
- Super+g & t               - Toggle gaps
- Super+g & 0               - Reset gaps
- Super+g & i               - Increase inner gaps by 1
- Super+g & o               - Increase outer gaps by 1
- Super+g & j/k             - Increase/decrease gaps between windows by 1
- Super+p & j/k             - Increase/decrease bar padding by 1
- Super+p & u/d             - Increase/decrease bar padding by 5
- Super+p & r               - Reset bar padding to default
- Super+p & t               - Toggle bar padding on/off
- Super+, & r               - Reset powerline options
- Super+q & o               - Kill every window except the focused
- Super+q & n/p             - Switch to the next/previous track
- Super+q & l/h             - Seek 3 seconds forward/backwards
- Super+q & u/d             - Seek 10 seconds forward/backwards
- Super+q & 0               - Seek to the beginning of the track
- Super+Shift+g & i         - Decrease inner gaps by 1
- Super+Shift+g & o         - Decrease outer gaps by 1
- Super+Shift+e & a         - Open the virtual keyboard
- Super+Shift+e & e         - Open a list of all emojis and copy the selection
- Super+Shift+e & r         - Randomize wallpaper
- Super+Shift+e & p         - Set wallpaper to the previous

These will only work if your keyboard has special multimedia buttons.

- Mute button               - Mutes your audio
- Up Volume button          - Increases your volume
- Next/Previous song button - Switch to the next/previous track      
- Down Volume button        - Decreases your volume
- Pause button              - Pauses the current track
- Stop button               - Stops your defined music player
- Browser button            - Opens your defined web browser
- Power button              - Ask if you wanna shut down, restart or lock your computer. (requires speedwm-extras)
- Email button              - Open your defined email client
- System button             - Open your defined status viewer in a terminal
- Music button              - Open your defined music player
- WLAN button               - Disconnect from WLAN (requires speedwm-extras)

These binds can be activated using your mouse

- Tag <num> (Left click)                  - Switch to tag <num>
- Tag (Scrolling up/down)                 - Switch to the next/previous tag
- Layout indicator (Left click)           - Switch to the next layout
- Layout indicator (Middle click)         - Switch to the previous layout
- Layout indicator (Right click)          - Open a dmenu list of all layouts (requires speedwm-extras)
- Window title (Right click)              - Open speedwm-utils (requires speedwm-extras)
- Focused window (Super+Alt+Left click)   - Move the focused window around
- Focused window (Super+Alt+Middle click) - Make the focused window floating
- Focused window title (Middle click)     - Rotate stack
- Dragging (Super+Right click)            - Increase/decrease size of each window (mfact)
- Dragging (SuperControl+Right click)     - Increase/decrease cfact
- Root window (Right click)               - List .desktop entries and open them (requires j4-dmenu-desktop)

## Dependencies

These are absolutely necessary, speedwm will NOT compile without them
- libxft
- pango
- libXinerama
  - Can be disabled through editing toggle.mk if you're not interested in multiple monitors.
- imlib2
  - Tag previews, Window icons. Can be disabled through editing toggle.mk and toggle.h if you don't want these features.
- yajl
  - Required for the IPC patch. If the IPC patch is disabled, you do not need this.
- tcc
  - Very minimal C compiler that speedwm uses to speed up compile times. If you don't want this dependency, edit host.mk and set CC to 'cc' (or what you prefer).

## Features

These are necessary for certain features and keybinds.
If you want to use an alternative, change it in options.h and/or keybinds.h and mouse.h.

- dmenu
  - NOTE: dmenu is required for most scripts included with this build of speedwm. My build is required for proper Pywal support.
- picom
  - Optional but speedwm looks sort of ugly without a compositor.
- xrdb (Install if you want .Xresources support)
- pywal (Install if you want pywal support. Requires swal (the default way to set wallpapers) or a tool that supports the correct signal for instant reloading)
- j4-dmenu-desktop
  - Required for dmenu desktop entries..
  
## Software

This build of speedwm comes with binds for software. 
You can add, change and remove keybinds by editing 'keybinds.h' and running 'make clean install'.

- libspeedwm
- speedwm-extras
  - Important metapackage containing extra scripts that speedwm makes use of. (https://codeberg.org/speedie/speedwm-extras)
- st (Terminal)
- Chromium (Web browser)
- htop (Status monitor)
- newsboat (RSS reader)
- zathura (PDF reader)
- cmus (Default music player)
  - Default status bar also has support for moc (Music On Console)
- neovim (Text editor)
- neomutt (Email client)
- lf (File manager)
- slock (Lock screen)
- weechat (IRC client)
- tmux (Used for the music player and IRC client)

And everything under 'Features'.

## Important

If you're used to dwm, speedwm might be a little unfamiliar to you at first. This is because speedwm doesn't use config.h (or config.def.h).
Instead, config.h is split into different parts to make it easier to edit. Instead of editing config.h you'll want to edit:

- autostart.h for starting stuff right before speedwm (For example xclip, pywal, etc.)
- options.h for changing colors and applications to use with keybinds.
- signal.h for adding fake signals
- colors.h for changing alpha options and color options, most users won't need to edit it.
- xresources.h for adding/removing .Xresources options
- rules.h for adding/removing rules
- keybinds.h for adding/removing keybinds.
- mouse.h for adding/removing mouse binds.
- statusbar.h for adding/removing statuscmd clicks.
- ipc.h for adding/removing IPC commands. (If support is compiled in)
- toggle.h for adding/removing features from getting compiled in. 

After you've edited one of the files, you need to run 'make clean install' to reinstall speedwm.
Remember that you can change colors through your .Xresources file (see .Xresources and Pywal) meaning you do not need to recompile speedwm.

Another important detail you must keep in mind is that this build comes with a status bar simply named 'speedwm_status'.
It can be found in the speedwm source code directory. It is just a shell script which adds stuff to your status bar. It will automatically be started when speedwm starts.

You can edit the status bar simply by editing 'speedwm_stellar' or its modules (modules_*) and running 'make clean install'.
You can also configure it by editing '~/.config/speedwm/statusrc'.

If you want to change status bar, edit options.h and set 'static char status' to your status bar binary (must be in $PATH).
Alternatively, you can also set it in .Xresources (See .Xresources and Pywal).

## .Xresources and Pywal

This fork of speedwm has .Xresources support thanks to the .Xresources patch. It also has pywal support (tool which grabs colors based on your wallpaper).

Colors do not reload automatically though, you must reload them manually. Use a signal for this (See list of signals above) or simply 'libspeedwm --perform core_wm_reload'.

To use .Xresources, make sure 'xrdb' is installed. If a .xinitrc is used, add 'xrdb /path/to/.Xresources/file' before 'speedwm'. If a .Xresources file is not used, add it to ~/.config/speedwm/autostart.sh instead.

If you don't want to define the options manually, there is an example .Xresources file containing speedwm default settings in docs/example.Xresources. You can copy this somewhere or you can simply '< docs/example.Xresources >> ~/.Xresources' to append the speedwm options to your .Xresources file.

The magic of .Xresources is that it is a universal configuration file. While you *can* use the col.value values, you can also use traditional colors 0 through 15 as well. These colors take priority over regular speedwm colors. This is so that speedwm is compatible with Pywal and more general/mainstream .Xresources configurations.

Below is a list of all .Xresources values you can define.

- speedwm.bar.height:                   4 
- speedwm.bar.position:                 1 
- speedwm.bar.paddingh:                 10 
- speedwm.bar.paddingv:                 10 
- speedwm.bar.hide:                     0 
- speedwm.bar.hide.tags:                0 
- speedwm.bar.hide.emptytags:           1 
- speedwm.bar.hide.floating:            0 
- speedwm.bar.hide.layout:              0 
- speedwm.bar.hide.sticky:              0 
- speedwm.bar.hide.status:              0 
- speedwm.bar.hide.systray:             0 
- speedwm.bar.hide.title:               0 
- speedwm.bar.hide.icon:                0 
- speedwm.bar.hide.clientindicator:     0 
- speedwm.bar.titleposition:            1 
- speedwm.border.roundedcorners:        0 
- speedwm.border.cornerradius:          2 
- speedwm.border.size:                  1 
- speedwm.client.allowurgent:           1 
- speedwm.client.automove:              1 
- speedwm.client.autofocus:             1 
- speedwm.client.autoresize:            1 
- speedwm.client.decorhints:            1 
- speedwm.client.hide.border:           0 
- speedwm.client.hide.singleborder:     1 
- speedwm.client.fade.inactive:         1 
- speedwm.client.fade.windows:          1 
- speedwm.client.floatscratchpad:       0 
- speedwm.client.savefloat:             1 
- speedwm.client.swallow:               1 
- speedwm.client.swallowfloating:       1 
- speedwm.client.wmclass:               1 
- speedwm.fonts.font:                   NotoSans Regular 8
- speedwm.col.background:               #222222
- speedwm.col.layout:                   #99b3ff
- speedwm.col.layouttext:               #000000
- speedwm.col.status0:                  #131210
- speedwm.col.status1:                  #bf616a
- speedwm.col.status2:                  #A16F9D
- speedwm.col.status3:                  #68ABAA
- speedwm.col.status4:                  #A89F93
- speedwm.col.status5:                  #D3A99B
- speedwm.col.status6:                  #AFC9AC
- speedwm.col.status7:                  #eae1cb
- speedwm.col.status8:                  #a39d8e
- speedwm.col.status9:                  #6D5E8E
- speedwm.col.status10:                 #a16f9d
- speedwm.col.status11:                 #d3a99b
- speedwm.col.status12:                 #afc9ac
- speedwm.col.status13:                 #eae1cb
- speedwm.col.status14:                 #6d5e8e
- speedwm.col.status15:                 #ffffff
- speedwm.col.powerline0:               #131210
- speedwm.col.powerline1:               #bf616a
- speedwm.col.powerline2:               #A16F9D
- speedwm.col.powerline3:               #68ABAA
- speedwm.col.powerline4:               #A89F93
- speedwm.col.powerline5:               #D3A99B
- speedwm.col.powerline6:               #AFC9AC
- speedwm.col.powerline7:               #eae1cb
- speedwm.col.powerline0_text:          #eeeeee
- speedwm.col.powerline1_text:          #131210
- speedwm.col.powerline2_text:          #131210
- speedwm.col.powerline3_text:          #131210
- speedwm.col.powerline4_text:          #131210
- speedwm.col.powerline5_text:          #131210
- speedwm.col.powerline6_text:          #131210
- speedwm.col.powerline7_text:          #131210
- speedwm.col.systray:                  #222222
- speedwm.col.tag1:                     #99b3ff
- speedwm.col.tag1.text:                #eeeeee
- speedwm.col.tag2:                     #99b3ff
- speedwm.col.tag2.text:                #eeeeee
- speedwm.col.tag3:                     #99b3ff
- speedwm.col.tag3.text:                #eeeeee
- speedwm.col.tag4:                     #99b3ff
- speedwm.col.tag4.text:                #eeeeee
- speedwm.col.tag5:                     #99b3ff
- speedwm.col.tag5.text:                #eeeeee
- speedwm.col.tag6:                     #99b3ff
- speedwm.col.tag6.text:                #eeeeee
- speedwm.col.tag7:                     #99b3ff
- speedwm.col.tag7.text:                #eeeeee
- speedwm.col.tag8:                     #99b3ff
- speedwm.col.tag8.text:                #eeeeee
- speedwm.col.tag9:                     #99b3ff
- speedwm.col.tag9.text:                #eeeeee
- speedwm.col.tagurgent:                #f0e68c
- speedwm.col.textnorm:                 #bbbbbb
- speedwm.col.textsel:                  #222222
- speedwm.col.titlenorm:                #222222
- speedwm.col.titlesel:                 #99b3ff
- speedwm.col.titlehid:                 #222222
- speedwm.col.windowbordernorm:         #000000
- speedwm.col.windowbordersel:          #eeeeee
- speedwm.col.windowborderurg:          #f0e68c
- speedwm.color.hiddentitle:            1 
- speedwm.color.layout:                 0 
- speedwm.color.selectedtitle:          1 
- speedwm.bar.powerline.tag.shape:      0 
- speedwm.cursor.warp:                  1 
- speedwm.focus.spawned:                0 
- speedwm.fullscreen.hidebar:           1 
- speedwm.fullscreen.lockfullscreen:    1 
- speedwm.fullscreen.movefullscreenmon: 0 
- speedwm.gaps.enable:                  1 
- speedwm.gaps.sizeih:                  10 
- speedwm.gaps.sizeiv:                  10 
- speedwm.gaps.sizeoh:                  10 
- speedwm.gaps.sizeov:                  10 
- speedwm.gaps.smartgaps:               0 
- speedwm.gaps.smartgapsize:            0 
- speedwm.icon.size:                    10 
- speedwm.icon.spacing:                 5 
- speedwm.layout.deck.count:            0 
- speedwm.layout.deck.format:           [%d]
- speedwm.layout.monocle.clientcount:   0 
- speedwm.layout.monocle.count:         0 
- speedwm.layout.monocle.format:        [%d/%d]
- speedwm.mfact:                        0.50 
- speedwm.mfact.lowest:                 0.05 
- speedwm.mouse.clicktofocus:           0 
- speedwm.mouse.mfact:				  1 
- speedwm.mouse.cfact:                  1 
- speedwm.rule.refresh:                 0 
- speedwm.stack.attachdirection:        3 
- speedwm.stack.centerfloating:         1 
- speedwm.stack.i3mcount:               0 
- speedwm.stack.mastercount:            1 
- speedwm.stack.snap:                   20 
- speedwm.status.defaultstatus:
- speedwm.switcher.maxheight:           200 
- speedwm.switcher.maxwidth:            600 
- speedwm.switcher.menupositionv:       1 
- speedwm.switcher.menupositionh:       1 
- speedwm.systray.padding:              2 
- speedwm.systray.pinning:              0 
- speedwm.systray.position:             0 
- speedwm.tag.pertag:                   1 
- speedwm.tag.preview:                  1 
- speedwm.tag.preview.bar:              1 
- speedwm.tag.preview.paddingh:         0 
- speedwm.tag.preview.paddingv:         0 
- speedwm.tag.preview.scale:            4 
- speedwm.tag.resetgaps:                0 
- speedwm.tag.resetlayout:              0 
- speedwm.tag.resetmfact:               0 
- speedwm.tag.start:                    1 
- speedwm.tag.underline:                0 
- speedwm.tag.underlineall:             0 
- speedwm.tag.underlinepad:             5 
- speedwm.tag.underlinestroke:          2 
- speedwm.tag.underlinevoffset:         0 
- speedwm.tag.urgentwindows:            1 
- speedwm.tiling.resizehints:           0 
- speedwm.run.shell:                    /bin/sh

## Signals

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
- 121 - Toggle border for when there's only one window.
- 122 - Reset bar padding and gaps

## Status bar

speedwm has a status bar. It's the right part of the bar. It supports Pango, status2d and statuscmd markup as well as regular plain text and colored emojis or glyphs. To override this status, you can use the 'speedwm -s <status>' command.

Bundled with speedwm is a fork of dwmblocks. dwmblocks is a dwm status bar that handles this all for you through a block system. This fork has been integrated into the Makefile and is (by default) installed when speedwm is compiled. The status bar can be configured in the status.c and status.h and clickability can be configured in statusbar.h.

By default the status bar runs modules that are also bundled with speedwm (see modules/ directory). To configure these modules, you can edit ~/.config/speedwm/statusrc which should be created when a module runs.

The bundled status bar is autostarted by speedwm if it is installed. If you want to use your own status bar, comment out 'USESTATUS' in toggle.mk and remove /usr/bin/status if speedwm has been installed previously. Then simply start the status bar through autostart.h, ~/.config/speedwm/autostart.sh or .xinitrc if you are using that.

## Additional note on autostart

If you wish to add autostart entries without recompiling, consider using $HOME/.config/speedwm/autostart.sh. This is a path added to autostart.h and you can fill it with anything you want.

Do note that it will NOT just run once. It will run every time you restart and start speedwm (restart signals count as well, color reloads do not).
To get around this, you may wrap stuff in a function in the script and have some way of checking if it has ran before such as pgrep. If it hasn't, call the function.

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
