
/* 
  Kangoroo v1.0 Feb 2007 Jan Poland
  
  Licence: GNU GPL v2
  
  WARNING: This source code has been made available to the public, in order to
  increase the amount of available source code for the Sharp Zaurus, and in order
  to allow interested individuals to customize and improve the software.
  THE SOURCE CODE IS NEITHER WELL STRUCTURED NOR DOCUMENTED!

*/

#ifndef KANJIPIC_H
#define KANJIPIC_H

#include <qframe.h>
#include <qevent.h>
#include <qstring.h>
#include <qimage.h>

#include "kangbrowser.h"


class Kanjipic : public QFrame
{ 
    Q_OBJECT

public:

    Kanjipic( QString &dir, KangBrowser *pp = 0, QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~Kanjipic();
    
    int setK(QString line);
    
protected:
    void paintEvent ( QPaintEvent* e);
    void mousePressEvent ( QMouseEvent* e );    

private:
    QPixmap pic;
    QImage im;
    KangBrowser* pKangBrowser;
    QString kdir;
    int P[7];
};

#endif 
