/* status.h
 *
 * This header is for configuring the status bar.
 * It is technically a separate program, but it is compiled and installed alongside speedwm.
 *
 * This separate program (status.c, status.h) based on dwmblocks is licensed under the GNU General Public License version 2.0. See license file for more information.
 *
 * Prefix: Text that will be printed before each module. This can be Pango/status2d/clickstatus markup or a unicode/ASCII character.
 * Command: Command that will run. The output of this command will be printed for that module.
 * Update interval: How often the module will be updated in seconds.
 * Update signal: What signal reloads the module with force. (using the kill command)
 *
 */

static int hideemptymodule  = 1; /* Hide the module if command returns nothing */
static char leftpadding[]   = ""; /* Padding on the left side of the status text */
static char rightpadding[]  = ""; /* Padding on the right side of the status text */
static char *separator      = ""; /* Delimiter between modules. Note that this will be applied even if a command is empty. */

static const Module modules[] = {
/*    prefix	         command		                        update interval	       update signal */
	{ "<\x01",           "module_time --print",	                1,	                   1 },
	{ "<\x02",           "module_date --print",	                60,	                   2 },
	{ "<\x03",           "module_bat  --print",	                2,	                   3 },
	{ "<\x04",           "module_vol  --print",	                1,	                   4 },
	{ "<\x05",           "module_ram  --print",	                6,	                   5 },
    { "<\x01",           "module_net  --print",	                10,	                   6 },
	{ "<\x02",           "module_temp  --print",	            5,	                   7 },
	{ "<\x03",           "module_weather  --print",	            60,	                   8 },
	{ "<\x04",           "module_music  --print",	            1,	                   9 },
	{ "<\x05",           "module_dfmpeg  --print",	            5,	                   10 },
	{ "<\x01",           "module_news  --print",	            30,	                   11 },
	{ "<\x02",           "module_email  --print",	            30,	                   12 },
};

/* Commands that will run when a part of the status bar is pressed. You can add:
 * '\x<num>
 * before you print anything. Keep in mind <num> MUST be two digits.
 *
 * It must also be wrapped in a printf. Example:
 *
 * Invalid: speedwm -s "\x01This is a test"
 * Valid: speedwm -s "$(printf '\x01This is a test')"
 *
 * As for the modules here, they are only compatible with the built-in status bar by default.
 * You may add support in your own bar, though.
 *
 * Syntax: { "<command", <clicknum> },
 *
 * On click, the file /tmp/speedwm-button is created. In this file:
 *
 * 1 - Left click
 * 2 - Middle click
 * 3 - Right click
 * 4 - Scroll (up)
 * 5 - Scroll (down)
 *
 * See https://dwm.suckless.org/patches/statuscmd for more information.
 *
 * Note that if you're using the powerline status, this is NOT compatible with that.
 * See bar.h for more information.
 *
 */
static const ClickStatus clickstatuss[] = {
/*    command                                   clicknum */
	{ "module_time --click",                    1  },
	{ "module_date --click",                    2  },
	{ "module_bat --click",                     3  },
	{ "module_vol --click",                     4  },
	{ "module_ram --click",                     5  },
	{ "module_net --click",                     6  },
	{ "module_temp --click",                    7  },
	{ "module_weather --click",                 8  },
	{ "module_music --click",                   9  },
	{ "module_dfmpeg --click",                  10 },
	{ "module_news --click",                    11 },
	{ "module_email --click",                   12 },
};

/* .Xresources values to load */
StatusResourcePref res[] = {
		{ "hideemptymodule",    INTEGER, &hideemptymodule },
        { "leftpadding",        STRING,  &leftpadding },
        { "rightpadding",       STRING,  &rightpadding },
        { "separator",          STRING,  &separator },
};
