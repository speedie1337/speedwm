/* These are all your keybinds.
 *
 * Event
 *
 * KeyPress - Activate when a key is pressed.
 * KeyRelease - Activate when a key is released.
 *
 * Modifiers
 *
 * MODIFIER1 is what you defined below, default is Super)
 * MODIFIER2 is what you defined below, default is Alt)
 * SHIFT is unless changed going to be your Shift key.
 * CONTROL is unless changed going to be your Control key.
 * ALT unless changed is going to be your left Alt key.
 * ALTR unless changed is going to be your right Alt key.
 * SUPER unless changed is going to be your left Super (Windows/macOS Command) key.
 * SUPERR unless changed is going to be your right Super (Windows/macOS Command) key.
 *
 * Example keybind:
 *
 * { KeyPress,   MODIFIER1, -1, XK_1, spawn, cmd( TERMINAL "echo 'Hello world!' ) },
 * { KeyRelease, MODIFIER1, XK_F1, XK_1, spawn, cmd( TERMINAL "echo 'Hello world! Pressing two keys in a row is based!' ) },
 *
 * It is recommended that you avoid using 'MODIFIER2' (Mod1Mask) by itself because it can break software defined shortcuts.
 *
 * If you need help, see the man page for speedwm.
 * Once you're done with your edits, run 'make clean install'.
 */

/* For terminal keybinds */
#define TERMINAL "st -e "


/* Modifier keys
 * Mod4Mask | Super (Windows/command) key
 * Mod1Mask | Alt key
 */
#define MODIFIER1 Mod4Mask
#define MODIFIER2 Mod1Mask

/* Tag related keybinds */
#define TAGKEYS(CHAIN,KEY,TAG) { KeyPress,      MODIFIER1,                   CHAIN, KEY, view,       {.ui = 1 << TAG } },  \
                               { KeyPress,      MODIFIER1|SHIFT,             CHAIN, KEY, previewtag, {.ui = TAG      } },  \
							   { KeyPress,      MODIFIER1|CONTROL,           CHAIN, KEY, toggleview, {.ui = 1 << TAG } },  \
							   { KeyPress,      MODIFIER1|SHIFT|CONTROL,     CHAIN, KEY, tag,        {.ui = 1 << TAG } },

