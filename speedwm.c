/* See LICENSE file for license and copyright details. */
#include <errno.h>
#include <locale.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "toggle.h"
#if USEIPC
#include <sys/epoll.h>
#endif
#include <X11/cursorfont.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/Xutil.h>
#if USEXRESOURCES
#include <X11/Xresource.h>
#endif
#ifdef XINERAMA
#include <X11/extensions/Xinerama.h>
#endif /* XINERAMA */
#include <X11/Xft/Xft.h>
#include <pango/pango.h>
#if USESWITCHER
#include <time.h>
#endif
#if USEIMLIB2
#include <Imlib2.h>
#endif
#include <X11/Xlib-xcb.h>
#include <xcb/res.h>
#ifdef __OpenBSD__
#include <sys/sysctl.h>
#include <kvm.h>
#endif /* __OpenBSD */

#include "draw.h"
#include "main.h"

/* macros */
#define STATUS "speedwm_status"
#define BARRULES                20
#define BUTTONMASK              (ButtonPressMask|ButtonReleaseMask)
#define CLEANMASK(mask)         (mask & ~(numlockmask|LockMask) & (ShiftMask|ControlMask|Mod1Mask|Mod2Mask|Mod3Mask|Mod4Mask|Mod5Mask))
#define INTERSECT(x,y,w,h,m)    (MAX(0, MIN((x)+(w),(m)->wx+(m)->mw) - MAX((x),(m)->mx)) \
                               * MAX(0, MIN((y)+(h),(m)->wy+(m)->mh) - MAX((y),(m)->my)))
#define INTERSECTC(x,y,w,h,z)   (MAX(0, MIN((x)+(w),(z)->x+(z)->w) - MAX((x),(z)->x)) \
                               * MAX(0, MIN((y)+(h),(z)->y+(z)->h) - MAX((y),(z)->y)))
#define HIDDEN(C)               ((getstate(C->win) == IconicState))
#define ISVISIBLEONTAG(C, T)    ((C->tags & T))
#define ISVISIBLE(C)            ((C->tags & C->mon->tagset[C->mon->seltags]) || C->issticky)
#define LENGTH(X)               (sizeof X / sizeof X[0])
#define MOUSEMASK               (BUTTONMASK|PointerMotionMask)
#define WIDTH(X)                ((X)->w + 2 * (X)->bw)
#define HEIGHT(X)               ((X)->h + 2 * (X)->bw)
#define TAGMASK                 ((1 << LENGTH(ftags)) - 1)
#define TAGSLENGTH              (LENGTH(ftags))
#define TEXTW(X)                (drw_font_getwidth(drw, (X), False) + lrpad)
#define TEXTWM(X)                (drw_font_getwidth(drw, (X), True) + lrpad)
#define RIGHTOF(a,b)            (a.y_org > b.y_org) || \
                               ((a.y_org == b.y_org) && (a.x_org > b.x_org))

#define opaque                  0xffU
#define transparent             0

#define MWM_HINTS_FLAGS_FIELD       0
#define MWM_HINTS_DECORATIONS_FIELD 2
#define MWM_HINTS_DECORATIONS       (1 << 1)
#define MWM_DECOR_ALL               (1 << 0)
#define MWM_DECOR_BORDER            (1 << 1)
#define MWM_DECOR_TITLE             (1 << 3)

/* enums */
enum { CurNormal, CurResize, CurMove, CurResizeHorzArrow, CurResizeVertArrow, CurLast }; /* cursor, CurLast must be last */

/* color schemes */
enum { SchemeBorderNorm,
       SchemeBorderSel,
	   SchemeBorderUrg,
	   SchemeTagsNorm,
	   SchemeTagsSel,
	   SchemeTagsUrg,
	   SchemeBar,
	   SchemeTitleNorm,
	   SchemeTitleSel,
	   SchemeTitleHidden,
	   SchemeTitleUrg,
	   SchemeLayout,
	   SchemeStatus,
	   SchemeSystray,
};

/* EWMH atoms */
enum { NetSupported,
       NetWMName,
	   NetWMState,
       NetWMCheck,
       NetWMFullscreen,
       NetActiveWindow,
       NetWMWindowType,
       NetWMWindowTypeDesktop,
       NetWMSticky,
       NetWMWindowTypeDialog,
       NetClientList, NetDesktopNames,
       NetDesktopViewport,
       NetNumberOfDesktops,
       NetCurrentDesktop,
       #if USEFADE
	   NetWMWindowsOpacity,
       #endif
       #if USESYSTRAY
       NetSystemTray,
       NetSystemTrayOP,
       NetSystemTrayOrientation,
       NetSystemTrayVisual,
       NetWMWindowTypeDock,
       NetSystemTrayOrientationHorz,
       #endif
       #if USEWINICON
       NetWMIcon,
       #endif
	   NetClientListStacking,
       NetClientInfo,
       NetLast,
};

/* default atoms */
enum { WMClass,
       WMProtocols,
       WMDelete,
       WMState,
       WMTakeFocus,
       WMLast,
};

/* clicks */
enum { clicktags,
       clicklayout,
       clickstatusbar,
       clicktitle,
       clickclient,
       clickroot,
       clicklast,
};

#if USEIPC
typedef struct TagState TagState;
struct TagState {
	int selected;
	int occupied;
	int urgent;
};

typedef struct ClientState ClientState;
struct ClientState {
	int isfixed, isfloating, isurgent, neverfocus, oldstate, isfullscreen;
};
#endif

#if USEQUERY
typedef struct {
    const char *query_text;
    int (*func)();
} Query;
#endif

enum {
	bar_align_left,
	bar_align_center,
	bar_align_right,
	bar_align_left_left,
	bar_align_left_right,
	bar_align_left_center,
	bar_align_none,
	bar_align_right_left,
	bar_align_right_right,
	bar_align_right_center,
	bar_align_last
}; /* bar alignment */

typedef union {
	long i;
	unsigned long ui;
	float f;
	const void *v;
} Arg;

typedef struct Monitor Monitor;
typedef struct Bar Bar;
struct Bar {
	Window win;
	Monitor *mon;
	Bar *next;
	int idx;
	int barposition;
	int bx, by, bw, bh; /* bar geometry */
	int w[BARRULES]; // module width
	int x[BARRULES]; // module position
};

typedef struct {
	int max_width;
} BarWidthArg;

typedef struct {
	int x;
	int w;
} BarDrawArg;

typedef struct {
	int rel_x;
	int rel_y;
	int rel_w;
	int rel_h;
} BarClickArg;

typedef struct {
	unsigned int click;
	unsigned int mask;
	unsigned int button;
	void (*func)(const Arg *arg);
	const Arg arg;
} Button;

typedef struct Client Client;
struct Client {
	char name[256]; /* client */
	float mina, maxa;
	float cfact; /* cfact for the client */
	int x, y, w, h; /* x/y position, width and height */
    int sfx, sfy, sfw, sfh; /* stored float geometry, used on mode revert */
	int oldx, oldy, oldw, oldh;
	int basew, baseh, incw, inch, maxw, maxh, minw, minh;
	int hintsvalid; /* https://git.suckless.org/dwm/commit/8806b6e2379372900e3d9e0bf6604bc7f727350b.html */
	int bw, oldbw;
	unsigned int tags; /* tags */
    #if USEWINICON
    unsigned int icw, ich; Picture icon;
    #endif
	int isfixed,
		ispermanent,
		isfloating,
		isurgent,
		neverfocus,
		oldstate,
		isfullscreen,
		ignoretransient,
		issticky,
		isterminal,
		noswallow,
		needresize; /* window rules */
	pid_t pid;
	char scratchkey; /* scratchpad key */
	int issteam; /* steam specific fix */
	int beingmoved;
	Client *next;
	Client *snext;
	Client *swallowing;
    int ishidden;
	Monitor *mon;
	Window win;
#if USEIPC
	ClientState prevstate;
#endif
};

typedef struct {
	int monitor;
	int bar;
	int alignment;
	int (*widthfunc)(Bar *bar, BarWidthArg *a);
	int (*drawfunc)(Bar *bar, BarDrawArg *a);
	int (*clickfunc)(Bar *bar, Arg *arg, BarClickArg *a);
    int val;
	char *name;
	int x, w;
} BarRule;

typedef struct {
    int type;
	unsigned int mod;
    KeySym chain;
	KeySym keysym;
	void (*func)(const Arg *);
	const Arg arg;
} Key;

typedef struct {
	int mastercount;
	int stackcount;
	int layout;
	int masteraxis; // master stack area
	int stack1axis; // primary stack area
	int stack2axis; // secondary stack area, e.g. centered master
	void (*symbolfunc)(Monitor *, unsigned int);
} LayoutPreset;

typedef struct {
	unsigned int signum;
	void (*func)(const Arg *);
	const Arg arg;
} Signal;

typedef struct {
	const char *symbol;
	void (*arrange)(Monitor *);
	LayoutPreset preset;
} Layout;

typedef struct Pertag Pertag;
struct Monitor {
	char ltsymbol[16];     /* current layout symbol */
    #if USEIPC
	char lastltsymbol[16]; /* previous layout symbol */
    #endif
    #if USETAGPREVIEW
    int previewshow;
    Window tagwin;
    Pixmap *tagmap;
    #endif
    int scalepreview;
	float mfact;          /* mfact value */
	float cfact;          /* cfact value */
	int ltaxis[4];
	int stackcount;
	int mastercount;      /* number of clients in the master stack */
	int num;
	int bh;               /* bar geometry */
	int btw;
	int bt;
	int mx, my, mw, mh;   /* screen size */
	int wx, wy, ww, wh;   /* window area  */
	int gapsizeih;        /* horizontal gap between windows */
	int gapsizeiv;        /* vertical gap between windows */
	int gapsizeoh;        /* horizontal outer gaps */
	int gapsizeov;        /* vertical outer gaps */
#if USESWITCHER
	int switchern;		  /* move that many clients forward */
	int nclients;		  /* number of active clients in tag */
	int isswitching; 	  /* 1,0 */
	int maxwidth;         /* max width of window */
	int maxheight;		  /* max height of window */
#endif
	unsigned int seltags;
	unsigned int sellt;
	unsigned int tagset[2];
#if USEIPC
	TagState tagstate;
#endif
	int hidebar;

	/* bar items */
	int hidelayout;
	int hidetitle;
    int hideborder;
    int hidesingleborder;
	int hidestatus;
	int hideicon;
	int hidetags;
	int hidesystray;
	int hideemptytags;
	int hidefloating;
	int hidesticky;
	int hideclientindicator;
    int hideunselectedtitle;
    int hideunselectedborder;

    int insetx;
    int insety;
    int inseth;
    int insetw;

    int allowwarp;

	/* powerline shape */
	int tagplshape;

	/* position */
	int layoutposition;

	/* coloring */
	int colorselectedtitle;
	int colorhiddentitle;
	int colorlayout;

	/* for padding funcs */
	int cpadv;
	int cpadh;

	int hidsel;
	int isreset;
	Client *clients;
	Client *sel;
    #if USEIPC
	Client *lastsel;
    #endif
	Client *stack;
	Client *tagmarked[32];
    #if USESWITCHER
    Client **clientsnext; /* array of all clients in the tag */
    #endif
	Monitor *next;
    Bar *bar;
	Window tabwin;
	Window traywin;
	const Layout *lt[2];
    #if USEIPC
	const Layout *lastlt;
    #endif
	Pertag *pertag;
};

/* Order of the rules */
typedef struct {
	const char *class;
	const char *instance;
	const char *title;
	unsigned int tags;
	int isfloating;
	int ispermanent;
	int isterminal;
	int noswallow;
	int monitor;
	int unmanaged;
	int ignoretransient;
	int floatx, floaty, floatw, floath;
	const char scratchkey;
} Rule;

typedef struct {
	const char *cmd;
	int id;
} ClickStatus;

#if USEMOUSE
typedef struct {
	char* icon;
	char* command;
	unsigned int interval;
	unsigned int signal;
} Module;
#endif

/* Xresources preferences */
enum resource_type {
	STRING = 0,
	INTEGER = 1,
	FLOAT = 2,
    DOUBLE = 3
};

typedef struct {
	char *name;
	enum resource_type type;
	void *dst;
} ResourcePref;

/* This is just here so we can include status.h without issues */
typedef struct {
	char *name;
	enum resource_type type;
	void *dst;
} StatusResourcePref;

/* function declarations */
static void applyrules(Client *c);
static void getgaps(Monitor *m, int *oh, int *ov, int *ih, int *iv, unsigned int *nc);
static int applysizehints(Client *c, int *x, int *y, int *w, int *h, int interact);
static void inplacerotate(const Arg *arg);
static void togglesticky(const Arg *arg);
static void setcurrentdesktop(void);
static void setdesktopnames(void);
static void setnumdesktops(void);
static void setviewport(void);
static void arrange(Monitor *m);
static void arrangemon(Monitor *m);
static void moveresizeaspect(const Arg *arg);
static void attach(Client *c);
static void attachabove(Client *c);
static void attachaside(Client *c);
static void attachbelow(Client *c);
static void attachbottom(Client *c);
static void attachtop(Client *c);
static void attachstack(Client *c);
#if USEMOUSE
static void buttonpress(XEvent *e);
#endif
static void checkotherwm(void);
static void cleanup(void);
static void cleanupmon(Monitor *mon);
static void clientmessage(XEvent *e);
static void configure(Client *c);
static void configurenotify(XEvent *e);
static void configurerequest(XEvent *e);
static void copyvalidchars(char *text, char *rawtext);
static void autostart_exec(void);
static void moveresize(const Arg *arg);
static void moveresizeedge(const Arg *arg);
static Monitor *createmon(void);
static void cyclelayout(const Arg *arg);
static void destroynotify(XEvent *e);
static void detach(Client *c);
static void detachstack(Client *c);
static Monitor *dirtomon(int dir);
#if USEMOUSE
static void dragmfact(const Arg *arg);
#endif
static void drawbaritems(Monitor *m);
static void drawbarwin(Bar *bar);
static void drawbar(void);
static void enternotify(XEvent *e);
static void expose(XEvent *e);
static void focus(Client *c);
static void focusin(XEvent *e);
static void focusmaster(const Arg *arg);
static void focusmon(const Arg *arg);
static void focusstack(int inc, int vis);
static void focusstackvis(const Arg *arg);
static void focusstackhid(const Arg *arg);
static Atom getatomprop(Client *c, Atom prop);
static int getrootptr(int *x, int *y);
static long getstate(Window w);

#if USEWINICON
static Picture geticonprop(Window w, unsigned int *icw, unsigned int *ich);
static void freeicon(Client *c);
static void updateicon(Client *c);
#endif

#if USESYSTRAY
static int sendevent(Window w, Atom proto, int m, long d0, long d1, long d2, long d3, long d4);
#else
static int sendevent(Client *c, Atom proto);
#endif

static int gettextprop(Window w, Atom atom, char *text, unsigned int size);
static void grabbuttons(Client *c, int focused);
static void hide(const Arg *arg);
static void togglewin(const Arg *arg);
static void hidewin(Client *c);
static void incmastercount(const Arg *arg);
#if USEIPC
static int handlexevent(struct epoll_event *ev);
#endif
static void keypress(XEvent *e);
static int getsignal(void);
static void killclient(const Arg *arg);
static void killunsel(const Arg *arg);
static void manage(Window w, XWindowAttributes *wa);
static void mappingnotify(XEvent *e);
static void maprequest(XEvent *e);
static void motionnotify(XEvent *e);
#if USEMOUSE
static void movemouse(const Arg *arg);
static void moveorplace(const Arg *arg);
#endif
static Client *nexttagged(Client *c);
static Client *nexttiled(Client *c);
static unsigned int nexttag(void);
static unsigned int nexttag_skip_vacant(void);
static unsigned int prevtag(void);
static unsigned int prevtag_skip_vacant(void);
#if USEMOUSE
static void placemouse(const Arg *arg);
#endif
#if USEFADE
static void opacity(Client *c, double opacity);
#endif
static void pop(Client *);
static void propertynotify(XEvent *e);
static void quit(const Arg *arg);
static Client *recttoclient(int x, int y, int w, int h);
static Monitor *recttomon(int x, int y, int w, int h);
static void resetmastercount(const Arg *arg);
static void reset_mfact(const Arg *arg);
static void reset_layout(const Arg *arg);
static void reorganizetags(const Arg *arg);
static void resize(Client *c, int x, int y, int w, int h, int interact);
static void resizeclient(Client *c, int x, int y, int w, int h);
#if USEMOUSE
static void resizemouse(const Arg *arg);
#endif
static void restack(Monitor *m);
static void run(void);
static void scan(void);

/* tile functions */
static void mirrorlayout(const Arg *arg);
static void rotatelayoutaxis(const Arg *arg);
static void incstackcount(const Arg *arg);

/* scratchpad functions */
static void scratchpad_hide();
static _Bool scratchpad_last_showed_is_killed(void);
static void scratchpad_remove();
static void scratchpad_show();
static void scratchpad_show_client(Client *c);
static void scratchpad_show_first(void);

static void sendmon(Client *c, Monitor *m);
static void setclientstate(Client *c, long state);
static void setclienttagprop(Client *c);
static void setfocus(Client *c);
static void setfullscreen(Client *c, int fullscreen);
static void setsticky(Client *c, int sticky);
static void setgaps(int oh, int ov, int ih, int iv);
static void incrgaps(const Arg *arg);
static void incrigaps(const Arg *arg);
static void incrogaps(const Arg *arg);
static void togglegaps(const Arg *arg);
static void defaultgaps(const Arg *arg);
static void setlayout(const Arg *arg);
#if USEMOUSE
static void dragcfact(const Arg *arg);
#endif
static void setcfact(const Arg *arg);
static void setmfact(const Arg *arg);
static void setup(void);

/* ipc */
#if USEIPC
static void setlayoutsafe(const Arg *arg);
static void setupepoll(void);
#endif

static void seturgent(Client *c, int urg);

/* show/hide */
static void show(const Arg *arg);
static void showall(const Arg *arg);
static void hideall(const Arg *arg);
static void showwin(Client *c);
static void showhide(Client *c);

static void sigchld(int unused);
#ifdef XINERAMA
static void sortscreens(XineramaScreenInfo *screens, int n);
#endif
static void sighup(int unused);
static void sigterm(int unused);
static void spawn(const Arg *arg);
static void swapclient(const Arg *arg);
static void swapfocus(const Arg *arg);
static void togglemark(const Arg *arg);
static void tagmon(const Arg *arg);

/* layouts */
static void setltsymbols(Monitor *m, unsigned int n);
static void monoclesymbols(Monitor *m, unsigned int n);
static void decksymbols(Monitor *m, unsigned int n);
static void layout_no_split(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n);
static void layout_split_vertical(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n);
static void layout_split_horizontal(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n);
static void layout_split_vertical_dual_stack(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n);
static void layout_split_horizontal_dual_stack(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n);
static void layout_split_centered_vertical(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n);
static void layout_split_centered_horizontal(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n);
static void layout_floating_master(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n);
static void layout_split_vertical_fixed(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n);
static void layout_split_horizontal_fixed(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n);
static void layout_split_vertical_dual_stack_fixed(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n);
static void layout_split_horizontal_dual_stack_fixed(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n);
static void layout_split_centered_vertical_fixed(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n);
static void layout_split_centered_horizontal_fixed(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n);
static void layout_floating_master_fixed(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n);
static void arrange_left_to_right(Monitor *m, int ax, int ay, int ah, int aw, int ih, int iv, int n, int an, int ai);
static void arrange_top_to_bottom(Monitor *m, int ax, int ay, int ah, int aw, int ih, int iv, int n, int an, int ai);
static void arrange_monocle(Monitor *m, int ax, int ay, int ah, int aw, int ih, int iv, int n, int an, int ai);
static void arrange_dynamicgrid(Monitor *m, int ax, int ay, int ah, int aw, int ih, int iv, int n, int an, int ai);
static void arrange_dynamicgrid_alt1(Monitor *m, int ax, int ay, int ah, int aw, int ih, int iv, int n, int an, int ai);
static void arrange_dynamicgrid_alt2(Monitor *m, int ax, int ay, int ah, int aw, int ih, int iv, int n, int an, int ai);
static void arrange_gridmode(Monitor *m, int ax, int ay, int ah, int aw, int ih, int iv, int n, int an, int ai);
static void arrange_horizgrid(Monitor *m, int ax, int ay, int ah, int aw, int ih, int iv, int n, int an, int ai);
static void arrange_dwindle(Monitor *m, int ax, int ay, int ah, int aw, int ih, int iv, int n, int an, int ai);
static void arrange_spiral(Monitor *m, int ax, int ay, int ah, int aw, int ih, int iv, int n, int an, int ai);
static void arrange_tatami(Monitor *m, int ax, int ay, int ah, int aw, int ih, int iv, int n, int an, int ai);

static void tile(Monitor *m);
static void empty(Monitor *m);
static void hgrid(Monitor *m);
static void ngrid(Monitor *m);
static void custom(Monitor *m);
static void set_s_layout(const Arg *arg);

/* bar items */
static void togglebar(const Arg *arg);
static void togglebartags(const Arg *arg);
static void togglesystray(const Arg *arg);
static void togglebaremptytags(const Arg *arg);
static void togglebarunseltitle(const Arg *arg);
static void togglebarlt(const Arg *arg);
static void togglebarsticky(const Arg *arg);
static void togglebaricon(const Arg *arg);
static void togglebartitle(const Arg *arg);
static void togglebarlt(const Arg *arg);
static void togglebarstatus(const Arg *arg);
static void togglebarfloat(const Arg *arg);

/* misc toggles */
static void togglefloating(const Arg *arg);
static void toggleopacity(const Arg *arg);
static void togglefullscr(const Arg *arg);
static void toggleborder(const Arg *arg);
static void togglesingleborder(const Arg *arg);

/* barheight */
static void setbarheight(const Arg *arg);
static void resetbarheight(const Arg *arg);

/* barpadding + gaps */
static void setbpgaps(const Arg *arg);
static void resetbpgaps(const Arg *arg);

/* barpadding */
static void setbarpaddingov(const Arg *arg);
static void setbarpaddingoh(const Arg *arg);
static void setbarpadding(const Arg *arg);
static void resetbarpaddingov(const Arg *arg);
static void resetbarpaddingoh(const Arg *arg);
static void resetbarpadding(const Arg *arg);
static void togglebarpaddingov(const Arg *arg);
static void togglebarpaddingoh(const Arg *arg);
static void togglebarpadding(const Arg *arg);

/* mouse */
#if USEMOUSE
static void resizemouse(const Arg *arg);
#endif
static void unfocus(Client *c, int setfocus);
static void unmanage(Client *c, int destroyed);
static void unmapnotify(XEvent *e);

/* update functions */
static void updatecurrentdesktop(void);
static void updatebarpos(Monitor *m);
static void updatebars(void);
static void updateclientlist(void);
static int updategeom(void);
static void updatemotifhints(Client *c);
static void updatenumlockmask(void);
static void updatesizehints(Client *c);
static void updatestatus(void);
static void updaterules(Client *c);
static void updatetitle(Client *c);
static void updatewindowtype(Client *c);
static void updatewmhints(Client *c);

/* tag functions */
static void toggleview(const Arg *arg);
static void tag(const Arg *arg);

/* warp functions */
static void warp(const Client *c);

/* view functions */
static void view(const Arg *arg);
static void viewtoleft(const Arg *arg);
static void viewtoright(const Arg *arg);
static void viewtoleft_vacant(const Arg *arg);
static void viewtoright_vacant(const Arg *arg);

/* window mapping */
static void window_set_state(Display *dpy, Window win, long state);
static void window_map(Display *dpy, Client *c, int deiconify);
static void window_unmap(Display *dpy, Window win, Window root, int iconify, Client *c);

static Client *wintoclient(Window w);
static Monitor *wintomon(Window w);

/* X11 error */
static int xerror(Display *dpy, XErrorEvent *ee);
static int xerrordummy(Display *dpy, XErrorEvent *ee);
static int xerrorstart(Display *dpy, XErrorEvent *ee);

static void xinitvisual();
static void zoom(const Arg *arg);

/* Tag preview functions */
#if USETAGPREVIEW
static void showtagpreview(unsigned int i);
static void takepreview(void);
static void updatepreviews(Monitor *m);
#endif

/* We will keep this one, in order to keep keybinds.h clean. */
static void previewtag(const Arg *arg);

static void centerwindow(const Arg *arg);

#include "bar/items.h"

/* switcher funcs */
#if USESWITCHER
void drawswitcher(int nwins, int first, Monitor *m);
void switcherstart(const Arg *arg);
static void switcherend();
#endif
void switcherstart(const Arg *arg);

/* Xresources funcs */
#if USEXRESOURCES
static void load_xresources(void);
static void reloadcolors(const Arg *arg);
static void resource_load(XrmDatabase db, char *name, enum resource_type rtype, void *dst);
#endif

static pid_t getparentprocess(pid_t p);
static int isdescprocess(pid_t p, pid_t c);
static Client *swallowingclient(Window w);
static Client *termforwin(const Client *c);
static pid_t winpid(Window w);

/* status text */
static char stext[1024];
static char rawstext[1024];
static char estext[1024];
static char rawestext[1024];

