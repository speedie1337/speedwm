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
};

static int hideemptymodule  = 1; /* Hide the module if command returns nothing */
static char leftpadding[]   = ""; /* Padding on the left side of the status text */ 
static char rightpadding[]  = ""; /* Padding on the right side of the status text */
static char *separator      = ""; /* Delimiter between modules. Note that this will be applied even if a command is empty. */
