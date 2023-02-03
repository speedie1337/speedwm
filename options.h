/* speedwm
 *
 * Below is a configuration file which is technically C source code.
 * See the man page (speedwm.1) or the speedwm wiki (https://speedwm.speedie.gq) for help.
 * See the LICENSE file for license details.
 */

/* Window alignment options */
static int bordersize                         = 1; /* How big your border is in pixels */
static int snap                               = 20; /* Snap pixel */
static int mastercount                        = 1; /* Number of clients in the master area */
static int stackcount                         = 0; /* Number of clients in the stack area */
static int resizehints                        = 0; /* Show resize hints */
static int decorhints                         = 1; /* Respect decoration hints */
static int savefloat                          = 1; /* Save position of floating windows */
static int refreshrules                       = 0; /* Refresh rules when a CLASS or TITLE changes */
static int i3mastercount                      = 0; /* Enable i3-gaps like mastercount (0/1) */
static int mousemfact                         = 1; /* Enable adjusting mfact using the mouse (0/1) */
static int mousecfact                         = 1; /* Enable adjusting cfact using the mouse (0/1) */
static float mfact                            = 0.50; /* Default mfact value. 0.50 = each gets half the available space */
static float lowestmfact                      = 0.05; /* Lowest possible mfact value on top of the existing. */

/* Window gap options */
static int enablegaps                         = 1; /* Enable gaps */
static int gapsizeih                          = 10; /* Horizontal inner gap between windows */
static int gapsizeiv                          = 10; /* Vertical inner gap between windows */
static int gapsizeoh                          = 10; /* Horizontal outer gap between windows and screen edge */
static int gapsizeov                          = 10; /* Vertical outer gap between windows and screen edge */
static int smartgapsize                       = 0; /* Gaps for smartgaps. If set to zero, gaps will be disabled. */
static int smartgaps                          = 0; /* 1 means use different gap size when there is only one window */

/* Window aesthetic options */
static int fadewindows                        = 1; /* Starts with opacity on any focused/unfocused windows */
static int fadeinactive                       = 1; /* Fade inactive windows */
static double activeopacity				      = 1.0f; /* Window opacity when it's focused (0 <= opacity <= 1) */
static double inactiveopacity                 = 0.875f; /* Window opacity when it's inactive (0 <= opacity <= 1) */

/* Tag preview options */
static int tagpreview                         = 1; /* Enable tag previews */
static int tagpreviewpaddingv                 = 0; /* Vertical tag padding */
static int tagpreviewpaddingh                 = 0; /* Horizontal tag padding */
static int barpreview                         = 1; /* Display the bar in the preview */
static int scalepreview                       = 4; /* Size of tag preview. Lower is bigger, higher is smaller. */

/* Window spawning options */
static int windowmap                          = 1; /* Map and unmap windows through X11 (0/1) */
static int allowurgent                        = 1; /* Allow windows to have the 'urgent' status */
static int attachdirection                    = 3; /* 0 default, 1 above, 2 aside, 3 below, 4 bottom, 5 top */
static int swallowclients                     = 1; /* Swallow windows or not */
static int swallowfloating                    = 1; /* Swallow floating windows by default */
static int centerfloating                     = 1; /* Center floating windows by default */
static int startontag                         = 1; /* Start on a tag or not? */
static int floatscratchpad                    = 0; /* Float the scratchpad window on hide (0/1) */

/* Client properties */
static int autofocus                          = 1; /* Allow clients to automatically be focused when they request it. This comes with the side effect of potentially being annoying */
static int automove                           = 1; /* Allow clients to automatically move when they request it */
static int autoresize                         = 1; /* Allow resizing clients automatically when they request it. */

/* Font options */
static char font[]                            = "Noto Sans Regular 9"; /* Font to draw the bar with */

/* Status options */
static char statussep                         = ';'; /* Separator used to switch from the main status bar to the next */
static char defaultstatus[]                   = ""; /* What to print when a status bar is not running */