/* variables */
static const char notitle[] = ""; /* Title when none can be grabbed. */
static Client *lastfocused = NULL;
static int screen;
static int tw, sh;           /* X display screen geometry width, height */
static int bh;               /* bar geometry */
static int unmanaged = 0;    /* whether the window manager should manage the new window or not */
static int lrpad;            /* sum of left and right padding for text */
static int vp;               /* vertical padding for bar */
static int sp;               /* side padding for bar */
static int (*xerrorxlib)(Display *, XErrorEvent *);
static unsigned int numlockmask = 0;
static void (*handler[LASTEvent]) (XEvent *) = {
    #if USEMOUSE
	[ButtonPress] = buttonpress,
    #endif
	[ClientMessage] = clientmessage,
	[ConfigureRequest] = configurerequest,
	[ConfigureNotify] = configurenotify,
	[DestroyNotify] = destroynotify,
	[EnterNotify] = enternotify,
	[Expose] = expose,
	[FocusIn] = focusin,
	[KeyPress] = keypress,
    [KeyRelease] = keypress,
	[MappingNotify] = mappingnotify,
	[MapRequest] = maprequest,
	[MotionNotify] = motionnotify,
	[PropertyNotify] = propertynotify,
    #if USESYSTRAY
    [ResizeRequest] = resizerequest,
    #endif
	[UnmapNotify] = unmapnotify
};
#if USESYSTRAY
static Atom wmatom[WMLast], netatom[NetLast], xatom[XLast], motifatom;
#else
static Atom wmatom[WMLast], netatom[NetLast], motifatom;
#endif

#if USEIPC
static int epoll_fd;
static int dpy_fd;
#endif
static int running = 1; /* yes, we are in fact running */
static int restart = 0; /* which means we are definitely not restarting */
static Cur *cursor[CurLast];
static Clr **scheme;
static Clr **tagscheme;
static Clr **powerlinescheme;
static Display *dpy;
static Drw *drw;
static Monitor *mons, *selmon;
#if USEIPC
static Monitor *lastselmon; /* IPC */
#endif

/* See flextile_deluxe for reference */
enum {
	layout,
	master,
	stack,
	stack2,
	ltaxislast,
};

enum {
	nosplit,
	splitvertical,
	splithorizontal,
	splitcenteredvertical,
	splitcenteredhorizontal,
	splitverticaldualstack,
	splithorizontaldualstack,
	floatingmaster,
	splitverticalfixed,
	splithorizontalfixed,
	splitcenteredverticalfixed,
	splitcenteredhorizontalfixed,
	splitverticaldualstackfixed,
	splithorizontaldualstackfixed,
	floatingmasterfixed,
	layoutlast,
};

enum {
	toptobottom,
	lefttoright,
	monocle,
	dynamicgrid,
	dynamicgridalt1,
	dynamicgridalt2,
	grid,
	horizgrid,
	dwindle,
	spiral,
	tatami,
	axislast,
};

static Window root, wmcheckwin;
static Client *mark;
static KeySym keychain = -1;

/* scratchpad */
#define SCRATCHPAD_MASK (1u << sizeof tags / sizeof * tags)
static Client * scratchpad_last_showed = NULL;

static int useargb = 0;
static Visual *visual;
static int depth;
static Colormap cmap;

static xcb_connection_t *xcon;
#define FORCE_VSPLIT forcevsplit

/* Modifiers */
#define CONTROL ControlMask
#define SHIFT ShiftMask
#define ALT Mod1Mask
#define ALTR Mod3Mask
#define SUPER Mod4Mask
#define SUPERR Mod5Mask

/* Media controls */
#include <X11/XF86keysym.h> /* Enable multimedia button support */

/* Configuration */
#if USEQUERY
#include "toggle/query.h"
#endif
#include "options.h" /* Include options */

/* Shell command */
#define cmd( cmd ) {.v = (const char*[]){ shell, "-c", cmd, NULL } },

static const char *clickstatus[] = { shell, "-c", NULL, NULL }; /* for running click scripts */

/* Rest of the headers */
#include "autostart.h" /* Add autostart support */
#include "colors.h" /* Include colors */
#include "rules.h" /* Include rules */
#include "bar.h" /* Include bar */
#if USEXRESOURCES
#include "xresources.h" /* Include .Xresources/Pywal support */
#endif

/* IPC support */
#if USEIPC
#include "toggle/ipc.c"
#include "toggle/ipc-cli.c"
#include "ipc.h" /* Include IPC */
#endif

#include "text.h" /* Include tags and layouts */
#include "keybinds.h" /* Include keybinds */

/* Mouse support */
#if USEMOUSE
#include "mouse.h" /* Include mouse support */
#include "status.h" /* Include mouse clickstatus support */
#endif

/* Signals */
#include "signal.h" /* Include signal support */

/* Rest of the IPC support */
#if USEIPC
#include "toggle/ipc-yajl.c"
#endif

#include "bar/items.c"

#if USEQUERY
#include "queries.h"
#include "toggle/query.c"
#endif

unsigned int tagw[LENGTH(ftags)];

struct Pertag {
	unsigned int curtag, prevtag; /* current and previous tag */
	int mastercounts[LENGTH(ftags) + 1]; /* number of windows in master area */
	float mfacts[LENGTH(ftags) + 1]; /* mfacts per tag */
	unsigned int sellts[LENGTH(ftags) + 1]; /* selected layouts */
	int stackcounts[LENGTH(ftags) + 1]; /* number of windows in primary stack area */
	int ltaxis[LENGTH(ftags) + 1][ltaxislast];
	const Layout *ltidxs[LENGTH(ftags) + 1][3]; /* matrix of tags and layouts indexes  */
	int hidebars[LENGTH(ftags) + 1]; /* display bar for the current tag */
};

/* compile-time check if all tags fit into an unsigned int bit array. */
struct NumTags { char limitexceeded[LENGTH(ftags) > 31 ? -1 : 1]; };

/* speedwm will keep pid's of processes from autostart array and kill them at quit */
static pid_t *autostart_pids;
static size_t autostart_len;

/* execute command from autostart array */
static void
autostart_exec() {
	const char *const *p;
	size_t i = 0;

    FILE *rsfile = fopen("/tmp/speedwm-isrestart", "r");

    if (rsfile) {
        /* delete autostart file and return */
        if (remove("/tmp/speedwm-isrestart"))
            fprintf(stderr, "speedwm: Failed to remove /tmp/speedwm-isrestart");

        return;
    }

    system(STATUS "&");

	/* count entries */
	for (p = autostart; *p; autostart_len++, p++)
		while (*++p);

	autostart_pids = malloc(autostart_len * sizeof(pid_t));
	for (p = autostart; *p; i++, p++) {
		if ((autostart_pids[i] = fork()) == 0) {
			setsid();
			execvp(*p, (char *const *)p);
			die("speedwm: execvp %s\n", *p);
		}
		/* skip arguments */
		while (*++p);
	}
}

/* function implementations */
void
applyrules(Client *c)
{
	const char *class, *instance;
	unsigned int i;
	const Rule *r;
	Monitor *m;
	XClassHint ch = { NULL, NULL };

	/* rule matching */
	c->isfloating = 0;
	c->tags = 0;
	c->scratchkey = 0;
	XGetClassHint(dpy, c->win, &ch);
	class    = ch.res_class ? ch.res_class : notitle;
	instance = ch.res_name  ? ch.res_name  : notitle;

	if (strstr(class, "Steam") || strstr(class, "steam_app_"))
		c->issteam = 1;

	for (i = 0; i < LENGTH(rules); i++) {
		r = &rules[i];
		if ((!r->title || strstr(c->name, r->title))
		&& (!r->class || strstr(class, r->class))
		&& (!r->instance || strstr(instance, r->instance)))
		{
			c->isterminal = r->isterminal;
			c->noswallow  = r->noswallow;
			c->isfloating = r->isfloating;
			c->ignoretransient = r->ignoretransient;
			c->ispermanent = r->ispermanent;
			c->tags |= r->tags;
			unmanaged = r->unmanaged;
			c->scratchkey = r->scratchkey;
			if (r->isfloating) {
				if (r->floatx >= 0)
						c->x = c->mon->mx + r->floatx;
				if (r->floaty >= 0)
						c->y = c->mon->my + r->floaty;
				if (r->floatw >= 0)
						c->w = r->floatw;
				if (r->floath >= 0)
						c->h = r->floath;
			}
			for (m = mons; m && m->num != r->monitor; m = m->next)
			    if (m)
				    c->mon = m;
		}
	}
	if (ch.res_class)
		XFree(ch.res_class);
	if (ch.res_name)
		XFree(ch.res_name);

	if(c->tags & TAGMASK)                    c->tags = c->tags & TAGMASK;
	else if(c->mon->tagset[c->mon->seltags]) c->tags = c->mon->tagset[c->mon->seltags];
	else                                     c->tags = 1;
    if (c->tags != SCRATCHPAD_MASK)
	    c->tags = c->tags & TAGMASK ? c->tags & TAGMASK : c->mon->tagset[c->mon->seltags];
}

int
applysizehints(Client *c, int *x, int *y, int *w, int *h, int interact)
{
	int baseismin;
	Monitor *m = c->mon;

	/* set minimum possible */
	*w = MAX(1, *w);
	*h = MAX(1, *h);
	if (interact) {
		if (*x > tw)
			*x = tw - WIDTH(c);
		if (*y > sh)
			*y = sh - HEIGHT(c);
		if (*x + *w + 2 * c->bw < 0)
			*x = 0;
		if (*y + *h + 2 * c->bw < 0)
			*y = 0;
	} else {
		if (*x >= m->wx + m->ww)
			*x = m->wx + m->ww - WIDTH(c);
		if (*y >= m->wy + m->wh)
			*y = m->wy + m->wh - HEIGHT(c);
		if (*x + *w + 2 * c->bw <= m->wx)
			*x = m->wx;
		if (*y + *h + 2 * c->bw <= m->wy)
			*y = m->wy;
	}
	if (*h < bh)
		*h = bh;
	if (*w < bh)
		*w = bh;
	if (resizehints || c->isfloating || !c->mon->lt[c->mon->sellt]->arrange) {
			if (!c->hintsvalid)
					updatesizehints(c);
		/* see last two sentences in ICCCM 4.1.2.3 */
		baseismin = c->basew == c->minw && c->baseh == c->minh;
		if (!baseismin) { /* temporarily remove base dimensions */
			*w -= c->basew;
			*h -= c->baseh;
		}
		/* adjust for aspect limits */
		if (c->mina > 0 && c->maxa > 0) {
			if (c->maxa < (float)*w / *h)
				*w = *h * c->maxa + 0.5;
			else if (c->mina < (float)*h / *w)
				*h = *w * c->mina + 0.5;
		}
		if (baseismin) { /* increment calculation requires this */
			*w -= c->basew;
			*h -= c->baseh;
		}
		/* adjust for increment value */
		if (c->incw)
			*w -= *w % c->incw;
		if (c->inch)
			*h -= *h % c->inch;
		/* restore base dimensions */
		*w = MAX(*w + c->basew, c->minw);
		*h = MAX(*h + c->baseh, c->minh);
		if (c->maxw)
			*w = MIN(*w, c->maxw);
		if (c->maxh)
			*h = MIN(*h, c->maxh);
	}
	return *x != c->x || *y != c->y || *w != c->w || *h != c->h;
}

void
arrange(Monitor *m)
{
	if (m)
		showhide(m->stack);
	else for (m = mons; m; m = m->next)
		showhide(m->stack);
	if (m) {
		arrangemon(m);
		restack(m);
	} else for (m = mons; m; m = m->next)
		arrangemon(m);
}

void
arrangemon(Monitor *m)
{
    memcpy(m->ltsymbol, m->lt[m->sellt]->symbol, sizeof(m->ltsymbol));

	if (m->lt[m->sellt]->arrange) {
		m->lt[m->sellt]->arrange(m);
	}
}

void
moveresizeaspect(const Arg *arg) {
	Client *c;
	c = selmon->sel;
	float ratio;
	int w, h,nw, nh;

	if (!c || !arg)
		return;
	if (selmon->lt[selmon->sellt]->arrange && !c->isfloating)
		return;

	ratio = (float)c->w / (float)c->h;
	h = arg->i;
	w = (int)(ratio * h);

	nw = c->w + w;
	nh = c->h + h;

	XRaiseWindow(dpy, c->win);
	resize(c, c->x, c->y, nw, nh, True);
}

void
attach(Client *c)
{
	c->next = c->mon->clients;
	c->mon->clients = c;
}

void
attachabove(Client *c)
{
	if (c->mon->sel == NULL || c->mon->sel == c->mon->clients || c->mon->sel->isfloating) {
		attach(c);
		return;
	}

	Client *at;
	for (at = c->mon->clients; at->next != c->mon->sel; at = at->next);
	c->next = at->next;
	at->next = c;
}

void
attachaside(Client *c) {
	Client *at = nexttagged(c);
	if(!at) {
		attach(c);
		return;
		}
	c->next = at->next;
	at->next = c;
}

void
attachbelow(Client *c)
{
	if(c->mon->sel == NULL || c->mon->sel == c || c->mon->sel->isfloating) {
		attach(c);
		return;
	}
	c->next = c->mon->sel->next;
	c->mon->sel->next = c;
}

void
attachbottom(Client *c)
{
	Client *below = c->mon->clients;
	for (; below && below->next; below = below->next);
	c->next = NULL;
	if (below)
		below->next = c;
	else
		c->mon->clients = c;
}

void
attachtop(Client *c)
{
	int n;
	Monitor *m = selmon;
	Client *below;

	for (n = 1, below = c->mon->clients;
		below && below->next && (below->isfloating || !ISVISIBLEONTAG(below, c->tags) || n != m->mastercount);
		n = below->isfloating || !ISVISIBLEONTAG(below, c->tags) ? n + 0 : n + 1, below = below->next);
	c->next = NULL;
	if (below) {
		c->next = below->next;
		below->next = c;
	}
	else
		c->mon->clients = c;
}

void
warp(const Client *c)
{
	int x, y;

    if (!warpcursor || !selmon->allowwarp) {
        return;
    }

    selmon->allowwarp = 1;

	if (!c) {
		XWarpPointer(dpy, None, root, 0, 0, 0, 0, selmon->wx + selmon->ww/2, selmon->wy + selmon->wh/2);
		return;
	}

	if (!getrootptr(&x, &y) ||
    	(x > c->x - c->bw &&
		 y > c->y - c->bw &&
		 x < c->x + c->w + c->bw*2 &&
		 y < c->y + c->h + c->bw*2))
		return;

   	XWarpPointer(dpy, None, c->win, 0, 0, 0, 0, c->w / 2, c->h / 2);
}

void
attachstack(Client *c)
{
	c->snext = c->mon->stack;
	c->mon->stack = c;
}

void
swallow(Client *p, Client *c)
{

	if (c->noswallow || c->isterminal)
		return;
	if (c->noswallow && !swallowfloating && c->isfloating)
		return;

	if (!swallowclients)
		return;

	detach(c);
	detachstack(c);

	setclientstate(c, WithdrawnState);
	XUnmapWindow(dpy, p->win);

	p->swallowing = c;
	c->mon = p->mon;

	Window w = p->win;
	p->win = c->win;
	c->win = w;
#if USEWINICON
    updateicon(p);
#endif
	updatetitle(p);
	XMoveResizeWindow(dpy, p->win, p->x, p->y, p->w, p->h);
	arrange(p->mon);
	configure(p);
	updateclientlist();
}

void
unswallow(Client *c)
{
	c->win = c->swallowing->win;

	free(c->swallowing);
	c->swallowing = NULL;

	/* unfullscreen the client */
	setfullscreen(c, 0);
#if USEWINICON
    updateicon(c);
#endif
	updatetitle(c);
	arrange(c->mon);
	XMapWindow(dpy, c->win);
	XMoveResizeWindow(dpy, c->win, c->x, c->y, c->w, c->h);
	setclientstate(c, NormalState);
	focus(NULL);
	arrange(c->mon);
}

#if USEMOUSE
void
buttonpress(XEvent *e)
{
   	int click, i, r, mi;
	Arg arg = {0};
	Client *c;
	Monitor *m;
	Bar *bar;
	XButtonPressedEvent *ev = &e->xbutton;
	const BarRule *br;
	BarClickArg carg = { 0, 0, 0, 0 };

	click = clickroot;
	/* focus monitor if necessary */
	if ((m = wintomon(ev->window)) && m != selmon
	) {
		unfocus(selmon->sel, 1);
		selmon = m;
		focus(NULL);
	}

	for (mi = 0, m = mons; m && m != selmon; m = m->next, mi++); /* get the monitor index */
	for (bar = selmon->bar; bar; bar = bar->next) {
		if (ev->window == bar->win) {
			for (r = 0; r < LENGTH(barrules); r++) {
				br = &barrules[r];
				if (br->bar != bar->idx || (br->monitor == 'A' && m != selmon) || br->clickfunc == NULL)
					continue;
				if (br->monitor != 'A' && br->monitor != -1 && br->monitor != mi)
					continue;
				if (bar->x[r] <= ev->x && ev->x <= bar->x[r] + bar->w[r]) {
					carg.rel_x = ev->x - bar->x[r];
					carg.rel_y = ev->y;
					carg.rel_w = bar->w[r];
					carg.rel_h = bar->bh;
					click = br->clickfunc(bar, &arg, &carg);
					if (click < 0)
						return;
					break;
				}
			}
			break;
		}
	}

	if (click == clickroot && (c = wintoclient(ev->window))) {
		focus(c);
		restack(selmon);
		XAllowEvents(dpy, ReplayPointer, CurrentTime);
		click = clickclient;
	}

	for (i = 0; i < LENGTH(buttons); i++) {
		if (click == buttons[i].click && buttons[i].func && buttons[i].button == ev->button
				&& CLEANMASK(buttons[i].mask) == CLEANMASK(ev->state)) {
            buttons[i].func((click == clicktags || click == clicktitle) && buttons[i].arg.i == 0 ? &arg : &buttons[i].arg);
		}
	}
}
#endif

void
checkotherwm(void)
{
	xerrorxlib = XSetErrorHandler(xerrorstart);
	/* this causes an error if some other window manager is running */
	XSelectInput(dpy, DefaultRootWindow(dpy), SubstructureRedirectMask);
	XSync(dpy, False);
	XSetErrorHandler(xerror);
	XSync(dpy, False);
}

/* clean up the wm */
void
cleanup(void)
{
	Arg a = {.ui = ~0};
	Layout foo = { "", NULL };
	Monitor *m;
	size_t i;
    #if USESWITCHER
	switcherend();
    #endif

	view(&a);
	selmon->lt[selmon->sellt] = &foo;
	for (m = mons; m; m = m->next)
		while (m->stack)
			unmanage(m->stack, 0);
	XUngrabKey(dpy, AnyKey, AnyModifier, root);
	while (mons)
		cleanupmon(mons);
    #if USESYSTRAY
    if (!selmon->hidesystray && systray) {
		while (systray->icons)
			removesystrayicon(systray->icons);
		if (systray->win) {
			XUnmapWindow(dpy, systray->win);
			XDestroyWindow(dpy, systray->win);
		}
		free(systray);
	}
    #endif

	for (i = 0; i < CurLast; i++)
		drw_cur_free(drw, cursor[i]);
	for (i = 0; i < LENGTH(colors) + 1; i++)
		free(scheme[i]);
	free(scheme);

	if (fullscreenhidebar)
		XDestroyWindow(dpy, wmcheckwin);

	drw_free(drw);
	XSync(dpy, False);
	XSetInputFocus(dpy, PointerRoot, RevertToPointerRoot, CurrentTime);
	XDeleteProperty(dpy, root, netatom[NetActiveWindow]);

#if USEIPC
	ipc_cleanup();

	if (close(epoll_fd) < 0) {
			fprintf(stderr, "Failed to close epoll file descriptor\n");
	}
#endif
}

void
cleanupmon(Monitor *mon)
{
	Monitor *m;

	if (mon == mons)
		mons = mons->next;
	else {
		for (m = mons; m && m->next != mon; m = m->next);
		m->next = mon->next;
	}
    #if USETAGPREVIEW
    size_t i;

    for (i = 0; i < LENGTH(ftags); i++)
		if (mon->tagmap[i])
			XFreePixmap(dpy, mon->tagmap[i]);
	free(mon->tagmap);
    XUnmapWindow(dpy, mon->tagwin);
	XDestroyWindow(dpy, mon->tagwin);
    #endif
	free(mon);
}

void
clientmessage(XEvent *e)
{
	XClientMessageEvent *cme = &e->xclient;
	Client *c = wintoclient(cme->window);
	unsigned int i;

    #if USESYSTRAY
    XWindowAttributes wa;
	XSetWindowAttributes swa;

    if (!selmon->hidesystray && systray && cme->window == systray->win && cme->message_type == netatom[NetSystemTrayOP]) {
		/* add systray icons */
		if (cme->data.l[1] == SYSTEM_TRAY_REQUEST_DOCK) {
			if (!(c = (Client *)calloc(1, sizeof(Client))))
				die("fatal: could not malloc() %u bytes\n", sizeof(Client));
			if (!(c->win = cme->data.l[2])) {
				free(c);
				return;
			}

			c->mon = selmon;
			c->next = systray->icons;
			systray->icons = c;
			XGetWindowAttributes(dpy, c->win, &wa);
			c->x = c->oldx = c->y = c->oldy = 0;
			c->w = c->oldw = wa.width;
			c->h = c->oldh = wa.height;
			c->oldbw = wa.border_width;
			c->bw = 0;
			c->isfloating = True;
			/* reuse tags field as mapped status */
			c->tags = 1;
			updatesizehints(c);
			updatesystrayicongeom(c, wa.width, wa.height);
			XAddToSaveSet(dpy, c->win);
			XSelectInput(dpy, c->win, StructureNotifyMask | PropertyChangeMask | ResizeRedirectMask);
			XReparentWindow(dpy, c->win, systray->win, 0, 0);
			/* use parents background color */
			swa.background_pixel = scheme[SchemeSystray][ColBg].pixel; /* might be a good idea to change this to SchemeBar */
			XChangeWindowAttributes(dpy, c->win, CWBackPixel, &swa);
			sendevent(c->win, netatom[Xembed], StructureNotifyMask, CurrentTime, XEMBED_EMBEDDED_NOTIFY, 0 , systray->win, XEMBED_EMBEDDED_VERSION);
			XSync(dpy, False);
			setclientstate(c, NormalState);
		}
		return;
	}
    #endif

	if (!c)
		return;
	if (cme->message_type == netatom[NetWMState]) {
		if (cme->data.l[1] == netatom[NetWMFullscreen]
		|| cme->data.l[2] == netatom[NetWMFullscreen])
			setfullscreen(c, (cme->data.l[0] == 1 /* _NET_WM_STATE_ADD    */
				|| (cme->data.l[0] == 2 /* _NET_WM_STATE_TOGGLE */ && !c->isfullscreen)));
    if (cme->data.l[1] == netatom[NetWMSticky]
                || cme->data.l[2] == netatom[NetWMSticky])
            setsticky(c, (cme->data.l[0] == 1 || (cme->data.l[0] == 2 && !c->issticky)));
	} else if (cme->message_type == netatom[NetActiveWindow]) {
    if (!autofocus) {
			if (c != selmon->sel && !c->isurgent)
				seturgent(c, 1);
		} else {
		for (i = 0; i < LENGTH(ftags) && !((1 << i) & c->tags); i++);
		if (i < LENGTH(ftags)) {
			const Arg a = {.ui = 1 << i};
			selmon = c->mon;
			view(&a);
			focus(c);
			restack(selmon);
		}
    }
	}
}

void
configure(Client *c)
{
	XConfigureEvent ce;

	ce.type = ConfigureNotify;
	ce.display = dpy;
	ce.event = c->win;
	ce.window = c->win;
	ce.x = c->x;
	ce.y = c->y;
	ce.width = c->w;
	ce.height = c->h;
	ce.border_width = c->bw;
	ce.above = None;
	ce.override_redirect = False;
	XSendEvent(dpy, c->win, False, StructureNotifyMask, (XEvent *)&ce);
}

void
configurenotify(XEvent *e)
{
	Monitor *m;
	Client *c;
	XConfigureEvent *ev = &e->xconfigure;
	int dirty;
    Bar *bar;

	if (ev->window == root) {
		dirty = (tw != ev->width || sh != ev->height);
		tw = ev->width;
		sh = ev->height;
		if (updategeom() || dirty) {
			drw_resize(drw, tw, bh);
			updatebars();
			for (m = mons; m; m = m->next) {
				for (c = m->clients; c; c = c->next)
					if (c->isfullscreen)
						resizeclient(c, m->mx, m->my, m->mw, m->mh);

                	for (bar = m->bar; bar; bar = bar->next)
			            XMoveResizeWindow(dpy, bar->win, bar->bx, bar->by, bar->bw, bar->bh);

                    #if USETAGPREVIEW
                    updatepreviews(m);
                    #endif
			}
			focus(NULL);
			arrange(NULL);
		}
	}
}

