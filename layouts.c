/* This C code handles all built in layouts. */

void
empty(Monitor *m) {
	Client *c;
	for (c = m->stack; c; c = c->snext) {
		if(ISVISIBLE(c)) {
			XMoveWindow(dpy, c->win, WIDTH(c) * -2, c->y);
		}
	}
}

typedef struct {
	void (*arrange)(Monitor *, int, int, int, int, int, int, int);
} LayoutArranger;

typedef struct {
	void (*arrange)(Monitor *, int, int, int, int, int, int, int, int, int);
} TileArranger;

static const LayoutArranger tilelayouts[] = {
	{ layout_no_split },
	{ layout_split_vertical },
	{ layout_split_horizontal },
	{ layout_split_centered_vertical },
	{ layout_split_centered_horizontal },
	{ layout_split_vertical_dual_stack },
	{ layout_split_horizontal_dual_stack },
	{ layout_floating_master },
	{ layout_split_vertical_fixed },
	{ layout_split_horizontal_fixed },
	{ layout_split_centered_vertical_fixed },
	{ layout_split_centered_horizontal_fixed },
	{ layout_split_vertical_dual_stack_fixed },
	{ layout_split_horizontal_dual_stack_fixed },
	{ layout_floating_master_fixed },
};

static const TileArranger tiles[] = {
	{ arrange_top_to_bottom },
	{ arrange_left_to_right },
	{ arrange_monocle },
	{ arrange_dynamicgrid },
	{ arrange_dynamicgrid_alt1 },
	{ arrange_dynamicgrid_alt2 },
	{ arrange_gridmode },
	{ arrange_horizgrid },
	{ arrange_dwindle },
	{ arrange_spiral },
	{ arrange_tatami },
};

static void
getfactsforrange(Monitor *m, int an, int ai, int size, int *rest, float *fact)
{
	int i;
	float facts;
	Client *c;
	int total = 0;

	facts = 0;
	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i >= ai && i < (ai + an))
			facts += c->cfact;

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i >= ai && i < (ai + an))
			total += size * (c->cfact / facts);

	*rest = size - total;
	*fact = facts;
}

static void
layout_no_split(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n)
{
	(&tiles[m->ltaxis[master]])->arrange(m, x, y, h, w, ih, iv, n, n, 0);
}

static void
layout_split_vertical(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n)
{
	/* Split master into master + stack if we have enough clients */
	if (m->mastercount && n > m->mastercount) {
		layout_split_vertical_fixed(m, x, y, h, w, ih, iv, n);
	} else {
		layout_no_split(m, x, y, h, w, ih, iv, n);
	}
}

static void
layout_split_vertical_fixed(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n)
{
	int sw, sx;

	sw = (w - iv) * (1 - m->mfact);
	w = (w - iv) * m->mfact;
	if (m->ltaxis[layout] < 0) { // mirror
		sx = x;
		x += sw + iv;
	} else {
		sx = x + w + iv;
	}

	(&tiles[m->ltaxis[master]])->arrange(m, x, y, h, w, ih, iv, n, m->mastercount, 0);
	(&tiles[m->ltaxis[stack]])->arrange(m, sx, y, h, sw, ih, iv, n, n - m->mastercount, m->mastercount);
}

static void
layout_split_vertical_dual_stack(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n)
{
	/* Split master into master + stack if we have enough clients */
	if (!m->mastercount || n <= m->mastercount) {
		layout_no_split(m, x, y, h, w, ih, iv, n);
	} else if (n <= m->mastercount + (m->stackcount ? m->stackcount : 1)) {
		layout_split_vertical(m, x, y, h, w, ih, iv, n);
	} else {
		layout_split_vertical_dual_stack_fixed(m, x, y, h, w, ih, iv, n);
	}
}

static void
layout_split_vertical_dual_stack_fixed(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n)
{
	int sh, sw, sx, oy, sc;

	if (m->stackcount)
		sc = m->stackcount;
	else
		sc = (n - m->mastercount) / 2 + ((n - m->mastercount) % 2 > 0 ? 1 : 0);

	sw = (w - iv) * (1 - m->mfact);
	sh = (h - ih) / 2;
	w = (w - iv) * m->mfact;
	oy = y + sh + ih;
	if (m->ltaxis[layout] < 0) { // mirror
		sx = x;
		x += sw + iv;
	} else {
		sx = x + w + iv;
	}

	(&tiles[m->ltaxis[master]])->arrange(m, x, y, h, w, ih, iv, n, m->mastercount, 0);
	(&tiles[m->ltaxis[stack]])->arrange(m, sx, y, sh, sw, ih, iv, n, sc, m->mastercount);
	(&tiles[m->ltaxis[stack2]])->arrange(m, sx, oy, sh, sw, ih, iv, n, n - m->mastercount - sc, m->mastercount + sc);
}

static void
layout_split_horizontal(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n)
{
	/* Split master into master + stack if we have enough clients */
	if (m->mastercount && n > m->mastercount) {
		layout_split_horizontal_fixed(m, x, y, h, w, ih, iv, n);
	} else {
		layout_no_split(m, x, y, h, w, ih, iv, n);
	}
}

static void
layout_split_horizontal_fixed(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n)
{
	int sh, sy;

	sh = (h - ih) * (1 - m->mfact);
	h = (h - ih) * m->mfact;
	if (m->ltaxis[layout] < 0) { // mirror
		sy = y;
		y += sh + ih;
	} else {
		sy = y + h + ih;
	}

	(&tiles[m->ltaxis[master]])->arrange(m, x, y, h, w, ih, iv, n, m->mastercount, 0);
	(&tiles[m->ltaxis[stack]])->arrange(m, x, sy, sh, w, ih, iv, n, n - m->mastercount, m->mastercount);
}

static void
layout_split_horizontal_dual_stack(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n)
{
	/* Split master into master + stack if we have enough clients */
	if (!m->mastercount || n <= m->mastercount) {
		layout_no_split(m, x, y, h, w, ih, iv, n);
	} else if (n <= m->mastercount + (m->stackcount ? m->stackcount : 1)) {
		layout_split_horizontal(m, x, y, h, w, ih, iv, n);
	} else {
		layout_split_horizontal_dual_stack_fixed(m, x, y, h, w, ih, iv, n);
	}
}

