/* Anything in here will automatically start before speedwm
 *
 * If you wish to run more commands, add a line below.
 *
 * Syntax: "shell, "-c", "<command>", NULL,"
 *
 * "shell" is automatically defined as your shell.
 *
 * Once you're done with your edits, run 'make clean install'.
 *************************************************************/

static const char *autostart[]          = {
   /* Run the built in status bar */
   shell, "-c", "pkill status; status &", NULL,

   /* Autostart script, if it exists */
   shell, "-c", "$HOME/.config/speedwm/autostart.sh", NULL,

   NULL
};