/* Task switcher options
 * NOTE: The modifier and cycle key is a constant and because of this, it cannot be changed in speedwmrc/.Xresources.
 */
static const int switchmodifier 	          = 0x85; /* Modifier to be used for the task switcher. Once this key is no longer held down, the task is selected. This is a keycode. */
static const int switchcyclekey				  = 0x17; /* Key to cycle forward in the task list. This is a keycode. */
static int menupositionv					  = 1; /* Menu position vertically. 0 bottom, 1 center, 2 top */
static int menupositionh					  = 1; /* Menu position horizontally, 0 left, 1 center, 2 right */
static int maxwidth							  = 600; /* Max menu width */
static int maxheight						  = 200; /* Max menu height */

/* Misc */
static char shell[]                           = "/bin/sh"; /* Shell to use */
static int warpcursor                         = 1; /* Warp cursor when switching client/monitor */
static int urgentwindows                      = 1; /* Color urgent tags */
static int pertag                             = 1; /* Use different mfact and layout for each layout */
static int wmclass                            = 1; /* Enable (1) workaround for when a class cannot be grabbed */
static int clicktofocus                       = 0; /* Click to focus, or focus when the mouse moves to another window */

/* Full screen options */
static int movefullscreenmon                  = 1; /* Move fullscreen windows to another monitor */
static int fullscreenhidebar                  = 1; /* Hide the bar when full screen */
static int lockfullscreen                     = 1;

/* Window icon options */
static int iconsize                           = 15; /* Size of the icon */
static int iconspacing                        = 5; /* Spacing between the title and icon */

/* Bar options */
static int alpha                              = 1; /* Enable alpha */
static int barposition                        = 1; /* Bar position. Top: 1, Bottom: 0 */
static int barheight                          = 3; /* Bar height in pixels, 0 = calculate automatically */
static int barpaddingov                       = 0; /* Vertical outer bar padding in pixels. */
static int barpaddingoh                       = 0; /* Horizontal outer bar padding in pixels. */
static int barpaddingiv                       = 0; /* Vertical inner bar padding in pixels. */
static int barpaddingih                       = 0; /* Horizontal inner bar padding in pixels. */

/* Inset options
 *
 * Insets allow you to add extra spacing on each edge of the screen.
 * This is useful if you use a non-speedwm bar.
 */
static int insetx                             = 0; /* x inset */
static int insety                             = 0; /* y inset */
static int insetw                             = 0; /* w inset */
static int inseth                             = 0; /* h inset */

/* Title options */
static int titleposition                      = 1; /* Title position. (0: Left, 1: Center) */

/* Systray options */
static int systraypinning                     = 0; /* Show systray on focused monitor (0) or (<monitor number>) to pin to a specific monitor */
static int systrayposition                    = 0; /* Systray position. (0: Right, 1: Left) */
static int systrayspacing                     = 2; /* Systray spacing in pixels */

/* Tag text options */
static int underline                          = 0; /* Underline tags (1) or not (0) */
static int underlineall 		              = 0; /* 1 to show underline on all tags, 0 for just the active ones */
static int underlinepad	                      = 5; /* Horizontal padding between the underline and tag */
static int underlinestroke	                  = 2; /* Height of the underline */
static int underlinevoffset	                  = 0; /* How far above the bottom of the bar the line should appear */