static void
layout_split_horizontal_dual_stack_fixed(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n)
{
	int sh, sy, ox, sc, sw;

	if (m->stackcount)
		sc = m->stackcount;
	else
		sc = (n - m->mastercount) / 2 + ((n - m->mastercount) % 2 > 0 ? 1 : 0);

	sh = (h - ih) * (1 - m->mfact);
	h = (h - ih) * m->mfact;
	sw = (w - iv) / 2;
	ox = x + sw + iv;
	if (m->ltaxis[layout] < 0) { // mirror
		sy = y;
		y += sh + ih;
	} else {
		sy = y + h + ih;
	}

	(&tiles[m->ltaxis[master]])->arrange(m, x, y, h, w, ih, iv, n, m->mastercount, 0);
	(&tiles[m->ltaxis[stack]])->arrange(m, x, sy, sh, sw, ih, iv, n, sc, m->mastercount);
	(&tiles[m->ltaxis[stack2]])->arrange(m, ox, sy, sh, sw, ih, iv, n, n - m->mastercount - sc, m->mastercount + sc);
}

static void
layout_split_centered_vertical(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n)
{
	/* Split master into master + stack if we have enough clients */
	if (!m->mastercount || n <= m->mastercount) {
		layout_no_split(m, x, y, h, w, ih, iv, n);
	} else if (n <= m->mastercount + (m->stackcount ? m->stackcount : 1)) {
		layout_split_vertical(m, x, y, h, w, ih, iv, n);
	} else {
		layout_split_centered_vertical_fixed(m, x, y, h, w, ih, iv, n);
	}
}

static void
layout_split_centered_vertical_fixed(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n)
{
	int sw, sx, ox, sc;

	if (m->stackcount)
		sc = m->stackcount;
	else
		sc = (n - m->mastercount) / 2 + ((n - m->mastercount) % 2 > 0 ? 1 : 0);

	sw = (w - 2*iv) * (1 - m->mfact) / 2;
	w = (w - 2*iv) * m->mfact;
	if (m->ltaxis[layout] < 0)  { // mirror
		sx = x;
		x += sw + iv;
		ox = x + w + iv;
	} else {
		ox = x;
		x += sw + iv;
		sx = x + w + iv;
	}

	(&tiles[m->ltaxis[master]])->arrange(m, x, y, h, w, ih, iv, n, m->mastercount, 0);
	(&tiles[m->ltaxis[stack]])->arrange(m, sx, y, h, sw, ih, iv, n, sc, m->mastercount);
	(&tiles[m->ltaxis[stack2]])->arrange(m, ox, y, h, sw, ih, iv, n, n - m->mastercount - sc, m->mastercount + sc);
}

static void
layout_split_centered_horizontal(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n)
{
	/* Split master into master + stack if we have enough clients */
	if (!m->mastercount || n <= m->mastercount) {
		layout_no_split(m, x, y, h, w, ih, iv, n);
	} else if (n <= m->mastercount + (m->stackcount ? m->stackcount : 1)) {
		layout_split_horizontal(m, x, y, h, w, ih, iv, n);
	} else {
		layout_split_centered_horizontal_fixed(m, x, y, h, w, ih, iv, n);
	}
}

static void
layout_split_centered_horizontal_fixed(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n)
{
	int sh, sy, oy, sc;

	if (m->stackcount)
		sc = m->stackcount;
	else
		sc = (n - m->mastercount) / 2 + ((n - m->mastercount) % 2 > 0 ? 1 : 0);

	sh = (h - 2*ih) * (1 - m->mfact) / 2;
	h = (h - 2*ih) * m->mfact;
	if (m->ltaxis[layout] < 0)  { // mirror
		sy = y;
		y += sh + ih;
		oy = y + h + ih;
	} else {
		oy = y;
		y += sh + ih;
		sy = y + h + ih;
	}

	(&tiles[m->ltaxis[master]])->arrange(m, x, y, h, w, ih, iv, n, m->mastercount, 0);
	(&tiles[m->ltaxis[stack]])->arrange(m, x, sy, sh, w, ih, iv, n, sc, m->mastercount);
	(&tiles[m->ltaxis[stack2]])->arrange(m, x, oy, sh, w, ih, iv, n, n - m->mastercount - sc, m->mastercount + sc);
}

static void
layout_floating_master(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n)
{
	/* Split master into master + stack if we have enough clients */
	if (!m->mastercount || n <= m->mastercount) {
		layout_no_split(m, x, y, h, w, ih, iv, n);
	} else {
		layout_floating_master_fixed(m, x, y, h, w, ih, iv, n);
	}
}

static void
layout_floating_master_fixed(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n)
{
	int mh, mw;

	/* Draw stack area first */
	(&tiles[m->ltaxis[stack]])->arrange(m, x, y, h, w, ih, iv, n, n - m->mastercount, m->mastercount);

	if (w > h) {
		mw = w * m->mfact;
		mh = h * 0.9;
	} else {
		mw = w * 0.9;
		mh = h * m->mfact;
	}
	x = x + (w - mw) / 2;
	y = y + (h - mh) / 2;

	(&tiles[m->ltaxis[master]])->arrange(m, x, y, mh, mw, ih, iv, n, m->mastercount, 0);
}

static void
arrange_left_to_right(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n, int an, int ai)
{
	int i, rest;
	float facts, fact = 1;
	Client *c;

	if (ai + an > n)
		an = n - ai;

	w -= iv * (an - 1);
	getfactsforrange(m, an, ai, w, &rest, &facts);
	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
		if (i >= ai && i < (ai + an)) {
			fact = c->cfact;
			resize(c, x, y, w * (fact / facts) + ((i - ai) < rest ? 1 : 0) - (2*c->bw), h - (2*c->bw), 0);
			x += WIDTH(c) + iv;
		}
	}
}

static void
arrange_top_to_bottom(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n, int an, int ai)
{
	int i, rest;
	float facts, fact = 1;
	Client *c;

	if (ai + an > n)
		an = n - ai;

	h -= ih * (an - 1);
	getfactsforrange(m, an, ai, h, &rest, &facts);
	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
		if (i >= ai && i < (ai + an)) {
			fact = c->cfact;
			resize(c, x, y, w - (2*c->bw), h * (fact / facts) + ((i - ai) < rest ? 1 : 0) - (2*c->bw), 0);
			y += HEIGHT(c) + ih;
		}
	}
}

