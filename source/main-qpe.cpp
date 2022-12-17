
/* 
  Kangoroo v1.0 Feb 2007 Jan Poland
  
  Licence: GNU GPL v2
  
  WARNING: This source code has been made available to the public, in order to
  increase the amount of available source code for the Sharp Zaurus, and in order
  to allow interested individuals to customize and improve the software.
  THE SOURCE CODE IS NEITHER WELL STRUCTURED NOR DOCUMENTED!

*/

#include "kangoroo.h"
#include <qpe/qpeapplication.h>

int main( int argc, char ** argv )
{
    QPEApplication a( argc, argv );

    Kangoroo mw;
    a.showMainWidget( &mw );

    return a.exec();
}
