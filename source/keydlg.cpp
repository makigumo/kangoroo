
/* 
  Kangoroo v1.0 Feb 2007 Jan Poland
  
  Licence: GNU GPL v2
  
  WARNING: This source code has been made available to the public, in order to
  increase the amount of available source code for the Sharp Zaurus, and in order
  to allow interested individuals to customize and improve the software.
  THE SOURCE CODE IS NEITHER WELL STRUCTURED NOR DOCUMENTED!

*/

#include <qlayout.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>

#include "keydlg.h"

KeyDlg::KeyDlg(QWidget *parent, const QString &s, bool bCk): QDialog(parent,0,true,0) {
  setCaption("KeyWord");
  QVBoxLayout* vb = new QVBoxLayout(this);
  vb->addWidget( new QLabel("Please enter keyword:",this) );
  le = new QLineEdit(s,this);
  le->selectAll();
  vb->addWidget(le);
  
  cb = new QCheckBox("&mark for training",this);
  cb->setChecked(bCk);
  vb->addWidget(cb);
  
  QHBoxLayout* hb = new QHBoxLayout; 
  QPushButton *pbok = new QPushButton("&Ok", this);
  pbok->setDefault(true);
  connect(pbok,SIGNAL(clicked()),this,SLOT(accept()));
  hb->addWidget(pbok);
  QPushButton *pbcancel = new QPushButton("&Cancel", this);
  connect(pbcancel,SIGNAL(clicked()),this,SLOT(reject()));
  hb->addWidget(pbcancel);
  vb->addLayout(hb);
}        

KeyDlg::~KeyDlg() {}