/* Bar item options */
static int hidebar                            = 0; /* Hide the bar (1) or show (0) */
static int hidelayout                         = 0; /* Hide layout indicator (1) or show (0) */
static int hidetitle                          = 0; /* Hide the title part of the bar (1) or show (0) */
static int hideunselectedtitle                = 0; /* Hide unselected title (1) or show (0) */
static int hideborder                         = 0; /* Hide window border (1) or show (0) */
static int hideunselectedborder               = 1; /* Hide unselected window border (1) or show (0) */
static int hidestatus                         = 0; /* Hide status bar (1) or show (0) */
static int hideicon                           = 0; /* Hide icon (1) or show (0) */
static int hidetags                           = 0; /* Hide status bar (1) or show (0) */
static int hidesystray                        = 1; /* Hide systray by default (1) or show (0) */
static int hideemptytags                      = 1; /* Hide empty tags (1) or show (0) */
static int hidefloating                       = 0; /* Hide floating indicator (1) or show (0) */
static int hidesticky                         = 0; /* Hide sticky indicator (1) or show (0) */
static int hideclientindicator                = 0; /* Hide client indicator (1) or show (0) */

/* Layout options
 *
 * Monocle layout */
static int monocleclientcount                 = 0; /* Display client count in the Monocle layout */
static int monoclecount					      = 0; /* Display focused client and number of total clients in the Monocle layout */
static char monocleformat[]                   = "[%d/%d]"; /* Format of the monocle count. Unsigned integer monoclecount must be set to 1 for this to be used. */

/* Deck layout */
static int deckcount						  = 0; /* Display deck count in the deck layout */
static char deckformat[]                      = "[%d]"; /* Format of the deck count. deckcount must be set to 1 for this to be used. */

/* Custom layout */
#define CUSTOM_HISTORY                      ".custom_history"

/* Resetting */
static int resetlayout                        = 0; /* Reset layout when there is only one client visible */
static int resetmfact                         = 0; /* Reset mfact when there is only one client visible */
static int resetgaps                          = 0; /* Reset gaps when there is only one client visible */

/* Colors
 *
 * Bar colors */
static char col_background[]                  = "#222222"; /* Bar background and status background color */
static char col_titlenorm[]                   = "#222222"; /* Normal taskbar background color */
static char col_titlesel[]                    = "#99b3ff"; /* Selected taskbar background color */
static char col_titlehid[]                    = "#222222"; /* Hidden taskbar background color */

/* General text colors */
static char col_textnorm[]                    = "#bbbbbb"; /* Normal Application title bar/font */
static char col_textsel[]                     = "#222222"; /* Selected Text/Font color */

/* Window border colors */
static char col_windowbordernorm[]            = "#000000"; /* Normal window border */
static char col_windowbordersel[]             = "#eeeeee"; /* Selected window border */
static char col_windowborderurg[]             = "#f0e68c"; /* Urgent window border */

/* Tag text/background colors */
static char col_tag1[]                        = "#99b3ff"; /* Tag 1 background */
static char col_tag1_text[]                   = "#eeeeee"; /* Tag 1 text */
static char col_tag2[]                        = "#99b3ff"; /* Tag 2 background */
static char col_tag2_text[]                   = "#eeeeee"; /* Tag 2 text */
static char col_tag3[]                        = "#99b3ff"; /* Tag 3 background */
static char col_tag3_text[]                   = "#eeeeee"; /* Tag 3 text */
static char col_tag4[]                        = "#99b3ff"; /* Tag 4 background */
static char col_tag4_text[]                   = "#eeeeee"; /* Tag 4 text */
static char col_tag5[]                        = "#99b3ff"; /* Tag 5 background */
static char col_tag5_text[]                   = "#eeeeee"; /* Tag 5 text */
static char col_tag6[]                        = "#99b3ff"; /* Tag 6 background */
static char col_tag6_text[]                   = "#eeeeee"; /* Tag 6 text */
static char col_tag7[]                        = "#99b3ff"; /* Tag 7 background */
static char col_tag7_text[]                   = "#eeeeee"; /* Tag 7 text */
static char col_tag8[]                        = "#99b3ff"; /* Tag 8 background */
static char col_tag8_text[]                   = "#eeeeee"; /* Tag 8 text */
static char col_tag9[]                        = "#99b3ff"; /* Tag 9 background */
static char col_tag9_text[]                   = "#eeeeee"; /* Tag 9 text */
static char col_tagurgent[]                   = "#f0e68c"; /* Urgent tag color */

