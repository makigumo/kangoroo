
/* 
  Kangoroo v1.0 Feb 2007 Jan Poland
  
  Licence: GNU GPL v2
  
  WARNING: This source code has been made available to the public, in order to
  increase the amount of available source code for the Sharp Zaurus, and in order
  to allow interested individuals to customize and improve the software.
  THE SOURCE CODE IS NEITHER WELL STRUCTURED NOR DOCUMENTED!

*/

#ifndef KEYDLG_H
#define KEYDLG_H

#include <qstring.h>
#include <qdialog.h>
#include <qlineedit.h>
#include <qcheckbox.h>

class QLineEdit;
class QComboBox;

class KeyDlg : public QDialog {
  Q_OBJECT
  
public:
    KeyDlg(QWidget *, const QString &, bool );
    ~KeyDlg();
    
    QLineEdit* le;
    QCheckBox* cb;
};

#endif