static void
arrange_monocle(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n, int an, int ai)
{
	int i;
	Client *c;

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i >= ai && i < (ai + an))
			resize(c, x, y, w - (2*c->bw), h - (2*c->bw), 0);
}

static void
arrange_gridmode(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n, int an, int ai)
{
	int i, cols, rows, ch, cw, cx, cy, cc, cr, chrest, cwrest; // counters
	Client *c;

	/* grid dimensions */
	for (rows = 0; rows <= an/2; rows++)
		if (rows*rows >= an)
			break;
	cols = (rows && (rows - 1) * rows >= an) ? rows - 1 : rows;

	/* window geoms (cell height/width) */
	ch = (h - ih * (rows - 1)) / (rows ? rows : 1);
	cw = (w - iv * (cols - 1)) / (cols ? cols : 1);
	chrest = h - ih * (rows - 1) - ch * rows;
	cwrest = w - iv * (cols - 1) - cw * cols;
	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
		if (i >= ai && i < (ai + an)) {
			cc = ((i - ai) / rows); // client column number
			cr = ((i - ai) % rows); // client row number
			cx = x + cc * (cw + iv) + MIN(cc, cwrest);
			cy = y + cr * (ch + ih) + MIN(cr, chrest);
			resize(c, cx, cy, cw + (cc < cwrest ? 1 : 0) - 2*c->bw, ch + (cr < chrest ? 1 : 0) - 2*c->bw, False);
		}
	}
}

static void
arrange_horizgrid(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n, int an, int ai)
{
	int ntop, nbottom, rh, rest;

	/* Exception when there is only one client; don't split into two rows */
	if (an == 1) {
		arrange_monocle(m, x, y, h, w, ih, iv, n, an, ai);
		return;
	}

	ntop = an / 2;
	nbottom = an - ntop;
	rh = (h - ih) / 2;
	rest = h - ih - rh * 2;
	arrange_left_to_right(m, x, y, rh + rest, w, ih, iv, n, ntop, ai);
	arrange_left_to_right(m, x, y + rh + ih + rest, rh, w, ih, iv, n, nbottom, ai + ntop);
}

static void
arrange_dynamicgrid(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n, int an, int ai)
{
	int i, cols, rows, ch, cw, cn, rn, cc, rrest, crest; // counters
	Client *c;

	/* grid dimensions */
	for (cols = 1; cols <= an/2; cols++)
		if (cols*cols >= an)
			break;
	if (an == 5) /* set layout against the general calculation: not 1:2:2, but 2:3 */
		cols = 2;
	rows = an/cols;
	cn = rn = cc = 0; // reset column no, row no, client count

	ch = (h - ih * (rows - 1)) / rows;
	rrest = (h - ih * (rows - 1)) - ch * rows;
	cw = (w - iv * (cols - 1)) / cols;
	crest = (w - iv * (cols - 1)) - cw * cols;

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
		if (i >= ai && i < (ai + an)) {
			if (cc/rows + 1 > cols - an%cols) {
				rows = an/cols + 1;
				ch = (h - ih * (rows - 1)) / rows;
				rrest = (h - ih * (rows - 1)) - ch * rows;
			}
			resize(c,
				x,
				y + rn*(ch + ih) + MIN(rn, rrest),
				cw + (cn < crest ? 1 : 0) - 2*c->bw,
				ch + (rn < rrest ? 1 : 0) - 2*c->bw,
				0);
			rn++;
			cc++;
			if (rn >= rows) {
				rn = 0;
				x += cw + ih + (cn < crest ? 1 : 0);
				cn++;
			}
		}
	}
}

static void
arrange_dynamicgrid_alt1(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n, int an, int ai)
{
	int i, cols, rows, rest, ch;

	/* grid dimensions */
	for (cols = 1; cols <= an/2; cols++)
		if (cols*cols >= an)
			break;
	rows = (cols && (cols - 1) * cols >= an) ? cols - 1 : cols;
	ch = (h - ih * (rows - 1)) / (rows ? rows : 1);
	rest = (h - ih * (rows - 1)) - ch * rows;

	for (i = 0; i < rows; i++) {
		arrange_left_to_right(m, x, y, ch + (i < rest ? 1 : 0), w, ih, iv, n, MIN(cols, an - i*cols), ai + i*cols);
		y += ch + (i < rest ? 1 : 0) + ih;
	}
}

static void
arrange_dynamicgrid_alt2(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n, int an, int ai)
{
	int i, cols, rows, rest, cw;

	/* grid dimensions */
	for (rows = 0; rows <= an/2; rows++)
		if (rows*rows >= an)
			break;
	cols = (rows && (rows - 1) * rows >= an) ? rows - 1 : rows;
	cw = (w - iv * (cols - 1)) / (cols ? cols : 1);
	rest = (w - iv * (cols - 1)) - cw * cols;

	for (i = 0; i < cols; i++) {
		arrange_top_to_bottom(m, x, y, h, cw + (i < rest ? 1 : 0), ih, iv, n, MIN(rows, an - i*rows), ai + i*rows);
		x += cw + (i < rest ? 1 : 0) + iv;
	}
}

static void
arrange_fibonacci(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n, int an, int ai, int s)
{
	int i, j, nv, hrest = 0, wrest = 0, nx = x, ny = y, nw = w, nh = h, r = 1;
	Client *c;

	for (i = 0, j = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), j++) {
		if (j >= ai && j < (ai + an)) {
			if (r) {
				if ((i % 2 && ((nh - ih) / 2) <= (bh + 2*c->bw)) || (!(i % 2) && ((nw - iv) / 2) <= (bh + 2*c->bw))) {
					r = 0;
				}
				if (r && i < an - 1) {
					if (i % 2) {
						nv = (nh - ih) / 2;
						hrest = nh - 2*nv - ih;
						nh = nv;
					} else {
						nv = (nw - iv) / 2;
						wrest = nw - 2*nv - iv;
						nw = nv;
					}

					if ((i % 4) == 2 && !s)
						nx += nw + iv;
					else if ((i % 4) == 3 && !s)
						ny += nh + ih;
				}
				if ((i % 4) == 0) {
					if (s) {
						ny += nh + ih;
						nh += hrest;
					} else {
						nh -= hrest;
						ny -= nh + ih;
					}
				} else if ((i % 4) == 1) {
					nx += nw + iv;
					nw += wrest;
				} else if ((i % 4) == 2) {
					ny += nh + ih;
					nh += hrest;
					if (i < n - 1)
						nw += wrest;
				} else if ((i % 4) == 3) {
					if (s) {
						nx += nw + iv;
						nw -= wrest;
					} else {
						nw -= wrest;
						nx -= nw + iv;
						nh += hrest;
					}
				}
				if (i == 0)	{
					if (an != 1) {
						nw = (w - iv) - (w - iv) * (1 - m->mfact);
						wrest = 0;
					}
					ny = y;
				} else if (i == 1)
					nw = w - nw - iv;
				i++;
			}

			resize(c, nx, ny, nw - 2 * c->bw, nh - 2*c->bw, False);
		}
	}
}

