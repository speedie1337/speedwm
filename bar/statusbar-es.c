int
width_status_es(Bar *bar, BarWidthArg *a)
{
    if (selmon->hidestatus) {
        return 0;
    } else {
        return textlength(rawestext) + lrpad;
    }
}

int
draw_status_es(Bar *bar, BarDrawArg *a)
{
    if (selmon->hidestatus) {
        return 0;
    } else {
        return drawstatustext(a->x, rawestext);
    }
}

int
click_status_es(Bar *bar, Arg *arg, BarClickArg *a)
{
    if (selmon->hidestatus) {
        return 0;
    } else {
        return click_status_text(arg, a->rel_x, rawestext);
    }
}
