/* This is the header for the IPC feature.
 * If you're not going to be using IPC, you can leave this as default, it will compile just fine.
 *
 * If you are going to use IPC you must follow the points below:
 *
 * - If you use an operating system that isn't GNU/Linux, edit config.mk and comment out the three lines, then edit toggle.h and set USEIPC to 0. IPC support is not available for non-GNU/Linux operating systems.
 * - Install the 'yajl' package using your distro's package manager. It will depend on your distribution but on Gentoo the package is named 'yajl'.
 */

static char *ipcsockpath        = "/tmp/speedwm.sock"; /* May be a good idea to change this to 'dwm.sock' for compatibility reasons. */
static IPCCommand ipccommands[] = {
  ipccmd( view,                1,      {  arg_uint  } ),
  ipccmd( tagmon,              1,      {  arg_uint  } ),
  ipccmd( focusmon,            1,      {  arg_sint  } ),
  ipccmd( focusstackvis,       1,      {  arg_sint  } ),
  ipccmd( zoom,                1,      {  arg_none  } ),
  ipccmd( incmastercount,      1,      {  arg_sint  } ),
  ipccmd( killclient,          1,      {  arg_sint  } ),
  ipccmd( togglefloating,      1,      {  arg_none  } ),
  ipccmd( setmfact,            1,      {  arg_float } ),
  ipccmd( setlayoutsafe,       1,      {  arg_ptr   } ),
  ipccmd( quit,                1,      {  arg_none  } )
};