static void
arrange_dwindle(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n, int an, int ai)
{
	arrange_fibonacci(m, x, y, h, w, ih, iv, n, an, ai, 1);
}

static void
arrange_spiral(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n, int an, int ai)
{
	arrange_fibonacci(m, x, y, h, w, ih, iv, n, an, ai, 0);
}

static void
arrange_tatami(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n, int an, int ai)
{
	unsigned int i, j, nx, ny, nw, nh, tnx, tny, tnw, tnh, nhrest, hrest, wrest, areas, mats, cats;
	Client *c;

	nx = x;
	ny = y;
	nw = w;
	nh = h;

	mats = an / 5;
	cats = an % 5;
	hrest = 0;
	wrest = 0;

	areas = mats + (cats > 0);
	nh = (h - ih * (areas - 1)) / areas;
	nhrest = (h - ih * (areas - 1)) % areas;

	for (i = 0, j = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), j++) {
		if (j >= ai && j < (ai + an)) {

			tnw = nw;
			tnx = nx;
			tnh = nh;
			tny = ny;

			if (j < ai + cats) {
				/* Arrange cats (all excess clients that can't be tiled as mats). Cats sleep on mats. */

	 			switch (cats) {
				case 1: // fill
					break;
				case 2: // up and down
					if ((i % 5) == 0) //up
						tnh = (nh - ih) / 2 + (nh - ih) % 2;
					else if ((i % 5) == 1) { //down
						tny += (nh - ih) / 2 + (nh - ih) % 2 + ih;
						tnh = (nh - ih) / 2;
					}
					break;
				case 3: //bottom, up-left and up-right
					if ((i % 5) == 0) { // up-left
						tnw = (nw - iv) / 2 + (nw - iv) % 2;
						tnh = (nh - ih) * 2 / 3 + (nh - ih) * 2 % 3;
					} else if ((i % 5) == 1) { // up-right
						tnx += (nw - iv) / 2 + (nw - iv) % 2 + iv;
						tnw = (nw - iv) / 2;
						tnh = (nh - ih) * 2 / 3 + (nh - ih) * 2 % 3;
					} else if ((i % 5) == 2) { //bottom
						tnh = (nh - ih) / 3;
						tny += (nh - ih) * 2 / 3 + (nh - ih) * 2 % 3 + ih;
					}
					break;
				case 4: // bottom, left, right and top
					if ((i % 5) == 0) { //top
						hrest = (nh - 2 * ih) % 4;
						tnh = (nh - 2 * ih) / 4 + (hrest ? 1 : 0);
					} else if ((i % 5) == 1) { // left
						tnw = (nw - iv) / 2 + (nw - iv) % 2;
						tny += (nh - 2 * ih) / 4 + (hrest ? 1 : 0) + ih;
						tnh = (nh - 2 * ih) * 2 / 4 + (hrest > 1 ? 1 : 0);
					} else if ((i % 5) == 2) { // right
						tnx += (nw - iv) / 2 + (nw - iv) % 2 + iv;
						tnw = (nw - iv) / 2;
						tny += (nh - 2 * ih) / 4 + (hrest ? 1 : 0) + ih;
						tnh = (nh - 2 * ih) * 2 / 4 + (hrest > 1 ? 1 : 0);
					} else if ((i % 5) == 3) { // bottom
						tny += (nh - 2 * ih) / 4 + (hrest ? 1 : 0) + (nh - 2 * ih) * 2 / 4 + (hrest > 1 ? 1 : 0) + 2 * ih;
						tnh = (nh - 2 * ih) / 4 + (hrest > 2 ? 1 : 0);
					}
					break;
				}

			} else {
				/* Arrange mats. One mat is a collection of five clients arranged tatami style */

				if (((i - cats) % 5) == 0) {
					if ((cats > 0) || ((i - cats) >= 5)) {
						tny = ny = ny + nh + (nhrest > 0 ? 1 : 0) + ih;
						--nhrest;
					}
				}

				switch ((i - cats) % 5) {
				case 0: // top-left-vert
					wrest = (nw - 2 * iv) % 3;
					hrest = (nh - 2 * ih) % 3;
					tnw = (nw - 2 * iv) / 3 + (wrest ? 1 : 0);
					tnh = (nh - 2 * ih) * 2 / 3 + hrest + iv;
					break;
				case 1: // top-right-hor
					tnx += (nw - 2 * iv) / 3 + (wrest ? 1 : 0) + iv;
					tnw = (nw - 2 * iv) * 2 / 3 + (wrest > 1 ? 1 : 0) + iv;
					tnh = (nh - 2 * ih) / 3 + (hrest ? 1 : 0);
					break;
				case 2: // center
					tnx += (nw - 2 * iv) / 3 + (wrest ? 1 : 0) + iv;
					tnw = (nw - 2 * iv) / 3 + (wrest > 1 ? 1 : 0);
					tny += (nh - 2 * ih) / 3 + (hrest ? 1 : 0) + ih;
					tnh = (nh - 2 * ih) / 3 + (hrest > 1 ? 1 : 0);
					break;
				case 3: // bottom-right-vert
					tnx += (nw - 2 * iv) * 2 / 3 + wrest + 2 * iv;
					tnw = (nw - 2 * iv) / 3;
					tny += (nh - 2 * ih) / 3 + (hrest ? 1 : 0) + ih;
					tnh = (nh - 2 * ih) * 2 / 3 + hrest + iv;
					break;
				case 4: // (oldest) bottom-left-hor
					tnw = (nw - 2 * iv) * 2 / 3 + wrest + iv;
					tny += (nh - 2 * ih) * 2 / 3 + hrest + 2 * iv;
					tnh = (nh - 2 * ih) / 3;
					break;
				}

			}

			resize(c, tnx, tny, tnw - 2 * c->bw, tnh - 2 * c->bw, False);
			++i;
		}
	}
}

