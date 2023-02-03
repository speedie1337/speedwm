/* Mouse bindings
 *
 * This header contains mouse binds.
 * Change them, or remove them if you prefer. You can also add more if you want.
 * Once you're done with your edits, run 'make clean install'.
 *
 * To disable all mouse binds, edit toggle.h.
 *
 * clicklayout - Layout icon
 * clicktitle - Window title
 * clickstatusbar - Status text
 * clickroot - Root window (background)
 * clicktags - Tags
 * clickclient - Window
 *
 * Button1 - Left click
 * Button2 - Middle click
 * Button3 - Right click
 * Button4 - Scroll up
 * Button5 - Scroll down
 */

/* Actions when the mouse clicks a part of the screen */
static const Button buttons[] = {
	/* click          event mask                button      function                 argument */
	{ clicklayout,    0,                        Button2,    spawn,                   cmd( "speedwm-utils layout" ) },
	{ clicklayout,    0,                        Button1,    cyclelayout,             {.i = +1 } },
	{ clicklayout,    0,                        Button3,    cyclelayout,             {.i = -1 } },
	{ clicklayout,    0,                        Button4,    cyclelayout,             {.i = +1 } },
	{ clicklayout,    0,                        Button5,    cyclelayout,             {.i = -1 } },
	{ clicktitle,     0,                        Button2,    zoom,                    {0} },
	{ clickclient,    MODIFIER1,                Button1,    moveorplace,             {.i = 1} },
	{ clickclient,    MODIFIER1,                Button2,    togglefloating,          {0} },
	{ clickclient,    MODIFIER1|ControlMask,    Button3,    dragcfact,               {0} },
	{ clickclient,    MODIFIER1,                Button3,    dragmfact,               {0} },
	{ clickstatusbar, 0,                        Button1,    spawn,                   {.v = clickstatus } },
	{ clickstatusbar, 0,                        Button2,    spawn,                   {.v = clickstatus } },
	{ clickstatusbar, 0,                        Button3,    spawn,                   {.v = clickstatus } },
	{ clicktitle,     0,                        Button1,    togglewin,               {0} },
	{ clicktitle,     0,                        Button4,    inplacerotate,           {.i = +2} },
	{ clicktitle,     0,                        Button5,    inplacerotate,           {.i = -2} },
	{ clicktags,      0,                        Button1,    view,                    {0} },
	{ clicktags,      0,                        Button4,    viewtoleft,              {0} },
	{ clicktags,      0,                        Button5,    viewtoright,             {0} },
	{ clicktags,      MODIFIER1,                Button4,    viewtoleft_vacant,       {0} },
	{ clicktags,      MODIFIER1,                Button5,    viewtoright_vacant,      {0} },
};
