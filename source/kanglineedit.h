
/* 
  Kangoroo v1.0 Feb 2007 Jan Poland
  
  Licence: GNU GPL v2
  
  WARNING: This source code has been made available to the public, in order to
  increase the amount of available source code for the Sharp Zaurus, and in order
  to allow interested individuals to customize and improve the software.
  THE SOURCE CODE IS NEITHER WELL STRUCTURED NOR DOCUMENTED!

*/


#ifndef KANGLINEEDIT_H
#define KANGLINEEDIT_H

#include <qlineedit.h>

class KangBrowser;

class KangLineEdit : public QLineEdit
{ 
    Q_OBJECT

public:
    KangLineEdit( QWidget* parent = 0, const char* name = 0 );
    ~KangLineEdit();
    
    void setKangBrowser( KangBrowser *p );

protected:
    void keyPressEvent ( QKeyEvent * e );
    
private:
    KangBrowser* pKangBrowser;    
    
};

#endif // KANGLINEEDIT_H
