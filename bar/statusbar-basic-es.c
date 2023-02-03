int
width_status_basic_es(Bar *bar, BarWidthArg *a)
{
    if (selmon->hidestatus) {
        return 0;
    } else {
	    return TEXTW(estext);
    }
}


int
draw_status_basic_es(Bar *bar, BarDrawArg *a)
{
    if (selmon->hidestatus) {
        return 0;
    } else {
	    return drw_text(drw, a->x, 0, a->w, bh, lrpad / 2, estext, 0, True);
    }
}


int
click_status_basic_es(Bar *bar, Arg *arg, BarClickArg *a)
{
    if (selmon->hidestatus) {
        return 0;
    } else {
	    return clickstatusbar;
    }
}