static void
tile(Monitor *m)
{
	unsigned int n;
	int oh = 0, ov = 0, ih = 0, iv = 0; // gaps outer/inner horizontal/vertical

	getgaps(m, &oh, &ov, &ih, &iv, &n);

	if (m->lt[m->sellt]->preset.layout != m->ltaxis[layout] ||
			m->lt[m->sellt]->preset.masteraxis != m->ltaxis[master] ||
			m->lt[m->sellt]->preset.stack1axis != m->ltaxis[stack] ||
			m->lt[m->sellt]->preset.stack2axis != m->ltaxis[stack2])
		setltsymbols(m, n);
	else if (m->lt[m->sellt]->preset.symbolfunc != NULL)
		m->lt[m->sellt]->preset.symbolfunc(m, n);

	if (n == 0)
		return;

	(&tilelayouts[abs(m->ltaxis[layout])])->arrange(m, m->wx + ov, m->wy + oh, m->wh - 2*oh, m->ww - 2*ov, ih, iv, n);
	return;
}

static void
setltsymbols(Monitor *m, unsigned int n)
{
	int l;
	Client *c;

	if (n == 0)
		for (c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);

	l = abs(m->ltaxis[layout]);
	if (m->ltaxis[master] == monocle && (l == nosplit || !m->mastercount || n <= m->mastercount) && monocleclientcount) {
		monoclesymbols(m, n);
		return;
	}

	if (m->ltaxis[stack] == monocle && (l == splitvertical || l == splithorizontalfixed) && deckcount) {
		decksymbols(m, n);
		return;
	}

	/* Layout symbols, Uncomment if you want to override these */
	/*
	if (l == nosplit || !m->mastercount) {
		sym1 = sym2 = sym3 = (int)tilesymb[m->ltaxis[master]];
	} else {
		sym2 = layoutsymb[l];
		if (m->ltaxis[layout] < 0) {
			sym1 = tilesymb[m->ltaxis[stack]];
			sym3 = tilesymb[m->ltaxis[master]];
		} else {
			sym1 = tilesymb[m->ltaxis[master]];
			sym3 = tilesymb[m->ltaxis[stack]];
		}
	}

	snprintf(m->ltsymbol, sizeof m->ltsymbol, "%c%c%c", sym1, sym2, sym3);
	*/
}

static void
monoclesymbols(Monitor *m, unsigned int n)
{
	if (n > 0)
		snprintf(m->ltsymbol, sizeof m->ltsymbol, monocleformat, n);
	else
		snprintf(m->ltsymbol, sizeof m->ltsymbol, text_layout3);
}

static void
decksymbols(Monitor *m, unsigned int n)
{
	if (n > m->mastercount)
		snprintf(m->ltsymbol, sizeof m->ltsymbol, deckformat, n);
	else
		snprintf(m->ltsymbol, sizeof m->ltsymbol, text_layout5);
}

void
ngrid(Monitor *m)
{
	unsigned int i, n;
	int cx, cy, cw, ch, cc, cr, chrest, cwrest, cols, rows;
	int oh, ov, ih, iv;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n);

	/* grid dimensions */
	for (rows = 0; rows <= n/2; rows++)
		if (rows*rows >= n)
			break;
	cols = (rows && (rows - 1) * rows >= n) ? rows - 1 : rows;

	/* window geoms (cell height/width) */
	ch = (m->wh - 2*oh - ih * (rows - 1)) / (rows ? rows : 1);
	cw = (m->ww - 2*ov - iv * (cols - 1)) / (cols ? cols : 1);
	chrest = (m->wh - 2*oh - ih * (rows - 1)) - ch * rows;
	cwrest = (m->ww - 2*ov - iv * (cols - 1)) - cw * cols;
	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
		cc = i / rows;
		cr = i % rows;
		cx = m->wx + ov + cc * (cw + iv) + MIN(cc, cwrest);
		cy = m->wy + oh + cr * (ch + ih) + MIN(cr, chrest);
		resize(c, cx, cy, cw + (cc < cwrest ? 1 : 0) - 2*c->bw, ch + (cr < chrest ? 1 : 0) - 2*c->bw, False);
	}
}

void
hgrid(Monitor *m) {
	Client *c;
	unsigned int n, i;
	int oh, ov, ih, iv;
	int mx = 0, my = 0, mh = 0, mw = 0;
	int sx = 0, sy = 0, sh = 0, sw = 0;
	int ntop, nbottom = 1;
	float mfacts = 0, sfacts = 0;
	int mrest, srest, mtotal = 0, stotal = 0;

	/* Count windows */
	getgaps(m, &oh, &ov, &ih, &iv, &n);
	if (n == 0)
		return;

	if (n <= 2)
		ntop = n;
	else {
		ntop = n / 2;
		nbottom = n - ntop;
	}
	sx = mx = m->wx + ov;
	sy = my = m->wy + oh;
	sh = mh = m->wh - 2*oh;
	sw = mw = m->ww - 2*ov;

	if (n > ntop) {
		sh = (mh - ih) / 2;
		mh = mh - ih - sh;
		sy = my + mh + ih;
		mw = m->ww - 2*ov - iv * (ntop - 1);
		sw = m->ww - 2*ov - iv * (nbottom - 1);
	}

	/* calculate facts */
	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < ntop)
			mfacts += c->cfact;
		else
			sfacts += c->cfact;

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < ntop)
			mtotal += mh * (c->cfact / mfacts);
		else
			stotal += sw * (c->cfact / sfacts);

	mrest = mh - mtotal;
	srest = sw - stotal;

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < ntop) {
			resize(c, mx, my, mw * (c->cfact / mfacts) + (i < mrest ? 1 : 0) - (2*c->bw), mh - (2*c->bw), 0);
			mx += WIDTH(c) + iv;
		} else {
			resize(c, sx, sy, sw * (c->cfact / sfacts) + ((i - ntop) < srest ? 1 : 0) - (2*c->bw), sh - (2*c->bw), 0);
			sx += WIDTH(c) + iv;
		}
}

enum node_type_t
{
   ndNULL,

   /* Containers */
   ndmonocle,
   ndhorizontallr,
   ndhorizontalrl,
   ndverticalud,
   ndverticaldu,
   ndvoid,

