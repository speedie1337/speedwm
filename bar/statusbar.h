static int width_status(Bar *bar, BarWidthArg *a);
static int draw_status(Bar *bar, BarDrawArg *a);
static int drawstatustext(int x, char *text);
static int textlength(char *stext);
static int click_status(Bar *bar, Arg *arg, BarClickArg *a);
static int click_status_text(Arg *arg, int rel_x, char *text);
static void copyvalidchars(char *text, char *rawtext);
