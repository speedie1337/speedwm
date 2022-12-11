/* Mouse bindings
 *
 * This header contains mouse binds.
 * Change them, or remove them if you prefer. You can also add more if you want.
 * Once you're done with your edits, run 'make clean install'.
 *
 * To disable all mouse binds, edit toggle.h.
 *
 * ClkLtSymbol - Layout icon
 * ClkWinTitle - Window title
 * ClkStatusText - Status text
 * ClkRootWin - Root window (background)
 * ClkTagBar - Tags
 *
 * Button1 - Left click
 * Button2 - Middle click
 * Button3 - Right click
 */

/* Actions when the mouse clicks a part of the screen */
static const Button buttons[] = {
	/* click         event mask                button      function                 argument */
	{ ClkLtSymbol,   0,                        Button3,    layoutmenu,              {0} },
	{ ClkLtSymbol,   0,                        Button1,    cyclelayout,             {.i = +1 } },
	{ ClkLtSymbol,   0,                        Button2,    cyclelayout,             {.i = -1 } },
	{ ClkWinTitle,   0,                        Button2,    zoom,                    {0} },
	{ ClkClientWin,  MODIFIER1,                Button1,    moveorplace,             {.i = 1} },
	{ ClkClientWin,  MODIFIER1,                Button2,    togglefloating,          {0} },
	{ ClkClientWin,  MODIFIER1|ControlMask,    Button3,    dragcfact,               {0} },
	{ ClkClientWin,  MODIFIER1,                Button3,    dragmfact,               {0} },
	{ ClkStatusText, 0,                        Button1,    spawn,                   {.v = clickstatus } },
	{ ClkStatusText, 0,                        Button2,    spawn,                   {.v = clickstatus } },
	{ ClkStatusText, 0,                        Button3,    spawn,                   {.v = clickstatus } },
	{ ClkWinTitle,   0,                        Button3,    spawn,                   cmd( "speedwm-utils" ) },
	{ ClkRootWin,    0,                        Button3,    spawn,                   cmd( "j4-dmenu-desktop --term=st --dmenu='dmenu -l 20 -p Open:'" ) },
	{ ClkTagBar,     0,                        Button1,    view,                    {0} },
	{ ClkTagBar,     0,                        Button4,    viewtoleft,              {0} },
	{ ClkTagBar,     0,                        Button5,    viewtoright,             {0} },
	{ ClkTagBar,     MODIFIER1,                Button4,    viewtoleft_vacant,       {0} },
	{ ClkTagBar,     MODIFIER1,                Button5,    viewtoright_vacant,      {0} },
};