   /* Elements */
   ndclient,
   ndclientnum,
   ndclientnth,
   ndclientclass,
   ndclientfloat,
   ndclientempty,
   ndrest,
};

typedef struct node_t node_t;
struct node_t
{
   enum node_type_t type;
   float weight;
   int x, y, w, h;
   int f;
   unsigned n;
   unsigned margin;
   char *s;
   Client *c;

   struct node_t *branch;
   struct node_t *next;
};

struct client_ref_t
{
   Client *c;
   struct client_ref_t *next;
};

static node_t *s_layout_scheme;

node_t* alloc_node(enum node_type_t type)
{
   node_t *node = (node_t*) malloc(sizeof(node_t));
   memset(node, 0, sizeof(node_t));
   node->type = type;
   return node;
}

node_t* clone_node(node_t *n)
{
   if (n == NULL)
      return NULL;

   node_t *node = alloc_node(n->type);
   node->weight = n->weight;
   node->x = n->x;
   node->y = n->y;
   node->w = n->w;
   node->h = n->h;
   node->f = n->f;
   node->n = n->n;
   node->margin = n->margin;
   node->s = n->s;
   node->c = n->c;
   node->next = NULL;
   node->branch = NULL;
   return node;
}

int is_nested(node_t *node)
{
   return node->type == ndhorizontallr
       || node->type == ndhorizontalrl
       || node->type == ndverticalud
       || node->type == ndverticaldu
       || node->type == ndmonocle;
}

int is_terminal(char c)
{
   return c == ' '
       || c == '\t'
       || c == '('
       || c == ')'
       || c == '\0';
}

void
free_node(node_t *node)
{
   for ( node_t *n = node;
         n != NULL; )
   {
      if (is_nested(n))
         free_node(n->branch);

      node_t *ns = n->next;

      if (n->s) free(n->s);
      free(n);
      n = ns;
   }
}

node_t* reverse_node(node_t *node)
{
   node_t *a = node, *b = NULL;

   if (a != NULL) {
      b = a->next;
      a->next = NULL;
   }

   while (b != NULL) {
      node_t *nx = b->next;
      b->next = a;
      a = b;
      b = nx;
   }

   return (b ? b : a);
}

void
node_length(node_t *node, unsigned *len, float *weight)
{
   unsigned n = 0;
   float w = 0.0;

   for (; node != NULL; node = node->next) {
      if (!node->f) {
         n ++;
         w += (node->weight == 0 ? 1 : node->weight);
      }
   }

   if (len != NULL)
      *len = n;

   if (weight != NULL)
      *weight = w;
}

struct client_ref_t* copy_clients(Client *clients)
{
   struct client_ref_t head;
   head.next = NULL;
   struct client_ref_t *tail = &head;

   for (Client *c = nexttiled(clients); c != NULL; c = nexttiled(c->next)) {
      tail->next = (struct client_ref_t*) malloc(sizeof(struct client_ref_t));
      tail = tail->next;
      
      tail->next = NULL;
      tail->c = c;
   }

   return head.next;
}

void
free_clients(struct client_ref_t *clients)
{
   struct client_ref_t *nxt = NULL;
   while (clients != NULL) {
      nxt = clients->next;
      free(clients);
      clients = nxt;
   }
}

struct s_recur_analyze_ret {
   node_t *head;
   node_t *tail;
}
s_recur_analyze(struct client_ref_t **clients, node_t *node)
{
   struct client_ref_t *c = *clients;
   unsigned i = 0;

   /* a single client, just assign a client. */
   if (node->type == ndclient) {
      struct s_recur_analyze_ret ret;
      ret.tail = ret.head = clone_node(node);
      *clients = c->next;
      ret.head->c = c->c;
      return ret;
   }

   /* An empty slot. */
   if (node->type == ndclientempty) {
      struct s_recur_analyze_ret ret;
      ret.tail = ret.head = clone_node(node);
      ret.tail->c = NULL;
      ret.tail->type = ndclient;
      return ret;
   }

   /* pick 'n' client from the list top. */
   if (node->type == ndclientnth) {
      struct client_ref_t *prev = NULL;
      struct s_recur_analyze_ret ret;
      ret.head = ret.tail = NULL;

      for ( i = 0, c = *clients;
            i < node->n && c != NULL;
            i ++, c = c->next)
      {
         prev = c;
      }

      if (c != NULL) {
         if (prev == NULL)
            *clients = c->next;
         else
            prev->next = c->next;

         ret.tail = ret.head = clone_node(node);
         ret.head->type = ndclient;
         ret.head->c = c->c;
      }

      return ret;
   }

   if (node->type == ndclientclass) {
      struct s_recur_analyze_ret ret;

      return ret;
   }

   /* Fixed number of clients. */
   if (node->type == ndclientnum) {
      struct s_recur_analyze_ret ret;
      node_t head, *p = &head;
      head.next = NULL;
      for ( i = 0, c = *clients;
            i < node->n && c != NULL;
            i ++, c = c->next )
      {
         p->next = clone_node(node);
         p = p->next;
         p->type = ndclient;
         p->c = c->c;
      }
      *clients = c;
      ret.head = head.next;
      ret.tail = p;
      return ret;
   }

   /* All leftover client. */
   if (node->type == ndrest) {
      struct s_recur_analyze_ret ret;
      node_t head, *p = &head;
      head.next = NULL;
      for (c = *clients; c != NULL; c = c->next) {
         p->next = clone_node(node);
         p = p->next;
         p->type = ndclient;
         p->c = c->c;
      }
      *clients = c;
      ret.head = head.next;
      ret.tail = p;
      return ret;
   }

   if (node->type == ndNULL) {
      struct s_recur_analyze_ret ret;
      ret.head = NULL;
      ret.tail = NULL;
      return ret;
   }

   /* In case the element is a container */
   if (is_nested(node)) {
      struct s_recur_analyze_ret ret;
      ret.head = clone_node(node);
      ret.tail = ret.head;

      struct s_recur_analyze_ret x;
      x.head = ret.head;
      x.tail = x.head;

      node_t branch, *tail = &branch;
      branch.next = NULL;

      node_t *n = NULL;

      /* For reversed containers the order must be reversed */
      if (node->type == ndhorizontalrl || node->type == ndverticaldu) {
         n = reverse_node(node->branch);
      } else {
         n = node->branch;
      }

      for (; *clients != NULL && n != NULL; n = n->next )
      {
         x = s_recur_analyze(clients, n);
         
         /* Attach the received tree to the tail of the previous element */
         if (x.head != NULL) {
            tail->next = x.head;
            tail = x.tail;
         }
      }

      if (node->type == ndhorizontalrl || node->type == ndverticaldu) {
         ret.head->branch = reverse_node(branch.next);
      } else {
         ret.head->branch = branch.next;
      }

      return ret;
   }

   struct s_recur_analyze_ret ret;
   ret.head = NULL;
   ret.tail = NULL;
   return ret;
}

