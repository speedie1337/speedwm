/* These are all your keybinds.
 *
 * Example keybind:
 *
 * { MODIFIER1, -1, XK_1, spawn, cmd( TERMINAL "echo 'Hello world!' ) },
 * { MODIFIER1, XK_F1, XK_1, spawn, cmd( TERMINAL "echo 'Hello world! Pressing two keys in a row is based!' ) },
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
 * It is recommended that you avoid using 'MODIFIER2' (Mod1Mask/ALT) by itself because it can break software defined shortcuts. 
 *
 * If you need help, see the man page for speedwm.
 * Once you're done with your edits, run 'make clean install'.
 */

/* For terminal applications */
#define TERMINAL "st -e "


/* Modifier keys
 * Mod4Mask | Super (Windows/command) key
 * Mod1Mask | Alt key
 */
#define MODIFIER1              Mod4Mask
#define MODIFIER2              Mod1Mask

/* Tag related keybinds */
#define TAGKEYS(CHAIN,KEY,TAG) { MODIFIER1,                   CHAIN, KEY, view,       {.ui = 1 << TAG } },  \
                               { MODIFIER1|SHIFT,             CHAIN, KEY, previewtag, {.ui = TAG      } },  \
							   { MODIFIER1|CONTROL,           CHAIN, KEY, toggleview, {.ui = 1 << TAG } },  \
							   { MODIFIER1|SHIFT|CONTROL,     CHAIN, KEY, tag,        {.ui = 1 << TAG } },  