/* Keybinds */
static Key keys[] = {
	/* type          modifier                     chain key      key           function              argument */
	{ KeyPress,      MODIFIER1|SHIFT,             -1,            XK_semicolon, spawn,                cmd( "spmenu_run -l 0 -p 'Run:' -na" ) },
	{ KeyPress,      MODIFIER1|SHIFT,             -1,            XK_Return,    spawn,                cmd( TERMINAL ) },
	{ KeyPress,      MODIFIER1|SHIFT,             -1,            XK_w,         spawn,                cmd( "chromium || chromium-bin" ) },
	{ KeyPress,      MODIFIER1|CONTROL,           -1,            XK_Tab,       spawn,                cmd( "speedwm-utils layout" ) },

	{ KeyPress,      MODIFIER1,                   -1,            XK_Tab,       switcherstart,        {0} },

	{ KeyPress,      MODIFIER1,                   -1,            XK_s,         togglesystray,        {0} },

	{ KeyPress,      MODIFIER1|CONTROL|SHIFT,     -1,            XK_a,         cyclelayout,          {.i = -1 } },
	{ KeyPress,      MODIFIER1|CONTROL|SHIFT,     -1,            XK_d,         cyclelayout,          {.i = +1 } },

	{ KeyPress,      MODIFIER1,                   -1,            XK_minus,     scratchpad_show,      {0} },
	{ KeyPress,      MODIFIER1|SHIFT,             -1,            XK_minus,     scratchpad_hide,      {0} },
	{ KeyPress,      MODIFIER1,                   -1,            XK_equal,     scratchpad_remove,    {0} },
	{ KeyPress,      MODIFIER1,                   -1,            XK_f,         togglefullscr,        {0} },
	{ KeyPress,      MODIFIER1,                   -1,            XK_b,         togglebar,            {0} },
	{ KeyPress,      MODIFIER1,                   -1,            XK_r,         resetmastercount,     {0} },
	{ KeyPress,      MODIFIER1,                   -1,            XK_m,         focusmaster,          {0} },
	{ KeyPress,      MODIFIER1,                   -1,            XK_j,         focusstackvis,        {.i = +1 } },
	{ KeyPress,      MODIFIER1,                   -1,            XK_k,         focusstackvis,        {.i = -1 } },
	{ KeyPress,      MODIFIER1|CONTROL,           -1,            XK_j,         focusstackhid,        {.i = +1 } },
	{ KeyPress,      MODIFIER1|CONTROL,           -1,            XK_k,         focusstackhid,        {.i = -1 } },
	{ KeyPress,      MODIFIER1,                   -1,            XK_a,         setmfact,             {.f = -0.05} },
	{ KeyPress,      MODIFIER1,                   -1,            XK_d,         setmfact,             {.f = +0.05} },
	{ KeyPress,      MODIFIER1,                   -1,            XK_0,         reset_mfact,          {0} },
	{ KeyPress,      MODIFIER1|CONTROL|SHIFT,     -1,            XK_k,         setcfact,             {.f = +0.25} },
	{ KeyPress,      MODIFIER1|CONTROL|SHIFT,     -1,            XK_j,         setcfact,             {.f = -0.25} },
	{ KeyPress,      MODIFIER1|CONTROL|SHIFT,     -1,            XK_0,         setcfact,             {.f =  0.00} },
	{ KeyPress,      MODIFIER1,                   -1,            XK_Return,    zoom,                 {0} },
	{ KeyPress,      MODIFIER1|CONTROL,           -1,            XK_Return,    mirrorlayout,         {0} },
	{ KeyPress,      MODIFIER1|SHIFT,             -1,            XK_q,         killclient,           {0} },
	{ KeyPress,      MODIFIER1|SHIFT,             -1,            XK_space,     togglefloating,       {0} },
	{ KeyPress,      MODIFIER1|CONTROL,           -1,            XK_0,         view,                 {.ui = ~0 } },
	{ KeyPress,      MODIFIER1,                   -1,            XK_d,         focusmon,             {.i = -1 } },
	{ KeyPress,      MODIFIER1,                   -1,            XK_slash,     focusmon,             {.i = +1 } },
	{ KeyPress,      MODIFIER1|SHIFT,             -1,            XK_d,         tagmon,               {.i = -1 } },
	{ KeyPress,      MODIFIER1|SHIFT,             -1,            XK_slash,     tagmon,               {.i = +1 } },
	{ KeyPress,      MODIFIER1|SHIFT,             -1,            XK_j,         inplacerotate,        {.i = +1} },
	{ KeyPress,      MODIFIER1|SHIFT,             -1,            XK_k,         inplacerotate,        {.i = -1} },
	{ KeyPress,      MODIFIER1|SHIFT,             -1,            XK_h,         inplacerotate,        {.i = +2} },
	{ KeyPress,      MODIFIER1|SHIFT,             -1,            XK_l,         inplacerotate,        {.i = -2} },
	{ KeyPress,      MODIFIER1,                   -1,            XK_n,         incmastercount,       {.i = -1 } },
	{ KeyPress,      MODIFIER1,                   -1,            XK_i,         incmastercount,       {.i = +1 } },
	{ KeyPress,      MODIFIER1,                   -1,            XK_y,         togglesticky,         {0} },
	{ KeyPress,      MODIFIER1|CONTROL|SHIFT,     -1,            XK_0,         resetbarheight,       {0} },
	{ KeyPress,      MODIFIER1|CONTROL|SHIFT,     -1,            XK_equal,     setbarheight,         {.i = +1} },
	{ KeyPress,      MODIFIER1|CONTROL|SHIFT,     -1,            XK_minus,     setbarheight,         {.i = -1} },
	{ KeyPress,      MODIFIER1,                   -1,            XK_period,    setbarpadding,        {.i = +1 } },
	{ KeyPress,      MODIFIER1,                   -1,            XK_comma,     setbarpadding,        {.i = -1 } },
    { KeyPress,      MODIFIER1,                   -1,            XK_v,         centerwindow,         {0} },
	{ KeyPress,      MODIFIER1|SHIFT,             -1,            XK_period,    setbpgaps,            {.i = +1 } },
	{ KeyPress,      MODIFIER1|SHIFT,             -1,            XK_comma,     setbpgaps,            {.i = -1 } },
	{ KeyPress,      MODIFIER1|SHIFT,             -1,            XK_slash,     resetbpgaps,          {0} },
	{ KeyPress,      MODIFIER1|CONTROL,           -1,            XK_i,		   incstackcount,		 {.i = +1 } },
	{ KeyPress,      MODIFIER1|CONTROL,           -1,            XK_u,		   incstackcount,		 {.i = -1 } },
	{ KeyPress,      MODIFIER1|CONTROL|SHIFT,     -1,            XK_period,    setbpgaps,            {.i = +5 } },
	{ KeyPress,      MODIFIER1|CONTROL|SHIFT,     -1,            XK_comma,     setbpgaps,            {.i = -5 } },

	/* Floating mode keybinds */
	{ KeyPress,      MODIFIER1,                   -1,            XK_w,         moveresizeaspect,    {.i = +24} },
	{ KeyPress,      MODIFIER1,                   -1,            XK_e,         moveresizeaspect,    {.i = -24} },
	{ KeyPress,      MODIFIER1,                   -1,            XK_Down,      moveresize,          {.v = "0x 25y 0w 0h" } },
	{ KeyPress,      MODIFIER1,                   -1,            XK_Up,        moveresize,          {.v = "0x -25y 0w 0h" } },
	{ KeyPress,      MODIFIER1,                   -1,            XK_Right,     moveresize,          {.v = "25x 0y 0w 0h" } },
	{ KeyPress,      MODIFIER1,                   -1,            XK_Left,      moveresize,          {.v = "-25x 0y 0w 0h" } },
	{ KeyPress,      MODIFIER1|SHIFT,             -1,            XK_Down,      moveresize,          {.v = "0x 0y 0w 25h" } },
	{ KeyPress,      MODIFIER1|SHIFT,             -1,            XK_Up,        moveresize,          {.v = "0x 0y 0w -25h" } },
	{ KeyPress,      MODIFIER1|SHIFT,             -1,            XK_Right,     moveresize,          {.v = "0x 0y 25w 0h" } },
	{ KeyPress,      MODIFIER1|SHIFT,             -1,            XK_Left,      moveresize,          {.v = "0x 0y -25w 0h" } },
	{ KeyPress,      MODIFIER1|CONTROL,           -1,            XK_Up,        moveresizeedge,      {.v = "t"} },
	{ KeyPress,      MODIFIER1|CONTROL,           -1,            XK_Down,      moveresizeedge,      {.v = "b"} },
	{ KeyPress,      MODIFIER1|CONTROL,           -1,            XK_Left,      moveresizeedge,      {.v = "l"} },
	{ KeyPress,      MODIFIER1|CONTROL,           -1,            XK_Right,     moveresizeedge,      {.v = "r"} },
	{ KeyPress,      MODIFIER1|CONTROL|SHIFT,     -1,            XK_Up,        moveresizeedge,      {.v = "T"} },
	{ KeyPress,      MODIFIER1|CONTROL|SHIFT,     -1,            XK_Down,      moveresizeedge,      {.v = "B"} },
	{ KeyPress,      MODIFIER1|CONTROL|SHIFT,     -1,            XK_Left,      moveresizeedge,      {.v = "L"} },
	{ KeyPress,      MODIFIER1|CONTROL|SHIFT,     -1,            XK_Right,     moveresizeedge,      {.v = "R"} },

 	/* Tag keybinds */
	TAGKEYS(                                      -1,            XK_1,         0)
	TAGKEYS(                                      -1,            XK_2,         1)
	TAGKEYS(                                      -1,            XK_3,         2)
	TAGKEYS(                                      -1,            XK_4,         3)
	TAGKEYS(                                      -1,            XK_5,         4)
	TAGKEYS(                                      -1,            XK_6,         5)
	TAGKEYS(                                      -1,            XK_7,         6)
	TAGKEYS(                                      -1,            XK_8,         7)
	TAGKEYS(                                      -1,            XK_9,         8)
	{ KeyPress,      MODIFIER1|CONTROL,           -1,            XK_a,         viewtoleft,          {0} },
	{ KeyPress,      MODIFIER1|CONTROL,           -1,            XK_d,         viewtoright,         {0} },
	{ KeyPress,      MODIFIER1|CONTROL|SHIFT,     -1,            XK_z,         viewtoleft_vacant,   {0} },
	{ KeyPress,      MODIFIER1|CONTROL|SHIFT,     -1,            XK_c,         viewtoright_vacant,  {0} },

    /* Marking */
    { KeyPress,      MODIFIER1,                   -1,            XK_bracketleft,                     swapfocus,                    {0} },
    { KeyPress,      MODIFIER1,                   -1,            XK_bracketright,                    swapclient,                   {0} },
    { KeyPress,      MODIFIER1,                   -1,            XK_backslash,                       togglemark,                   {0} },

    /* Hide/Show keybinds */
	{ KeyPress,      MODIFIER1,			    	  -1,            XK_o, 	       hide, 	            {0} },
    { KeyPress,      MODIFIER1|CONTROL, 		  -1,            XK_o, 	       show, 	            {0} },
	{ KeyPress,      MODIFIER1|CONTROL|SHIFT,     -1,            XK_o,         showall,             {0} },
	{ KeyPress,      MODIFIER1|CONTROL|SHIFT,     -1,            XK_p,         hideall,             {0} },

    /* Chained keybinds */
	{ KeyPress,      MODIFIER1,                   XK_t,          XK_r,         reorganizetags,      {0} },
	{ KeyPress,      MODIFIER1,                   XK_p,          XK_t,         togglebarpadding,    {0} },
	{ KeyPress,      MODIFIER1,                   XK_p,          XK_u,         setbarpadding,       {.i = +5 } },
	{ KeyPress,      MODIFIER1,                   XK_p,          XK_d,         setbarpadding,       {.i = -5 } },
	{ KeyPress,      MODIFIER1,                   XK_p,          XK_r,         resetbarpadding,     {0} },

	/* Chained layout keybinds */
	{ KeyPress,      MODIFIER1,                   XK_l,          XK_q,         rotatelayoutaxis,	{.i = +1 } }, /* layout axis */
	{ KeyPress,      MODIFIER1,                   XK_l,          XK_w,         rotatelayoutaxis,	{.i = +2 } }, /* master axis */
	{ KeyPress,      MODIFIER1,                   XK_l,          XK_e,         rotatelayoutaxis,	{.i = +3 } }, /* stack axis */
	{ KeyPress,      MODIFIER1,                   XK_l,          XK_r,         rotatelayoutaxis,	{.i = +4 } }, /* secondary stack axis */
	{ KeyPress,      MODIFIER1,                   XK_l,          XK_a,         rotatelayoutaxis,	{.i = -1 } }, /* layout axis */
	{ KeyPress,      MODIFIER1,                   XK_l,          XK_s,         rotatelayoutaxis,	{.i = -2 } }, /* master axis */
	{ KeyPress,      MODIFIER1,                   XK_l,          XK_d,         rotatelayoutaxis,	{.i = -3 } }, /* stack axis */
	{ KeyPress,      MODIFIER1,                   XK_l,          XK_f,         rotatelayoutaxis,	{.i = -4 } }, /* secondary stack axis */
	{ KeyPress,      MODIFIER1,                   XK_l,          XK_1,         setlayout,	        {.v = &layouts[0]} },
	{ KeyPress,      MODIFIER1,                   XK_l,          XK_2,         setlayout,	        {.v = &layouts[1]} },
	{ KeyPress,      MODIFIER1,                   XK_l,          XK_3,         setlayout,	        {.v = &layouts[2]} },
	{ KeyPress,      MODIFIER1,                   XK_l,          XK_4,         setlayout,	        {.v = &layouts[3]} },
	{ KeyPress,      MODIFIER1,                   XK_l,          XK_5,         setlayout,	        {.v = &layouts[4]} },
	{ KeyPress,      MODIFIER1,                   XK_l,          XK_6,         setlayout,	        {.v = &layouts[5]} },
	{ KeyPress,      MODIFIER1,                   XK_l,          XK_7,         setlayout,	        {.v = &layouts[6]} },
	{ KeyPress,      MODIFIER1,                   XK_l,          XK_8,         setlayout,	        {.v = &layouts[7]} },
	{ KeyPress,      MODIFIER1,                   XK_l,          XK_9,         setlayout,	        {.v = &layouts[8]} },
	{ KeyPress,      MODIFIER1,                   XK_l,          XK_0,         setlayout,	        {.v = &layouts[9]} },

	/* Chained toggle keybinds */
	{ KeyPress,      MODIFIER1,                   XK_t,          XK_t,         togglebartags,	    {0} },
   	{ KeyPress,      MODIFIER1,                   XK_t,          XK_y,         togglebaremptytags,  {0} },
	{ KeyPress,      MODIFIER1,                   XK_t,          XK_w,         togglebartitle,      {0} },
   	{ KeyPress,      MODIFIER1,                   XK_t,          XK_u,         togglebarunseltitle, {0} },
	{ KeyPress,      MODIFIER1,                   XK_t,          XK_s,         togglebarstatus,     {0} },
	{ KeyPress,      MODIFIER1,                   XK_t,          XK_l,         togglebarlt,         {0} },
	{ KeyPress,      MODIFIER1,                   XK_t,          XK_i,         togglebaricon,       {0} },
	{ KeyPress,      MODIFIER1,                   XK_t,          XK_o,         toggleopacity,       {0} },

    /* Misc */
	{ KeyPress,      MODIFIER1|SHIFT,             -1,            XK_backslash, killunsel,           {0} },

	/* Gap keybinds */
	{ KeyPress,      MODIFIER1|CONTROL,           -1,            XK_z,         incrgaps,            {.i = +5 } },
	{ KeyPress,      MODIFIER1|CONTROL,           -1,            XK_x,         incrgaps,            {.i = -5 } },

	/* Media buttons */
    { KeyPress,      0,                           -1,            XF86XK_WWW,	                    spawn,		                   cmd( "chromium || chromium-bin" ) },
};
