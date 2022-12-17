TEMPLATE	= app
TARGET		= kangoroo

CONFIG		+= qt warn_on release
DEPENDPATH	= $$QTDIR/include

HEADERS		= kangoroo.h \
                  kangbrowser.h \
                  kanglineedit.h \
                  kangpushbutton.h \
                  kanjipic.h \
                  keydlg.h
SOURCES		= main-qt.cpp \
                  kangoroo.cpp \
                  kangbrowser.cpp \
                  kanglineedit.cpp \
                  kangpushbutton.cpp \
                  kanjipic.cpp \ 
                  keydlg.cpp

RC_FILE		= ../icon/kangoroo.icns