/* Layout indicator colors */
static char col_layouttext[]                  = "#000000"; /* Layout indicator text */
static char col_layout[]                      = "#99b3ff"; /* Layout indicator background */

/* Systray colors */
static char col_systray[]					  = "#222222"; /* Systray background color */

/* Status colors */
static char col_status0[]                     = "#131210"; /* Status color 0 */
static char col_status1[]                     = "#bf616a"; /* Status color 1 */
static char col_status2[]                     = "#A16F9D"; /* Status color 2 */
static char col_status3[]                     = "#68ABAA"; /* Status color 3 */
static char col_status4[]                     = "#A89F93"; /* Status color 4 */
static char col_status5[]                     = "#D3A99B"; /* Status color 5 */
static char col_status6[]                     = "#AFC9AC"; /* Status color 6 */
static char col_status7[]                     = "#eae1cb"; /* Status color 7 */
static char col_status8[]                     = "#a39d8e"; /* Status color 8 */
static char col_status9[]                     = "#6D5E8E"; /* Status color 9 */
static char col_status10[]                    = "#A16F9D"; /* Status color 10 */
static char col_status11[]                    = "#D3A99B"; /* Status color 11 */
static char col_status12[]                    = "#AFC9AC"; /* Status color 12 */
static char col_status13[]                    = "#eae1cb"; /* Status color 13 */
static char col_status14[]                    = "#6D5E8E"; /* Status color 14 */
static char col_status15[]                    = "#ffffff"; /* Status color 15 */

/* Powerline colors */
static char col_powerline0[]                  = "#131210"; /* Powerline color 0 */
static char col_powerline1[]                  = "#bf616a"; /* Powerline color 1 */
static char col_powerline2[]                  = "#A16F9D"; /* Powerline color 2 */
static char col_powerline3[]                  = "#68ABAA"; /* Powerline color 3 */
static char col_powerline4[]                  = "#A89F93"; /* Powerline color 4 */
static char col_powerline5[]                  = "#D3A99B"; /* Powerline color 5 */
static char col_powerline6[]                  = "#AFC9AC"; /* Powerline color 6 */
static char col_powerline7[]                  = "#eae1cb"; /* Powerline color 7 */
static char col_powerline0_text[]             = "#eeeeee"; /* Powerline text color 0 */
static char col_powerline1_text[]             = "#131210"; /* Powerline text color 1 */
static char col_powerline2_text[]             = "#131210"; /* Powerline text color 2 */
static char col_powerline3_text[]             = "#131210"; /* Powerline text color 3 */
static char col_powerline4_text[]             = "#131210"; /* Powerline text color 4 */
static char col_powerline5_text[]             = "#131210"; /* Powerline text color 5 */
static char col_powerline6_text[]             = "#131210"; /* Powerline text color 6 */
static char col_powerline7_text[]             = "#131210"; /* Powerline text color 7 */

/* Color options
 *
 * Title
 */
static int colorselectedtitle                 = 0; /* Color the selected title. */
static int colorhiddentitle                   = 0; /* Color the hidden title. */

/* Layout */
static int colorlayout                        = 0; /* Color the layout indicator. If you're going to be using Powerlines, the tags look better with this set to 0. */

/* Powerline options */
static int tagplshape                         = 0; /* Shape of the tag powerlines (0: >, 1: /) */

/* Text options
 *
 * Unused/empty tags
 */
static char text_tag1_empty[]                 = ""; /* Text for tag 1 when empty */
static char text_tag2_empty[]                 = ""; /* Text for tag 2 when empty */
static char text_tag3_empty[]                 = ""; /* Text for tag 3 when empty */
static char text_tag4_empty[]                 = ""; /* Text for tag 4 when empty */
static char text_tag5_empty[]                 = ""; /* Text for tag 5 when empty */
static char text_tag6_empty[]                 = ""; /* Text for tag 6 when empty */
static char text_tag7_empty[]                 = ""; /* Text for tag 7 when empty */
static char text_tag8_empty[]                 = ""; /* Text for tag 8 when empty */
static char text_tag9_empty[]                 = ""; /* Text for tag 9 when empty */