void
configurerequest(XEvent *e)
{
	Client *c;
	Monitor *m;
	XConfigureRequestEvent *ev = &e->xconfigurerequest;
	XWindowChanges wc;

	if ((c = wintoclient(ev->window))) {
		if (ev->value_mask & CWBorderWidth)
			c->bw = ev->border_width;
		else if (c->isfloating || !selmon->lt[selmon->sellt]->arrange) {
			m = c->mon;
			if (!c->issteam) {
				if (ev->value_mask & CWX) {
					c->oldx = c->x;
					c->x = m->mx + ev->x;
				}
				if (ev->value_mask & CWY) {
					c->oldy = c->y;
					c->y = m->my + ev->y;
				}
			}
			if (ev->value_mask & CWWidth) {
				c->oldw = c->w;
				c->w = ev->width;
			}
			if (ev->value_mask & CWHeight) {
				c->oldh = c->h;
				c->h = ev->height;
			}
			if ((c->x + c->w) > m->mx + m->mw && c->isfloating)
				c->x = m->mx + (m->mw / 2 - WIDTH(c) / 2); /* center in x direction */
			if ((c->y + c->h) > m->my + m->mh && c->isfloating)
				c->y = m->my + (m->mh / 2 - HEIGHT(c) / 2); /* center in y direction */
			if ((ev->value_mask & (CWX|CWY)) && !(ev->value_mask & (CWWidth|CWHeight)))
				configure(c);
			if (ISVISIBLE(c))
				XMoveResizeWindow(dpy, c->win, c->x, c->y, c->w, c->h);
			else if (autoresize)
				c->needresize = 1;
		} else
			configure(c);
	} else {
		wc.x = ev->x;
		wc.y = ev->y;
		wc.width = ev->width;
		wc.height = ev->height;
		wc.border_width = ev->border_width;
		wc.sibling = ev->above;
		wc.stack_mode = ev->detail;
		XConfigureWindow(dpy, ev->window, ev->value_mask, &wc);
	}
	XSync(dpy, False);
}

/* init monitor */
Monitor *
createmon(void)
{
	Monitor *m, *mon;
	int i, n, mi, max_bars = 2, isbarposition = barposition;

	const BarRule *br;
	Bar *bar;

	m = ecalloc(1, sizeof(Monitor));
	m->tagset[0] = m->tagset[1] = startontag ? 1 : 0;
	m->mfact = mfact;
	m->mastercount = mastercount;
	m->stackcount = stackcount;
	m->hidebar = hidebar;
    m->scalepreview = scalepreview;

	/* bar items */
	m->hidelayout = hidelayout;
	m->hidetitle = hidetitle;
    m->hideborder = hideborder;
	m->hidestatus = hidestatus;
	m->hideicon = hideicon;
	m->hidetags = hidetags;
	m->hidesystray = hidesystray;
	m->hideemptytags = hideemptytags;
	m->hidefloating = hidefloating;
	m->hidesticky = hidesticky;
    m->hideclientindicator = hideclientindicator;
    m->hideunselectedtitle = hideunselectedtitle;
    m->hideunselectedborder = hideunselectedborder;

    /* insets */
    m->insetx = insetx;
    m->insety = insety;
    m->insetw = insetw;
    m->inseth = inseth;

    m->allowwarp = 0;

	/* shape of powerlines */
	m->tagplshape = tagplshape;

    /* monitor index */
    for (mi = 0, mon = mons; mon; mon = mon->next, mi++) {};

	/* coloring */
	m->colorselectedtitle = colorselectedtitle;
	m->colorhiddentitle = colorhiddentitle;
	m->colorlayout = colorlayout;

	/* bar height */
	m->bh = bh;

	/* gaps */
	m->gapsizeih = gapsizeih;
	m->gapsizeiv = gapsizeiv;
	m->gapsizeoh = gapsizeoh;
	m->gapsizeov = gapsizeov;

	/* layout */
	m->lt[0] = &layouts[0];
	m->lt[1] = &layouts[1 % LENGTH(flayouts)];

    #if USETAGPREVIEW
    /* map our tags */
  	m->tagmap = ecalloc(LENGTH(ftags), sizeof(Pixmap));
    #endif

    #if USESWITCHER
    /* number of clients, defaults to 0 */
	m->nclients = 0;
    #endif

	m->ltaxis[layout] = m->lt[0]->preset.layout;
	m->ltaxis[master] = m->lt[0]->preset.masteraxis;
	m->ltaxis[stack]  = m->lt[0]->preset.stack1axis;
	m->ltaxis[stack2] = m->lt[0]->preset.stack2axis;
	m->pertag = ecalloc(1, sizeof(Pertag));
	m->pertag->curtag = m->pertag->prevtag = 1;

	/* pertag */
	for (i = 0; i <= LENGTH(ftags); i++) {
		m->pertag->mastercounts[i] = m->mastercount;
		m->pertag->stackcounts[i] = m->stackcount;
		m->pertag->mfacts[i] = m->mfact;
		m->pertag->ltidxs[i][0] = m->lt[0];
		m->pertag->ltidxs[i][1] = m->lt[1];
		m->pertag->sellts[i] = m->sellt;
		m->pertag->hidebars[i] = m->hidebar;
		m->pertag->ltaxis[i][layout] = m->ltaxis[layout];
		m->pertag->ltaxis[i][master] = m->ltaxis[master];
		m->pertag->ltaxis[i][stack]  = m->ltaxis[stack];
		m->pertag->ltaxis[i][stack2] = m->ltaxis[stack2];
	}

	/* pertag */
	strncpy(m->ltsymbol, layouts[0].symbol, sizeof(m->ltsymbol + 1));

    /* Derive the number of bars for this monitor based on bar rules */
	for (n = -1, i = 0; i < LENGTH(barrules); i++) {
		br = &barrules[i];
		if (br->monitor == 'A' || br->monitor == -1 || br->monitor == mi)
			n = MAX(br->bar, n);
	}

	for (i = 0; i <= n && i < max_bars; i++) {
		bar = ecalloc(1, sizeof(Bar));
		bar->mon = m;
		bar->idx = i;
		bar->next = m->bar;
		bar->barposition = isbarposition;
		m->bar = bar;
		isbarposition = !isbarposition;
	}

	return m;
}

void
cyclelayout(const Arg *arg) {
	Layout *l;
	for(l = (Layout *)layouts; l != selmon->lt[selmon->sellt]; l++);
	if(arg->i > 0) {
		if(l->symbol && (l + 1)->symbol)
			setlayout(&((Arg) { .v = (l + 1) }));
		else
			setlayout(&((Arg) { .v = layouts }));
	} else {
		if(l != layouts && (l - 1)->symbol)
			setlayout(&((Arg) { .v = (l - 1) }));
		else
			setlayout(&((Arg) { .v = &layouts[LENGTH(flayouts) - 2] }));
	}
}

void
destroynotify(XEvent *e)
{
	Client *c;
	XDestroyWindowEvent *ev = &e->xdestroywindow;

	if ((c = wintoclient(ev->window))) {
		unmanage(c, 1);

    #if USESYSTRAY
    } else if (!selmon->hidesystray && (c = wintosystrayicon(ev->window))) {
		removesystrayicon(c);
		drawbarwin(systray->bar);
    #endif

    } else if ((c = swallowingclient(ev->window))) {
		unmanage(c->swallowing, 1);
    }
}

void
detach(Client *c)
{
	Client **tc;

	for (int i = 1; i < LENGTH(ftags); i++)
		if (c == c->mon->tagmarked[i])
			c->mon->tagmarked[i] = NULL;

	for (tc = &c->mon->clients; *tc && *tc != c; tc = &(*tc)->next);
	*tc = c->next;
}

void
detachstack(Client *c)
{
	Client **tc, *t;

	for (tc = &c->mon->stack; *tc && *tc != c; tc = &(*tc)->snext);
	*tc = c->snext;

	if (c == c->mon->sel) {
		for (t = c->mon->stack; t && !ISVISIBLE(t); t = t->snext);
		c->mon->sel = t;
	}
}

Monitor *
dirtomon(int dir)
{
	Monitor *m = NULL;

	if (dir > 0) {
		if (!(m = selmon->next))
			m = mons;
	} else if (selmon == mons)
		for (m = mons; m->next; m = m->next);
	else
		for (m = mons; m->next != selmon; m = m->next);
	return m;
}

/* drag cfact with mouse */
#if USEMOUSE
void
dragcfact(const Arg *arg)
{

    if (!mousecfact)
		return;

	int prev_x, prev_y, dist_x, dist_y;
	float fact;
	Client *c;
	XEvent ev;
	Time lasttime = 0;

	if (!(c = selmon->sel))
		return;
	if (c->isfloating) {
		resizemouse(arg);
		return;
	}
	if (c->isfullscreen) /* no support resizing fullscreen windows by mouse */
		return;

	restack(selmon);

	if (XGrabPointer(dpy, root, False, MOUSEMASK, GrabModeAsync, GrabModeAsync,
		None, cursor[CurResize]->cursor, CurrentTime) != GrabSuccess)
		return;
	XWarpPointer(dpy, None, c->win, 0, 0, 0, 0, c->w/2, c->h/2);

	prev_x = prev_y = -999999;

	do {
		XMaskEvent(dpy, MOUSEMASK|ExposureMask|SubstructureRedirectMask, &ev);
		switch(ev.type) {
		case ConfigureRequest:
		case Expose:
		case MapRequest:
			handler[ev.type](&ev);
			break;
		case MotionNotify:
			if ((ev.xmotion.time - lasttime) <= (1000 / 60))
				continue;
			lasttime = ev.xmotion.time;
			if (prev_x == -999999) {
				prev_x = ev.xmotion.x_root;
				prev_y = ev.xmotion.y_root;
			}

			dist_x = ev.xmotion.x - prev_x;
			dist_y = ev.xmotion.y - prev_y;

			if (abs(dist_x) > abs(dist_y)) {
				fact = (float) 4.0 * dist_x / c->mon->ww;
			} else {
				fact = (float) -4.0 * dist_y / c->mon->wh;
			}

			if (fact)
				setcfact(&((Arg) { .f = fact }));

			prev_x = ev.xmotion.x;
			prev_y = ev.xmotion.y;
			break;
		}
	} while (ev.type != ButtonRelease);


	XWarpPointer(dpy, None, c->win, 0, 0, 0, 0, c->w/2, c->h/2);

	XUngrabPointer(dpy, CurrentTime);
	while (XCheckMaskEvent(dpy, EnterWindowMask, &ev));
}
#endif

/* drag mfact with mouse */
#if USEMOUSE
void
dragmfact(const Arg *arg)
{
	unsigned int n;
	int py, px;
	int ax, ay, aw, ah;
    int layout = 0;
	int center = 0, horizontal = 0, mirror = 0, fixed = 0;
	double fact;
	Monitor *m;
	XEvent ev;
	Time lasttime = 0;

	m = selmon;

	int oh, ov, ih, iv;
	getgaps(m, &oh, &ov, &ih, &iv, &n);

	ax = m->wx;
	ay = m->wy;
	ah = m->wh;
	aw = m->ww;

	if (!n)
		return;
	else if (m->lt[m->sellt]->arrange == &tile) {
		layout = m->ltaxis[layout];
		if (layout < 0) {
			mirror = 1;
			layout *= -1;
		}
		if (layout > floatingmaster) {
			layout -= floatingmaster;
			fixed = 1;
		}

		if (layout == splithorizontal || layout == splithorizontaldualstack)
			horizontal = 1;
		else if (layout == splitcenteredvertical && (fixed || n - m->mastercount > 1))
			center = 1;
		else if (layout == floatingmaster) {
			center = 1;
			if (aw < ah)
				horizontal = 1;
		}
		else if (layout == splitcenteredhorizontal) {
			if (fixed || n - m->mastercount > 1)
				center = 1;
			horizontal = 1;
		}
	}
	/* do not allow mfact to be modified under certain conditions */
	if (!m->lt[m->sellt]->arrange
		|| (!fixed && m->mastercount && n <= m->mastercount)
		|| m->lt[m->sellt]->arrange == &ngrid
		|| m->lt[m->sellt]->arrange == &hgrid
		|| (m->lt[m->sellt]->arrange == &tile && m->ltaxis[layout] == nosplit)
	)
	return;

	ay += oh;
	ax += ov;
	aw -= 2*ov;
	ah -= 2*oh;

	if (center) {
		if (horizontal) {
			px = ax + aw / 2;
			py = ay + ah / 2 + (ah - 2*ih) * (m->mfact / 2.0) + ih / 2;
		} else { // vertical split
			px = ax + aw / 2 + (aw - 2*iv) * m->mfact / 2.0 + iv / 2;
			py = ay + ah / 2;
		}
	} else if (horizontal) {
		px = ax + aw / 2;
		if (mirror)
			py = ay + (ah - ih) * (1.0 - m->mfact) + ih / 2;
		else
			py = ay + ((ah - ih) * m->mfact) + ih / 2;
	} else { // vertical split
		if (mirror)
			px = ax + (aw - iv) * (1.0 - m->mfact) + iv / 2;
		else
			px = ax + ((aw - iv) * m->mfact) + iv / 2;
		py = ay + ah / 2;
	}

	if (XGrabPointer(dpy, root, False, MOUSEMASK, GrabModeAsync, GrabModeAsync,
		None, cursor[horizontal ? CurResizeVertArrow : CurResizeHorzArrow]->cursor, CurrentTime) != GrabSuccess)
		return;
	XWarpPointer(dpy, None, root, 0, 0, 0, 0, px, py);

	do {
		XMaskEvent(dpy, MOUSEMASK|ExposureMask|SubstructureRedirectMask, &ev);
		switch(ev.type) {
		case ConfigureRequest:
		case Expose:
		case MapRequest:
			handler[ev.type](&ev);
			break;
		case MotionNotify:
			if ((ev.xmotion.time - lasttime) <= (1000 / 40))
				continue;
			if (lasttime != 0) {
				px = ev.xmotion.x;
				py = ev.xmotion.y;
			}
			lasttime = ev.xmotion.time;

			if (center)
				if (horizontal)
					if (py - ay > ah / 2)
						fact = (double) 1.0 - (ay + ah - py - ih / 2) * 2 / (double) (ah - 2*ih);
					else
						fact = (double) 1.0 - (py - ay - ih / 2) * 2 / (double) (ah - 2*ih);
				else
					if (px - ax > aw / 2)
						fact = (double) 1.0 - (ax + aw - px - iv / 2) * 2 / (double) (aw - 2*iv);
					else
						fact = (double) 1.0 - (px - ax - iv / 2) * 2 / (double) (aw - 2*iv);
			else
				if (horizontal)
					fact = (double) (py - ay - ih / 2) / (double) (ah - ih);
				else
					fact = (double) (px - ax - iv / 2) / (double) (aw - iv);
			if (center)
				if (horizontal)
					if (py - ay > ah / 2)
						fact = (double) 1.0 - (ay + ah - py) * 2 / (double) ah;
					else
						fact = (double) 1.0 - (py - ay) * 2 / (double) ah;
				else
					if (px - ax > aw / 2)
						fact = (double) 1.0 - (ax + aw - px) * 2 / (double) aw;
					else
						fact = (double) 1.0 - (px - ax) * 2 / (double) aw;
			else
				if (horizontal)
					fact = (double) (py - ay) / (double) ah;
				else
					fact = (double) (px - ax) / (double) aw;

			if (!center && mirror)
				fact = 1.0 - fact;

			setmfact(&((Arg) { .f = 1.0 + fact }));
			px = ev.xmotion.x;
			py = ev.xmotion.y;
			break;
		}
	} while (ev.type != ButtonRelease);

	XUngrabPointer(dpy, CurrentTime);
	while (XCheckMaskEvent(dpy, EnterWindowMask, &ev));
}
#endif

/* draw the bar items (such as status, tags, indicators, etc.) */
void
drawbaritems(Monitor *m)
{
    Bar *bar;
	for (bar = m->bar; bar; bar = bar->next)
		drawbarwin(bar);
}

/* draw the full bar
 * arg 'selmon' = selected mon
 * arg 'm' = all mons
 */
void
drawbar(void)
{
	Monitor *m;

	for (m = mons; m; m = m->next)
		drawbaritems(m);
}

void
drawbarwin(Bar *bar)
{
	if (!bar->win)
		return;
	Monitor *mon;
	int r, w, mi;
	int rx, lx, rw, lw; // bar size, split between left and right if a center module is added
	const BarRule *br;
	BarWidthArg warg = { 0 };
	BarDrawArg darg  = { 0, 0 };

	for (mi = 0, mon = mons; mon && mon != bar->mon; mon = mon->next, mi++); // get the monitor index
	rw = lw = bar->bw;
	rx = lx = 0;

	drw_setscheme(drw, scheme[SchemeBar]);
	drw_rect(drw, lx, 0, lw, bh, 1, 1);
	for (r = 0; r < LENGTH(barrules); r++) {
		br = &barrules[r];
		if (br->bar != bar->idx || br->drawfunc == NULL || (br->monitor == 'A' && bar->mon != selmon))
			continue;
		if (br->monitor != 'A' && br->monitor != -1 && br->monitor != mi)
			continue;
		drw_setscheme(drw, scheme[SchemeBar]);
		warg.max_width = (br->alignment < bar_align_right_left ? lw : rw);
		w = br->widthfunc(bar, &warg);
		w = MIN(warg.max_width, w);

		if (lw <= 0) { // if left is exhausted then switch to right side, and vice versa
			lw = rw;
			lx = rx;
		} else if (rw <= 0) {
			rw = lw;
			rx = lx;
		}

		switch(br->alignment) {
		default:
		case bar_align_none:
		case bar_align_left_left:
		case bar_align_left:
			bar->x[r] = lx;
			if (lx == rx) {
				rx += w;
				rw -= w;
			}
			lx += w;
			lw -= w;
			break;
		case bar_align_left_right:
		case bar_align_right:
			bar->x[r] = lx + lw - w;
			if (lx == rx)
				rw -= w;
			lw -= w;
			break;
		case bar_align_left_center:
		case bar_align_center:
			bar->x[r] = lx + lw / 2 - w / 2;
			if (lx == rx) {
				rw = rx + rw - bar->x[r] - w;
				rx = bar->x[r] + w;
			}
			lw = bar->x[r] - lx;
			break;
		case bar_align_right_left:
			bar->x[r] = rx;
			if (lx == rx) {
				lx += w;
				lw -= w;
			}
			rx += w;
			rw -= w;
			break;
		case bar_align_right_right:
			bar->x[r] = rx + rw - w;
			if (lx == rx)
				lw -= w;
			rw -= w;
			break;
		case bar_align_right_center:
			bar->x[r] = rx + rw / 2 - w / 2;
			if (lx == rx) {
				lw = lx + lw - bar->x[r] + w;
				lx = bar->x[r] + w;
			}
			rw = bar->x[r] - rx;
			break;
		}
		bar->w[r] = w;
		darg.x = bar->x[r];
		darg.w = bar->w[r];
		br->drawfunc(bar, &darg);
	}
	drw_map(drw, bar->win, 0, 0, bar->bw, bar->bh);
}

void
enternotify(XEvent *e)
{
	Client *c;
	Monitor *m;

	if (clicktofocus)
		return;

	XCrossingEvent *ev = &e->xcrossing;

	if ((ev->mode != NotifyNormal || ev->detail == NotifyInferior) && ev->window != root)
		return;
	c = wintoclient(ev->window);
	m = c ? c->mon : wintomon(ev->window);
	if (m != selmon) {
		unfocus(selmon->sel, 1);
		selmon = m;
	} else if (!c || c == selmon->sel)
		return;
	focus(c);
}

void
expose(XEvent *e)
{
	Monitor *m;
	XExposeEvent *ev = &e->xexpose;

	if (ev->count == 0 && (m = wintomon(ev->window))) {
		drawbaritems(m);

    }
}

void
focus(Client *c)
{
    if (!c || !ISVISIBLE(c))
			for (c = selmon->stack; c && (!ISVISIBLE(c) || HIDDEN(c)); c = c->snext);
	if (selmon->sel && selmon->sel != c) {
		unfocus(selmon->sel, 0);

		if (selmon->hidsel) {
			hidewin(selmon->sel);
			if (c)
				arrange(c->mon);
			selmon->hidsel = 0;
		}
	}

	if (c) {
		if (c->mon != selmon)
			selmon = c->mon;
		if (c->isurgent && urgentwindows)
			seturgent(c, 0);
		detachstack(c);
		attachstack(c);
		grabbuttons(c, 1);

		/* draw window border for the focused window */
		XSetWindowBorder(dpy, c->win, scheme[SchemeBorderSel][ColBorder].pixel);

        /* lastfocused may be us if another window was unmanaged */
		if (lastfocused && lastfocused != c)
			XSetWindowBorder(dpy, lastfocused->win, scheme[SchemeBorderNorm][ColBorder].pixel);

		/* set focus */
		setfocus(c);

#if USEFADE
		/* set opacity to activeopacity */
		opacity(c, activeopacity);
#endif
	} else {
		XSetInputFocus(dpy, root, RevertToPointerRoot, CurrentTime);
		XDeleteProperty(dpy, root, netatom[NetActiveWindow]);
	}
	selmon->sel = c;
    if (selmon->sel && selmon->hideunselectedborder) {
        resizeclient(c, c->x, c->y, c->w, c->h);
    }
	drawbar();
}

/* there are some notitle focus acquiring clients needing extra handling */
void
focusin(XEvent *e)
{
	XFocusChangeEvent *ev = &e->xfocus;

	if (selmon->sel && ev->window != selmon->sel->win)
		setfocus(selmon->sel);
}

void
focusmaster(const Arg *arg)
{
	Client *master;

	if (selmon->mastercount > 1)
		return;
	if (!selmon->sel || (selmon->sel->isfullscreen && lockfullscreen))
		return;

	master = nexttiled(selmon->clients);

	if (!master)
		return;

	int i;
	for (i = 0; !(selmon->tagset[selmon->seltags] & 1 << i); i++);
	i++;

	if (selmon->sel == master) {
		if (selmon->tagmarked[i] && ISVISIBLE(selmon->tagmarked[i]))
			focus(selmon->tagmarked[i]);
	} else {
		selmon->tagmarked[i] = selmon->sel;
		focus(master);
	}
}

void
focusmon(const Arg *arg)
{
	Monitor *m;

	if (!mons->next)
		return;
	if ((m = dirtomon(arg->i)) == selmon)
		return;
	unfocus(selmon->sel, 0);
	selmon = m;
	focus(NULL);
    selmon->allowwarp = 1;
    warp(selmon->sel);
}

void
focusstackvis(const Arg *arg)
{
	focusstack(arg->i, 0);
}

void
focusstackhid(const Arg *arg)
{
	focusstack(arg->i, 1);
}

void
focusstack(int inc, int hid)
{
	Client *c = NULL, *i;
    selmon->allowwarp = 1;

	if ((!selmon->sel && !hid) || (selmon->sel && selmon->sel->isfullscreen && lockfullscreen))
 		return;

	if (!selmon->clients)
		return;

	if (inc > 0) {
		if (selmon->sel)
			for (c = selmon->sel->next;
					 c && (!ISVISIBLE(c) || (!hid && HIDDEN(c)));
					 c = c->next);
		if (!c)
			  for (c = selmon->clients;
						       c && (!ISVISIBLE(c) || (!hid && HIDDEN(c)));
							   c = c->next);
	} else {
		    if (selmon->sel) {
				    for (i = selmon->clients; i != selmon->sel; i = i->next)
						    if (ISVISIBLE(i) && !(!hid && HIDDEN(i)))
								    c = i;
		    } else
				    c = selmon->clients;
		if (!c)
			for (; i; i = i->next)
					if (ISVISIBLE(i) && !(!hid && HIDDEN(i)))
					c = i;
	}

	if (c) {
		focus(c);
		restack(selmon);

		if (HIDDEN(c)) {
			showwin(c);
			c->mon->hidsel = 1;
		}
	}
}