struct frame_t
{
   int x, y, w, h;
};

void
s_recur_resize(node_t *node, struct frame_t frame)
{
   if (node == NULL) return;

   if (node->type == ndclient) {
      if (node->c != NULL) {
         if (node->f)
            resize(node->c, node->x, node->y, node->w, node->h, 0);
         else
            resize(node->c,
                  frame.x + node->margin,
                  frame.y + node->margin,
                  frame.w - 2 * node->margin - 2 * node->c->bw,
                  frame.h - 2 * node->margin - 2 * node->c->bw,
                  0);
      }
      return;
   }

   if (node->type == ndverticalud || node->type == ndverticaldu) {
      unsigned len = 0;
      float wgt = 0.0;
      int delta = 0;
      float avg_wgt = 1;

      frame.x += node->margin;
      frame.y += node->margin;
      frame.w -= 2 * node->margin;
      frame.h -= 2 * node->margin;
      
      node_length(node->branch, &len, &wgt);
      if (len != 0) {
         delta = frame.h / len;
         avg_wgt = wgt / len;
      }

      for (node_t *n = node->branch; n != NULL; n = n->next) {
         if (!n->f) {
            frame.h = (n->weight == 0 ? 1 : n->weight) / avg_wgt * delta;
            s_recur_resize(n, frame);
            frame.y += frame.h;
         } else {
            s_recur_resize(n, frame);
         }
      }
      return;
   }

   if (node->type == ndhorizontalrl || node->type == ndhorizontallr) {
      unsigned len = 0;
      float wgt = 0.0;
      int delta = 0;
      float avg_wgt = 1;

      frame.x += node->margin;
      frame.y += node->margin;
      frame.w -= 2 * node->margin;
      frame.h -= 2 * node->margin;
      
      node_length(node->branch, &len, &wgt);
      if (len != 0) {
         delta = frame.w / len;
         avg_wgt = wgt / len;
      }

      for (node_t *n = node->branch; n != NULL; n = n->next) {
         if (!n->f) {
            frame.w = (n->weight == 0 ? 1 : n->weight) / avg_wgt * delta;
            s_recur_resize(n, frame);
            frame.x += frame.w;
         } else {
            s_recur_resize(n, frame);
         }
      }
      return;
   }

   if (node->type == ndmonocle) {
      frame.x += node->margin;
      frame.y += node->margin;
      frame.w -= 2 * node->margin;
      frame.h -= 2 * node->margin;

      for (node_t *n = node->branch; n != NULL; n = n->next)
         s_recur_resize(n, frame);
   }
}

/* Main layout function. */
void
custom(Monitor *m)
{
   /* Need to clone the client stack, as we might need to pull items from it. */
   struct client_ref_t *clients = copy_clients(m->clients),
                       *clients_root = clients;

   if (s_layout_scheme == NULL)
      return;

   struct s_recur_analyze_ret ret = s_recur_analyze(&clients, s_layout_scheme);

   struct frame_t frame;
   frame.x = m->wx;
   frame.y = m->wy;
   frame.w = m->ww;
   frame.h = m->wh;

   s_recur_resize(ret.head, frame);

   /* Free the resources we allocated. */
   free_clients(clients_root);
   free_node(ret.head);
}

/* tokenize string */
typedef struct string_token_t string_token_t;
struct string_token_t {
   char token[32];
   struct string_token_t *next;
};

string_token_t* parse_string(char *str, unsigned *i)
{
   unsigned char escape = 0;
   unsigned j = 0;
   string_token_t *ret = (string_token_t*) malloc(sizeof(string_token_t));
   ret->next = NULL;

   while (str[*i] != '\0' && j < sizeof(ret->token) - 1) {
      if (str[*i] == '\\' && !escape) {
         escape = 1;
         (*i) ++;
         continue;
      }

      if (str[*i] == '"' && !escape) {
         break;
      }

      ret->token[j++] = str[(*i)++];
      escape = 0;
   }
   ret->token[j] = '\0';

   if (str[*i] == '\0')
      (*i) --;
   return ret;
}

struct string_token_t* tokenize_string(char *str)
{
   struct string_token_t head;
   struct string_token_t *node = &head;
   head.next = NULL;
   unsigned word_start = UINT_MAX;
   unsigned len = 0;

   for (unsigned i = 0;; i ++) {
      switch (str[i]) {
         /* end of line */
         case '\0':
            if (word_start != UINT_MAX) {
               node->next = (struct string_token_t*) malloc(sizeof(struct string_token_t));
               node = node->next;
               node->next = NULL;

               len = MIN(i - word_start, sizeof(node->token) - 1);
               strncpy(node->token, &str[word_start], len);
               node->token[len] = '\0';
            }
            return head.next;

         /* comment */
         case ';':
            return head.next;

         /* beginning of a string */
         case '"':
            node->next = parse_string(str, &i);
            if (node->next) node = node->next;

         case ' ':
         case '\t':
         case '(':
         case ')':
            if (word_start != UINT_MAX) {
               node->next = (struct string_token_t*) malloc(sizeof(struct string_token_t));
               node = node->next;
               node->next = NULL;
               len = MIN(i - word_start, sizeof(node->token) - 1);
               strncpy(node->token, &str[word_start], len);
               node->token[len] = '\0';
               word_start = UINT_MAX;
            }
            if (str[i] == '(' || str[i] == ')') {
               node->next = (struct string_token_t*) malloc(sizeof(struct string_token_t));
               node = node->next;
               node->next = NULL;
               node->token[0] = str[i];
               node->token[1] = '\0';
            }
            break;

         // A piece of a word
         default:
            if (word_start == UINT_MAX)
               word_start = i;
      }
   }
   return NULL;
}

