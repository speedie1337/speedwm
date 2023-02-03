# speedwm options
# The default configuration is only compiled for your host.
# Change CFLAGS to override this.
###################################################################################

# speedwm version
VERSION = 1.9

X11INC = /usr/X11R6/include
X11LIB = /usr/X11R6/lib

# includes and libs
INCS = -I${X11INC} -I${FREETYPEINC} -I${YAJLINC} `pkg-config --cflags xft pango pangoxft`
LIBS = -L${X11LIB} -lX11 ${XINERAMALIBS} ${FREETYPELIBS} -lXrender ${IMLIB2LIBS} -lX11-xcb -lxcb -lxcb-res -lXext ${YAJLLIBS} `pkg-config --libs xft pango pangoxft`

# flags
CPPFLAGS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_POSIX_C_SOURCE=200809L -DVERSION=\"${VERSION}\" ${XINERAMAFLAGS}

# NOTE: host.mk and toggle.mk will both override this.
