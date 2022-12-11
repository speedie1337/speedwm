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
  ipccmd( view,                1,      { ARG_TYPE_UINT } ),
  ipccmd( tagmon,              1,      { ARG_TYPE_UINT } ),
  ipccmd( focusmon,            1,      { ARG_TYPE_SINT } ),
  ipccmd( focusstackvis,       1,      { ARG_TYPE_SINT } ),
  ipccmd( zoom,                1,      { ARG_TYPE_NONE } ),
  ipccmd( incmastercount,      1,      { ARG_TYPE_SINT } ),
  ipccmd( killclient,          1,      { ARG_TYPE_SINT } ),
  ipccmd( togglefloating,      1,      { ARG_TYPE_NONE } ),
  ipccmd( setmfact,            1,      { ARG_TYPE_FLOAT } ),
  ipccmd( setlayoutsafe,       1,      { ARG_TYPE_PTR } ),
  ipccmd( quit,                1,      { ARG_TYPE_NONE } )
};