/* Parse s-expression to node_t structure */
node_t* parse_sexp(string_token_t **token)
{
   node_t *head = NULL;
   node_t branch, *p = &branch;
   branch.next = NULL;

   string_token_t *t = *token;

   while (t != NULL) {
      if (strcmp(t->token, ")") == 0) {
         t = t->next;
         break;
      }

      if (strcmp(t->token, "(") == 0) {
         t = t->next;
         *token = t;

         if (head == NULL) {
            head = parse_sexp(token);
         } else {
            p->next = parse_sexp(token);
            if (p->next) p = p->next;
         }
         t = *token;
         continue;
      }

      /* client slots
       * single client */
      if (strcmp(t->token, "c") == 0 || strcmp(t->token, "client") == 0) {
         if (head == NULL) {
            head = alloc_node(ndclient);
         } else {
            p->next = alloc_node(ndclient);
            p = p->next;
         }
         t = t->next;
         continue;
      }

      /* Empty viewport */
      if (strcmp(t->token, "e") == 0 || strcmp(t->token, "empty") == 0) {
         if (head == NULL) {
            head = alloc_node(ndclientempty);
         } else {
            p->next = alloc_node(ndclientempty);
            p = p->next;
         }
         t = t->next;
         continue;
      }

      /* the rest of the clients */
      if (strcmp(t->token, "...") == 0 || strcmp(t->token, "rest") == 0) {
         if (head == NULL) {
            head = alloc_node(ndrest);
         } else {
            p->next = alloc_node(ndrest);
            p = p->next;
         }
         t = t->next;
         continue;
      }

      /* choose the client by class */
      if (strcmp(t->token, "class") == 0) {
         if (head == NULL) {
            head = alloc_node(ndclientclass);
            if (t->next) {
               t = t->next;
               head->s = strdup(t->token);
            }
         } else {
            p->next = alloc_node(ndclientclass);
            p = p->next;
            if (t->next) {
               t = t->next;
               p->s = strdup(t->token);
            }
         }
         t = t->next;
         continue;
      }
      
      /* 'n' client */
      unsigned long n = 0;
      char *endp = NULL;
      n = strtoul(t->token, &endp, 10);
      if (is_terminal(*endp)) {
         if (head == NULL) {
            head = alloc_node(ndclientnth);
            head->n = n;
         } else {
            p->next = alloc_node(ndclientnth);
            p->next->n = n;
            p = p->next;
         }
         t = t->next;
         continue;
      }

      /* max number of clients */
      if (strcmp(t->token, "max") == 0) {
         if (head == NULL) {
            head = alloc_node(ndclientnum);

            t = t->next;
            if (t != NULL) {
               head->n = (unsigned) atoi(t->token);
               t = t->next;
            }
         }
         continue;
      }

      /* parameters
      * weight */
      if ((strcmp(t->token, "w:") == 0 || strcmp(t->token, ":w") == 0
               || strcmp(t->token, "weight:") == 0
               || strcmp(t->token, ":weight") == 0) && head != NULL) {
         t = t->next;

         if (t != NULL) {
            head->weight = (float) atof(t->token);
            t = t->next;
         }
         continue;
      }

      /* margin */
      if (((strcmp(t->token, "m:") == 0) || strcmp(t->token, ":m") == 0 
               || strcmp(t->token, "margin:") == 0
               || strcmp(t->token, ":margin") == 0) && head != NULL) {
         t = t->next;

         if (t != NULL) {
            head->margin = (unsigned) atoi(t->token);
            t = t->next;
         }
         continue;
      }

      /* floating geometry */
      if ((strcmp(t->token, "f:") == 0 || strcmp(t->token, ":f") == 0
               || strcmp(t->token, "float:") == 0
               || strcmp(t->token, ":float") == 0) && head != NULL) {
         head->f = 1;
         t = t->next;

         if (t != NULL) {
            head->x = atoi(t->token);
            t = t->next;
         }
         if (t != NULL) {
            head->y = atoi(t->token);
            t = t->next;
         }
         if (t != NULL) {
            head->w = atoi(t->token);
            t = t->next;
         }
         if (t != NULL) {
            head->h = atoi(t->token);
            t = t->next;
         }
         continue;
      }

      /* containers */
      if ((strcmp(t->token, "h") == 0 || strcmp(t->token, "horizontal") == 0) && head == NULL)
         head = alloc_node(ndhorizontallr);

      if ((strcmp(t->token, "hr") == 0 || strcmp(t->token, "h-reversed") == 0) && head == NULL)
         head = alloc_node(ndhorizontalrl);

      if ((strcmp(t->token, "v") == 0 || strcmp(t->token, "vertical") == 0) && head == NULL)
         head = alloc_node(ndverticalud);

      if ((strcmp(t->token, "vr") == 0 || strcmp(t->token, "v-reversed") == 0) && head == NULL)
         head = alloc_node(ndverticalud);

      if ((strcmp(t->token, "m") == 0 || strcmp(t->token, "monocle") == 0) && head == NULL)
         head = alloc_node(ndmonocle);

      t = t->next;
   }

   if (head)
      head->branch = branch.next;

   *token = t;
   return head;
}

void
set_s_layout(const Arg *arg)
{
   FILE *pp, *hf;

   char pathbuf[1024];
   char *home = getenv("HOME");
   if (home != NULL) {
      snprintf(pathbuf, 1023, "%s/", customhistfile, home);
      pathbuf[1023] = '\0';
      
      /* make sure the history file exists */
      hf = fopen(customhistfile, "a"); fclose(hf);
      if (!system("sort" customhistfile " | uniq > " customhistfile "~"))
          return;

      if (!system("mv " customhistfile "~ " customhistfile))
          return;

      pp = popen(customprompt customhistfile, "r");
   } else {
      pp = popen(customprompt, "r");
   }


   if (!pp) 
		return;
   
   char buf[2048];
   buf[2048] = '\0';

   if (!fgets(buf, 1024, pp))
       return;

   fclose(pp);
   if (buf[0] == '\0')
		   return;

   hf = fopen(customhistfile, "a");
   fprintf(hf, "%s", buf);
   fclose(hf);

   if (s_layout_scheme != NULL) {
      free_node(s_layout_scheme);
      s_layout_scheme = NULL;
   }

   struct string_token_t *token_root = tokenize_string(buf),
                         *token = token_root;

   s_layout_scheme = parse_sexp(&token);

   /* Free the token list */
   while (token_root != NULL) {
      token = token_root->next;
      free(token_root);
      token_root = token;
   }
}
