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

