
/* 
  Kangoroo v1.0 Feb 2007 Jan Poland
  
  Licence: GNU GPL v2
  
  WARNING: This source code has been made available to the public, in order to
  increase the amount of available source code for the Sharp Zaurus, and in order
  to allow interested individuals to customize and improve the software.
  THE SOURCE CODE IS NEITHER WELL STRUCTURED NOR DOCUMENTED!

*/

#include "kanglineedit.h"
#include "kangbrowser.h"

KangLineEdit::KangLineEdit( QWidget* parent,  const char* name ) : 
  QLineEdit( parent, name ) {}

KangLineEdit::~KangLineEdit() {}

void KangLineEdit::setKangBrowser( KangBrowser *p ) { pKangBrowser=p; }

void KangLineEdit::keyPressEvent ( QKeyEvent * e ) {
  int k = e->key(), i;
  if (k==Key_Up || k==Key_Down) {for (i=0;i<3;i++) pKangBrowser->keyPressEvent(e);} else
  if (k==Key_PageUp || k==Key_PageDown)
    pKangBrowser->keyPressEvent(e); else QLineEdit::keyPressEvent(e);
}
