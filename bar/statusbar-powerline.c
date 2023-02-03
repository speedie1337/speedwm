int
click_status_pwl(Bar *bar, Arg *arg, BarClickArg *a)
{
    if (selmon->hidestatus) {
        return 0;
    } else {
        return clickstatusbar;
    }
}

int
width_status_pwl(Bar *bar, BarWidthArg *a)
{
    if (selmon->hidestatus) {
        return 0;
    } else {
	    return widthpowerlinestatus(rawstext);
    }
}

int
draw_status_pwl(Bar *bar, BarDrawArg *a)
{
    if (selmon->hidestatus) {
        return 0;
    } else {
        return drawpowerlinestatus(a->x + a->w, rawstext);
    }
}

int
widthpowerlinestatus(char *stext)
{
	char status[512];
	int w = 0, i, n = strlen(stext);
	int plw = drw->font->h / 2 + 1;
	char *bs, bp = '|';
	strcpy(status, stext);

	for (i = n, bs = &status[n-1]; i >= 0; i--, bs--) {
		if (*bs == '<' || *bs == '\\' || *bs == '>' || *bs == '|') { /* block start */
			if (bp != '|')
				w += plw;
			w += TEXTW(bs+2);
			bp = *bs;
			*bs = 0;
		}
	}
	if (bp != '|')
		w += plw * 2;

	return w;
}

int
drawpowerlinestatus(int xpos, char *stext)
{
	char status[512];
	int i, n = strlen(stext), cn = 0;
	int x = xpos, w = 0;
	int plw = drw->font->h / 2 + 1;
	char *bs, bp = '|';
	Clr *prevscheme = scheme[SchemeBar], *nxtscheme;
	strcpy(status, stext);

	for (i = n, bs = &status[n-1]; i >= 0; i--, bs--) {
		if (*bs == '<' || *bs == '\\' || *bs == '>' || *bs == '|') { /* block start */
            cn = ((int) *(bs+1)) + 1;

			if (cn < LENGTH(powerlinecolors)) {
				drw_settrans(drw, prevscheme, (nxtscheme = powerlinescheme[cn]));
			} else {
				drw_settrans(drw, prevscheme, (nxtscheme = powerlinescheme[0]));
			}

			if (bp != '|') {
               	drw_arrow(drw, x - plw, 0, plw, bh, bp == '\\' || bp == '>' ? 1 : 0, bp == '<' ? 0 : 1);

				x -= plw;
			}

			drw_setscheme(drw, nxtscheme);
			w = TEXTW(bs+2);
     		drw_text(drw, x - w, 0, w, bh, lrpad / 2, bs+2, 0, True);
			x -= w;

			bp = *bs;
			*bs = 0;
			prevscheme = nxtscheme;
		}
	}
	if (bp != '|') {
        drw_settrans(drw, prevscheme, scheme[SchemeStatus]);
		drw_arrow(drw, x - plw, 0, plw, bh, bp == '\\' || bp == '>' ? 1 : 0, bp == '<' ? 0 : 1);
		drw_rect(drw, x - 2 * plw, 0, plw, bh, 1, 1);
		x -= plw * 2;
	}

	return xpos - x;
}
