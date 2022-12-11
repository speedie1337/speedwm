# speedwm compilation options
#
# Depending on your configuration, you may need to uncomment some lines here.

# IPC
# Whether or not to compile in IPC support.
# If you use OpenBSD or FreeBSD, this is not supported. In that case comment out these three lines and set USEIPC in toggle.h to 0.
YAJLINC       = /usr/include/yajl # Comment if you don't use IPC
YAJLLIBS      = -lyajl # Comment if you don't use IPC

# Built in status
# Whether or not to compile the built in status bar (fork of dwmblocks).
USESTATUS     = true # Comment if you do not want it.

# Tag previews and window icons
# If you use tag previews or window icons (enabled by default), these must be uncommented.
# If you do not use them, disable them in toggle.h and comment these lines.
IMLIB2LIBS    = -lImlib2

# Multimonitor support
# If you use multiple monitors (enabled by default), these must be uncommented.
# This also requires libXinerama.
# If you do not use them, comment these lines.
XINERAMALIBS  = -lXinerama
XINERAMAFLAGS = -DXINERAMA
