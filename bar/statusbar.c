static int clickstatusn;
static char lastbutton[] = "-";

int
width_status(Bar *bar, BarWidthArg *a)
{
    if (selmon->hidestatus) {
        return 0;
    } else {
        return textlength(rawstext) + lrpad;
    }
}

int
draw_status(Bar *bar, BarDrawArg *a)
{
    if (selmon->hidestatus) {
        return 0;
    } else {
        return drawstatustext(a->x, rawstext);
    }
}

int
drawstatustext(int x, char* stext)
{
	int i, w, len;
	short isCode = 0;
	char *text;
	char *p;

	len = strlen(stext);
	if (!(text = (char*) malloc(sizeof(char)*(len + 1))))
		die("malloc");
	p = text;
	copyvalidchars(text, stext);
	text[len] = '\0';

	x += lrpad / 2;
	drw_setscheme(drw, scheme[LENGTH(colors)]);
	drw->scheme[ColFg] = scheme[SchemeStatus][ColFg];
	drw->scheme[ColBg] = scheme[SchemeStatus][ColBg];

	/* process status text */
	i = -1;
	while (text[++i]) {
		if (text[i] == '^' && !isCode) {
			isCode = 1;

			text[i] = '\0';
			w = TEXTW(text) - lrpad;
			drw_text(drw, x, 0, w, bh, 0, text, 0, True);

			x += w;

			/* process code */
			while (text[++i] != '^') {
				if (text[i] == 'c') {
					char buf[8];
					if (i + 7 > len - 1) {
						i += 7;
						len = 0;
						break;
					}
					memcpy(buf, (char*)text+i+1, 7);
					buf[7] = '\0';
					drw_clr_create(drw, &drw->scheme[ColFg], buf, 0xff);
					i += 7;
				} else if (text[i] == 'b') {
					char buf[8];
					if (i + 7 > len - 1) {
						i += 7;
						len = 0;
						break;
					}
					memcpy(buf, (char*)text+i+1, 7);
					buf[7] = '\0';
					drw_clr_create(drw, &drw->scheme[ColBg], buf, 0xff);
					i += 7;
				} else if (text[i] == 'd') {
					drw->scheme[ColFg] = scheme[SchemeStatus][ColFg];
					drw->scheme[ColBg] = scheme[SchemeStatus][ColBg];
          		} else if (text[i] == 'C') {
					int c = atoi(text + ++i);
					drw_clr_create(drw, &drw->scheme[ColFg], colstatus[c], alphas[SchemeStatus][ColBg]);
				} else if (text[i] == 'B') {
					int c = atoi(text + ++i);
					drw_clr_create(drw, &drw->scheme[ColBg], colstatus[c], alphas[SchemeStatus][ColBg]);
				} else if (text[i] == 'r') {
					int rx = atoi(text + ++i);
					while (text[++i] != ',');
					int ry = atoi(text + ++i);
					while (text[++i] != ',');
					int rw = atoi(text + ++i);
					while (text[++i] != ',');
					int rh = atoi(text + ++i);

					if (ry < 0)
						ry = 0;
					if (rx < 0)
						rx = 0;

					drw_rect(drw, rx + x, ry, rw, rh, 1, 0);
				} else if (text[i] == 'f') {
					x += atoi(text + ++i);
				}
			}

			text = text + i + 1;
			len -= i + 1;
			i=-1;
			isCode = 0;
		}
	}
	if (!isCode && len) {
		w = TEXTW(text) - lrpad;
		drw_text(drw, x, 0, w, bh, 0, text, 0, True);
		x += w;
	}
	free(p);

	drw_setscheme(drw, scheme[SchemeStatus]);

	return x;
}

int
textlength(char* stext)
{
	int i, w, len;
	short isCode = 0;
	char *text;
	char *p;

	len = strlen(stext) + 1;
	if (!(text = (char*) malloc(sizeof(char)*len)))
		die("malloc");
	p = text;
	copyvalidchars(text, stext);

	/* compute width of the status text */
	w = 0;
	i = -1;
	while (text[++i]) {
		if (text[i] == '^') {
			if (!isCode) {
				isCode = 1;
				text[i] = '\0';
				w += TEXTW(text) - lrpad;
				text[i] = '^';
				if (text[++i] == 'f')
					w += atoi(text + ++i);
			} else {
				isCode = 0;
				text = text + i + 1;
				i = -1;
			}
		}
	}
	if (!isCode)
		w += TEXTW(text) - lrpad;
	free(p);
	return w;
}

int
click_status(Bar *bar, Arg *arg, BarClickArg *a)
{
    if (selmon->hidestatus) {
        return 0;
    } else {
        return click_status_text(arg, a->rel_x, rawstext);
    }
}

int
click_status_text(Arg *arg, int rel_x, char *text)
{
	int i = -1;
	int x = 0;
	char ch;
    clickstatusn = 0;
	while (text[++i]) {
		if ((unsigned char)text[i] < ' ') {
			ch = text[i];
			text[i] = '\0';
			x += textlength(text);
			text[i] = ch;
			text += i+1;
			i = -1;
           	if (x >= rel_x)
				break;
			if (ch <= LENGTH(clickstatuss))
				clickstatusn = ch;
		}
	}
	return ClkStatusText;
}

void
copyvalidchars(char *text, char *rawtext)
{
	int i = -1, j = 0;

	while (rawtext[++i]) {
		if ((unsigned char)rawtext[i] >= ' ') {
			text[j++] = rawtext[i];
		}
	}
	text[j] = '\0';
}
