int
width_title(Bar *bar, BarWidthArg *a)
{
    if (selmon->hidetitle) {
        return 0;
    }

	return a->max_width;
}

int
draw_title(Bar *bar, BarDrawArg *a)
{
    if (selmon->hidetitle) {
        return 0;
    }

	int n = 0, scm, remainder = 0, tabw, padding;
	unsigned int i;
	int x = a->x, w = a->w;
    Client *c;

	for (c = bar->mon->clients; c; c = c->next)
		if (ISVISIBLE(c))
			n++;

	if (n > 0) {
        if (!selmon->hideunselectedtitle) {
            remainder = w % n;
            tabw = w / n;
        } else {
            remainder = w;
            tabw = w;
        }

		for (i = 0, c = bar->mon->clients; c; c = c->next, i++) {
			if (!ISVISIBLE(c))
				continue;

			if (bar->mon->sel == c)
				scm = SchemeTitleSel;
            else if (HIDDEN(c))
				scm = SchemeTitleHidden;
            else
				scm = SchemeTitleNorm;

            if (!colorselectedtitle && !HIDDEN(c))
                scm = SchemeTitleNorm;
            else if (!colorselectedtitle && HIDDEN(c))
                scm = SchemeTitleHidden;

           	/* hide unselected title */
			if (bar->mon->sel != c && selmon->hideunselectedtitle) {
			    continue;
			}

			padding = lrpad / 2;
			if (TEXTW(c->name) < tabw && titleposition)
				padding = (tabw - TEXTW(c->name) + lrpad) / 2;

            if (!selmon->hideunselectedtitle) drw_setscheme(drw, scheme[scm]);
            #if USEWINICON
            if (!selmon->hideicon) {
			    drw_text(drw, x, 0, tabw + (i < remainder ? 1 : 0), bh, padding + (c->icw + iconspacing), c->name, 0, False);
            } else {
			    drw_text(drw, x, 0, tabw + (i < remainder ? 1 : 0), bh, padding, c->name, 0, False);
            }

            if (c->icon && !selmon->hideicon)
                drw_pic(drw, x + padding, (bh - c->ich) / 2, c->icw, c->ich, c->icon);
            #else
			drw_text(drw, x, 0, tabw + (i < remainder ? 1 : 0), bh, padding, c->name, 0, False);
            #endif

			x += tabw + (i < remainder ? 1 : 0);
		}
	}
	return a->x + a->w;
}

int
click_title(Bar *bar, Arg *arg, BarClickArg *a)
{
    if (selmon->hidetitle) {
        return 0;
    }

	int x = 0, n = 0;
	Client *c;

	for (c = bar->mon->clients; c; c = c->next)
		if (ISVISIBLE(c))
			n++;

	c = bar->mon->clients;

	do {
		if (!c || !ISVISIBLE(c))
			continue;
		else
			x += (1.0 / (double)n) * a->rel_w;
	} while (c && a->rel_x > x && (c = c->next));

	if (c) {
		arg->v = c;
		return clicktitle;
	}
	return -1;
}
