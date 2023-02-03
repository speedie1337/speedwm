int
click_status_pwl_es(Bar *bar, Arg *arg, BarClickArg *a)
{
    if (selmon->hidestatus) {
        return 0;
    } else {
        return clickstatusbar;
    }
}

int
width_status_pwl_es(Bar *bar, BarWidthArg *a)
{
    if (selmon->hidestatus) {
        return 0;
    } else {
	    return widthpowerlinestatus(rawestext);
    }
}

int
draw_status_pwl_es(Bar *bar, BarDrawArg *a)
{
    if (selmon->hidestatus) {
        return 0;
    } else {
        return drawpowerlinestatus(a->x + a->w, rawestext);
    }
}
