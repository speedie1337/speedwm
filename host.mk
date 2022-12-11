# speedwm host options
# See README.md for more options.

# Compiler
CC           = tcc

# Paths
# These should be fine for most users but if you use a distribution of GNU/Linux like NixOS or GNU Guix, consider changing this to fit your use case.
# PAGEDIR is only useful if you are using 'make release'.
PREFIX       = /usr
MANPREFIX 	 = ${PREFIX}/share/man
PAGEDIR      = "/home/anon/Projects/speediegq/projects"

# GNU/Linux support
# If you use GNU/Linux, uncomment these lines (remove the # at the start of the line below)
FREETYPELIBS = -lfontconfig -lXft
FREETYPEINC  = /usr/include/freetype2
EXCFLAGS     = -Wno-unused-variable -Wno-unused-function -Wno-unused-result -Wno-array-bounds
CFLAGS       = -std=c99 -pedantic -Wall -Wno-deprecated-declarations -Ofast -march=native ${INCS} ${CPPFLAGS} ${EXCFLAGS}
LDFLAGS      = ${LIBS} -g

# OpenBSD support
# If you use OpenBSD, uncomment these lines. (remove the # at the start of the line below)
#FREETYPEINC = ${X11INC}/freetype2

# Solaris support
# If you use Solaris, uncomment these lines. (remove the # at the start of the line below)
#CFLAGS      = -fast ${INCS} -DVERSION=\"${VERSION}\"
#LDFLAGS     = ${LIBS}
