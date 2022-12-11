int
width_ltsymbol(Bar *bar, BarWidthArg *a)
{
    if (selmon->hidelayout) {
        return 0;
    } else {
	    return TEXTW(bar->mon->ltsymbol);
    }
}

int
draw_ltsymbol(Bar *bar, BarDrawArg *a)
{
    if (selmon->hidelayout) {
        return 0;
    } else {
	    return drw_text(drw, a->x, 0, a->w, bh, lrpad / 2, bar->mon->ltsymbol, 0, False);
    }
}

int
click_ltsymbol(Bar *bar, Arg *arg, BarClickArg *a)
{
    if (selmon->hidelayout) {
        return 0;
    } else {
	   return ClkLtSymbol;
    }
}
