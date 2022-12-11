/* Bar configuration
 *
 * This header is for configuring the bar along with its modules. Below are some usage notes.
 *
 * Monitor:
 *   -1:                     Show the module on all monitors.
 *   0":                     Show on the main monitor (monitor 0).
 *   'A':                    Show on the focused monitor.
 *
 * Bar:
 *   0:                      Main bar.
 *   1:                      Extra bar.
 *
 * Alignment:
 *   bar_align_left:         Force the module to be placed on the left side of the bar if possible.
 *   bar_align_right:        Force the module to be placed on the right side of the bar if possible.
 *   bar_align_center:       Force the module to be placed in the center of the bar if possible.
 *   bar_align_left_left:    Force the module to be placed on the left side of the bar next to the center split.
 *   bar_align_left_right:   Force the module to be placed on the right side of the bar next to the center split.
 *   bar_align_left_center:  Force the module to be placed on the center in the middle of the remaining space left of the center split on the left..
 *   bar_align_right_left:   Force the module to be placed on the left side of the bar next to the center split.
 *   bar_align_right_right:  Force the module to be placed on the right side of the bar next to the center split.
 *   bar_align_right_center: Force the module to be placed on the center in the middle of the remaining space left of the center split on the right.
 *   bar_align_right_none:   No specific alignment. This will give the module the remaining space.
 * 
 * Width:
 *   Function to run when getting the width of the module.
 *   
 *   Syntax: width_<module>
 *   Example: width_tags_pwl
 *   
 * Draw
 *   Function to run when drawing the module.
 *
 *   Syntax: draw_<module>
 *   Example: draw_tags_pwl
 *
 * Click
 *   Function to run when getting a click.
 *
 *   Syntax: click_<module>
 *   Example: click_tags_pwl
 *
 * Module name
 *   Does nothing special, just helpful for debugging among other things.
 *
 * Module list
 *
 * List of modules that can be used.
 *
 *   ltsymbol:     Standard, basic layout icon.
 *   tags:         Basic tags, without powerlines.
 *   tags_pwl:     Tags with powerlines.
 *   systray:      System tray.
 *   status_basic: Basic status bar.
 *   status:       Clickable status bar with color support.
 *   status_pwl:   Status with powerlines. NOTE: It is not clickable.
 *   title:        Title, shows all windows, including hidden windows.
 *   title_basic:  Basic title, shows focused window.
 *
 */
static const BarRule barrules[] = {
	/* Monitor  Bar    Alignment         Width                  Draw                  Click               Module name */
	{ -1,       0,     bar_align_left,   width_ltsymbol,        draw_ltsymbol,        click_ltsymbol,     "layout" },
	{ -1,       0,     bar_align_left,   width_tags_pwl,        draw_tags_pwl,        click_tags_pwl,     "powerline tags" },
   	{ 'A',      0,     bar_align_right,  width_systray,         draw_systray,         click_systray,      "systray" },
    { 'A',      0,     bar_align_right,  width_status_pwl,      draw_status_pwl,      click_status_pwl,   "powerline status" },
	{ -1,       0,     bar_align_none,   width_title,           draw_title,           click_title,        "title" },
};
