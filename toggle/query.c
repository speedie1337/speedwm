/* This contains functions for getting speedwm queries.
 *
 * If you don't need some of these, you can simply remove them from this file and query.h.
 * query handles all the other functions.
 */

int
query(char *args)
{
    int i = 0;

    for (i = 0; i < LENGTH(queries); i++) {
        if (strcmp(queries[i].query_text, args) == 0) {
            return queries[i].func();
        }
    }

    int ps = fprintf(stderr, "speedwm: Not a valid query.\n");
    if (ps) return 0; else return 1;
}

int
get_queries()
{
    int i = 0;
    int ps = 0;

    for (i = 0; i < LENGTH(queries); i++) {
        ps = fprintf(stdout, "%s\n", queries[i].query_text);
    }

    if (ps) return 0; else return 1;
}

int
get_font()
{
    int i = 0;
    int ps = 0;

    ps = fprintf(stdout, "%s\n", font);

    if (ps) return 0; else return 1;
}

int
get_barpos()
{
    int ps = fprintf(stdout, "%d\n", barposition);
    if (ps) return 0; else return 1;
}

int
get_mfact()
{
    int ps = fprintf(stdout, "%f\n", mfact);
    if (ps) return 0; else return 1;
}

int
get_mastercount()
{
    int ps = fprintf(stdout, "%d\n", mastercount);
    if (ps) return 0; else return 1;
}

int
get_resizehints()
{
    int ps = fprintf(stdout, "%d\n", mastercount);
    if (ps) return 0; else return 1;
}

int
get_lockfullscreen()
{
    int ps = fprintf(stdout, "%d\n", lockfullscreen);
    if (ps) return 0; else return 1;
}

int
get_snap()
{
    int ps = fprintf(stdout, "%d\n", snap);
    if (ps) return 0; else return 1;
}

int
get_hidebar()
{
    int ps = fprintf(stdout, "%d\n", hidebar);
    if (ps) return 0; else return 1;
}

int
get_bordersize()
{
    int ps = fprintf(stdout, "%d\n", bordersize);
    if (ps) return 0; else return 1;
}

int
get_layouts()
{
    int i = 0;
    int ps = 0;

    for (i = 0; i < LENGTH(layouts); i++) {
        ps = fprintf(stdout, "%s\n", layouts[i].symbol);
    }

    if (ps) return 0; else return 1;
}

int
get_numlayouts()
{
    int i = 0;
    int ps = 0;

    for (i = 0; i < LENGTH(layouts); i++) {}

    ps = fprintf(stdout, "%d\n", i);

    if (ps) return 0; else return 1;
}

int
get_numtags()
{
    int i = 0;
    int ps = 0;

    for (i = 0; i < LENGTH(tags); i++) {}

    ps = fprintf(stdout, "%d\n", i);

    if (ps) return 0; else return 1;
}

int
get_tags()
{
    int i = 0;
    int ps = 0;

    for (i = 0; i < LENGTH(tags); i++) {
        ps = fprintf(stdout, "%s\n", tags[i]);
    }

    if (ps) return 0; else return 1;
}

int
get_used_tags()
{
    int i = 0;
    int ps = 0;

    for (i = 0; i < LENGTH(usedtags); i++) {
        ps = fprintf(stdout, "%s\n", usedtags[i]);
    }

    if (ps) return 0; else return 1;
}

int
get_barpadding_ov() {
    int ps = fprintf(stdout, "%d\n", barpaddingov);
    if (ps) return 0; else return 1;
}

int
get_barpadding_oh() {
    int ps = fprintf(stdout, "%d\n", barpaddingoh);
    if (ps) return 0; else return 1;
}

int
get_barpadding_iv() {
    int ps = fprintf(stdout, "%d\n", barpaddingiv);
    if (ps) return 0; else return 1;
}

int
get_barpadding_ih() {
    int ps = fprintf(stdout, "%d\n", barpaddingih);
    if (ps) return 0; else return 1;
}
