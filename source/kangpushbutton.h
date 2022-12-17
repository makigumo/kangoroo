
/* 
  Kangoroo v1.0 Feb 2007 Jan Poland
  
  Licence: GNU GPL v2
  
  WARNING: This source code has been made available to the public, in order to
  increase the amount of available source code for the Sharp Zaurus, and in order
  to allow interested individuals to customize and improve the software.
  THE SOURCE CODE IS NEITHER WELL STRUCTURED NOR DOCUMENTED!

*/


#ifndef KANGPUSHBUTTON_H
#define KANGPUSHBUTTON_H

#include <qpushbutton.h>

class KangBrowser;

class KangPushButton : public QPushButton
{ 
    Q_OBJECT

public:
    KangPushButton( QWidget* parent = 0, const char* name = 0 );
    ~KangPushButton();
    
    void setKangBrowser( KangBrowser *p );

protected:
    void keyPressEvent ( QKeyEvent * e );
    
private:
    KangBrowser* pKangBrowser;    
    
};

#endif // KANGPUSHBUTTON_H
