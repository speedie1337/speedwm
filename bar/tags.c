int
width_tags(Bar *bar, BarWidthArg *a)
{
    if (selmon->hidetags) {
        return 0;
    }

	int w, i;
    Client *c;
    unsigned int occ = 0;

    if (!selmon->hideemptytags) {
	    for (c = bar->mon->clients; c; c = c->next)
            occ |= c->tags;
    } else {
	    for (c = bar->mon->clients; c; c = c->next)
		    occ |= c->tags == 255 ? 0 : c->tags;
    }

	for (w = 0, i = 0; i < LENGTH(tags); i++) {
       	if (!(occ & 1 << i || bar->mon->tagset[bar->mon->seltags] & 1 << i) && selmon->hideemptytags)
			continue;
        w += TEXTW(occ & 1 << i ? usedtags[i] : tags[i]);
	}
	return w;
}

int
draw_tags(Bar *bar, BarDrawArg *a)
{
    if (selmon->hidetags) {
        return 0;
    }

	int invert;
	int w, x = a->x;
	int boxs = drw->font->h / 9;
	int boxw = drw->font->h / 6 + 2;
	unsigned int i, occ = 0, urg = 0;
    const char *tagtext;
	Client *c;
	Monitor *m = bar->mon;

	for (c = m->clients; c; c = c->next) {
        if (!selmon->hideemptytags) {
            occ |= c->tags;
        } else {
       		occ |= c->tags == 255 ? 0 : c->tags;
        }

		if (c->isurgent)
			urg |= c->tags;
	}

	for (i = 0; i < LENGTH(tags); i++) {
       	if (!(occ & 1 << i || bar->mon->tagset[bar->mon->seltags] & 1 << i) && selmon->hideemptytags)
			continue;

		invert = urg & 1 << i;
        tagtext = occ & 1 << i ? usedtags[i] : tags[i];
        w = TEXTW(tagtext);
       	drw_setscheme(drw, (bar->mon->tagset[bar->mon->seltags] & 1 << i ? tagscheme[i] : scheme[SchemeBar]));
		drw_text(drw, x, 0, w, bh, lrpad / 2, tagtext, invert, True);
		if (occ & 1 << i && !selmon->hideemptytags && !selmon->hideclientindicator)
			drw_rect(drw, x + boxs, boxs, boxw, boxw,
				m == selmon && selmon->sel && selmon->sel->tags & 1 << i, invert);
        if (underline)
     	    if (underlineall || bar->mon->tagset[bar->mon->seltags] & 1 << i)
			    drw_rect(drw, x + underlinepad, bh - underlinestroke - underlinevoffset, w - (underlinepad * 2), underlinestroke, 1, 0);

		x += w;
	}

	return x;
}

int
click_tags(Bar *bar, Arg *arg, BarClickArg *a)
{
    if (selmon->hidetags) {
        return 0;
    }

	int i = 0, x = lrpad / 2;
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
        x += TEXTW(occ & 1 << i ? usedtags[i] : tags[i]);
	} while (a->rel_x >= x && ++i < LENGTH(tags));
	if (i < LENGTH(tags)) {
		arg->ui = 1 << i;
	}
	return clicktags;
}
