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
 *
 * See https://dwm.suckless.org/patches/statuscmd for more information.
 *
 * Note that if you're using the powerline status, this is NOT compatible with that.
 * See bar.h for more information.
 *
 */
static const ClickStatus clickstatuss[] = {
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
};
