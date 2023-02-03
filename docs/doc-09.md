## 10. Additional note on autostart

If you wish to add autostart entries without recompiling, consider using $HOME/.config/speedwm/autostart.sh. This is a path added to autostart.h and you can fill it with anything you want.
Make sure your user has permission to execute the script.

Note that this script or any other commands in autostart.h will **not** run when speedwm is restarted, only when speedwm is first started.

