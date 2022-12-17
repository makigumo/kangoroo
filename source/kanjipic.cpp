
/* 
  Kangoroo v1.0 Feb 2007 Jan Poland
  
  Licence: GNU GPL v2
  
  WARNING: This source code has been made available to the public, in order to
  increase the amount of available source code for the Sharp Zaurus, and in order
  to allow interested individuals to customize and improve the software.
  THE SOURCE CODE IS NEITHER WELL STRUCTURED NOR DOCUMENTED!

*/

#include <math.h>

#include "kanjipic.h"

#define SIZE 80

Kanjipic::Kanjipic( QString &dir, KangBrowser* pp, QWidget* parent, const char* name, WFlags fl ) : QFrame( parent, name, fl )
{
  pKangBrowser = pp;
  kdir=dir;
  pic.load(QString("kang.png").prepend(kdir));
  for (int i=0;i<7;i++) P[i]=0;
  setFixedSize(SIZE+3,SIZE+3);
  setFrameStyle(QFrame::WinPanel|QFrame::Raised);
}

Kanjipic::~Kanjipic() {}

int Kanjipic::setK(QString line) {
  int i,k;
  QString s;
  for (i=0;i<7;i++) P[i]=0;
  for (i=0;i<7 && line.length()>0 && (k=line.find(QRegExp("\\s")));line.remove(0,k+1),i++) 
    P[i] = line.left(k).toInt();
  if (P[0]>0) {
    s.sprintf("%.4i.svg.png",P[0]),
    pic.load(s.prepend(kdir));
    im=pic.convertToImage();
  } else pic.load(QString("kang.png").prepend(kdir)); 
  repaint();
  return P[0];
}    

void Kanjipic::paintEvent ( QPaintEvent* e ) {
  QFrame::paintEvent(e);
  bitBlt(this, 0, 0, &pic);
}


void Kanjipic::mousePressEvent ( QMouseEvent* e ) {
  int x0=e->x(), y0=e->y(), x, y, i, r=-1, h, s, v, j=-1;
  QString str;
  double d;
  if (P[0]>0) {
    while (j==-1 && r++<25) {
      d=(r==0)?0.4:(double)((r<<1)+1);
      for (i=0;j==-1 && i<(int)(M_PI*d);i++) {
        x=x0+(int)(r*cos((double)(i<<1)/d)); y=y0+(int)(r*sin((double)(i<<1)/d));
        if (x>=0 && y>=0 && x<SIZE && y<SIZE) {
          QColor(im.pixel(x,y)).hsv(&h,&s,&v);
          if (v<64) j=0; else if (h>=0 && s>128) {
            if (h<10 || h>350) j=1; else
            if (h>110 && h<130) j=2; else
            if (h>230 && h<250) j=3; else
            if (h>170 && h<190) j=4; else
            if (h>270 && h<290) j=5; else
            if (h>20 && h<40) j=6;
            if (j>0) j=P[j];
          }
        }
      }
    }
  }
  if (j>0) pKangBrowser->setSource(str.setNum(j).prepend('s'));
}

