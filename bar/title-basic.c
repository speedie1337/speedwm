int
width_title_basic(Bar *bar, BarWidthArg *a)
{
    if (selmon->hidetitle) {
        return 0;
    }

	return a->max_width;
}

int
draw_title_basic(Bar *bar, BarDrawArg *a)
{
    if (selmon->hidetitle) {
        return 0;
    }

	int boxs = drw->font->h / 9;
	int boxw = drw->font->h / 6 + 2;
	int x = a->x, w = a->w;
    int padding = lrpad / 2;
	Monitor *m = bar->mon;
    Client *c = m->sel;

	if (m->sel) {
        if (colorselectedtitle)
		    drw_setscheme(drw, scheme[m == selmon ? SchemeTitleSel : SchemeTitleNorm]);
        else
		    drw_setscheme(drw, scheme[m == selmon ? SchemeTitleNorm : SchemeTitleNorm]);

		if (TEXTW(c->name) < w && titleposition)
			padding = (w - TEXTW(c->name) + lrpad) / 2;

        #if USEWINICON
        if (!selmon->hideicon)
		    drw_text(drw, x, 0, w, bh, padding + (c->icon ? c->icw + iconspacing : 0), c->name, 0, False);
        else
		    drw_text(drw, x, 0, w, bh, padding, c->name, 0, False);
        if (c->icon && !selmon->hideicon)
            drw_pic(drw, x + padding, (bh - c->ich) / 2, c->icw, c->ich, c->icon);
        #else
		drw_text(drw, x, 0, w, bh, padding, c->name, 0, False);
        #endif
		if (c->isfloating && !selmon->hidefloating)
			drw_rect(drw, x + boxs, boxs, boxw, boxw, c->isfixed, 0);
        if (c->issticky && !selmon->hidesticky)
			drw_polygon(drw, x + boxs, c->isfloating ? boxs * 2 + boxw : boxs, stickyiconbb.x, stickyiconbb.y, boxw, boxw * stickyiconbb.y / stickyiconbb.x, stickyicon, LENGTH(stickyicon), Nonconvex, c->tags & bar->mon->tagset[bar->mon->seltags]);
	} else {
		drw_setscheme(drw, scheme[SchemeBar]);
		drw_rect(drw, x, 0, w, bh, 1, 1);
	}
	return x + w;
}

int
click_title_basic(Bar *bar, Arg *arg, BarClickArg *a)
{
    if (selmon->hidetitle) {
        return 0;
    }

	return clicktitle;
}