Atom
getatomprop(Client *c, Atom prop)
{
	int di;
	unsigned long dl;
	unsigned char *p = NULL;
	Atom da, atom = None;

    #if USESYSTRAY
    Atom req = XA_ATOM;
	if (prop == xatom[XembedInfo])
		req = xatom[XembedInfo];
    #endif

    #if USESYSTRAY
    if (XGetWindowProperty(dpy, c->win, prop, 0L, sizeof atom, False, req,
    #else
	if (XGetWindowProperty(dpy, c->win, prop, 0L, sizeof atom, False, XA_ATOM,
    #endif
		&da, &di, &dl, &dl, &p) == Success && p) {
		atom = *(Atom *)p;

    #if USESYSTRAY
     	if (da == xatom[XembedInfo] && dl == 2)
		    atom = ((Atom *)p)[1];
    #endif

		XFree(p);
	}
	return atom;
}

#if USEWINICON
static uint32_t prealpha(uint32_t p) {
	uint8_t a = p >> 24u;
	uint32_t rb = (a * (p & 0xFF00FFu)) >> 8u;
	uint32_t g = (a * (p & 0x00FF00u)) >> 8u;
	return (rb & 0xFF00FFu) | (g & 0x00FF00u) | (a << 24u);
}

Picture
geticonprop(Window win, unsigned int *picw, unsigned int *pich)
{
	int format;
	unsigned long n, extra, *p = NULL;
	Atom real;

	if (XGetWindowProperty(dpy, win, netatom[NetWMIcon], 0L, LONG_MAX, False, AnyPropertyType,
						   &real, &format, &n, &extra, (unsigned char **)&p) != Success)
		return None;
	if (n == 0 || format != 32) { XFree(p); return None; }

	unsigned long *bstp = NULL;
	uint32_t w, h, sz;
	{
		unsigned long *i; const unsigned long *end = p + n;
		uint32_t bstd = UINT32_MAX, d, m;
		for (i = p; i < end - 1; i += sz) {
			if ((w = *i++) >= 16384 || (h = *i++) >= 16384) { XFree(p); return None; }
			if ((sz = w * h) > end - i) break;
			if ((m = w > h ? w : h) >= iconsize && (d = m - iconsize) < bstd) { bstd = d; bstp = i; }
		}
		if (!bstp) {
			for (i = p; i < end - 1; i += sz) {
				if ((w = *i++) >= 16384 || (h = *i++) >= 16384) { XFree(p); return None; }
				if ((sz = w * h) > end - i) break;
				if ((d = iconsize - (w > h ? w : h)) < bstd) { bstd = d; bstp = i; }
			}
		}
		if (!bstp) { XFree(p); return None; }
	}

	if ((w = *(bstp - 2)) == 0 || (h = *(bstp - 1)) == 0) { XFree(p); return None; }

	uint32_t icw, ich;
	if (w <= h) {
		ich = iconsize; icw = w * iconsize / h;
		if (icw == 0) icw = 1;
	}
	else {
		icw = iconsize; ich = h * iconsize / w;
		if (ich == 0) ich = 1;
	}
	*picw = icw; *pich = ich;

	uint32_t i, *bstp32 = (uint32_t *)bstp;
	for (sz = w * h, i = 0; i < sz; ++i) bstp32[i] = prealpha(bstp[i]);

	Picture ret = drw_picture_create_resized(drw, (char *)bstp, w, h, icw, ich);
	XFree(p);

	return ret;
}

void
freeicon(Client *c)
{
	if (c->icon) {
		XRenderFreePicture(dpy, c->icon);
		c->icon = None;
	}
}

void
updateicon(Client *c)
{
	freeicon(c);
	c->icon = geticonprop(c->win, &c->icw, &c->ich);
}
#endif

int
getrootptr(int *x, int *y)
{
	int di;
	unsigned int dui;
	Window dummy;

	return XQueryPointer(dpy, root, &dummy, &dummy, x, y, &di, &di, &dui);
}

long
getstate(Window w)
{
	int format;
	long result = -1;
	unsigned char *p = NULL;
	unsigned long n, extra;
	Atom real;

	if (XGetWindowProperty(dpy, w, wmatom[WMState], 0L, 2L, False, wmatom[WMState],
		&real, &format, &n, &extra, (unsigned char **)&p) != Success)
		return -1;
	if (n != 0)
		result = *p;
	XFree(p);
	return result;
}


int
gettextprop(Window w, Atom atom, char *text, unsigned int size)
{
	char **list = NULL;
	int n;
	XTextProperty name;

	if (!text || size == 0)
		return 0;
	text[0] = '\0';
	if (!XGetTextProperty(dpy, w, &name, atom) || !name.nitems)
		return 0;
	if (name.encoding == XA_STRING) {
		strncpy(text, (char *)name.value, size - 1);
    } else if (XmbTextPropertyToTextList(dpy, &name, &list, &n) >= Success && n > 0 && *list) {
		strncpy(text, *list, size - 1);
		XFreeStringList(list);
	}
	text[size - 1] = '\0';
	XFree(name.value);
	return 1;
}

void
grabbuttons(Client *c, int focused)
{
	updatenumlockmask();
	{
		#if USEMOUSE
		unsigned int i, j;
		unsigned int modifiers[] = { 0, LockMask, numlockmask, numlockmask|LockMask };
		#endif
		XUngrabButton(dpy, AnyButton, AnyModifier, c->win);
		if (!focused)
			XGrabButton(dpy, AnyButton, AnyModifier, c->win, False,
				BUTTONMASK, GrabModeSync, GrabModeSync, None, None);
		#if USEMOUSE
		for (i = 0; i < LENGTH(buttons); i++)
			if (buttons[i].click == clickclient)
				for (j = 0; j < LENGTH(modifiers); j++)
					XGrabButton(dpy, buttons[i].button,
						buttons[i].mask | modifiers[j],
						c->win, False, BUTTONMASK,
						GrabModeAsync, GrabModeSync, None, None);
		#endif
	}
}

void
grabkeys(void)
{
	updatenumlockmask();
	{
		unsigned int i, j, k;
		unsigned int modifiers[] = { 0, LockMask, numlockmask, numlockmask|LockMask };
		int start, end, skip;
		KeySym *syms;

		XUngrabKey(dpy, AnyKey, AnyModifier, root);
		XDisplayKeycodes(dpy, &start, &end);
		syms = XGetKeyboardMapping(dpy, start, end - start + 1, &skip);
		if (!syms)
			return;
		for (k = start; k <= end; k++)
			for (i = 0; i < LENGTH(keys); i++)
				/* skip modifier codes, we do that ourselves */
				if (keys[i].keysym == syms[(k - start) * skip])
					for (j = 0; j < LENGTH(modifiers); j++)
						XGrabKey(dpy, k,
							 keys[i].mod | modifiers[j],
							 root, True,
							 GrabModeAsync, GrabModeAsync);
		XFree(syms);
	}
}

void
hide(const Arg *arg)
{
	hidewin(selmon->sel);
	focus(NULL);
	arrange(selmon);
}

void
hidewin(Client *c) {
	if (!c || HIDDEN(c))
		return;

    c->ishidden = 1; /* is hidden */

	Window w = c->win;
	static XWindowAttributes ra, ca;

	XGrabServer(dpy);
	XGetWindowAttributes(dpy, root, &ra);
	XGetWindowAttributes(dpy, w, &ca);
	XSelectInput(dpy, root, ra.your_event_mask & ~SubstructureNotifyMask);
	XSelectInput(dpy, w, ca.your_event_mask & ~StructureNotifyMask);
	XUnmapWindow(dpy, w);
	setclientstate(c, IconicState);
	XSelectInput(dpy, root, ra.your_event_mask);
	XSelectInput(dpy, w, ca.your_event_mask);
	XUngrabServer(dpy);
}

#if USEIPC
int
handlexevent(struct epoll_event *ev)
{
	if (ev->events & EPOLLIN) {
		XEvent ev;
		while (running && XPending(dpy)) {
			XNextEvent(dpy, &ev);
			if (handler[ev.type]) {
				handler[ev.type](&ev); /* call handler */
				ipc_send_events(mons, &lastselmon, selmon);
			}
		}
	} else if (ev-> events & EPOLLHUP) {
		return -1;
	}

	return 0;
}
#endif

void
incmastercount(const Arg *arg)
{
    if (i3mastercount) {
        selmon->mastercount = selmon->pertag->mastercounts[selmon->pertag->curtag] = !selmon->mastercount;
    } else {
        selmon->mastercount = selmon->pertag->mastercounts[selmon->pertag->curtag] = MAX(selmon->mastercount + arg->i, 0);
    }

	arrange(selmon);
}

void
mirrorlayout(const Arg *arg)
{
	if (!selmon->lt[selmon->sellt]->arrange)
		return;
	selmon->ltaxis[layout] *= -1;
	selmon->pertag->ltaxis[selmon->pertag->curtag][0] = selmon->ltaxis[layout];
	arrange(selmon);
}

void
rotatelayoutaxis(const Arg *arg)
{
	int incr = (arg->i > 0 ? 1 : -1);
	int axis = abs(arg->i) - 1;

	if (!selmon->lt[selmon->sellt]->arrange)
		return;
	if (axis == layout) {
		if (selmon->ltaxis[layout] >= 0) {
			selmon->ltaxis[layout] += incr;
			if (selmon->ltaxis[layout] >= layoutlast)
				selmon->ltaxis[layout] = 0;
			else if (selmon->ltaxis[layout] < 0)
				selmon->ltaxis[layout] = layoutlast - 1;
		} else {
			selmon->ltaxis[layout] -= incr;
			if (selmon->ltaxis[layout] <= -layoutlast)
				selmon->ltaxis[layout] = 0;
			else if (selmon->ltaxis[layout] > 0)
				selmon->ltaxis[layout] = -layoutlast + 1;
		}
	} else {
		selmon->ltaxis[axis] += incr;
		if (selmon->ltaxis[axis] >= axislast)
			selmon->ltaxis[axis] = 0;
		else if (selmon->ltaxis[axis] < 0)
			selmon->ltaxis[axis] = axislast - 1;
	}
	selmon->pertag->ltaxis[selmon->pertag->curtag][axis] = selmon->ltaxis[axis];
	arrange(selmon);
	setltsymbols(selmon, 0);
}

void
incstackcount(const Arg *arg)
{
	selmon->stackcount = selmon->pertag->stackcounts[selmon->pertag->curtag] = MAX(selmon->stackcount + arg->i, 0);
	arrange(selmon);
}

#ifdef XINERAMA
static int
isuniquegeom(XineramaScreenInfo *unique, size_t n, XineramaScreenInfo *info)
{
	while (n--)
		if (unique[n].x_org == info->x_org && unique[n].y_org == info->y_org
		&& unique[n].width == info->width && unique[n].height == info->height)
			return 0;
	return 1;
}
#endif /* XINERAMA */

void
keypress(XEvent *e)
{
	unsigned int i, j;
	KeySym keysym;
	XKeyEvent *ev;
	int current = 0;
	unsigned int modifiers[] = { 0, LockMask, numlockmask, numlockmask|LockMask };

	ev = &e->xkey;
	keysym = XKeycodeToKeysym(dpy, (KeyCode)ev->keycode, 0);
	for (i = 0; i < LENGTH(keys); i++) {
		if (keysym == keys[i].keysym && keys[i].chain == -1
                && ev->type == keys[i].type
				&& CLEANMASK(keys[i].mod) == CLEANMASK(ev->state)
				&& keys[i].func)
			keys[i].func(&(keys[i].arg));
		else if (keysym == keys[i].chain && keychain == -1
                && ev->type != keys[i].type
				&& CLEANMASK(keys[i].mod) == CLEANMASK(ev->state)
				&& keys[i].func) {
			current = 1;
			keychain = keysym;
			for (j = 0; j < LENGTH(modifiers); j++)
				XGrabKey(dpy, AnyKey, 0 | modifiers[j], root,
						True, GrabModeAsync, GrabModeAsync);
		} else if (!current && keysym == keys[i].keysym
				&& keychain != -1
				&& keys[i].chain == keychain
				&& keys[i].func)
			keys[i].func(&(keys[i].arg));
	}
	if (!current) {
		keychain = -1;
		grabkeys();
	}
}

int
getsignal(void)
{
	char signal[256];
	char indicator[9] = "#cmd:";
	char str_signum[16];
	int i, v, signum;
	size_t len_signal, len_indicator = strlen(indicator);

	/* Get root name property */
	if (gettextprop(root, XA_WM_NAME, signal, sizeof(signal))) {
		len_signal = strlen(signal);

		/* Check if this is indeed a fake signal */
		if (len_indicator > len_signal ? 0 : strncmp(indicator, signal, len_indicator) == 0) {
			memcpy(str_signum, &signal[len_indicator], len_signal - len_indicator);
			str_signum[len_signal - len_indicator] = '\0';

			/* Convert string value into managable integer */
			for (i = signum = 0; i < strlen(str_signum); i++) {
				v = str_signum[i] - '0';
				if (v >= 0 && v <= 9) {
					signum = signum * 10 + v;
				}
			}

			/* Check if a signal was found, and if so handle it */
			if (signum)
				for (i = 0; i < LENGTH(signals); i++)
					if (signum == signals[i].signum && signals[i].func)
						signals[i].func(&(signals[i].arg));

			/* A fake signal was sent */
			return 1;
		}
	}

	/* No fake signal was sent, so proceed with update */
	return 0;
}

void
setgaps(int oh, int ov, int ih, int iv)
{
	if (oh < 0) oh = 0;
	if (ov < 0) ov = 0;
	if (ih < 0) ih = 0;
	if (iv < 0) iv = 0;

	selmon->gapsizeoh = oh;
	selmon->gapsizeov = ov;
	selmon->gapsizeih = ih;
	selmon->gapsizeiv = iv;

	arrange(selmon);
    updatebars();
}

void
getgaps(Monitor *m, int *oh, int *ov, int *ih, int *iv, unsigned int *nc)
{
	unsigned int n, oe, ie;
	oe = ie = enablegaps;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	if (smartgaps && n == 1) {
		oe = smartgapsize; /* outer gaps disabled when only one client */
	}

	*oh = m->gapsizeoh*oe; /* outer horizontal gap */
	*ov = m->gapsizeov*oe; /* outer vertical gap */
	*ih = m->gapsizeih*ie; /* inner horizontal gap */
	*iv = m->gapsizeiv*ie; /* inner vertical gap */
	*nc = n;            /* number of clients */
}

void
show(const Arg *arg)
{
	if (selmon->hidsel)
		selmon->hidsel = 0;
	showwin(selmon->sel);
}

void
showall(const Arg *arg)
{
	Client *c = NULL;
	selmon->hidsel = 0;
	for (c = selmon->clients; c; c = c->next) {
		if (ISVISIBLE(c))
			showwin(c);
	}
	if (!selmon->sel) {
		for (c = selmon->clients; c && !ISVISIBLE(c); c = c->next);
		if (c)
			focus(c);
	}
	restack(selmon);
}

void
hideall(const Arg *arg)
{
	Client *c = NULL;
	for (c = selmon->clients; c; c = c->next) {
			hidewin(c);
			hidewin(selmon->sel);
	}

	restack(selmon);
}

void
showwin(Client *c)
{
	if (!c || !HIDDEN(c))
		return;

    c->ishidden = 0; /* is no longer hidden */

	XMapWindow(dpy, c->win);
	setclientstate(c, NormalState);
	arrange(c->mon);
}

void
showhide(Client *c)
{
	if (!c)
		return;
	if (ISVISIBLE(c)) {
		/* show clients top down */
		if (c->needresize && autoresize) {
			c->needresize = 0;
			XMoveResizeWindow(dpy, c->win, c->x, c->y, c->w, c->h);
		} else if (autoresize) {
		    c->needresize = 0;
            if (selmon->sel == c || !selmon->hideunselectedborder) {
                XMoveWindow(dpy, c->win, c->x, c->y);
            } else {
                XMoveWindow(dpy, c->win, c->x + bordersize, c->y + bordersize);
            }
		}

        window_map(dpy, c, 1);
        showhide(c->snext);
	} else {
		/* hide clients bottom up */
		showhide(c->snext);
        window_unmap(dpy, c->win, root, 1, c);
	}
}

void
resetbarpadding(const Arg *arg)
{
		resetbarpaddingov(NULL);
		resetbarpaddingoh(NULL);
}

void
resetbarpaddingov(const Arg *arg)
{
        Bar *bar;
		vp = (barposition == 1) ? barpaddingov : - barpaddingov;

        updatebarpos(selmon);
        for (bar = selmon->bar; bar; bar = bar->next)
		    XMoveResizeWindow(dpy, bar->win, bar->bx, bar->by, bar->bw, bar->bh);
		arrange(selmon);

}

void
resetbarpaddingoh(const Arg *arg)
{
        Bar *bar;
		sp = barpaddingoh;

        updatebarpos(selmon);
        for (bar = selmon->bar; bar; bar = bar->next)
		    XMoveResizeWindow(dpy, bar->win, bar->bx, bar->by, bar->bw, bar->bh);
		arrange(selmon);
}

void
togglebarpadding(const Arg *arg)
{
		togglebarpaddingov(arg);
		togglebarpaddingoh(arg);
}

void
togglebarpaddingov(const Arg *arg)
{
        Bar *bar;
		/* if its more than 1, disable padding, else enable padding */
		if (vp > 0) {
				/* save padding */
				selmon->cpadv = vp;
				vp = 0;
		} else
				vp = selmon->cpadv;

        updatebarpos(selmon);
        for (bar = selmon->bar; bar; bar = bar->next)
		    XMoveResizeWindow(dpy, bar->win, bar->bx, bar->by, bar->bw, bar->bh);
		arrange(selmon);
        updatebars();

}

void
togglebarpaddingoh(const Arg *arg)
{
        Bar *bar;
		/* if its more than 1, disable padding, else enable padding */
		if (sp > 0) {
				/* save padding */
				selmon->cpadh = sp;
				sp = 0;
		} else
				sp = selmon->cpadh;

        updatebarpos(selmon);
        for (bar = selmon->bar; bar; bar = bar->next)
		    XMoveResizeWindow(dpy, bar->win, bar->bx, bar->by, bar->bw, bar->bh);
		arrange(selmon);
        updatebars();
}

void
setbarpadding(const Arg *arg)
{
		setbarpaddingov(arg);
		setbarpaddingoh(arg);
}

void
setbarpaddingov(const Arg *arg)
{
        Bar *bar;

		vp += arg->i;

        /* set a max of 100 */
        if (vp >= 100) {
            vp -= arg->i;
            return;
        }

		/* prevent bar weirdness */
		if (vp < 0) {
			vp = 0;
			return;
		}

        updatebarpos(selmon);
        for (bar = selmon->bar; bar; bar = bar->next)
		    XMoveResizeWindow(dpy, bar->win, bar->bx, bar->by, bar->bw, bar->bh);
		arrange(selmon);
        updatebars();
}

void
setbarpaddingoh(const Arg *arg)
{
        Bar *bar;

		sp += arg->i;

        /* set a max of 100 */
        if (sp > 100) {
            sp -= arg->i;
            return;
        }

		/* prevent bar weirdness */
		if (sp < 0) {
			sp = 0;
			return;
		}

        updatebarpos(selmon);
        for (bar = selmon->bar; bar; bar = bar->next)
		    XMoveResizeWindow(dpy, bar->win, bar->bx, bar->by, bar->bw, bar->bh);
		arrange(selmon);
        updatebars();
}

void
setbarheight(const Arg *arg)
{
		bh += arg->i;
        Bar *bar;

		if (bh < drw->font->h)
				bh = drw->font->h + barpaddingiv;

        updatebarpos(selmon);
        for (bar = selmon->bar; bar; bar = bar->next)
		    XMoveResizeWindow(dpy, bar->win, bar->bx, bar->by, bar->bw, bar->bh);
		arrange(selmon);
        updatebars();
}

void
setbpgaps(const Arg *arg)
{
		setbarpaddingov(arg);
		setbarpaddingoh(arg);
        incrgaps(arg);

        updatebarpos(selmon);
        arrange(selmon);
        updatebars();
}

void
resetbpgaps(const Arg *arg)
{
        resetbarpaddingoh(arg);
        resetbarpaddingov(arg);
        defaultgaps(arg);
}

void
resetbarheight(const Arg *arg)
{
		bh = drw->font->h + barheight + barpaddingiv;
        Bar *bar;

        updatebarpos(selmon);
        for (bar = selmon->bar; bar; bar = bar->next)
		    XMoveResizeWindow(dpy, bar->win, bar->bx, bar->by, bar->bw, bar->bh);
		arrange(selmon);
        updatebars();
}

void
togglegaps(const Arg *arg)
{
        enablegaps = !enablegaps;
        arrange(selmon);
        updatebars();
}

void
defaultgaps(const Arg *arg)
{
	setgaps(gapsizeoh, gapsizeov, gapsizeih, gapsizeiv);
}

void
incrgaps(const Arg *arg)
{
	setgaps(
		selmon->gapsizeoh + arg->i,
		selmon->gapsizeov + arg->i,
		selmon->gapsizeih + arg->i,
		selmon->gapsizeiv + arg->i
	);
}

void
incrigaps(const Arg *arg)
{
	setgaps(
		selmon->gapsizeoh,
		selmon->gapsizeov,
		selmon->gapsizeih + arg->i,
		selmon->gapsizeiv + arg->i
	);
}

void
incrogaps(const Arg *arg)
{
	setgaps(
		selmon->gapsizeoh + arg->i,
		selmon->gapsizeov + arg->i,
		selmon->gapsizeih,
		selmon->gapsizeiv
	);
}

void
killclient(const Arg *arg)
{
	if (!selmon->sel || selmon->sel->ispermanent)
		return;
    #if USESYSTRAY
    if (!sendevent(selmon->sel->win, wmatom[WMDelete], NoEventMask, wmatom[WMDelete], CurrentTime, 0, 0, 0)) {
    #else
	if (!sendevent(selmon->sel, wmatom[WMDelete])) {
    #endif
		XGrabServer(dpy);
		XSetErrorHandler(xerrordummy);
		XSetCloseDownMode(dpy, DestroyAll);
		XKillClient(dpy, selmon->sel->win);
		XSync(dpy, False);
		XSetErrorHandler(xerror);
		XUngrabServer(dpy);
	}
}

void
killunsel(const Arg *arg)
{
	Client *i = NULL;

	if (!selmon->sel)
		return;

	for (i = selmon->clients; i; i = i->next) {
		if (ISVISIBLE(i) && i != selmon->sel) {
            #if USESYSTRAY
		    if (!sendevent(i->win, wmatom[WMDelete], NoEventMask, wmatom[WMDelete], CurrentTime, 0, 0, 0)) {
            #else
		    if (!sendevent(selmon->sel, wmatom[WMDelete])) {
            #endif
				XGrabServer(dpy);
				XSetErrorHandler(xerrordummy);
				XSetCloseDownMode(dpy, DestroyAll);
				XKillClient(dpy, i->win);
				XSync(dpy, False);
				XSetErrorHandler(xerror);
				XUngrabServer(dpy);
			}
		}
	}
}

void
manage(Window w, XWindowAttributes *wa)
{
	Client *c, *t = NULL, *term = NULL;
	Window trans = None;
	XWindowChanges wc;

	c = ecalloc(1, sizeof(Client));
	c->win = w;
	c->pid = winpid(w);

	if (getatomprop(c, netatom[NetWMWindowType]) == netatom[NetWMWindowTypeDesktop]) {
		XMapWindow(dpy, c->win);
		XLowerWindow(dpy, c->win);
		free(c);
		return;
	}

	/* geometry */
	c->x = c->oldx = wa->x;
	c->y = c->oldy = wa->y;
	c->w = c->oldw = wa->width;
	c->h = c->oldh = wa->height;
	c->oldbw = wa->border_width;
	c->cfact = 1.0;

    #if USEWINICON
    updateicon(c);
    #endif
	updatetitle(c);
	if (XGetTransientForHint(dpy, w, &trans) && (t = wintoclient(trans))) {
		c->mon = t->mon;
		c->tags = t->tags;
	} else {
		c->mon = selmon;
		applyrules(c);
		term = termforwin(c);
	}

	if (unmanaged) {
		XMapWindow(dpy, c->win);
		if (unmanaged == 1)
			XRaiseWindow(dpy, c->win);
		else if (unmanaged == 2)
			XLowerWindow(dpy, c->win);
		free(c);
		unmanaged = 0;
		return;
	}

	if (c->x + WIDTH(c) > c->mon->wx + c->mon->ww)
		c->x = c->mon->wx + c->mon->ww - WIDTH(c);
	if (c->y + HEIGHT(c) > c->mon->wy + c->mon->wh)
		c->y = c->mon->wy + c->mon->wh - HEIGHT(c);
	c->x = MAX(c->x, c->mon->wx);
	c->x = MAX(c->x, c->mon->mx);
    c->y = MAX(c->y, ((c->mon->bar->by == c->mon->my) && (c->x + (c->w / 2) >= c->mon->wx)
 		&& (c->x + (c->w / 2) < c->mon->wx + c->mon->ww)) ? bh : c->mon->my);
	c->bw = bordersize;

    wc.border_width = c->bw;

	XConfigureWindow(dpy, w, CWBorderWidth, &wc);
	XSetWindowBorder(dpy, w, scheme[SchemeBorderNorm][ColBorder].pixel);
	configure(c); /* propagates border_width, if size doesn't change */
	updatewindowtype(c);
	updatesizehints(c);
	updatewmhints(c);
	{
		int format;
		unsigned long *data, n, extra;
		Monitor *m;
		Atom atom;
		if (XGetWindowProperty(dpy, c->win, netatom[NetClientInfo], 0L, 2L, False, XA_CARDINAL,
				&atom, &format, &n, &extra, (unsigned char **)&data) == Success && n == 2) {
			c->tags = *data;
			for (m = mons; m; m = m->next) {
				if (m->num == *(data+1)) {
					c->mon = m;
					break;
				}
			}
		}
		if (n > 0)
			XFree(data);
	}
	setclienttagprop(c);

	if (centerfloating) {
		c->x = c->mon->mx + (c->mon->mw - WIDTH(c)) / 2;
		c->y = c->mon->my + (c->mon->mh - HEIGHT(c)) / 2;
	}

	if (savefloat)
	{
	   c->sfx = c->x;
	   c->sfy = c->y;
	   c->sfw = c->w;
	   c->sfh = c->h;
	}

	updatemotifhints(c);
	XSelectInput(dpy, w, EnterWindowMask|FocusChangeMask|PropertyChangeMask|StructureNotifyMask);
	grabbuttons(c, 0);
	if (!c->isfloating) {
		c->isfloating = c->oldstate = t || c->isfixed;
    }

	XRaiseWindow(dpy, c->win);

	switch(attachdirection){
		case 1:
			attachabove(c);
			break;
		case 2:
			attachaside(c);
			break;
		case 3:
			attachbelow(c);
			break;
		case 4:
			attachbottom(c);
			break;
		case 5:
			attachtop(c);
			break;
		default:
			attach(c);
	}
	attachstack(c);
	setclienttagprop(c);
	XChangeProperty(dpy, root, netatom[NetClientList], XA_WINDOW, 32, PropModeAppend,
		(unsigned char *) &(c->win), 1);
    XChangeProperty(dpy, root, netatom[NetClientListStacking], XA_WINDOW, 32, PropModePrepend,
		(unsigned char *) &(c->win), 1);
	XMoveResizeWindow(dpy, c->win, c->x + 2 * tw, c->y, c->w, c->h); /* some windows require this */
	if (!HIDDEN(c))
		    setclientstate(c, NormalState);
	if (c->mon == selmon)
		unfocus(selmon->sel, 0);
	c->mon->sel = c;
	if (!HIDDEN(c))
		XMapWindow(dpy, c->win);
	if (term)
		swallow(term, c);
	arrange(c->mon);
	focus(NULL);
}

void
mappingnotify(XEvent *e)
{
	XMappingEvent *ev = &e->xmapping;

	XRefreshKeyboardMapping(ev);
	if (ev->request == MappingKeyboard)
		grabkeys();
}

void
maprequest(XEvent *e)
{
	static XWindowAttributes wa;
	XMapRequestEvent *ev = &e->xmaprequest;

    #if USESYSTRAY
    Client *i;

    if (!selmon->hidesystray && systray && (i = wintosystrayicon(ev->window))) {
		sendevent(i->win, netatom[Xembed], StructureNotifyMask, CurrentTime, XEMBED_WINDOW_ACTIVATE, 0, systray->win, XEMBED_EMBEDDED_VERSION);
		drawbarwin(systray->bar);
	}
    #endif

	if (!XGetWindowAttributes(dpy, ev->window, &wa) || wa.override_redirect)
		return;
    if (!wa.depth)
		return;
	if (!wintoclient(ev->window))
		manage(ev->window, &wa);
}

void
motionnotify(XEvent *e)
{
	static Monitor *mon = NULL;
	Monitor *m;
	m = 0;
	Client *sel;
	XMotionEvent *ev = &e->xmotion;

	if (clicktofocus || ev->window != root)
		return;
	if ((m = recttomon(ev->x_root, ev->y_root, 1, 1)) != mon && mon) {
		sel = selmon->sel;
		selmon = m;
		unfocus(sel, 1);
		focus(NULL);
	}
	mon = m;
}

void
window_set_state(Display *dpy, Window win, long state)
{
	long data[] = { state, None };

	XChangeProperty(dpy, win, wmatom[WMState], wmatom[WMState], 32,
		PropModeReplace, (unsigned char*)data, 2);
}

void
window_map(Display *dpy, Client *c, int deiconify)
{
    if (!windowmap) {
        XMoveWindow(dpy, c->win, c->x, c->y);
        if ((!c->mon->lt[c->mon->sellt]->arrange || c->isfloating) && !c->isfullscreen)
            resize(c, c->x, c->y, c->w, c->h, 0);

        return;
    }

	Window win = c->win;

    /* fix: hidden windows immediately get mapped */
    if (c->ishidden)
      return;

	if (deiconify)
		window_set_state(dpy, win, NormalState);

	XMoveResizeWindow(dpy, c->win, c->x, c->y, c->w, c->h);
	XSetInputFocus(dpy, win, RevertToPointerRoot, CurrentTime);

    XMapWindow(dpy, win);

    focus(NULL);
}

void
window_unmap(Display *dpy, Window win, Window root, int iconify, Client *c)
{
	static XWindowAttributes ca, ra;

    if (!windowmap) {
        XMoveWindow(dpy, c->win, WIDTH(c) * -2, c->y);
        return;
    }

	XGrabServer(dpy);
	XGetWindowAttributes(dpy, root, &ra);
	XGetWindowAttributes(dpy, win, &ca);

	/* Prevent UnmapNotify events */
	XSelectInput(dpy, root, ra.your_event_mask & ~SubstructureNotifyMask);
	XSelectInput(dpy, win, ca.your_event_mask & ~StructureNotifyMask);

	XUnmapWindow(dpy, win);

	if (iconify)
		window_set_state(dpy, win, IconicState);

	XSelectInput(dpy, root, ra.your_event_mask);
	XSelectInput(dpy, win, ca.your_event_mask);
	XUngrabServer(dpy);

    focus(NULL);
}

void
toggleview(const Arg *arg)
{
	unsigned int newtagset = selmon->tagset[selmon->seltags] ^ (arg->ui & TAGMASK);

	if (newtagset) {
        #if USETAGPREVIEW
        takepreview();
        #endif
		selmon->tagset[selmon->seltags] = newtagset;
		focus(NULL);
		arrange(selmon);
	}
}

void
moveresize(const Arg *arg) {
	/* only floating windows can be moved */
	Client *c;
	c = selmon->sel;
	int x, y, w, h, nx, ny, nw, nh, ox, oy, ow, oh;
	char xAbs, yAbs, wAbs, hAbs;
	int msx, msy, dx, dy, nmx, nmy;
	unsigned int dui;
	Window dummy;

	if (!c || !arg)
		return;
	if (selmon->lt[selmon->sellt]->arrange && !c->isfloating)
		return;
	if (sscanf((char *)arg->v, "%d%c %d%c %d%c %d%c", &x, &xAbs, &y, &yAbs, &w, &wAbs, &h, &hAbs) != 8)
		return;

	/* compute new window position; prevent window from be positioned outside the current monitor */
	nw = c->w + w;
	if (wAbs == 'W')
		nw = w < selmon->mw - 2 * c->bw ? w : selmon->mw - 2 * c->bw;

	nh = c->h + h;
	if (hAbs == 'H')
		nh = h < selmon->mh - 2 * c->bw ? h : selmon->mh - 2 * c->bw;

	nx = c->x + x;
	if (xAbs == 'X') {
		if (x < selmon->mx)
			nx = selmon->mx;
		else if (x > selmon->mx + selmon->mw)
			nx = selmon->mx + selmon->mw - nw - 2 * c->bw;
		else
			nx = x;
	}

	ny = c->y + y;
	if (yAbs == 'Y') {
		if (y < selmon->my)
			ny = selmon->my;
		else if (y > selmon->my + selmon->mh)
			ny = selmon->my + selmon->mh - nh - 2 * c->bw;
		else
			ny = y;
	}

	ox = c->x;
	oy = c->y;
	ow = c->w;
	oh = c->h;

	XRaiseWindow(dpy, c->win);
	Bool xqp = XQueryPointer(dpy, root, &dummy, &dummy, &msx, &msy, &dx, &dy, &dui);
	resize(c, nx, ny, nw, nh, True);

	/* move cursor along with the window to avoid problems caused by the sloppy focus */
	if (xqp && ox <= msx && (ox + ow) >= msx && oy <= msy && (oy + oh) >= msy)
	{
		nmx = c->x - ox + c->w - ow;
		nmy = c->y - oy + c->h - oh;
		/* make sure the cursor stays inside the window */
		if ((msx + nmx) > c->x && (msy + nmy) > c->y)
			XWarpPointer(dpy, None, None, 0, 0, 0, 0, nmx, nmy);
	}
}

#if USESWITCHER
void
switcher()
{
	/* move to next window */
	if (selmon->sel != NULL && selmon->sel->snext != NULL) {
		selmon->switchern++;
		if (selmon->switchern >= selmon->nclients)
			selmon->switchern = 0; /* reset switchern */

		focus(selmon->clientsnext[selmon->switchern]);
		restack(selmon);
	}

	/* redraw tab */
	XRaiseWindow(dpy, selmon->tabwin);
	drawswitcher(selmon->nclients, 0, selmon);
}

void
switcherend()
{
	if (selmon->isswitching == 0)
		return;

	/*
	* move all clients between 1st and choosen position,
	* one down in stack and put choosen client to the first position
	* so they remain in right order for the next time that alt-tab is used
	*/
	if (selmon->nclients > 1) {
		if (selmon->switchern != 0) { /* if user picked original client do nothing */
			Client *buff = selmon->clientsnext[selmon->switchern];
			if (selmon->switchern > 1)
				for (int i = selmon->switchern;i > 0;i--)
					selmon->clientsnext[i] = selmon->clientsnext[i - 1];
			else /* swap them if there are just 2 clients */
				selmon->clientsnext[selmon->switchern] = selmon->clientsnext[0];
			selmon->clientsnext[0] = buff;
		}

		/* restack clients */
		for (int i = selmon->nclients - 1;i >= 0;i--) {
			focus(selmon->clientsnext[i]);
			restack(selmon);
		}

		free(selmon->clientsnext); /* free list of clients */
	}

	/* turn off/destroy the window */
	selmon->isswitching = 0;
	selmon->nclients = 0;
	XUnmapWindow(dpy, selmon->tabwin);
	XDestroyWindow(dpy, selmon->tabwin);

    arrange(selmon);
}

void
drawswitcher(int nwins, int first, Monitor *m)
{
	Client *c;
	int h;

	if (first) {
		Monitor *m = selmon;
		XSetWindowAttributes wa = {
			.override_redirect = True,
		    .background_pixel = 0,
			.border_pixel = 0,
			.colormap = cmap,
			.event_mask = ButtonPressMask|ExposureMask
		};

		selmon->maxwidth = maxwidth;
		selmon->maxheight = maxheight;

		/* decide position of tabwin */
		int posX = selmon->mx;
		int posY = selmon->my;
		if (menupositionh == 0)
			posX += 0;
		if (menupositionh == 1)
			posX += (selmon->mw / 2) - (maxwidth / 2);
		if (menupositionh == 2)
			posX += selmon->mw - maxwidth;

		if (menupositionv == 0)
			posY += selmon->mh - maxheight;
		if (menupositionv == 1)
			posY += (selmon->mh / 2) - (maxheight / 2);
		if (menupositionv == 2)
			posY += 0;

		h = selmon->maxheight;
		m->tabwin = XCreateWindow(dpy, root, posX, posY, maxwidth, maxheight, 2, depth,
		                             InputOutput, visual,
		                             CWOverrideRedirect|CWBackPixel|CWBorderPixel|CWColormap|CWEventMask, &wa);
		XDefineCursor(dpy, m->tabwin, cursor[CurNormal]->cursor);
		XMapRaised(dpy, m->tabwin);

	}

	h = selmon->maxheight / m->nclients;

	int y = 0;
	int n = 0;

	for (int i = 0; i < m->nclients; i++) {
		c = m->clientsnext[i];
		if(!ISVISIBLE(c)) continue;
		if (HIDDEN(c)) continue;

		n++;

		drw_setscheme(drw, scheme[(c == m->sel) ? SchemeTitleSel : SchemeBar]);
		drw_text(drw, 0, y, selmon->maxwidth, h, 0, c->name, 0, False);
        #if USEWINICON
        if (c->icon && !selmon->hideicon && c->ich + 30 < h) drw_pic(drw, 0, y + 5, selmon->maxwidth, h, c->icon);
        #endif
		y += h;
	}

	drw_setscheme(drw, scheme[SchemeBar]);
	drw_map(drw, m->tabwin, 0, 0, selmon->maxwidth, selmon->maxheight);
}
#endif

void
switcherstart(const Arg *arg)
{
#if USESWITCHER
	selmon->clientsnext = NULL;
	if (selmon->tabwin)
		switcherend();

	if (selmon->isswitching == 1) {
		switcherend();
	} else {
		selmon->isswitching = 1;
		selmon->switchern = 0;

		Client *c;
		Monitor *m = selmon;

		m->nclients = 0;
		for(c = m->clients; c; c = c->next) { /* count clients */
			if(!ISVISIBLE(c)) continue;
			if (HIDDEN(c)) continue;

			++m->nclients;
		}

		if (m->nclients > 1) {
			m->clientsnext = (Client **) malloc(m->nclients * sizeof(Client *));

			int listIndex = 0;
			for(c = m->stack; c; c = c->snext) { /* add clients to the list */
				if(!ISVISIBLE(c)) continue;
				if (HIDDEN(c)) continue;

				m->clientsnext[listIndex++] = c;
			}

			drawswitcher(m->nclients, 1, m);

			struct timespec ts = { .tv_sec = 0, .tv_nsec = 1000000 };

			/* grab keyboard (take all input from keyboard) */
			int grabbed = 1;
			for (int i = 0;i < 1000;i++) {
				if (XGrabKeyboard(dpy, DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync, CurrentTime) == GrabSuccess)
					break;
				nanosleep(&ts, NULL);
				if (i == 1000 - 1)
					grabbed = 0;
			}

			XEvent event;
			switcher();
			if (grabbed == 0) {
				switcherend();
			} else {
				while (grabbed) {
					XNextEvent(dpy, &event);
					if (event.type == KeyPress || event.type == KeyRelease) {
						if (event.type == KeyRelease && event.xkey.keycode == switchmodifier) { /* if super key is released break cycle */
							break;
						} else if (event.type == KeyPress) {
							if (event.xkey.keycode == switchcyclekey) {/* if XK_s is pressed move to the next window */
								switcher();
							}
						}
					}
				}

				switcherend(); /* end the alt-tab functionality */
				XUngrabKeyboard(dpy, CurrentTime); /* stop taking all input from keyboard */
				focus(selmon->sel);

                selmon->allowwarp = 1;
				restack(selmon);
			}
		} else {
			switcherend(); /* end the alt-tab functionality */
		}
	}
#else
    return;
#endif
}

void
moveresizeedge(const Arg *arg) {
	/* move or resize floating window to edge of screen */
	Client *c;
	c = selmon->sel;
	char e;
	int nx, ny, nw, nh, ox, oy, ow, oh, bp;
	int msx, msy, dx, dy, nmx, nmy;
	int starty;
	unsigned int dui;
	Window dummy;

	nx = c->x;
	ny = c->y;
	nw = c->w;
	nh = c->h;

	starty = !selmon->hidebar && barposition ? bh : 0;
	bp = !selmon->hidebar && !barposition ? bh : 0;

	if (!c || !arg)
		return;
	if (selmon->lt[selmon->sellt]->arrange && !c->isfloating)
		return;
	if(sscanf((char *)arg->v, "%c", &e) != 1)
		return;

	if(e == 't')
		ny = starty;

	if(e == 'b')
		ny = c->h > selmon->mh - 2 * c->bw ? c->h - bp : selmon->mh - c->h - 2 * c->bw - bp;

	if(e == 'l')
		nx = selmon->mx;

	if(e == 'r')
		nx = c->w > selmon->mw - 2 * c->bw ? selmon->mx + c->w : selmon->mx + selmon->mw - c->w - 2 * c->bw;

	if(e == 'T') {
		/* if you click to resize again, it will return to old size/position */
		if(c->h + starty == c->oldh + c->oldy) {
			nh = c->oldh;
			ny = c->oldy;
		} else {
			nh = c->h + c->y - starty;
			ny = starty;
		}
	}

	if(e == 'B')
		nh = c->h + c->y + 2 * c->bw + bp == selmon->mh ? c->oldh : selmon->mh - c->y - 2 * c->bw - bp;

	if(e == 'L') {
		if(selmon->mx + c->w == c->oldw + c->oldx) {
			nw = c->oldw;
			nx = c->oldx;
		} else {
			nw = c->w + c->x - selmon->mx;
			nx = selmon->mx;
		}
	}

	if(e == 'R')
		nw = c->w + c->x + 2 * c->bw == selmon->mx + selmon->mw ? c->oldw : selmon->mx + selmon->mw - c->x - 2 * c->bw;

	ox = c->x;
	oy = c->y;
	ow = c->w;
	oh = c->h;

	XRaiseWindow(dpy, c->win);
	Bool xqp = XQueryPointer(dpy, root, &dummy, &dummy, &msx, &msy, &dx, &dy, &dui);
	resize(c, nx, ny, nw, nh, True);

	/* move cursor along with the window to avoid problems caused by the sloppy focus */
	if (xqp && ox <= msx && (ox + ow) >= msx && oy <= msy && (oy + oh) >= msy) {
		nmx = c->x - ox + c->w - ow;
		nmy = c->y - oy + c->h - oh;
		/* make sure the cursor stays inside the window */
		if ((msx + nmx) > c->x && (msy + nmy) > c->y)
			XWarpPointer(dpy, None, None, 0, 0, 0, 0, nmx, nmy);
	}

}

Client *
nexttagged(Client *c) {
	Client *walked = c->mon->clients;
	for(;
		walked && (walked->isfloating || !ISVISIBLEONTAG(walked, c->tags));
		walked = walked->next
	);
	return walked;
}

#if USEMOUSE
void
moveorplace(const Arg *arg) {
	if ((!selmon->lt[selmon->sellt]->arrange || (selmon->sel && selmon->sel->isfloating)))
		movemouse(arg);
	else
		placemouse(arg);
}

void
movemouse(const Arg *arg)
{
	int x, y, ocx, ocy, nx, ny;
	Client *c;
	Monitor *m;
	XEvent ev;
	Time lasttime = 0;

	if (!(c = selmon->sel))
		return;
	if (c->isfullscreen) /* no support moving fullscreen windows by mouse */
		return;
	restack(selmon);
	ocx = c->x;
	ocy = c->y;
	if (XGrabPointer(dpy, root, False, MOUSEMASK, GrabModeAsync, GrabModeAsync,
		None, cursor[CurMove]->cursor, CurrentTime) != GrabSuccess)
		return;
	if (!getrootptr(&x, &y))
		return;
	do {
		XMaskEvent(dpy, MOUSEMASK|ExposureMask|SubstructureRedirectMask, &ev);
		switch(ev.type) {
		case ConfigureRequest:
		case Expose:
		case MapRequest:
			handler[ev.type](&ev);
			break;
		case MotionNotify:
			if ((ev.xmotion.time - lasttime) <= (1000 / 60))
				continue;
			lasttime = ev.xmotion.time;

			nx = ocx + (ev.xmotion.x - x);
			ny = ocy + (ev.xmotion.y - y);
			if (abs(selmon->wx - nx) < snap)
				nx = selmon->wx;
			else if (abs((selmon->wx + selmon->ww) - (nx + WIDTH(c))) < snap)
				nx = selmon->wx + selmon->ww - WIDTH(c);
			if (abs(selmon->wy - ny) < snap)
				ny = selmon->wy;
			else if (abs((selmon->wy + selmon->wh) - (ny + HEIGHT(c))) < snap)
				ny = selmon->wy + selmon->wh - HEIGHT(c);
			if (!c->isfloating && selmon->lt[selmon->sellt]->arrange
			&& (abs(nx - c->x) > snap || abs(ny - c->y) > snap))
				togglefloating(NULL);
			if (!selmon->lt[selmon->sellt]->arrange || c->isfloating)
				resize(c, nx, ny, c->w, c->h, 1);
			break;
		}
	} while (ev.type != ButtonRelease);
	XUngrabPointer(dpy, CurrentTime);
	if ((m = recttomon(c->x, c->y, c->w, c->h)) != selmon) {
		sendmon(c, m);
		selmon = m;
		focus(NULL);
	}
}

void
resizemouse(const Arg *arg)
{
	int ocx, ocy, nw, nh;
	Client *c;
	Monitor *m;
	XEvent ev;
	Time lasttime = 0;

	if (!(c = selmon->sel))
		return;
	if (c->isfullscreen) /* no support resizing fullscreen windows by mouse */
		return;
	restack(selmon);
	ocx = c->x;
	ocy = c->y;
	if (XGrabPointer(dpy, root, False, MOUSEMASK, GrabModeAsync, GrabModeAsync,
		None, cursor[CurResize]->cursor, CurrentTime) != GrabSuccess)
		return;
	XWarpPointer(dpy, None, c->win, 0, 0, 0, 0, c->w + c->bw - 1, c->h + c->bw - 1);

	do {
		XMaskEvent(dpy, MOUSEMASK|ExposureMask|SubstructureRedirectMask, &ev);
		switch(ev.type) {
		case ConfigureRequest:
		case Expose:
		case MapRequest:
			handler[ev.type](&ev);
			break;
		case MotionNotify:
			if ((ev.xmotion.time - lasttime) <= (1000 / 60))
				continue;
			lasttime = ev.xmotion.time;

			nw = MAX(ev.xmotion.x - ocx - 2 * c->bw + 1, 1);
			nh = MAX(ev.xmotion.y - ocy - 2 * c->bw + 1, 1);

			if (c->mon->wx + nw >= selmon->wx && c->mon->wx + nw <= selmon->wx + selmon->ww
			&& c->mon->wy + nh >= selmon->wy && c->mon->wy + nh <= selmon->wy + selmon->wh)
				{
						if (!c->isfloating && selmon->lt[selmon->sellt]->arrange
						&& (abs(nw - c->w) > snap || abs(nh - c->h) > snap))
						togglefloating(NULL);
				}

			if (!selmon->lt[selmon->sellt]->arrange || c->isfloating)
				resize(c, c->x, c->y, nw, nh, 1);
			break;
		}
	} while (ev.type != ButtonRelease);

    XWarpPointer(dpy, None, c->win, 0, 0, 0, 0, c->w + c->bw - 1, c->h + c->bw - 1);
	XUngrabPointer(dpy, CurrentTime);
	while (XCheckMaskEvent(dpy, EnterWindowMask, &ev));
	if ((m = recttomon(c->x, c->y, c->w, c->h)) != selmon) {
		sendmon(c, m);
		selmon = m;
		focus(NULL);
	}
}
#endif

Client *
nexttiled(Client *c)
{
    for (; c && (c->isfloating || !ISVISIBLE(c) || HIDDEN(c)); c = c->next);
    return c;
}

unsigned int
nexttag(void)
{
	unsigned int seltag = selmon->tagset[selmon->seltags];
	return seltag == (1 << (LENGTH(ftags) - 1)) ? 1 : seltag << 1;
}

unsigned int
nexttag_skip_vacant(void)
{
	unsigned int seltag = selmon->tagset[selmon->seltags];
	unsigned int usedtags = 0;
	Client *c = selmon->clients;

	if (!c)
		return seltag;

	/* skip vacant tags */
	do {
		usedtags |= c->tags;
		c = c->next;
	} while (c);

	do {
		seltag = seltag == (1 << (LENGTH(ftags) - 1)) ? 1 : seltag << 1;
	} while (!(seltag & usedtags));

	return seltag;
}


unsigned int
prevtag(void)
{
	unsigned int seltag = selmon->tagset[selmon->seltags];
	return seltag == 1 ? (1 << (LENGTH(ftags) - 1)) : seltag >> 1;
}

unsigned int
prevtag_skip_vacant(void)
{
	unsigned int seltag = selmon->tagset[selmon->seltags];
	unsigned int usedtags = 0;
	Client *c = selmon->clients;
	if (!c)
		return seltag;

	/* skip vacant tags */
	do {
		usedtags |= c->tags;
		c = c->next;
	} while (c);

	do {
		seltag = seltag == 1 ? (1 << (LENGTH(ftags) - 1)) : seltag >> 1;
	} while (!(seltag & usedtags));

	return seltag;
}



#if USEFADE
void
opacity(Client *c, double opacity)
{
	if(fadewindows && opacity > 0 && opacity < 1 && fadeinactive) {
		unsigned long real_opacity[] = { opacity * 0xffffffff };
		XChangeProperty(dpy, c->win, netatom[NetWMWindowsOpacity], XA_CARDINAL,
				32, PropModeReplace, (unsigned char *)real_opacity,
				1);
	} else
		XDeleteProperty(dpy, c->win, netatom[NetWMWindowsOpacity]);
}
#endif

void
placemouse(const Arg *arg)
{
	int x, y, px, py, ocx, ocy, nx = -9999, ny = -9999, freemove = 0;
	Client *c, *r = NULL, *at, *prevr;
	Monitor *m;
	XEvent ev;
	XWindowAttributes wa;
	Time lasttime = 0;
	int attachmode, prevattachmode;
	attachmode = prevattachmode = -1;

	if (!(c = selmon->sel) || !c->mon->lt[c->mon->sellt]->arrange) /* no support for placemouse when floating layout is used */
		return;
	if (c->isfullscreen) /* no support placing fullscreen windows by mouse */
		return;
	restack(selmon);
	prevr = c;
	if (XGrabPointer(dpy, root, False, MOUSEMASK, GrabModeAsync, GrabModeAsync,
		None, cursor[CurMove]->cursor, CurrentTime) != GrabSuccess)
		return;

	c->isfloating = 0;
	c->beingmoved = 1;

	XGetWindowAttributes(dpy, c->win, &wa);
	ocx = wa.x;
	ocy = wa.y;

	if (arg->i == 2) /* warp cursor to client center */
		XWarpPointer(dpy, None, c->win, 0, 0, 0, 0, WIDTH(c) / 2, HEIGHT(c) / 2);

	if (!getrootptr(&x, &y))
		return;

	do {
		XMaskEvent(dpy, MOUSEMASK|ExposureMask|SubstructureRedirectMask, &ev);
		switch (ev.type) {
		case ConfigureRequest:
		case Expose:
		case MapRequest:
			handler[ev.type](&ev);
			break;
		case MotionNotify:
			if ((ev.xmotion.time - lasttime) <= (1000 / 60))
				continue;
			lasttime = ev.xmotion.time;

			nx = ocx + (ev.xmotion.x - x);
			ny = ocy + (ev.xmotion.y - y);

			if (!freemove && (abs(nx - ocx) > snap || abs(ny - ocy) > snap))
				freemove = 1;

			if (freemove)
				XMoveWindow(dpy, c->win, nx, ny);

			if ((m = recttomon(ev.xmotion.x, ev.xmotion.y, 1, 1)) && m != selmon)
				selmon = m;

			if (arg->i == 1) { /* tiled position is relative to the client window center point */
				px = nx + wa.width / 2;
				py = ny + wa.height / 2;
			} else { /* tiled position is relative to the mouse cursor */
				px = ev.xmotion.x;
				py = ev.xmotion.y;
			}

			r = recttoclient(px, py, 1, 1);

			if (!r || r == c)
				break;

			attachmode = 0; /* below */
			if (((float)(r->y + r->h - py) / r->h) > ((float)(r->x + r->w - px) / r->w)) {
				if (abs(r->y - py) < r->h / 2)
					attachmode = 1; /* above */
			} else if (abs(r->x - px) < r->w / 2)
				attachmode = 1; /* above */

			if ((r && r != prevr) || (attachmode != prevattachmode)) {
				detachstack(c);
				detach(c);
				if (c->mon != r->mon) {
					arrangemon(c->mon);
					c->tags = r->mon->tagset[r->mon->seltags];
				}

				c->mon = r->mon;
				r->mon->sel = r;

				if (attachmode) {
					if (r == r->mon->clients)
						attach(c);
					else {
						for (at = r->mon->clients; at->next != r; at = at->next);
						c->next = at->next;
						at->next = c;
					}
				} else {
					c->next = r->next;
					r->next = c;
				}

				attachstack(c);
				arrangemon(r->mon);
				prevr = r;
				prevattachmode = attachmode;
			}
			break;
		}
	} while (ev.type != ButtonRelease);
	XUngrabPointer(dpy, CurrentTime);

	if ((m = recttomon(ev.xmotion.x, ev.xmotion.y, 1, 1)) && m != c->mon) {
		detach(c);
		detachstack(c);
		arrangemon(c->mon);
		c->mon = m;
		c->tags = m->tagset[m->seltags];
		attach(c);
		attachstack(c);
		selmon = m;
	}

	focus(c);
	c->beingmoved = 0;

	if (nx != -9999)
		resize(c, nx, ny, c->w, c->h, 0);
	arrangemon(c->mon);
}

void
pop(Client *c)
{
    int i;
	for (i = 0; !(selmon->tagset[selmon->seltags] & 1 << i); i++);
	i++;

	detach(c);
	attach(c);
	focus(c);
	arrange(c->mon);
}

void
propertynotify(XEvent *e)
{
	Client *c;
	Window trans;
	XPropertyEvent *ev = &e->xproperty;

    #if USESYSTRAY
    if (!selmon->hidesystray && (c = wintosystrayicon(ev->window))) {
		if (ev->atom == XA_WM_NORMAL_HINTS) {
			updatesizehints(c);
			updatesystrayicongeom(c, c->w, c->h);
		}
		else
			updatesystrayiconstate(c, ev);
		drawbarwin(systray->bar);
	}
    #endif

	if ((ev->window == root) && (ev->atom == XA_WM_NAME)) {
		if (!getsignal())
			updatestatus();
    }
	else if (ev->state == PropertyDelete)
		return; /* ignore */
	else if ((c = wintoclient(ev->window))) {
		switch(ev->atom) {
		default: break;
		case XA_WM_TRANSIENT_FOR:
			if (!c->ignoretransient && !c->isfloating &&
			(XGetTransientForHint(dpy, c->win, &trans)) &&
			(c->isfloating = (wintoclient(trans)) != NULL))
			    arrange(c->mon);
			break;
		case XA_WM_NORMAL_HINTS:
			c->hintsvalid = 0;
			break;
		case XA_WM_HINTS:
			updatewmhints(c);
            if (c->isurgent)
                drawbar();
			break;
		}
		if (ev->atom == XA_WM_NAME || ev->atom == netatom[NetWMName]) {
			updatetitle(c);

            if (refreshrules) updaterules(c);
			if (c == c->mon->sel && !selmon->hidetitle)
				drawbaritems(c->mon);
		}
        #if USEWINICON
        else if (ev->atom == netatom[NetWMIcon]) {
			updateicon(c);
			if (c == c->mon->sel)
				drawbaritems(c->mon);
        }
        #endif
        if (ev->atom == netatom[NetWMWindowType])
			updatewindowtype(c);
		if (ev->atom == motifatom)
			updatemotifhints(c);
		if (ev->atom == wmatom[WMClass]) {
				if (wmclass) {
						applyrules(c);
				}
		}
	}
}

void
quit(const Arg *arg)
{

	Monitor *m;
	Client *c;
    size_t i;

	for (m = mons; m; m = m->next) {
		if (m) {
			for (c = m->stack; c; c = c->next)
				if (c && HIDDEN(c)) showwin(c);
		}
	}

    /* kill child processes */
	for (i = 0; i < autostart_len; i++) {
		if (0 < autostart_pids[i]) {
			kill(autostart_pids[i], SIGTERM);
			waitpid(autostart_pids[i], NULL, 0);
            system("pkill " STATUS);
		}
	}

    /* it will not be 0 because 0 means exit, not restart */
    if (arg->i) {
       restart = 1;

       FILE *rsfile = fopen("/tmp/speedwm-isrestart", "w");
       int ws = fputs("\n", rsfile);

       if (ws)
           fprintf(stderr, "speedwm: failed to write restart file.\n");

       fclose(rsfile);
    }

	running = 0;
}

Client *
recttoclient(int x, int y, int w, int h)
{
	Client *c, *r = NULL;
	int a, area = 0;

	for (c = nexttiled(selmon->clients); c; c = nexttiled(c->next)) {
		if ((a = INTERSECTC(x, y, w, h, c)) > area) {
			area = a;
			r = c;
		}
	}
	return r;
}

Monitor *
recttomon(int x, int y, int w, int h)
{
	Monitor *m, *r = selmon;
	int a, area = 0;

	for (m = mons; m; m = m->next)
		if ((a = INTERSECT(x, y, w, h, m)) > area) {
			area = a;
			r = m;
		}
	return r;
}

void
reset_layout(const Arg *arg)
{
	Arg default_layout = {.v = &layouts[0]};
	Arg default_mfact = {.f = mfact + 1};

	if (resetlayout) {
		setlayout(&default_layout);
		selmon->isreset = 1;
    }

	if (resetmfact) {
		setmfact(&default_mfact);
    }

	if (enablegaps && resetgaps) {
		defaultgaps(NULL);
    }
}

void
setmark(Client *c)
{
	if (c == mark)
		return;
	if (mark) {
		mark = 0;
	}
	if (c) {
		mark = c;
	}
}

void
swapclient(const Arg *arg)
{
	Client *s, *m, t;

	if (!mark || !selmon->sel || mark == selmon->sel
	    || !selmon->lt[selmon->sellt]->arrange)
		return;
	s = selmon->sel;
	m = mark;
	t = *s;
	strcpy(s->name, m->name);
	s->win = m->win;
	s->x = m->x;
	s->y = m->y;
	s->w = m->w;
	s->h = m->h;

	m->win = t.win;
	strcpy(m->name, t.name);
	m->x = t.x;
	m->y = t.y;
	m->w = t.w;
	m->h = t.h;

	selmon->sel = m;
	mark = s;
	focus(s);
	setmark(m);

	arrange(s->mon);
	if (s->mon != m->mon) {
		arrange(m->mon);
	}
}

void
swapfocus(const Arg *arg)
{
	Client *t;

	if (!selmon->sel || !mark || selmon->sel == mark)
		return;
	t = selmon->sel;
	if (mark->mon != selmon) {
		unfocus(selmon->sel, 0);
		selmon = mark->mon;
	}
	if (ISVISIBLE(mark)) {
		focus(mark);
		restack(selmon);
	} else {
		selmon->seltags ^= 1;
		selmon->tagset[selmon->seltags] = mark->tags;
		focus(mark);
		arrange(selmon);
	}
	setmark(t);
}

void
togglemark(const Arg *arg)
{
	if (!selmon->sel)
		return;
	setmark(selmon->sel == mark ? 0 : selmon->sel);
}

void
reset_mfact(const Arg *arg) {
		Arg new_mfact = {.f = mfact + 1};
		setmfact(&new_mfact);
}

void
reorganizetags(const Arg *arg) {
	Client *c;
	unsigned int occ, unocc, i;
	unsigned int tagdest[LENGTH(ftags)];

	occ = 0;
	for (c = selmon->clients; c; c = c->next)
		occ |= (1 << (ffs(c->tags)-1));
	unocc = 0;
	for (i = 0; i < LENGTH(ftags); ++i) {
		while (unocc < i && (occ & (1 << unocc)))
			unocc++;
		if (occ & (1 << i)) {
			tagdest[i] = unocc;
			occ &= ~(1 << i);
			occ |= 1 << unocc;
		}
	}

	for (c = selmon->clients; c; c = c->next)
		c->tags = 1 << tagdest[ffs(c->tags)-1];
	if (selmon->sel)
		selmon->tagset[selmon->seltags] = selmon->sel->tags;
	arrange(selmon);
}

void
resetmastercount(const Arg *arg)
{
	selmon->mastercount = selmon->pertag->mastercounts[selmon->pertag->curtag] = 1;
	arrange(selmon);
}

void
resize(Client *c, int x, int y, int w, int h, int interact)
{
	if (applysizehints(c, &x, &y, &w, &h, interact)) {
		resizeclient(c, x, y, w, h);
		selmon->isreset = 1;
    } else {
		selmon->isreset = 0;
    }
}

void
resizeclient(Client *c, int x, int y, int w, int h)
{
	XWindowChanges wc;

	c->oldx = c->x; c->x = wc.x = x;
	c->oldy = c->y; c->y = wc.y = y;
	c->oldw = c->w; c->w = wc.width = w;
	c->oldh = c->h; c->h = wc.height = h;

	if (c->beingmoved)
		return;

    if ((nexttiled(c->mon->clients) == c) && !(nexttiled(c->next)))
		reset_layout(NULL);

    if (selmon->hideborder) {
		c->w = wc.width += c->bw * 2;
		c->h = wc.height += c->bw * 2;
		wc.border_width = 0;
    } else {
        if (selmon->hideunselectedborder) {
            if (c == selmon->sel)
                wc.border_width = c->bw;
            else {
                wc.border_width = 0;
                wc.x += c->bw;
                wc.y += c->bw;
            }
        } else {
	        wc.border_width = c->bw;
        }
    }

	XConfigureWindow(dpy, c->win, CWX|CWY|CWWidth|CWHeight|CWBorderWidth, &wc);
	configure(c);
	XSync(dpy, False);
}

void
restack(Monitor *m)
{
	Client *c;
	XWindowChanges wc;

	drawbaritems(m);
	if (!m->sel)
		return;
	if (m->sel->isfloating || !m->lt[m->sellt]->arrange)
		XRaiseWindow(dpy, m->sel->win);
	if (m->lt[m->sellt]->arrange) {
		wc.stack_mode = Below;
		wc.sibling = m->bar->win;
		for (c = m->stack; c; c = c->snext)
			if (!c->isfloating && ISVISIBLE(c)) {
				XConfigureWindow(dpy, c->win, CWSibling|CWStackMode, &wc);
				wc.sibling = c->win;
			}
	}
	XSync(dpy, False);
  	warp(m->sel);
}

void
run(void)
{
	XSync(dpy, False);
#if USEIPC
	int event_count = 0;
	const int MAX_EVENTS = 10;
	struct epoll_event events[MAX_EVENTS];
#else
	XEvent ev;
	while (running && !XNextEvent(dpy, &ev))
		if (handler[ev.type])
			handler[ev.type](&ev); /* call handler */
#endif

#if USEIPC
	/* main event loop */
	while (running) {
		event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

		for (int i = 0; i < event_count; i++) {
			int event_fd = events[i].data.fd;
			DEBUG("Got event from fd %d\n", event_fd);

			if (event_fd == dpy_fd) {
				/* -1 means EPOLLHUP */
				if (handlexevent(events + i) == -1)
					return;
			} else if (event_fd == ipc_get_sock_fd()) {
				ipc_handle_socket_epoll_event(events + i);
			} else if (ipc_is_client_registered(event_fd)){
				if (ipc_handle_client_epoll_event(events + i, mons, &lastselmon, selmon,
							LENGTH(ftags), layouts, LENGTH(flayouts)) < 0) {
					fprintf(stderr, "Error handling IPC event on fd %d\n", event_fd);
				}
			} else {
				fprintf(stderr, "Got event from unknown fd %d, ptr %p, u32 %d, u64 %lu",
						event_fd, events[i].data.ptr, events[i].data.u32,
						events[i].data.u64);
				fprintf(stderr, " with events %d\n", events[i].events);
				return;
			}
		}
	}
#endif
}

void
scan(void)
{
	unsigned int i, num;
	Window d1, d2, *wins = NULL;
	XWindowAttributes wa;

	if (XQueryTree(dpy, root, &d1, &d2, &wins, &num)) {
		for (i = 0; i < num; i++) {
			if (!XGetWindowAttributes(dpy, wins[i], &wa)
			|| wa.override_redirect || XGetTransientForHint(dpy, wins[i], &d1))
				continue;
			if (wa.map_state == IsViewable || getstate(wins[i]) == IconicState)
				manage(wins[i], &wa);
		}
		for (i = 0; i < num; i++) { /* now the transients */
			if (!XGetWindowAttributes(dpy, wins[i], &wa))
				continue;
			if (XGetTransientForHint(dpy, wins[i], &d1)
			&& (wa.map_state == IsViewable || getstate(wins[i]) == IconicState))
				manage(wins[i], &wa);
		}
		if (wins)
			XFree(wins);
	}
}

static void scratchpad_hide ()
{
	if (selmon -> sel)
	{
		selmon -> sel -> tags = SCRATCHPAD_MASK;
		if (floatscratchpad)
				selmon -> sel -> isfloating = 1;

		focus(NULL);
		arrange(selmon);
	}
}

static _Bool scratchpad_last_showed_is_killed (void)
{
	_Bool killed = 1;
	for (Client * c = selmon -> clients; c != NULL; c = c -> next)
	{
		if (c == scratchpad_last_showed)
		{
			killed = 0;
			break;
		}
	}
	return killed;
}

void
scratchpad_remove()
{
	if (selmon->sel && scratchpad_last_showed != NULL && selmon->sel == scratchpad_last_showed)
		scratchpad_last_showed = NULL;
}

void
scratchpad_show()
{
	if (scratchpad_last_showed == NULL || scratchpad_last_showed_is_killed ())
		scratchpad_show_first ();
	else
	{
		if (scratchpad_last_showed -> tags != SCRATCHPAD_MASK)
		{
			scratchpad_last_showed -> tags = SCRATCHPAD_MASK;
			focus(NULL);
			arrange(selmon);
		}
		else
		{
			_Bool found_current = 0;
			_Bool found_next = 0;
			for (Client * c = selmon -> clients; c != NULL; c = c -> next)
			{
				if (found_current == 0)
				{
					if (c == scratchpad_last_showed)
					{
						found_current = 1;
						continue;
					}
				}
				else
				{
					if (c -> tags == SCRATCHPAD_MASK)
					{
						found_next = 1;
						scratchpad_show_client (c);
						break;
					}
				}
			}
			if (found_next == 0) scratchpad_show_first ();
		}
	}

	show(NULL);
}

void
scratchpad_show_client(Client *c)
{
	scratchpad_last_showed = c;
	c -> tags = selmon->tagset[selmon->seltags];
	focus(c);
	arrange(selmon);
}

void
scratchpad_show_first(void)
{
	for (Client * c = selmon -> clients; c != NULL; c = c -> next)
	{
		if (c -> tags == SCRATCHPAD_MASK)
		{
			scratchpad_show_client (c);
			break;
		}
	}
}

void
sendmon(Client *c, Monitor *m)
{
	if (c->mon == m)
		return;
	unfocus(c, 1);
	detach(c);
	detachstack(c);
	c->mon = m;
	c->tags = (m->tagset[m->seltags] ? m->tagset[m->seltags] : 1);
    if (centerfloating) {
        c->x = c->mon->mx + (c->mon->mw - WIDTH(c)) / 2;
        c->y = c->mon->my + (c->mon->mh - HEIGHT(c)) / 2;
    }
	switch(attachdirection){
		case 1:
				attachabove(c);
				break;
		case 2:
				attachaside(c);
				break;
		case 3:
				attachbelow(c);
				break;
		case 4:
				attachbottom(c);
				break;
		case 5:
				attachtop(c);
				break;
		default:
				attach(c);
    }
	attachstack(c);
	focus(NULL);
	arrange(NULL);
}

void
setclientstate(Client *c, long state)
{
	long data[] = { state, None };

	XChangeProperty(dpy, c->win, wmatom[WMState], wmatom[WMState], 32,
		PropModeReplace, (unsigned char *)data, 2);
}
void
setcurrentdesktop(void){

    if (!automove)
		return;

	long data[] = { 0 };
	XChangeProperty(dpy, root, netatom[NetCurrentDesktop], XA_CARDINAL, 32, PropModeReplace, (unsigned char *)data, 1);
}
void
setdesktopnames(void)
{
    if (!automove)
		return;

	XTextProperty text;
	Xutf8TextListToTextProperty(dpy, tags, TAGSLENGTH, XUTF8StringStyle, &text);
	XSetTextProperty(dpy, root, &text, netatom[NetDesktopNames]);
}

int
#if USESYSTRAY
sendevent(Window w, Atom proto, int mask, long d0, long d1, long d2, long d3, long d4)
#else
sendevent(Client *c, Atom proto)
#endif
{
	int n;
#if USESYSTRAY
	Atom *protocols, mt;
#else
	Atom *protocols;
#endif
	int exists = 0;
	XEvent ev;

#if USESYSTRAY
	if (proto == wmatom[WMTakeFocus] || proto == wmatom[WMDelete]) {
		mt = wmatom[WMProtocols];
#endif

#if USESYSTRAY
	if (XGetWMProtocols(dpy, w, &protocols, &n)) {
#else
	if (XGetWMProtocols(dpy, c->win, &protocols, &n)) {
#endif
		while (!exists && n--)
			exists = protocols[n] == proto;
		XFree(protocols);
	}
#if USESYSTRAY
	} else {
		exists = True;
		mt = proto;
    }
#endif
	if (exists) {
		ev.type = ClientMessage;
		ev.xclient.format = 32;
#if USESYSTRAY
		ev.xclient.window = w;
		ev.xclient.message_type = mt;
		ev.xclient.data.l[0] = d0;
		ev.xclient.data.l[1] = d1;
		ev.xclient.data.l[2] = d2;
		ev.xclient.data.l[3] = d3;
		ev.xclient.data.l[4] = d4;
		XSendEvent(dpy, w, False, mask, &ev);
#else
		ev.xclient.window = c->win;
		ev.xclient.message_type = wmatom[WMProtocols];
		ev.xclient.data.l[0] = proto;
		ev.xclient.data.l[1] = CurrentTime;
		XSendEvent(dpy, c->win, False, NoEventMask, &ev);
#endif
	}
	return exists;
}

void
setnumdesktops(void){

    if (!automove)
		return;

	long data[] = { TAGSLENGTH };
	XChangeProperty(dpy, root, netatom[NetNumberOfDesktops], XA_CARDINAL, 32, PropModeReplace, (unsigned char *)data, 1);
}

void
setfocus(Client *c)
{
	if (!c->neverfocus) {
		XSetInputFocus(dpy, c->win, RevertToPointerRoot, CurrentTime);
		XChangeProperty(dpy, root, netatom[NetActiveWindow],
			XA_WINDOW, 32, PropModeReplace,
			(unsigned char *) &(c->win), 1);
	}
    #if USESYSTRAY
    sendevent(c->win, wmatom[WMTakeFocus], NoEventMask, wmatom[WMTakeFocus], CurrentTime, 0, 0, 0);
    #else
	sendevent(c, wmatom[WMTakeFocus]);
    #endif
}

void
setfullscreen(Client *c, int fullscreen)
{
	if (fullscreen && !c->isfullscreen) {
		XChangeProperty(dpy, c->win, netatom[NetWMState], XA_ATOM, 32,
			PropModeReplace, (unsigned char*)&netatom[NetWMFullscreen], 1);
		c->isfullscreen = 1;
		c->oldstate = c->isfloating;
		c->oldbw = c->bw;
		c->bw = 0;
		c->isfloating = 1;
		resizeclient(c, c->mon->mx, c->mon->my, c->mon->mw, c->mon->mh);
		XRaiseWindow(dpy, c->win);
	} else if (!fullscreen && c->isfullscreen){
		XChangeProperty(dpy, c->win, netatom[NetWMState], XA_ATOM, 32,
			PropModeReplace, (unsigned char*)0, 0);
		c->isfullscreen = 0;
		c->isfloating = c->oldstate;
		c->bw = c->oldbw;
		c->x = c->oldx;
		c->y = c->oldy;
		c->w = c->oldw;
		c->h = c->oldh;
		resizeclient(c, c->x, c->y, c->w, c->h);
		arrange(c->mon);
	}
}


void
setlayout(const Arg *arg)
{
    selmon->allowwarp = 0;
	if (!arg || !arg->v || arg->v != selmon->lt[selmon->sellt])
		selmon->sellt = selmon->pertag->sellts[selmon->pertag->curtag] ^= 1;
	if (arg && arg->v)
		selmon->lt[selmon->sellt] = selmon->pertag->ltidxs[selmon->pertag->curtag][selmon->sellt] = (Layout *)arg->v;

    if (selmon->lt[selmon->sellt]->preset.mastercount && selmon->lt[selmon->sellt]->preset.mastercount != -1)
		selmon->mastercount = selmon->lt[selmon->sellt]->preset.mastercount;
	if (selmon->lt[selmon->sellt]->preset.stackcount && selmon->lt[selmon->sellt]->preset.stackcount != -1)
		selmon->stackcount = selmon->lt[selmon->sellt]->preset.stackcount;

	selmon->ltaxis[layout] = selmon->lt[selmon->sellt]->preset.layout;
	selmon->ltaxis[master] = selmon->lt[selmon->sellt]->preset.masteraxis;
	selmon->ltaxis[stack]  = selmon->lt[selmon->sellt]->preset.stack1axis;
	selmon->ltaxis[stack2] = selmon->lt[selmon->sellt]->preset.stack2axis;

	selmon->pertag->ltaxis[selmon->pertag->curtag][layout] = selmon->ltaxis[layout];
	selmon->pertag->ltaxis[selmon->pertag->curtag][master] = selmon->ltaxis[master];
	selmon->pertag->ltaxis[selmon->pertag->curtag][stack]  = selmon->ltaxis[stack];
	selmon->pertag->ltaxis[selmon->pertag->curtag][stack2] = selmon->ltaxis[stack2];

	arrange(selmon);
}

void
setcfact(const Arg *arg)
{
	float f;
	Client *c;

	c = selmon->sel;

	if (!arg || !c || !selmon->lt[selmon->sellt]->arrange)
		return;
	if (!arg->f)
		f = 1.0;
	else if (arg->f > 4.0) /* set fact absolutely */
		f = arg->f - 4.0;
	else
		f = arg->f + c->cfact;
	if (f < 0.25)
		f = 0.25;
	else if (f > 4.0)
		f = 4.0;
	c->cfact = f;
	arrange(selmon);
}

#if USEIPC
void
setlayoutsafe(const Arg *arg)
{
	const Layout *ltptr = (Layout *)arg->v;
	if (ltptr == 0)
			setlayout(arg);
	for (int i = 0; i < LENGTH(flayouts); i++) {
		if (ltptr == &layouts[i])
			setlayout(arg);
	}
}
#endif

/* arg > 1.0 will set mfact absolutely */
void
setmfact(const Arg *arg)
{
	float f;

	if (!arg || !selmon->lt[selmon->sellt]->arrange)
		return;
	f = arg->f < 1.0 ? arg->f + selmon->mfact : arg->f - 1.0;
	if (f < 0.05 - lowestmfact || f > 0.95 + lowestmfact )
		return;
	selmon->mfact = selmon->pertag->mfacts[selmon->pertag->curtag] = f;
	arrange(selmon);
}

#if USEFADE
void
toggleopacity(const Arg *arg) {
	fadewindows = !fadewindows;
	for (Monitor* m = mons; m; m = m->next)
		for (Client* c = m->clients; c; c = c->next)
			opacity(c, (fadewindows && c != selmon->sel) ? inactiveopacity : activeopacity);
}
#else
void
toggleopacity(const Arg *arg)
{
    return;
}
#endif

void
setup(void)
{
	XSetWindowAttributes wa;
    Atom utf8string;

	/* clean up any zombies immediately */
	if (signal(SIGCHLD, sigchld) == SIG_ERR)
		die("can't install SIGCHLD handler:");
	sigchld(0);

	signal(SIGHUP, sighup);
	signal(SIGTERM, sigterm);

	/* init screen */
	screen = DefaultScreen(dpy);
	tw = DisplayWidth(dpy, screen);
	sh = DisplayHeight(dpy, screen);
	root = RootWindow(dpy, screen);

	xinitvisual();
	drw = drw_create(dpy, screen, root, tw, sh, visual, depth, cmap);
	if (!drw_font_create(drw, font))
		die("no fonts could be loaded.");
	lrpad = drw->font->h + barpaddingih;
	bh = drw->font->h + barheight + barpaddingiv;

	/* barpadding */
	sp = barpaddingoh;
	vp = (barposition == 1) ? barpaddingov : - barpaddingov;

	updategeom();

	int i = 0;

	/* init atoms */
    utf8string = XInternAtom(dpy, "UTF8_STRING", False);
	wmatom[WMProtocols] = XInternAtom(dpy, "WM_PROTOCOLS", False);
	wmatom[WMDelete] = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
	wmatom[WMState] = XInternAtom(dpy, "WM_STATE", False);
	wmatom[WMTakeFocus] = XInternAtom(dpy, "WM_TAKE_FOCUS", False);
	netatom[NetActiveWindow] = XInternAtom(dpy, "_NET_ACTIVE_WINDOW", False);
	netatom[NetSupported] = XInternAtom(dpy, "_NET_SUPPORTED", False);
	netatom[NetWMName] = XInternAtom(dpy, "_NET_WM_NAME", False);
    #if USEWINICON
    netatom[NetWMIcon] = XInternAtom(dpy, "_NET_WM_ICON", False);
    #endif
	netatom[NetWMState] = XInternAtom(dpy, "_NET_WM_STATE", False);

    #if USESYSTRAY
    netatom[NetSystemTray] = XInternAtom(dpy, "_NET_SYSTEM_TRAY_S0", False);
	netatom[NetSystemTrayOP] = XInternAtom(dpy, "_NET_SYSTEM_TRAY_OPCODE", False);
	netatom[NetSystemTrayOrientation] = XInternAtom(dpy, "_NET_SYSTEM_TRAY_ORIENTATION", False);
	netatom[NetSystemTrayOrientationHorz] = XInternAtom(dpy, "_NET_SYSTEM_TRAY_ORIENTATION_HORZ", False);
	netatom[NetSystemTrayVisual] = XInternAtom(dpy, "_NET_SYSTEM_TRAY_VISUAL", False);
	netatom[NetWMWindowTypeDock] = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DOCK", False);
	xatom[Manager] = XInternAtom(dpy, "MANAGER", False);
	xatom[Xembed] = XInternAtom(dpy, "_XEMBED", False);
	xatom[XembedInfo] = XInternAtom(dpy, "_XEMBED_INFO", False);
    #endif

	if (fullscreenhidebar)
		netatom[NetWMCheck] = XInternAtom(dpy, "_NET_SUPPORTING_WM_CHECK", False);

	netatom[NetWMFullscreen] = XInternAtom(dpy, "_NET_WM_STATE_FULLSCREEN", False);
    netatom[NetWMSticky] = XInternAtom(dpy, "_NET_WM_STATE_STICKY", False);
	netatom[NetWMWindowType] = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE", False);
	netatom[NetWMWindowTypeDialog] = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DIALOG", False);
	netatom[NetWMWindowTypeDesktop] = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DESKTOP", False);
	netatom[NetClientList] = XInternAtom(dpy, "_NET_CLIENT_LIST", False);
	netatom[NetClientInfo] = XInternAtom(dpy, "_NET_CLIENT_INFO", False);
	netatom[NetClientListStacking] = XInternAtom(dpy, "_NET_CLIENT_LIST_stackING", False);
    #if USEFADE
	netatom[NetWMWindowsOpacity] = XInternAtom(dpy, "_NET_WM_WINDOW_OPACITY", False);
    #endif
	netatom[NetDesktopViewport] = XInternAtom(dpy, "_NET_DESKTOP_VIEWPORT", False);
	netatom[NetNumberOfDesktops] = XInternAtom(dpy, "_NET_NUMBER_OF_DESKTOPS", False);
	netatom[NetCurrentDesktop] = XInternAtom(dpy, "_NET_CURRENT_DESKTOP", False);
	netatom[NetDesktopNames] = XInternAtom(dpy, "_NET_DESKTOP_NAMES", False);
	motifatom = XInternAtom(dpy, "_MOTIF_WM_HINTS", False);
	/* init cursors */
	cursor[CurNormal] = drw_cur_create(drw, XC_left_ptr);
	cursor[CurResize] = drw_cur_create(drw, XC_sizing);
	cursor[CurMove] = drw_cur_create(drw, XC_fleur);
	cursor[CurResizeHorzArrow] = drw_cur_create(drw, XC_sb_h_double_arrow);
	cursor[CurResizeVertArrow] = drw_cur_create(drw, XC_sb_v_double_arrow);
	/* init appearance */
	scheme = ecalloc(LENGTH(colors) + 1, sizeof(Clr *));
	scheme[LENGTH(colors)] = drw_scm_create(drw, colors[0], alphas[i], 3);
	if (LENGTH(ftags) > LENGTH(tagsel))
	    die("too few color schemes for the tags");
	for (i = 0; i < LENGTH(colors); i++)
		scheme[i] = drw_scm_create(drw, colors[i], alphas[i], 3);
    tagscheme = ecalloc(LENGTH(tagsel), sizeof(Clr *));
	for (i = 0; i < LENGTH(tagsel); i++)
	     tagscheme[i] = drw_scm_create(drw, tagsel[i], tagalpha, 2);
    powerlinescheme = ecalloc(LENGTH(powerlinecolors), sizeof(Clr *));
	for (i = 0; i < LENGTH(powerlinecolors); i++)
		powerlinescheme[i] = drw_scm_create(drw, powerlinecolors[i], powerlinealphas[i], 2);
	/* init bars */
	updatebars();
	updatestatus();

	/* supporting window for NetWMCheck */
	if (fullscreenhidebar) {
	  wmcheckwin = XCreateSimpleWindow(dpy, root, 0, 0, 1, 1, 0, 0, 0);
	  XChangeProperty(dpy, wmcheckwin, netatom[NetWMCheck], XA_WINDOW, 32,
		  PropModeReplace, (unsigned char *) &wmcheckwin, 1);
	  XChangeProperty(dpy, wmcheckwin, netatom[NetWMName], utf8string, 8,
		  PropModeReplace, (unsigned char *) "speedwm", 7);
	  XChangeProperty(dpy, root, netatom[NetWMCheck], XA_WINDOW, 32,
		  PropModeReplace, (unsigned char *) &wmcheckwin, 1);
	}

	/* EWMH support per view */
	XChangeProperty(dpy, root, netatom[NetSupported], XA_ATOM, 32,
		PropModeReplace, (unsigned char *) netatom, NetLast);
	setnumdesktops();
	setcurrentdesktop();
	setdesktopnames();
	setviewport();

	XDeleteProperty(dpy, root, netatom[NetClientList]);
	XDeleteProperty(dpy, root, netatom[NetClientInfo]);
	XDeleteProperty(dpy, root, netatom[NetClientListStacking]);

	/* select events */
	wa.cursor = cursor[CurNormal]->cursor;
	wa.event_mask = SubstructureRedirectMask|SubstructureNotifyMask
		|ButtonPressMask|PointerMotionMask|EnterWindowMask
		|LeaveWindowMask|StructureNotifyMask|PropertyChangeMask|KeyPressMask;
	XChangeWindowAttributes(dpy, root, CWEventMask|CWCursor, &wa);
	XSelectInput(dpy, root, wa.event_mask);
	grabkeys();
	focus(NULL);
#if USEIPC
	setupepoll();
#endif
}

void
setviewport(void){

    if (!automove)
		return;

	long data[] = { 0, 0 };
	XChangeProperty(dpy, root, netatom[NetDesktopViewport], XA_CARDINAL, 32, PropModeReplace, (unsigned char *)data, 2);
}

#if USEIPC
void
setupepoll(void)
{
	epoll_fd = epoll_create1(0);
	dpy_fd = ConnectionNumber(dpy);
	struct epoll_event dpy_event;

	/* Initialize struct to 0 */
	memset(&dpy_event, 0, sizeof(dpy_event));

	DEBUG("Display socket is fd %d\n", dpy_fd);

	if (epoll_fd == -1) {
		fputs("Failed to create epoll file descriptor", stderr);
	}

	dpy_event.events = EPOLLIN;
	dpy_event.data.fd = dpy_fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, dpy_fd, &dpy_event)) {
		fputs("Failed to add display file descriptor to epoll", stderr);
		close(epoll_fd);
		exit(1);
	}

	if (ipc_init(ipcsockpath, epoll_fd, ipccommands, LENGTH(ipccommands)) < 0) {
		fputs("Failed to initialize IPC\n", stderr);
	}
}
#endif

void
seturgent(Client *c, int urg)
{
	XWMHints *wmh;

	c->isurgent = urg;
	if (!(wmh = XGetWMHints(dpy, c->win)))
		return;
	wmh->flags = urg ? (wmh->flags | XUrgencyHint) : (wmh->flags & ~XUrgencyHint);
	XSetWMHints(dpy, c->win, wmh);
	XFree(wmh);
}

#if USEXRESOURCES
void
reloadcolors(const Arg *arg)
{
		/* reload colors */
		load_xresources();
		setup();
}
#endif

void
sigchld(int unused)
{
	pid_t pid;

	while (0 < (pid = waitpid(-1, NULL, WNOHANG))) {
		pid_t *p, *lim;

		if (!(p = autostart_pids))
			continue;
		lim = &p[autostart_len];

		for (; p < lim; p++) {
			if (*p == pid) {
				*p = -1;
				break;
			}
		}

	}
}

void
sighup(int unused)
{
	Arg a = {.i = 1};
	quit(&a);
}

void
sigterm(int unused)
{
	Arg a = {.i = 0};
	quit(&a);
}

#ifdef XINERAMA
void
sortscreens(XineramaScreenInfo *screens, int n)
{
	int i, j;
	XineramaScreenInfo *screen = ecalloc(1, sizeof(XineramaScreenInfo));

	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++)
			if (RIGHTOF(screens[i], screens[j])) {
				memcpy(&screen[0], &screens[i], sizeof(XineramaScreenInfo));
				memcpy(&screens[i], &screens[j], sizeof(XineramaScreenInfo));
				memcpy(&screens[j], &screen[0], sizeof(XineramaScreenInfo));
			}
	XFree(screen);
}
#endif /* XINERAMA */

void
spawn(const Arg *arg)
{
	if (fork() == 0) {
		if (dpy)
			close(ConnectionNumber(dpy));
		#if USEMOUSE
		if (arg->v == clickstatus) {
			for (int i = 0; i < LENGTH(clickstatuss); i++) {
				if (clickstatusn == clickstatuss[i].id) {
					clickstatus[2] = clickstatuss[i].cmd;

                    /* compatibility with dwm bars */
					setenv("BUTTON", lastbutton, 1);

                    FILE *buttonfile = fopen("/tmp/speedwm-button", "w");

					int writestatus = fputs(lastbutton, buttonfile);
					if (writestatus == EOF) {
						fprintf(stderr, "speedwm: failed to write clickstatus button.\n");
						return;
				    }

				    fclose(buttonfile);

					break;
				}
			}
			if (!clickstatus[2])
				exit(EXIT_SUCCESS);
		}
		#endif

		setsid();
		execvp(((char **)arg->v)[0], (char **)arg->v);

		die("speedwm: execvp '%s' failed:", ((char **)arg->v)[0]);
	}
}

void
tag(const Arg *arg)
{
	if (selmon->sel && arg->ui & TAGMASK) {
		selmon->sel->tags = arg->ui & TAGMASK;
		focus(NULL);
		arrange(selmon);
	}
}

void
tagmon(const Arg *arg)
{
	Client *c = selmon->sel;
	if (!c || !mons->next)
		return;
    if (movefullscreenmon) {
	if (c->isfullscreen) {
		c->isfullscreen = 0;
		sendmon(c, dirtomon(arg->i));
		c->isfullscreen = 1;
		resizeclient(c, c->mon->mx, c->mon->my, c->mon->mw, c->mon->mh);
		XRaiseWindow(dpy, c->win);
	} else
		sendmon(c, dirtomon(arg->i));
    } else {
		sendmon(selmon->sel, dirtomon(arg->i));
    }
}

void
viewtoright(const Arg *arg)
{
	view(&(const Arg){.ui = nexttag()});
}

void
viewtoleft(const Arg *arg)
{
	view(&(const Arg){.ui = prevtag()});
}

void
viewtoright_vacant(const Arg *arg)
{
	view(&(const Arg){.ui = nexttag_skip_vacant()});
}

void
viewtoleft_vacant(const Arg *arg)
{
	view(&(const Arg){.ui = prevtag_skip_vacant()});
}

void
togglebar(const Arg *arg)
{

    Bar *bar;

	selmon->hidebar = selmon->pertag->hidebars[selmon->pertag->curtag] = !selmon->hidebar;
	updatebarpos(selmon);
    for (bar = selmon->bar; bar; bar = bar->next)
		XMoveResizeWindow(dpy, bar->win, bar->bx, bar->by, bar->bw, bar->bh);
	arrange(selmon);
}

void
togglesystray(const Arg *arg)
{
#if USESYSTRAY
    selmon->hidesystray = !selmon->hidesystray;

    updatebarpos(selmon);
    updatestatus();
#else
    return;
#endif
}

void
toggleborder(const Arg *arg)
{
    selmon->sel->bw = (selmon->sel->bw == bordersize ? 0 : bordersize);
    arrange(selmon);
}

void
togglesingleborder(const Arg *arg)
{
    selmon->hidesingleborder = !selmon->hidesingleborder;
    arrange(selmon);
}

void
togglebartags(const Arg *arg)
{
    selmon->hidetags = !selmon->hidetags;
	arrange(selmon);
}

void
togglebaremptytags(const Arg *arg)
{
    selmon->hideemptytags = !selmon->hideemptytags;
	arrange(selmon);
}

void
togglebarunseltitle(const Arg *arg)
{
    selmon->hideunselectedtitle = !selmon->hideunselectedtitle;
	arrange(selmon);
}

void
togglebartitle(const Arg *arg)
{
    selmon->hidetitle = !selmon->hidetitle;
	arrange(selmon);
}

void
togglebarsticky(const Arg *arg)
{
    selmon->hidesticky = !selmon->hidesticky;
	arrange(selmon);
}

void
togglebaricon(const Arg *arg)
{
    selmon->hideicon = !selmon->hideicon;
	arrange(selmon);
}

void
togglebarlt(const Arg *arg)
{
    selmon->hidelayout = !selmon->hidelayout;
	arrange(selmon);
}

void
togglebarstatus(const Arg *arg)
{
    selmon->hidestatus = !selmon->hidestatus;
	arrange(selmon);
}

void
togglebarfloat(const Arg *arg)
{
    selmon->hidefloating = !selmon->hidefloating;
	arrange(selmon);
}

void
togglefloating(const Arg *arg)
{
	if (!selmon->sel)
		return;
	if (selmon->sel->isfullscreen) /* no support for fullscreen windows */
		return;
	selmon->sel->isfloating = !selmon->sel->isfloating || selmon->sel->isfixed;
	if (selmon->sel->isfloating)
    {
		if (savefloat)
				resize(selmon->sel, selmon->sel->sfx, selmon->sel->sfy, selmon->sel->sfw, selmon->sel->sfh, False);
		else
				resize(selmon->sel, selmon->sel->x, selmon->sel->y, selmon->sel->w, selmon->sel->h, 0);
   }
   else
   {
      selmon->sel->sfx = selmon->sel->x;
	  selmon->sel->sfy = selmon->sel->y;
	  selmon->sel->sfw = selmon->sel->w;
	  selmon->sel->sfh = selmon->sel->h;
   }

   arrange(selmon);
}

void
setsticky(Client *c, int sticky)
{

    if(sticky && !c->issticky) {
        XChangeProperty(dpy, c->win, netatom[NetWMState], XA_ATOM, 32,
                PropModeReplace, (unsigned char *) &netatom[NetWMSticky], 1);
        c->issticky = 1;
    } else if(!sticky && c->issticky){
        XChangeProperty(dpy, c->win, netatom[NetWMState], XA_ATOM, 32,
                PropModeReplace, (unsigned char *)0, 0);
        c->issticky = 0;
        arrange(c->mon);
    }
}

void
togglesticky(const Arg *arg)
{
   if (!selmon->sel)
       return;
   selmon->sel->issticky = !selmon->sel->issticky;
   arrange(selmon);
}

void
togglefullscr(const Arg *arg)
{
  if(selmon->sel)
    setfullscreen(selmon->sel, !selmon->sel->isfullscreen);
}

void
unfocus(Client *c, int setfocus)
{
	if (!c)
		return;
	grabbuttons(c, 0);
    #if USEFADE
	opacity(c, inactiveopacity);
    #endif
    lastfocused = c;
	if (setfocus) {
		XSetInputFocus(dpy, root, RevertToPointerRoot, CurrentTime);
		XDeleteProperty(dpy, root, netatom[NetActiveWindow]);
	}

    if (selmon->hideunselectedborder) {
        if (c == selmon->sel)
            selmon->sel = NULL;

        resizeclient(c, c->x, c->y, c->w, c->h);
    }

	updatecurrentdesktop();
}

void
unmanage(Client *c, int destroyed)
{
	Monitor *m = c->mon;
	XWindowChanges wc;

    if (c == mark) {
        setmark(0);
    }

	if (c->swallowing) {
		unswallow(c);
		return;
	}

	Client *s = swallowingclient(c->win);
	if (s) {
		free(s->swallowing);
		s->swallowing = NULL;
		arrange(m);
		focus(NULL);
		return;
	}

	detach(c);
	detachstack(c);
    #if USEWINICON
    freeicon(c);
    #endif
	if (!destroyed) {
		wc.border_width = c->oldbw;
		XGrabServer(dpy); /* avoid race conditions */
		XSetErrorHandler(xerrordummy);
		XSelectInput(dpy, c->win, NoEventMask);
		XConfigureWindow(dpy, c->win, CWBorderWidth, &wc); /* restore border */
		XUngrabButton(dpy, AnyButton, AnyModifier, c->win);
		setclientstate(c, WithdrawnState);
		XSync(dpy, False);
		XSetErrorHandler(xerror);
		XUngrabServer(dpy);
	}
	if (scratchpad_last_showed == c)
		scratchpad_last_showed = NULL;

    if (lastfocused == c)
		lastfocused = NULL;

	free(c);

	if (!s) {
		arrange(m);
		focus(NULL);
		updateclientlist();
	}
}

void
unmapnotify(XEvent *e)
{
	Client *c;
	XUnmapEvent *ev = &e->xunmap;

	if ((c = wintoclient(ev->window))) {
		if (ev->send_event)
			setclientstate(c, WithdrawnState);
		else
			unmanage(c, 0);
        #if USESYSTRAY
        } else if (!selmon->hidesystray && (c = wintosystrayicon(ev->window))) {
		XMapRaised(dpy, c->win);
		drawbarwin(systray->bar);
        #endif
		}
}

void
updatebars(void)
{
    Bar *bar;
    int x_pad = sp;
    int y_pad = vp;

	Monitor *m;
	XSetWindowAttributes wa = {
		.override_redirect = True,
		.background_pixel = 0,
		.border_pixel = 0,
		.colormap = cmap,
		.event_mask = ButtonPressMask|ExposureMask|PointerMotionMask
	};

	XClassHint ch = {"speedwm", "speedwm"};
	for (m = mons; m; m = m->next) {
    	for (bar = m->bar; bar; bar = bar->next) {
			if (!bar->win) {
				bar->win = XCreateWindow(dpy, root, bar->bx, bar->by, bar->bw, bar->bh, 0, depth,
						InputOutput, visual,
						CWOverrideRedirect|CWBackPixel|CWBorderPixel|CWColormap|CWEventMask, &wa);
				XDefineCursor(dpy, bar->win, cursor[CurNormal]->cursor);
				XMapRaised(dpy, bar->win);
				XSetClassHint(dpy, bar->win, &ch);
			}
		}
	}
}

void
updatebarpos(Monitor *m)
{
	m->wy = m->my;
	m->wh = m->mh;
    int num_bars;
	Bar *bar;
	int y_pad = vp;
	int x_pad = sp;

	m->wx += m->insetx;
	m->wy += m->insety;
	m->ww -= m->insetw + m->insetx;
	m->wh -= m->inseth + m->insety;

	for (bar = m->bar; bar; bar = bar->next) {
		bar->bx = m->mx + x_pad;
		bar->bw = m->ww - 2 * x_pad;
		bar->bh = bh;
	}

	if (m->hidebar) {
		for (bar = m->bar; bar; bar = bar->next)
			bar->by = -bh - y_pad;
		return;
	}

	for (num_bars = 0, bar = m->bar; bar; bar = bar->next, num_bars++)
		if (bar->barposition)
			m->wy = m->my + bh + y_pad;
	m->wh = m->wh - y_pad * num_bars - bh * num_bars;

	for (bar = m->bar; bar; bar = bar->next)
		bar->by = (bar->barposition ? m->wy - bh : m->wy + m->wh);
}

void
updateclientlist()
{
	Client *c;
	Monitor *m;

	XDeleteProperty(dpy, root, netatom[NetClientList]);
	for (m = mons; m; m = m->next)
		for (c = m->clients; c; c = c->next)
			XChangeProperty(dpy, root, netatom[NetClientList],
				XA_WINDOW, 32, PropModeAppend,
				(unsigned char *) &(c->win), 1);

	XDeleteProperty(dpy, root, netatom[NetClientListStacking]);
	for (m = mons; m; m = m->next)
		for (c = m->stack; c; c = c->snext)
			XChangeProperty(dpy, root, netatom[NetClientListStacking],
				XA_WINDOW, 32, PropModeAppend,
				(unsigned char *) &(c->win), 1);
}

void
updatecurrentdesktop(void){

    if (!automove)
		return;

	long rawdata[] = { selmon->tagset[selmon->seltags] };
	int i=0;
	while(*rawdata >> (i+1)){
		i++;
    }
	long data[] = { i };
	XChangeProperty(dpy, root, netatom[NetCurrentDesktop], XA_CARDINAL, 32, PropModeReplace, (unsigned char *)data, 1);
}

int
updategeom(void)
{
	int dirty = 0;

#ifdef XINERAMA
	if (XineramaIsActive(dpy)) {
		int i, j, n, nn;
		Client *c;
		Monitor *m;
		XineramaScreenInfo *info = XineramaQueryScreens(dpy, &nn);
		XineramaScreenInfo *unique = NULL;

		for (n = 0, m = mons; m; m = m->next, n++);
		/* only consider unique geometries as separate screens */
		unique = ecalloc(nn, sizeof(XineramaScreenInfo));
		for (i = 0, j = 0; i < nn; i++)
			if (isuniquegeom(unique, j, &info[i]))
				memcpy(&unique[j++], &info[i], sizeof(XineramaScreenInfo));
		XFree(info);
		nn = j;
		sortscreens(unique, nn);
		if (n <= nn) { /* new monitors available */
			for (i = 0; i < (nn - n); i++) {
				for (m = mons; m && m->next; m = m->next);
				if (m)
					m->next = createmon();
				else
					mons = createmon();
			}
			for (i = 0, m = mons; i < nn && m; m = m->next, i++)
				if (i >= n
				|| unique[i].x_org != m->mx || unique[i].y_org != m->my
				|| unique[i].width != m->mw || unique[i].height != m->mh)
				{
					dirty = 1;
					m->num = i;
					m->mx = m->wx = unique[i].x_org;
					m->my = m->wy = unique[i].y_org;
					m->mw = m->ww = unique[i].width;
					m->mh = m->wh = unique[i].height;
					updatebarpos(m);
				}
		} else { /* less monitors available nn < n */
			for (i = nn; i < n; i++) {
				for (m = mons; m && m->next; m = m->next);
				while ((c = m->clients)) {
					dirty = 1;
					m->clients = c->next;
					detachstack(c);
					c->mon = mons;
					switch(attachdirection){
					case 1:
						attachabove(c);
						break;
					case 2:
						attachaside(c);
						break;
					case 3:
						attachbelow(c);
						break;
					case 4:
						attachbottom(c);
						break;
					case 5:
						attachtop(c);
						break;
					default:
						attach(c);
					}
					attachstack(c);
				}
				if (m == selmon)
					selmon = mons;
				cleanupmon(m);
			}
		}
		free(unique);
	} else
#endif /* XINERAMA */
	{ /* default monitor setup */
		if (!mons)
			mons = createmon();
		if (mons->mw != tw || mons->mh != sh) {
			dirty = 1;
			mons->mw = mons->ww = tw;
			mons->mh = mons->wh = sh;
			updatebarpos(mons);
		}
	}
	if (dirty) {
		selmon = mons;
		selmon = wintomon(root);
	}
	return dirty;
}

void
updatemotifhints(Client *c)
{
	Atom real;
	int format;
	unsigned char *p = NULL;
	unsigned long n, extra;
	unsigned long *motif;
	int width, height;

	if (!decorhints)
		return;

	if (XGetWindowProperty(dpy, c->win, motifatom, 0L, 5L, False, motifatom,
	                       &real, &format, &n, &extra, &p) == Success && p != NULL) {
		motif = (unsigned long*)p;
		if (motif[MWM_HINTS_FLAGS_FIELD] & MWM_HINTS_DECORATIONS) {
			width = WIDTH(c);
			height = HEIGHT(c);

			if (motif[MWM_HINTS_DECORATIONS_FIELD] & MWM_DECOR_ALL ||
			    motif[MWM_HINTS_DECORATIONS_FIELD] & MWM_DECOR_BORDER ||
			    motif[MWM_HINTS_DECORATIONS_FIELD] & MWM_DECOR_TITLE)
				c->bw = c->oldbw = bordersize;
			else
				c->bw = c->oldbw = 0;

			resize(c, c->x, c->y, width - (2*c->bw), height - (2*c->bw), 0);
		}
		XFree(p);
	}
}

void
updatenumlockmask(void)
{
	unsigned int i, j;
	XModifierKeymap *modmap;

	numlockmask = 0;
	modmap = XGetModifierMapping(dpy);
	for (i = 0; i < 8; i++)
		for (j = 0; j < modmap->max_keypermod; j++)
			if (modmap->modifiermap[i * modmap->max_keypermod + j]
				== XKeysymToKeycode(dpy, XK_Num_Lock))
				numlockmask = (1 << i);
	XFreeModifiermap(modmap);
}

void
updatesizehints(Client *c)
{
	long msize;
	XSizeHints size;

	if (!XGetWMNormalHints(dpy, c->win, &size, &msize))
		/* size is uninitialized, ensure that size.flags aren't used */
		size.flags = PSize;
	if (size.flags & PBaseSize) {
		c->basew = size.base_width;
		c->baseh = size.base_height;
	} else if (size.flags & PMinSize) {
		c->basew = size.min_width;
		c->baseh = size.min_height;
	} else
		c->basew = c->baseh = 0;
	if (size.flags & PResizeInc) {
		c->incw = size.width_inc;
		c->inch = size.height_inc;
	} else
		c->incw = c->inch = 0;
	if (size.flags & PMaxSize) {
		c->maxw = size.max_width;
		c->maxh = size.max_height;
	} else
		c->maxw = c->maxh = 0;
	if (size.flags & PMinSize) {
		c->minw = size.min_width;
		c->minh = size.min_height;
	} else if (size.flags & PBaseSize) {
		c->minw = size.base_width;
		c->minh = size.base_height;
	} else
		c->minw = c->minh = 0;
	if (size.flags & PAspect) {
		c->mina = (float)size.min_aspect.y / size.min_aspect.x;
		c->maxa = (float)size.max_aspect.x / size.max_aspect.y;
	} else
		c->maxa = c->mina = 0.0;
	c->isfixed = (c->maxw && c->maxh && c->maxw == c->minw && c->maxh == c->minh);
	c->hintsvalid = 1;
}

void
updatestatus(void)
{
    Monitor* m;
	if (!gettextprop(root, XA_WM_NAME, rawstext, sizeof(rawstext)) && !selmon->hidestatus) {
		strcpy(stext, defaultstatus);
        estext[0] = '\0';
	} else {
		if (!selmon->hidestatus) {
            char *e = strchr(rawstext, statussep);

            if (e) {
                *e = '\0'; e++;
                strncpy(rawestext, e, sizeof(estext) - 1);
                copyvalidchars(estext, rawestext);
            } else {
                estext[0] = '\0';
            }

            copyvalidchars(stext, rawstext);
        }
    }

    for (m = mons; m; m = m->next)
		drawbaritems(m);
}

void
updaterules(Client *c)
{
	const char *class, *instance;
	unsigned int i;
	const Rule *r;
	Monitor *m;
	XClassHint ch = { NULL, NULL };

	/* rule matching */
	XGetClassHint(dpy, c->win, &ch);
	class    = ch.res_class ? ch.res_class : notitle;
	instance = ch.res_name  ? ch.res_name  : notitle;
    char found_rule = 0;

	for (i = 0; i < LENGTH(rules); i++) {
		r = &rules[i];
		if ((!r->title || strstr(c->name, r->title))
		&& (!r->class || strstr(class, r->class))
		&& (!r->instance || strstr(instance, r->instance)))
		{
			c->isfloating = r->isfloating;

            if(!found_rule)
            {
                c->tags=0;
			    found_rule=1;
            }
            c->tags |= r->tags;
            for (m = mons; m && m->num != r->monitor; m = m->next)
			if (m)
				c->mon = m;
		}
	}

	if (ch.res_class)
		XFree(ch.res_class);
	if (ch.res_name)
		XFree(ch.res_name);
	c->tags = c->tags & TAGMASK ? c->tags & TAGMASK : c->mon->tagset[c->mon->seltags];

    /* end apply rules */
	if (c->isfloating)
		resize(c, c->x, c->y,
			c->w, c->h, 0);
	arrange(c->mon);
}

void
updatetitle(Client *c)
{
#if USEIPC
    char oldname[sizeof(c->name)];
	strcpy(oldname, c->name);
#endif
	if (!gettextprop(c->win, netatom[NetWMName], c->name, sizeof c->name))
		gettextprop(c->win, XA_WM_NAME, c->name, sizeof c->name);

	if (c->name[0] == '\0')
		strcpy(c->name, notitle);

#if USEIPC
	for (Monitor *m = mons; m; m = m->next) {
		if (m->sel == c && strcmp(oldname, c->name) != 0)
			ipc_focused_title_change_event(m->num, c->win, oldname, c->name);
	}
#endif
}

void
updatewindowtype(Client *c)
{
	Atom state = getatomprop(c, netatom[NetWMState]);
	Atom wtype = getatomprop(c, netatom[NetWMWindowType]);

    if (state == netatom[NetWMFullscreen])
        setfullscreen(c, 1);
    if (state == netatom[NetWMSticky]) {
        setsticky(c, 1);
    }
    if (wtype == netatom[NetWMWindowTypeDialog])
        c->isfloating = 1;
}

void
updatewmhints(Client *c)
{
	XWMHints *wmh;

	if ((wmh = XGetWMHints(dpy, c->win))) {
		if (c == selmon->sel && wmh->flags & XUrgencyHint && allowurgent) {
			wmh->flags &= ~XUrgencyHint;
			XSetWMHints(dpy, c->win, wmh);
		} else if (allowurgent) {
			c->isurgent = (wmh->flags & XUrgencyHint) ? 1 : 0;
		   	if (c->isurgent)
				XSetWindowBorder(dpy, c->win, scheme[SchemeBorderUrg][ColBorder].pixel);
		    }
		if (wmh->flags & InputHint)
			c->neverfocus = !wmh->input;
		else
			c->neverfocus = 0;
		XFree(wmh);
	}
}

void
view(const Arg *arg)
{
    Monitor *m;
    int i;
	unsigned int tmptag;

	if(arg->ui && (arg->ui & TAGMASK) == selmon->tagset[selmon->seltags])
		return;
    #if USETAGPREVIEW
    takepreview();
    #endif
	for (m = mons; m; m = m->next)
		m->seltags ^= 1;
	if (arg->ui & TAGMASK) {
		for (m = mons; m; m = m->next)
				m->tagset[m->seltags] = arg->ui & TAGMASK;

    if (pertag) {
		selmon->pertag->prevtag = selmon->pertag->curtag;

		if (arg->ui == ~0)
			selmon->pertag->curtag = 0;
		else {
			for (i = 0; !(arg->ui & 1 << i); i++) ;
			selmon->pertag->curtag = i + 1;
		}
	} else {
		tmptag = selmon->pertag->prevtag;
		selmon->pertag->prevtag = selmon->pertag->curtag;
		selmon->pertag->curtag = tmptag;
	}

	selmon->mastercount = selmon->pertag->mastercounts[selmon->pertag->curtag];
	selmon->stackcount = selmon->pertag->stackcounts[selmon->pertag->curtag];
	selmon->mfact = selmon->pertag->mfacts[selmon->pertag->curtag];
	selmon->sellt = selmon->pertag->sellts[selmon->pertag->curtag];
	selmon->lt[selmon->sellt] = selmon->pertag->ltidxs[selmon->pertag->curtag][selmon->sellt];
	selmon->lt[selmon->sellt^1] = selmon->pertag->ltidxs[selmon->pertag->curtag][selmon->sellt^1];
	selmon->ltaxis[layout] = selmon->pertag->ltaxis[selmon->pertag->curtag][layout];
	selmon->ltaxis[master] = selmon->pertag->ltaxis[selmon->pertag->curtag][master];
	selmon->ltaxis[stack]  = selmon->pertag->ltaxis[selmon->pertag->curtag][stack];
	selmon->ltaxis[stack2] = selmon->pertag->ltaxis[selmon->pertag->curtag][stack2];

	if (!selmon->hidebar != !selmon->pertag->hidebars[selmon->pertag->curtag])
		togglebar(NULL);
    }

	focus(NULL);
	arrange(NULL);
	updatecurrentdesktop();
}

pid_t
winpid(Window w)
{

	pid_t result = 0;

#ifdef __linux__
	xcb_res_client_id_spec_t spec = {0};
	spec.client = w;
	spec.mask = XCB_RES_CLIENT_ID_MASK_LOCAL_CLIENT_PID;

	xcb_generic_error_t *e = NULL;
	xcb_res_query_client_ids_cookie_t c = xcb_res_query_client_ids(xcon, 1, &spec);
	xcb_res_query_client_ids_reply_t *r = xcb_res_query_client_ids_reply(xcon, c, &e);

	if (!r)
		return (pid_t)0;

	xcb_res_client_id_value_iterator_t i = xcb_res_query_client_ids_ids_iterator(r);
	for (; i.rem; xcb_res_client_id_value_next(&i)) {
		spec = i.data->spec;
		if (spec.mask & XCB_RES_CLIENT_ID_MASK_LOCAL_CLIENT_PID) {
			uint32_t *t = xcb_res_client_id_value_value(i.data);
			result = *t;
			break;
		}
	}

	free(r);

	if (result == (pid_t)-1)
		result = 0;

#endif /* __linux__ */

#ifdef __OpenBSD__
        Atom type;
        int format;
        unsigned long len, bytes;
        unsigned char *prop;
        pid_t ret;

        if (XGetWindowProperty(dpy, w, XInternAtom(dpy, "_NET_WM_PID", 0), 0, 1, False, AnyPropertyType, &type, &format, &len, &bytes, &prop) != Success || !prop)
               return 0;

        ret = *(pid_t*)prop;
        XFree(prop);
        result = ret;

#endif /* __OpenBSD__ */
	return result;
}

pid_t
getparentprocess(pid_t p)
{
	unsigned int v = 0;

#ifdef __linux__
	FILE *f;
	char buf[256];
	snprintf(buf, sizeof(buf) - 1, "/proc/%u/stat", (unsigned)p);

	if (!(f = fopen(buf, "r")))
		return 0;

	if (fscanf(f, "%*u %*s %*c %u", (unsigned *)&v) != 1)
		v = (pid_t)0;
	fclose(f);
#endif /* __linux__*/

#ifdef __OpenBSD__
	int n;
	kvm_t *kd;
	struct kinfo_proc *kp;

	kd = kvm_openfiles(NULL, NULL, NULL, KVM_NO_FILES, NULL);
	if (!kd)
		return 0;

	kp = kvm_getprocs(kd, KERN_PROC_PID, p, sizeof(*kp), &n);
	v = kp->p_ppid;
#endif /* __OpenBSD__ */

	return (pid_t)v;
}

int
isdescprocess(pid_t p, pid_t c)
{
	while (p != c && c != 0)
		c = getparentprocess(c);

	return (int)c;
}

Client *
termforwin(const Client *w)
{
	Client *c;
	Monitor *m;

	if (!w->pid || w->isterminal)
		return NULL;

	for (m = mons; m; m = m->next) {
		for (c = m->clients; c; c = c->next) {
			if (c->isterminal && !c->swallowing && c->pid && isdescprocess(c->pid, w->pid))
				return c;
		}
	}

	return NULL;
}

Client *
swallowingclient(Window w)
{
	Client *c;
	Monitor *m;

	for (m = mons; m; m = m->next) {
		for (c = m->clients; c; c = c->next) {
			if (c->swallowing && c->swallowing->win == w)
				return c;
		}
	}

	return NULL;
}

Client *
wintoclient(Window w)
{
	Client *c;
	Monitor *m;

	for (m = mons; m; m = m->next)
		for (c = m->clients; c; c = c->next)
			if (c->win == w)
				return c;
	return NULL;
}

Monitor *
wintomon(Window w)
{
	int x, y;
	Client *c;
	Monitor *m;
    Bar *bar;

	if (w == root && getrootptr(&x, &y))
		return recttomon(x, y, 1, 1);
	for (m = mons; m; m = m->next)
        for (bar = m->bar; bar; bar = bar->next)
			if (w == bar->win)
				return m;
	if ((c = wintoclient(w)))
		return c->mon;
	return selmon;
}

/* There's no way to check accesses to destroyed windows, thus those cases are
 * ignored (especially on UnmapNotify's). Other types of errors call Xlibs
 * default error handler, which may call exit. */
int
xerror(Display *dpy, XErrorEvent *ee)
{
	if (ee->error_code == BadWindow
	|| (ee->request_code == X_SetInputFocus && ee->error_code == BadMatch)
	|| (ee->request_code == X_PolyText8 && ee->error_code == BadDrawable)
	|| (ee->request_code == X_PolyFillRectangle && ee->error_code == BadDrawable)
	|| (ee->request_code == X_PolySegment && ee->error_code == BadDrawable)
	|| (ee->request_code == X_ConfigureWindow && ee->error_code == BadMatch)
	|| (ee->request_code == X_GrabButton && ee->error_code == BadAccess)
	|| (ee->request_code == X_GrabKey && ee->error_code == BadAccess)
	|| (ee->request_code == X_CopyArea && ee->error_code == BadDrawable))
		return 0;
	fprintf(stderr, "speedwm: fatal error: request code=%d, error code=%d\n",
		ee->request_code, ee->error_code);
	return xerrorxlib(dpy, ee); /* may call exit */
}

int
xerrordummy(Display *dpy, XErrorEvent *ee)
{
	return 0;
}

/* Startup Error handler to check if another window manager
 * is already running. */
int
xerrorstart(Display *dpy, XErrorEvent *ee)
{
	die("speedwm: Another instance of a window manager is already running, will not start speedwm.");
	return -1;
}

void
xinitvisual()
{
	XVisualInfo *infos;
	XRenderPictFormat *fmt;
	int nitems;
	int i;

	XVisualInfo tpl = {
		.screen = screen,
		.depth = 32,
		.class = TrueColor
	};
	long masks = VisualScreenMask | VisualDepthMask | VisualClassMask;

	infos = XGetVisualInfo(dpy, masks, &tpl, &nitems);
	visual = NULL;
	for(i = 0; i < nitems; i ++) {
		fmt = XRenderFindVisualFormat(dpy, infos[i].visual);
		if (fmt->type == PictTypeDirect && fmt->direct.alphaMask) {
			visual = infos[i].visual;
			depth = infos[i].depth;
			cmap = XCreateColormap(dpy, root, visual, AllocNone);
			useargb = 1;
			break;
		}
	}

	XFree(infos);

	if (!visual || !alpha) {
		visual = DefaultVisual(dpy, screen);
		depth = DefaultDepth(dpy, screen);
		cmap = DefaultColormap(dpy, screen);
	}
}

void
zoom(const Arg *arg)
{
	Client *c = selmon->sel;

	if (!selmon->lt[selmon->sellt]->arrange || !c || c->isfloating)
		return;
	if (c == nexttiled(selmon->clients) && !(c = nexttiled(c->next)))
		return;
	pop(c);
}

#if USEXRESOURCES
void
resource_load(XrmDatabase db, char *name, enum resource_type rtype, void *dst)
{
	char *sdst = NULL;
	int *idst = NULL;
	float *fdst = NULL;

	sdst = dst;
	idst = dst;
	fdst = dst;

	char fullname[256];
	char *type;
	XrmValue ret;

	snprintf(fullname, sizeof(fullname), "%s.%s", "speedwm", name);
	fullname[sizeof(fullname) - 1] = '\0';

	XrmGetResource(db, fullname, "*", &type, &ret);
	if (!(ret.addr == NULL || strncmp("String", type, 64)))
	{
		switch (rtype) {
		case STRING:
			strcpy(sdst, ret.addr);
			break;
		case INTEGER:
			*idst = strtoul(ret.addr, NULL, 10);
			break;
		case FLOAT:
			*fdst = strtof(ret.addr, NULL);
			break;
		case DOUBLE:
			strcpy(sdst, ret.addr);
			break;
		}
	}
}

void
load_xresources(void)
{
	Display *display;
	char *resm;
	XrmDatabase db;
	ResourcePref *p;

	display = XOpenDisplay(NULL);
	resm = XResourceManagerString(display);
	if (!resm)
		return;

	db = XrmGetStringDatabase(resm);
	for (p = resources; p < resources + LENGTH(resources); p++)
		resource_load(db, p->name, p->type, p->dst);
	XCloseDisplay(display);
}
#endif

void
setclienttagprop(Client *c)
{
	long data[] = { (long) c->tags, (long) c->mon->num };
	XChangeProperty(dpy, c->win, netatom[NetClientInfo], XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) data, 2);
}

int
main(int argc, char *argv[])
{
	if (argc == 2 && !strcmp("-v", argv[1]))
		die("speedwm-"VERSION);
#if USEQUERY
    if (argc > 1 && !strcmp("-q", argv[1])) {
        if (argc == 2)
            die("speedwm: You must specify a query");
        char *args = argv[2];
        return query(args);
    }
#endif
	else if (argc != 1 && strcmp("-s", argv[1]))
		die("Invalid argument.");
	if (!setlocale(LC_CTYPE, "") || !XSupportsLocale())
		fputs("Warning: no locale support\n", stderr);
	if (!(dpy = XOpenDisplay(NULL)))
		die("speedwm: Cannot open display.");
	if (argc > 1 && !strcmp("-s", argv[1])) {
		XStoreName(dpy, RootWindow(dpy, DefaultScreen(dpy)), argv[2]);
		XCloseDisplay(dpy);
		return 0;
	}
	if (!(xcon = XGetXCBConnection(dpy)))
	    die("speedwm: Cannot get xcb connection\n");
	checkotherwm();
	XrmInitialize();

    /* dotfiles */
    int homexrdb = system("[ -e $HOME/.speedwmrc ] && xrdb -override $HOME/.speedwmrc");
    int confxrdb = system("[ -e $HOME/.config/speedwm/speedwmrc ] && xrdb -override $HOME/.config/speedwm/speedwmrc");

    /* autostart */
    if (homexrdb || confxrdb || !homexrdb || !confxrdb)
        autostart_exec();

    /* load colors */
    #if USEXRESOURCES
	load_xresources();
    #endif

	setup();
#ifdef __OpenBSD__
	if (pledge("stdio rpath proc exec ps", NULL) == -1)
		die("pledge");
#endif /* __OpenBSD__ */
	scan();
	run();
    if (restart) {
        execvp(argv[0], argv);
    }
	cleanup();
	XCloseDisplay(dpy);
	return EXIT_SUCCESS;
}

void
insertclient(Client *item, Client *insertItem, int after) {
	Client *c;
	if (item == NULL || insertItem == NULL || item == insertItem) return;
	detach(insertItem);
	if (!after && selmon->clients == item) {
		attach(insertItem);
		return;
	}
	if (after) {
		c = item;
	} else {
		for (c = selmon->clients; c; c = c->next) { if (c->next == item) break; }
	}
	insertItem->next = c->next;
	c->next = insertItem;
}

void
inplacerotate(const Arg *arg)
{
	if(!selmon->sel || (selmon->sel->isfloating && !arg->f)) return;

	unsigned int selidx = 0, i = 0;
	Client *c = NULL, *stail = NULL, *mhead = NULL, *mtail = NULL, *shead = NULL;

	/* determine positionings for insertclient */
	for (c = selmon->clients; c; c = c->next) {
		if (ISVISIBLE(c) && !(c->isfloating)) {
		if (selmon->sel == c) { selidx = i; }
		if (i == selmon->mastercount - 1) { mtail = c; }
		if (i == selmon->mastercount) { shead = c; }
		if (mhead == NULL) { mhead = c; }
		stail = c;
		i++;
		}
	}

	/* all clients rotate */
	if (arg->i == 2) insertclient(selmon->clients, stail, 0);
	if (arg->i == -2) insertclient(stail, selmon->clients, 1);


	/* stack xor master rotate */
	if (arg->i == -1 && selidx >= selmon->mastercount) insertclient(stail, shead, 1);
	if (arg->i == 1 && selidx >= selmon->mastercount) insertclient(shead, stail, 0);
	if (arg->i == -1 && selidx < selmon->mastercount)  insertclient(mtail, mhead, 1);
	if (arg->i == 1 && selidx < selmon->mastercount)  insertclient(mhead, mtail, 0);

	/* restore focus position */
	i = 0;
	for (c = selmon->clients; c; c = c->next) {
		if (!ISVISIBLE(c) || (c->isfloating)) continue;
		if (i == selidx) { focus(c); break; }
		i++;
	}
	arrange(selmon);
	focus(c);
}

#if USETAGPREVIEW
void
showtagpreview(unsigned int i)
{
    if (!selmon->tagwin)
        return;

    if (selmon->tagmap[i] && selmon->previewshow) {
        XSetWindowBackgroundPixmap(dpy, selmon->tagwin, selmon->tagmap[i]);
        XCopyArea(dpy, selmon->tagmap[i], selmon->tagwin, drw->gc, 0, 0,
                selmon->mw / selmon->scalepreview, selmon->mh / selmon->scalepreview,
                0, 0);
        XSync(dpy, False);
        XMapRaised(dpy, selmon->tagwin);
    } else {
        XUnmapWindow(dpy, selmon->tagwin);
    }
}

void
takepreview(void)
{
	Client *c;
	Imlib_Image image;
	unsigned int occ = 0, i;
    Monitor *m = selmon;

    if (!m->tagwin) {
        updatepreviews(m);

        /* failsafe, should prevent crashing if updatepreviews(m) doesn't create tagwin */
        if (!m->tagwin) {
            return;
        }
    }

	for (c = m->clients; c; c = c->next)
        if (!m->hideemptytags)
            occ |= c->tags;
        else
            occ |= c->tags == 255 ? 0 : c->tags;

	for (i = 0; i < LENGTH(ftags); i++) {
		/* searching for tags that are occupied && selected */
		if (!(occ & 1 << i) || !(m->tagset[m->seltags] & 1 << i))
			continue;

		if (m->tagmap[i]) { /* tagmap exist, clean it */
			XFreePixmap(dpy, m->tagmap[i]);
			m->tagmap[i] = 0;
		}

		m->previewshow = 0;
		XUnmapWindow(dpy, m->tagwin);
		XSync(dpy, False);

		if (!(image = imlib_create_image(tw, sh))) {
			fprintf(stderr, "speedwm: imlib: failed to create image, skipping.");
			continue;
		}
		imlib_context_set_image(image);
		imlib_context_set_display(dpy);
		imlib_image_set_has_alpha(1);
		imlib_context_set_blend(0);
		imlib_context_set_visual(visual);
		imlib_context_set_drawable(root);

       	if (!barpreview)
				imlib_copy_drawable_to_image(0, m->wx, m->wy, m->ww, m->wh, 0, 0, 1);
    	else
				imlib_copy_drawable_to_image(0, m->mx, m->my, m->mw, m->mh, 0, 0, 1);

      	m->tagmap[i] = XCreatePixmap(dpy, m->tagwin, m->mw / m->scalepreview, m->mh / m->scalepreview, depth);
		imlib_context_set_drawable(m->tagmap[i]);
		imlib_render_image_part_on_drawable_at_size(0, 0, m->mw, m->mh, 0, 0, m->mw / m->scalepreview, m->mh / m->scalepreview);
		imlib_free_image();
	}
}

void
updatepreviews(Monitor *m)
{
    int x_pad = sp;
    int y_pad = vp;

    if (m->tagwin) {
        XMoveResizeWindow(dpy, m->tagwin, m->mx, m->bar->by + bh, m->mw / m->scalepreview, m->mh / m->scalepreview);
        return;
    }

	XSetWindowAttributes wa = {
		.override_redirect = True,
		.background_pixel = 0,
		.border_pixel = 0,
		.colormap = cmap,
		.event_mask = ButtonPressMask|ExposureMask|PointerMotionMask
	};

     m->tagwin = XCreateWindow(dpy, root, m->wx + x_pad, m->bar->by + bh + y_pad + m->gapsizeov / 2, m->mw / m->scalepreview, m->mh / m->scalepreview, 0,
	    depth, CopyFromParent, visual, CWOverrideRedirect|CWBackPixel|CWBorderPixel|CWColormap|CWEventMask, &wa);

	XDefineCursor(dpy, m->tagwin, cursor[CurNormal]->cursor);
    XMapRaised(dpy, m->tagwin);
	XUnmapWindow(dpy, m->tagwin);
}
#endif

void
previewtag(const Arg *arg)
{
    #if USETAGPREVIEW
	if (selmon->previewshow != (arg->ui + 1))
		selmon->previewshow = arg->ui + 1;
	else
		selmon->previewshow = 0;
	showtagpreview(arg->ui);
    #else
    return;
    #endif
}

void
centerwindow(const Arg *arg)
{
    if (!selmon->sel)
        return;

    resizeclient(selmon->sel, (selmon->mw - selmon->mw * 0.5) / 2, (selmon->mh - selmon->mh * 0.5) / 2, selmon->mw * 0.5, selmon->mh * 0.5);
}

void
togglewin(const Arg *arg)
{
	Client *c = (Client*)arg->v;
	if (!c)
		c = selmon->sel;
	if (!c)
		return;

    /* because it's useless with only one window */
    if (selmon->hideunselectedtitle) {
        return;
    }

	if (HIDDEN(c)) {
		showwin(c);
	} else {
		hidewin(c);
	}

    selmon->allowwarp = 0;
    restack(c->mon);
    focus(c);
    arrange(selmon);
}

/* Layout code */
#include "layouts.c" /* Enable patched layouts */
