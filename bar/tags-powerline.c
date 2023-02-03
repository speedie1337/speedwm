int
width_tags_pwl(Bar *bar, BarWidthArg *a)
{
    if (selmon->hidetags) {
        return 0;
    }

	int w, i;
    unsigned int occ = 0;
	int plw = drw->font->h / 2 + 1;
    char *tagtext;
	Client *c;

    if (selmon->hideemptytags) {
	    for (c = bar->mon->clients; c; c = c->next)
		    occ |= c->tags == 255 ? 0 : c->tags;
    } else {
	    for (c = bar->mon->clients; c; c = c->next)
		    occ |= c->tags;
    }

	for (w = 0, i = 0; i < LENGTH(tags); i++) {
		if (!(occ & 1 << i || bar->mon->tagset[bar->mon->seltags] & 1 << i) && selmon->hideemptytags)
			continue;

    tagtext = occ & 1 << i ? usedtags[i] : tags[i];

        w += TEXTW(tagtext) + plw;
	}
	return w + lrpad;
}

int
draw_tags_pwl(Bar *bar, BarDrawArg *a)
{
    if (selmon->hidetags) {
        return 0;
    }

	int x, w;
	int invert;
	int plw = drw->font->h / 2 + 1;
	unsigned int i, occ = 0, urg = 0;
	Client *c;
	Clr *prevscheme, *nxtscheme;
	int boxs = drw->font->h / 9;
	int boxw = drw->font->h / 6 + 2;
    char *tagtext;

	for (c = bar->mon->clients; c; c = c->next) {
        if (selmon->hideemptytags)
		    occ |= c->tags == 255 ? 0 : c->tags;
        else
		    occ |= c->tags;

		if (c->isurgent)
			urg |= c->tags;
	}
	x = a->x;
	prevscheme = scheme[SchemeBar];
	for (i = 0; i < LENGTH(tags); i++) {
		/* do not draw vacant tags */
		if (!(occ & 1 << i || bar->mon->tagset[bar->mon->seltags] & 1 << i) && selmon->hideemptytags)
			continue;
        tagtext = occ & 1 << i ? usedtags[i] : tags[i];
		invert = urg & 1 << i;
        w = TEXTW(tagtext);

        if (urg & 1 << i && allowurgent)
		    drw_settrans(drw, prevscheme, (nxtscheme = scheme[bar->mon->tagset[bar->mon->seltags] & 1 << i ? SchemeTagsSel : SchemeTagsUrg]));
        else
		    drw_settrans(drw, prevscheme, (nxtscheme = (bar->mon->tagset[bar->mon->seltags] & 1 << i ? tagscheme[i] : scheme[SchemeTagsNorm])));

        if (selmon->tagplshape)
		    drw_arrow(drw, x, 0, plw, bh, 1, 1);
        else
		    drw_arrow(drw, x, 0, plw, bh, 1, 0);

		x += plw;
		drw_setscheme(drw, nxtscheme);
		drw_text(drw, x, 0, w, bh, lrpad / 2, tagtext, invert, True);
		if (occ & 1 << i && !selmon->hideemptytags && !selmon->hideclientindicator)
			drw_rect(drw, x + boxs, boxs, boxw, boxw, bar->mon == selmon && selmon->sel && selmon->sel->tags & 1 << i, invert);
        if (underline)
     	    if (underlineall || bar->mon->tagset[bar->mon->seltags] & 1 << i)
			    drw_rect(drw, x + underlinepad, bh - underlinestroke - underlinevoffset, w - (underlinepad * 2), underlinestroke, 1, 0);

		x += w;
		prevscheme = nxtscheme;
	}
	nxtscheme = scheme[SchemeBar];

	drw_settrans(drw, prevscheme, nxtscheme);

    if (selmon->tagplshape)
	    drw_arrow(drw, x, 0, plw, bh, 1, 1);
    else
	    drw_arrow(drw, x, 0, plw, bh, 1, 0);

	return a->x + a->w;
}

int
click_tags_pwl(Bar *bar, Arg *arg, BarClickArg *a)
{
    if (selmon->hidetags) {
        return 0;
    }

	int i = 0, x = lrpad / 2;
	int plw = drw->font->h / 2 + 1;
	unsigned int occ = 0;
	Client *c;

    if (selmon->hideemptytags) {
	    for (c = bar->mon->clients; c; c = c->next)
		    occ |= c->tags == 255 ? 0 : c->tags;
    } else {
	    for (c = bar->mon->clients; c; c = c->next)
		    occ |= c->tags;
    }

	do {
		if (!(occ & 1 << i || bar->mon->tagset[bar->mon->seltags] & 1 << i) && selmon->hideemptytags)
			continue;
        x += TEXTW(occ & 1 << i ? usedtags[i] : tags[i]) + plw;
	} while (a->rel_x >= x && ++i < LENGTH(tags));
	if (i < LENGTH(tags)) {
		arg->ui = 1 << i;
	}
	return clicktags;
}
