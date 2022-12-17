call setenv.bat
qmake -makefile kangoroo-qt-win.pro
make clean
windres kangico.rc kangico.o
make