/* Keybinds */
static Key keys[] = {
	/* modifier                     chain key     key           function              argument */

	/* Run keybinds */
	{ MODIFIER1,                   -1,            XK_semicolon, spawn,                cmd( "j4-dmenu-desktop --term=st --dmenu='dmenu -l 20 -p Open:'" ) },
	{ MODIFIER1|SHIFT,             -1,            XK_semicolon, spawn,                cmd( "dmenu_run -l 0 -p 'Run:'" ) },
	{ MODIFIER1|CONTROL|SHIFT,     -1,            XK_semicolon, spawn,                cmd( "speedwm-applist" ) },

	/* Application keybinds */
	{ MODIFIER1|SHIFT,             -1,            XK_Return,    spawn,                cmd( TERMINAL ) },
	{ MODIFIER1|SHIFT,             -1,            XK_s,         spawn,                cmd( "speedwm-screenshotutil -s" ) },
	{ MODIFIER1|SHIFT,             -1,            XK_f,         spawn,                cmd( TERMINAL "lfrun || lf" ) },
	{ MODIFIER1|SHIFT,             -1,            XK_w,         spawn,                cmd( "chromium || chromium-bin" ) },
	{ MODIFIER1|SHIFT,             -1,            XK_o,         spawn,                cmd( "speedwm-dfmpeg" ) },
	{ MODIFIER1|SHIFT,             -1,            XK_d,         spawn,                cmd( TERMINAL "iron" ) },
	{ MODIFIER1|SHIFT,             -1,            XK_t,	        spawn,                cmd( TERMINAL "nvim" ) },
	{ MODIFIER1|SHIFT,             -1,            XK_a,         spawn,                cmd( TERMINAL "speedwm-audioctrl -runmixer" ) },
	{ MODIFIER1|SHIFT,             -1,            XK_m,         spawn,                cmd( TERMINAL "tmux new-session -A -D -s cmus $(which --skip-alias cmus)" ) },
	{ MODIFIER1|SHIFT,             -1,            XK_Tab,       spawn,                cmd( "speedwm-winnav" ) },
    { MODIFIER1|SHIFT,             -1,            XK_x,         spawn,                cmd( TERMINAL "htop" ) },
    { MODIFIER1|SHIFT,             -1,            XK_c,         spawn,                cmd( TERMINAL "tmux new-session -A -D -s weechat $(which --skip-alias weechat)" ) },
	{ MODIFIER1|SHIFT,             -1,            XK_Escape,    spawn,                cmd( "speedwm-shutdown" ) },
	{ MODIFIER1|SHIFT,             -1,            XK_u,         spawn,                cmd( TERMINAL "rssread" ) },
	{ MODIFIER1|SHIFT,             -1,            XK_r,         spawn,                cmd( TERMINAL "neomutt" ) },
	{ MODIFIER1|CONTROL,           -1,            XK_Tab,       spawn,                cmd( "speedwm-utils layout" ) },
	{ MODIFIER1|CONTROL,           -1,            XK_h,         spawn,                cmd( TERMINAL "man speedwm" ) },
	{ MODIFIER1|CONTROL,           -1,            XK_s,         spawn,                cmd( "speedwm-screenshotutil -f" ) },
    { MODIFIER1|CONTROL,           -1,            XK_m,         spawn,                cmd( "pkill cmus" ) },
	{ MODIFIER1|CONTROL,           -1,            XK_q,         spawn,                cmd( "speedwm-audioctrl -mute" ) },
	{ MODIFIER1|CONTROL,           -1,            XK_w,         spawn,                cmd( "speedwm-audioctrl -lower" ) },
	{ MODIFIER1|CONTROL,           -1,            XK_e,         spawn,                cmd( "speedwm-audioctrl -raise" ) },
	{ MODIFIER1|CONTROL|SHIFT,     -1,            XK_q,         spawn,                cmd( "cmus-remote --pause" ) },
	{ MODIFIER1|CONTROL|SHIFT,     -1,            XK_w,         spawn,                cmd( "cmus-remote --volume -3000" ) },
	{ MODIFIER1|CONTROL|SHIFT,     -1,            XK_e,         spawn,                cmd( "cmus-remote --volume +3000" ) },
	{ MODIFIER1|CONTROL|SHIFT,     -1,			  XK_Escape,    spawn,                cmd( "speedwm-utils" ) },
	{ MODIFIER1|CONTROL|SHIFT,     -1,            XK_s,         spawn,                cmd( "speedwm-swal" ) },
	{ MODIFIER1|CONTROL|SHIFT,     -1,            XK_n,         spawn,                cmd( "speedwm-netctrl" ) },
	{ MODIFIER1|CONTROL|SHIFT,     -1,            XK_b,         spawn,                cmd( "speedwm-btctrl" ) },
	{ MODIFIER1|CONTROL|SHIFT,     -1,            XK_r,         spawn,                cmd( "libspeedwm --perform core_wm_restart; speedwm -s 'Loading'" ) },
    
    /* Switcher */
    #if USESWITCHER
	{ MODIFIER1,                   -1,            XK_Tab,       switcherstart,        {0} },
    #endif

    /* Systray */
    #if USESYSTRAY
	{ MODIFIER1,                   -1,            XK_s,         togglesystray,        {0} },
    #endif

	/* Layout keybinds */
	{ MODIFIER1|CONTROL|SHIFT,     -1,            XK_a,         cyclelayout,          {.i = -1 } },
	{ MODIFIER1|CONTROL|SHIFT,     -1,            XK_d,         cyclelayout,          {.i = +1 } },

	/* Scratchpad keybinds */
	{ MODIFIER1,                   -1,            XK_minus,     scratchpad_show,      {0} },
	{ MODIFIER1|SHIFT,             -1,            XK_minus,     scratchpad_hide,      {0} },
	{ MODIFIER1,                   -1,            XK_equal,     scratchpad_remove,    {0} },

	/* speedwm general binds */
	{ MODIFIER1,                   -1,            XK_f,         togglefullscr,        {0} },
	{ MODIFIER1,                   -1,            XK_b,         togglebar,            {0} },
	{ MODIFIER1,                   -1,            XK_r,         resetmastercount,     {0} },
	{ MODIFIER1,                   -1,            XK_m,         focusmaster,          {0} },
	{ MODIFIER1,                   -1,            XK_j,         focusstackvis,        {.i = +1 } },
	{ MODIFIER1,                   -1,            XK_k,         focusstackvis,        {.i = -1 } },
	{ MODIFIER1|CONTROL,           -1,            XK_j,         focusstackhid,        {.i = +1 } },
	{ MODIFIER1|CONTROL,           -1,            XK_k,         focusstackhid,        {.i = -1 } },
	{ MODIFIER1,                   -1,            XK_a,         setmfact,             {.f = -0.05} },
	{ MODIFIER1,                   -1,            XK_d,         setmfact,             {.f = +0.05} },
	{ MODIFIER1,                   -1,            XK_0,         reset_mfact,          {0} },
	{ MODIFIER1|CONTROL|SHIFT,     -1,            XK_k,         setcfact,             {.f = +0.25} },
	{ MODIFIER1|CONTROL|SHIFT,     -1,            XK_j,         setcfact,             {.f = -0.25} },
	{ MODIFIER1|CONTROL|SHIFT,     -1,            XK_0,         setcfact,             {.f =  0.00} },
	{ MODIFIER1,                   -1,            XK_Return,    zoom,                 {0} },
	{ MODIFIER1|CONTROL,           -1,            XK_Return,    mirrorlayout,         {0} },
	{ MODIFIER1|SHIFT,             -1,            XK_q,         killclient,           {0} },
	{ MODIFIER1|SHIFT,             -1,            XK_space,     togglefloating,       {0} },
	{ MODIFIER1|CONTROL,           -1,            XK_0,         view,                 {.ui = ~0 } },
	{ MODIFIER1,                   -1,            XK_d,         focusmon,             {.i = -1 } },
	{ MODIFIER1,                   -1,            XK_slash,     focusmon,             {.i = +1 } },
	{ MODIFIER1|SHIFT,             -1,            XK_d,         tagmon,               {.i = -1 } },
	{ MODIFIER1|SHIFT,             -1,            XK_slash,     tagmon,               {.i = +1 } },
	{ MODIFIER1|SHIFT,             -1,            XK_j,         inplacerotate,        {.i = +1} },
	{ MODIFIER1|SHIFT,             -1,            XK_k,         inplacerotate,        {.i = -1} },
	{ MODIFIER1|SHIFT,             -1,            XK_h,         inplacerotate,        {.i = +2} },
	{ MODIFIER1|SHIFT,             -1,            XK_l,         inplacerotate,        {.i = -2} },
	{ MODIFIER1,                   -1,            XK_n,         incmastercount,       {.i = -1 } },
	{ MODIFIER1,                   -1,            XK_i,         incmastercount,       {.i = +1 } },
	{ MODIFIER1,                   -1,            XK_y,         togglesticky,         {0} },
	{ MODIFIER1|CONTROL|SHIFT,     -1,            XK_0,         resetbarheight,       {0} },
	{ MODIFIER1|CONTROL|SHIFT,     -1,            XK_equal,     setbarheight,         {.i = +1} },
	{ MODIFIER1|CONTROL|SHIFT,     -1,            XK_minus,     setbarheight,         {.i = -1} },
	{ MODIFIER1,                   -1,            XK_period,    setbarpadding,        {.i = +1 } },
	{ MODIFIER1,                   -1,            XK_comma,     setbarpadding,        {.i = -1 } },
    { MODIFIER1,                   -1,            XK_v,         centerwindow,         {0} },
	{ MODIFIER1|SHIFT,             -1,            XK_period,    setbpgaps,            {.i = +1 } },
	{ MODIFIER1|SHIFT,             -1,            XK_comma,     setbpgaps,            {.i = -1 } },
	{ MODIFIER1|SHIFT,             -1,            XK_slash,     resetbpgaps,          {0} },
	{ MODIFIER1|CONTROL,           -1,            XK_i,			incstackcount,		  {.i = +1 } },
	{ MODIFIER1|CONTROL,           -1,            XK_u,			incstackcount,		  {.i = -1 } },
	{ MODIFIER1|CONTROL|SHIFT,     -1,            XK_period,    setbpgaps,            {.i = +5 } },
	{ MODIFIER1|CONTROL|SHIFT,     -1,            XK_comma,     setbpgaps,            {.i = -5 } },

	/* Floating mode keybinds */
	{ MODIFIER1,                   -1,            XK_w,         moveresizeaspect,    {.i = +24} },
	{ MODIFIER1,                   -1,            XK_e,         moveresizeaspect,    {.i = -24} },
	{ MODIFIER1,                   -1,            XK_Down,      moveresize,          {.v = "0x 25y 0w 0h" } },
	{ MODIFIER1,                   -1,            XK_Up,        moveresize,          {.v = "0x -25y 0w 0h" } },
	{ MODIFIER1,                   -1,            XK_Right,     moveresize,          {.v = "25x 0y 0w 0h" } },
	{ MODIFIER1,                   -1,            XK_Left,      moveresize,          {.v = "-25x 0y 0w 0h" } },
	{ MODIFIER1|SHIFT,             -1,            XK_Down,      moveresize,          {.v = "0x 0y 0w 25h" } },
	{ MODIFIER1|SHIFT,             -1,            XK_Up,        moveresize,          {.v = "0x 0y 0w -25h" } },
	{ MODIFIER1|SHIFT,             -1,            XK_Right,     moveresize,          {.v = "0x 0y 25w 0h" } },
	{ MODIFIER1|SHIFT,             -1,            XK_Left,      moveresize,          {.v = "0x 0y -25w 0h" } },
	{ MODIFIER1|CONTROL,           -1,            XK_Up,        moveresizeedge,      {.v = "t"} },
	{ MODIFIER1|CONTROL,           -1,            XK_Down,      moveresizeedge,      {.v = "b"} },
	{ MODIFIER1|CONTROL,           -1,            XK_Left,      moveresizeedge,      {.v = "l"} },
	{ MODIFIER1|CONTROL,           -1,            XK_Right,     moveresizeedge,      {.v = "r"} },
	{ MODIFIER1|CONTROL|SHIFT,     -1,            XK_Up,        moveresizeedge,      {.v = "T"} },
	{ MODIFIER1|CONTROL|SHIFT,     -1,            XK_Down,      moveresizeedge,      {.v = "B"} },
	{ MODIFIER1|CONTROL|SHIFT,     -1,            XK_Left,      moveresizeedge,      {.v = "L"} },
	{ MODIFIER1|CONTROL|SHIFT,     -1,            XK_Right,     moveresizeedge,      {.v = "R"} },
 
 	/* Tag keybinds */
	TAGKEYS(                       -1,            XK_1,         0)
	TAGKEYS(                       -1,            XK_2,         1)
	TAGKEYS(                       -1,            XK_3,         2)
	TAGKEYS(                       -1,            XK_4,         3)
	TAGKEYS(                       -1,            XK_5,         4)
	TAGKEYS(                       -1,            XK_6,         5)
	TAGKEYS(                       -1,            XK_7,         6)
	TAGKEYS(                       -1,            XK_8,         7)
	TAGKEYS(                       -1,            XK_9,         8)
	{ MODIFIER1|CONTROL,           -1,            XK_a,         viewtoleft,          {0} },
	{ MODIFIER1|CONTROL,           -1,            XK_d,         viewtoright,         {0} },
	{ MODIFIER1|CONTROL|SHIFT,     -1,            XK_z,         viewtoleft_vacant,   {0} },
	{ MODIFIER1|CONTROL|SHIFT,     -1,            XK_c,         viewtoright_vacant,  {0} },
 
    /* Hide/Show keybinds */
	{ MODIFIER1,				   -1,            XK_o, 	    hide, 	              {0} },
    { MODIFIER1|CONTROL, 		   -1,            XK_o, 	    show, 	              {0} },
	{ MODIFIER1|CONTROL|SHIFT,     -1,            XK_o,         showall,             {0} },
	{ MODIFIER1|CONTROL|SHIFT,     -1,            XK_p,         hideall,             {0} },

    /* Chained keybinds */
	{ MODIFIER1,                   XK_c,          XK_w,         spawn,               cmd( TERMINAL "speedwm-core -curl-weather" ) },
    { MODIFIER1,                   XK_c,          XK_n,         spawn,               cmd( "cmus-remote --next" ) },
    { MODIFIER1,                   XK_c,          XK_p,         spawn,               cmd( "cmus-remote --prev" ) },
	{ MODIFIER1,                   XK_r,          XK_s,         spawn,               cmd( "screenkey" ) },
	{ MODIFIER1,                   XK_r,          XK_d,         spawn,               cmd( "pkill screenkey" ) },
	{ MODIFIER1,                   XK_t,          XK_r,         reorganizetags,      {0} },
	{ MODIFIER1,                   XK_p,          XK_t,         togglebarpadding,    {0} },
	{ MODIFIER1,                   XK_p,          XK_u,         setbarpadding,       {.i = +5 } },
	{ MODIFIER1,                   XK_p,          XK_d,         setbarpadding,       {.i = -5 } },
	{ MODIFIER1,                   XK_p,          XK_r,         resetbarpadding,     {0} },

	/* Chained toggle keybinds */
	{ MODIFIER1,                   XK_t,          XK_t,         togglebartags,	     {0} },
   	{ MODIFIER1,                   XK_t,          XK_y,         togglebaremptytags,  {0} },
	{ MODIFIER1,                   XK_t,          XK_w,         togglebartitle,      {0} },
   	{ MODIFIER1,                   XK_t,          XK_u,         togglebarunseltitle, {0} },
	{ MODIFIER1,                   XK_t,          XK_s,         togglebarstatus,     {0} },
	{ MODIFIER1,                   XK_t,          XK_l,         togglebarlt,         {0} },
	{ MODIFIER1,                   XK_t,          XK_i,         togglebaricon,       {0} },
	{ MODIFIER1,                   XK_t,          XK_o,         toggleopacity,       {0} },
 
	/* Chained music keybinds */
	{ MODIFIER1,                   XK_q,          XK_n,         spawn,               cmd( "cmus-remote --next" ) },         
	{ MODIFIER1,                   XK_q,	  	  XK_p,         spawn,               cmd( "cmus-remote --prev" ) },         
	{ MODIFIER1,                   XK_q,		  XK_l,         spawn,               cmd( "cmus-remote --seek +3" ) },         
	{ MODIFIER1,                   XK_q,		  XK_h,         spawn,               cmd( "cmus-remote --seek -3" ) },         
	{ MODIFIER1,                   XK_q,		  XK_u,         spawn,               cmd( "cmus-remote --seek +10" ) },         
	{ MODIFIER1,                   XK_q,		  XK_d,         spawn,               cmd( "cmus-remote --seek -10" ) },         
	{ MODIFIER1,                   XK_q,		  XK_0,         spawn,               cmd( "cmus-remote --seek 0" ) },         

    /* Misc */
	{ MODIFIER1,                   XK_q,          XK_o,         killunsel,           {0} },
	{ MODIFIER1|SHIFT,             XK_e,          XK_p,         spawn,               cmd( "speedwm-swal --previous" ) },
	{ MODIFIER1|SHIFT,             XK_e,          XK_r,         spawn,               cmd( "speedwm-swal --randomize" ) },
	{ MODIFIER1|SHIFT,             XK_e,          XK_a,         spawn,               cmd( "speedwm-virtualkeyboard" ) },
	{ MODIFIER1|SHIFT,             XK_e,          XK_e,         spawn,               cmd( "speedwm-virtualkeyboard -e" ) },
 
	/* Gap keybinds */
	{ MODIFIER1|CONTROL,           -1,            XK_z,         incrgaps,            {.i = +5 } },
	{ MODIFIER1|CONTROL,           -1,            XK_x,         incrgaps,            {.i = -5 } },
 
	/* Chained gap keybinds */
	{ MODIFIER1,                   XK_g,          XK_t,         togglegaps,          {0} },
	{ MODIFIER1,                   XK_g,          XK_0,         defaultgaps,         {0} },
	{ MODIFIER1,                   XK_g,          XK_i,         incrigaps,           {.i = +1} },
	{ MODIFIER1|SHIFT,             XK_g,          XK_i,         incrigaps,           {.i = -1} },
	{ MODIFIER1,                   XK_g,          XK_o,         incrogaps,           {.i = +1} },
	{ MODIFIER1|SHIFT,             XK_g,          XK_o,         incrogaps,           {.i = -1} },
	{ MODIFIER1|SHIFT,             XK_g,          XK_j,         incrgaps,            {.i = +1 } },
	{ MODIFIER1|SHIFT,             XK_g,          XK_k,         incrgaps,            {.i = -1 } },

	/* Media buttons */
    #if USEMEDIA
	{ 0,                           -1,            XF86XK_AudioMute,		              spawn,		                   cmd( "speedwm-audioctrl -mute" ) },
    { 0,                           -1,            XF86XK_AudioRaiseVolume,	          spawn,		                   cmd( "speedwm-audioctrl -raise" ) },
    { 0,                           -1,            XF86XK_AudioLowerVolume,	          spawn,		                   cmd( "speedwm-audioctrl -lower" ) },
	{ 0,                           -1,            XF86XK_AudioPrev,                   spawn,                           cmd( "cmus-remote --prev" ) },
	{ 0,                           -1,            XF86XK_AudioNext,                   spawn,                           cmd( "cmus-remote --next" ) },
	{ 0,                           -1,            XF86XK_AudioStop,                   spawn,                           cmd( "pkill cmus" ) },
	{ 0,                           -1,            XF86XK_AudioPause,                  spawn,                           cmd( "cmus-remote --pause" ) },
    { 0,                           -1,            XF86XK_WWW,	                      spawn,		                   cmd( "qutebrowser" ) },
    { 0,                           -1,            XF86XK_PowerOff,                    spawn,                           cmd( "speedwm-shutdown" ) },
	{ 0,                           -1,            XF86XK_Sleep,                       spawn,                           cmd( "slock" ) },
	{ 0,                           -1,            XF86XK_Mail,                        spawn,                           cmd( TERMINAL "neomutt" ) },
	{ 0,                           -1,            XF86XK_TaskPane,                    spawn,                           cmd( TERMINAL "htop" ) },
	{ 0,                           -1,            XF86XK_WLAN,                        spawn,                           cmd( "speedwm-netctrl disconnect" ) },
	{ 0,                           -1,            XF86XK_Music,                       spawn,                           cmd( TERMINAL "tmux new-session -A -D -s cmus $(which --skip-alias cmus)" ) },
    #endif

    /* Marking */
    { MODIFIER1,                   -1,            XK_bracketleft,                     swapfocus,                       {0} },
    { MODIFIER1,                   -1,            XK_bracketright,                    swapclient,                      {0} },
    { MODIFIER1,                   -1,            XK_backslash,                       togglemark,                      {0} },
};
