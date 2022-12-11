/* Rules
 * Any applications defined here must follow the rules specified.
 * The user can get the class by running a program like xprop (must be installed by the user) and then selecting the software.
 *
 * - tags mask: Disable tags where the CLASS, INSTANCE or TITLE will spawn. (X << Y)
 * - isfloating: Choose whether a CLASS, INSTANCE or TITLE will spawn as floating. (1/0)
 * - ispermanent: Whether or not you can kill a client
 * - isterminal: Whether or not the CLASS, INSTANCE or TITLE is a terminal or not (1/0)
 * - noswallow: Whether or not the CLASS, INSTANCE or TITLE gets swallowed by a terminal or not (1/0)
 * - ignoretransient: Whether or not to ignore transient windows for CLASS, INSTANCE or TITLE (1/0)
 * - float x/y/w/h: Position floating windows will spawn at. -1 will disable the rule.
 * - unmanaged: Whether or not to allow speedwm to manage the window (tile it, focus it, move it, etc.)
 *
 * For the rest, leave as they currently are and copy them for new rules.
 * Once you're done with your edits, run 'make clean install'.
 * 
 ***************************************************************/

/* For terminal applications */
#define TERMINAL_CLASS "st"

/* Web browser */
#define BROWSER_CLASS "chromium-bin-browser-chromium" /* This is the class for chromium-bin on Gentoo. Use xprop to find out your class. */

static const Rule rules[]                     = {
    	/* class                instance    title                          tags mask isfloating ispermanent isterminal noswallow monitor unmanaged ignoretransient float x,y,w,h scratch key */
		{ TERMINAL_CLASS,       NULL,       NULL,                          0,        0,         0,          1,         0,        -1,     0,        0,			   -1,           0 },
        { "Zathura",            NULL,       NULL,                          0,        0,         0,          0,         0,        -1,     0,        0,			   -1,           0 },
	    { "qutebrowser",        NULL,       NULL,                          0,        0,         0,          0,         0,        -1,     0,        0,              -1,           0 },
	    { "Navigator",          NULL,       NULL,                          0,        0,         0,          0,         0,        -1,     0,        0,              -1,           0 },
	    { BROWSER_CLASS,        NULL,       NULL,                          0,        0,         0,          0,         0,        -1,     0,        0,              -1,           0 },
        { "mpv",                NULL,       NULL,                          0,        0,         0,          0,         0,        -1,     0,        0,              -1,           0 },
        { "tabbed",             NULL,       NULL,                          0,        0,         0,          0,         1,        -1,     0,        0,              -1,           0 },
        { "trayer",             NULL,       NULL,                          0,        1,         1,          0,         1,        -1,     1,        0,              -1,           0 },
        { "Gsimplecal",         NULL,       NULL,                          0,        1,         0,          0,         1,        -1,     0,        0,              -1,           0 },
		{ NULL,                 NULL,       "CustomizeMii 3.11 by Leathl", 0,        1,         0,          0,         1,        -1,     0,        0,              -1,           0 },
	    { NULL,                 NULL,       "Picture-in-Picture",          0,        1,         0,          0,         1,        -1,     0,        0,              -1,           0 },
	    { NULL,                 NULL,       "About GNU IceCat",            0,        1,         0,          0,         1,        -1,     0,        0,              -1,           0 },
	    { NULL,                 NULL,       "About LibreWolf",             0,        1,         0,          0,         1,        -1,     0,        0,              -1,           0 },
	    { NULL,                 NULL,       "About Mozilla Firefox",       0,        1,         0,          0,         1,        -1,     0,        0,              -1,           0 },
	    { NULL,                 NULL,       "Save Image",                  0,        1,         0,          0,         1,        -1,     0,        0,              -1,           0 },
		{ NULL,                 NULL,       "scratchpad",                  0,                   0,          0,                   -1,										   's' },
};