/* Used/occupied tags */
static char text_tag1_used[]                  = ""; /* Text for tag 1 when used */
static char text_tag2_used[]                  = ""; /* Text for tag 2 when used */
static char text_tag3_used[]                  = ""; /* Text for tag 3 when used */
static char text_tag4_used[]                  = ""; /* Text for tag 4 when used */
static char text_tag5_used[]                  = ""; /* Text for tag 5 when used */
static char text_tag6_used[]                  = ""; /* Text for tag 6 when used */
static char text_tag7_used[]                  = ""; /* Text for tag 7 when used */
static char text_tag8_used[]                  = ""; /* Text for tag 8 when used */
static char text_tag9_used[]                  = ""; /* Text for tag 9 when used */

/* Layouts */
static char text_layout1[]                    = " "; /* Text for layout 1 */
static char text_layout2[]                    = " "; /* Text for layout 2 */
static char text_layout3[]                    = " "; /* Text for layout 3 */
static char text_layout4[]                    = " "; /* Text for layout 4 */
static char text_layout5[]                    = " "; /* Text for layout 5 */
static char text_layout6[]                    = " "; /* Text for layout 6 */
static char text_layout7[]                    = " "; /* Text for layout 7 */
static char text_layout8[]                    = " "; /* Text for layout 8 */
static char text_layout9[]                    = " "; /* Text for layout 9 */
static char text_layout10[]                   = " "; /* Text for layout 10 */
static char text_layout11[]                   = " "; /* Text for layout 11 */
static char text_layout12[]                   = " "; /* Text for layout 12 */
static char text_layout13[]                   = " "; /* Text for layout 13 */
static char text_layout14[]                   = " "; /* Text for layout 14 */
static char text_layout15[]                   = " "; /* Text for layout 15 */

/* Alpha/opacity settings
 *
 * These options set the opacity of the status bar modules.
 * They can be anything from 0 (fully transparent) to 255 (fully opaque).
 *
 * You can also set opaque and transparent which are the same as 0 and 255 respectively.
 * To completely disable alpha, disable it in toggle.h.
 */
#define baropacity                           160 /* Opacity for the overall bar */
#define layoutopacity					     160 /* Opacity for the layout indicator */
#define tagnormopacity						 opaque /* Opacity for other tags */
#define tagselopacity						 160 /* Opacity for the selected tag */
#define normtitleopacity					 160 /* Opacity for all other windows in the speedwm bar */
#define seltitleopacity						 160 /* Opacity for the focused window in the speedwm bar */
#define normborderopacity				     opaque /* Opacity for the all window borders except selected */
#define selborderopacity					 opaque /* Opacity for the selected window border */
#define hiddenopacity						 transparent /* Opacity for hidden/minimized windows */
#define powerlineopacity                     opaque /* Opacity for the speedwm powerline in the status bar */
#define statusopacity						 160 /* Opacity for speedwm status bar */
#define systrayopacity						 160 /* Opacity for the speedwm built in systray */

/* Icon misc options
 *
 * Sticky
 */
static XPoint stickyicon[]                  = { {0,0}, /* Represents the icon as an array of vertices */
												 {4,0},
												 {4,8},
												 {2,6},
												 {0,8},
												 {0,0},
};

static XPoint stickyiconbb                  = {4,8}; /* Defines the bottom right corner of the polygon's bounding box (speeds up scaling) */

/* You have reached the end of the configuration file.
 * For keybinds, see 'keybinds.h'
 * For feature toggles, see 'toggle.h'
 */
