
/* 
  Kangoroo v1.0 Feb 2007 Jan Poland
  
  Licence: GNU GPL v2
  
  WARNING: This source code has been made available to the public, in order to
  increase the amount of available source code for the Sharp Zaurus, and in order
  to allow interested individuals to customize and improve the software.
  THE SOURCE CODE IS NEITHER WELL STRUCTURED NOR DOCUMENTED!

*/


#ifdef QTOPIA
  #include <qpe/qpeapplication.h>
#else
  #include <qapplication.h>
#endif  

#include "kangoroo.h"

#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

#include "kangpushbutton.h"
#include "kanglineedit.h"
#include <qlayout.h>
#include <qdir.h>
#include <qlabel.h>
#include <qpixmap.h>
#include <stdlib.h>

static int menuCodes[] = {3, 1, 2, 0, 1, 2, 1, 1, 20, 50, 100};
static int mcMemo[] = {0, 1, 2, 0};    

#ifdef WINDOWS
  static char conffname[] = "\\kangoroo.cnf";
  static char resfname[] = "\\kangoroo.res";
  static char kdirdefault[] = "\\kangoroo\\";
  static char learndirdefault[] = "\\kangoroo-learn\\";
  static char slash='\\';
#else  
  static char conffname[] = "/.kangoroo";
  static char resfname[] = "/.kangoroo.res";
  static char kdirdefault[] = "/kangoroo/";
  static char learndirdefault[] = "/kangoroo-learn/";
  static char slash='/';
#endif

static char kangname[] = "Kangoroo";

Kangoroo::Kangoroo( QWidget* parent,  const char* name, WFlags fl ) : QWidget( parent, name, fl )
{    
    if ( !name ) setName( kangname );    
    setCaption( kangname );
    QString kdir, learndir;
    char pbuf[128], *home;
    QFile f;
    QHBoxLayout* hbox;
    QVBoxLayout* vbox;

    
#ifdef WINDOWS
    home = getenv("WINDIR");
#else
    home = getenv("HOME");
#endif

    sFont = 0;
    resfilename = QString(resfname).prepend(home);
    f.setName(resfilename);
    if (f.open(IO_ReadOnly)) {
      int w, h;
      f.readLine(pbuf,127); w = QString::fromLatin1(pbuf).toInt();
      f.readLine(pbuf,127); h = QString::fromLatin1(pbuf).toInt();
      f.readLine(pbuf,127); sFont = QString::fromLatin1(pbuf).toInt();
      if (sFont<0 || sFont>2) sFont=0;
#ifndef QTOPIA
      if (w>1 && h>1) resize( w, h ); 
#endif      
      f.close();
    }

    f.setName(QString(conffname).prepend(home));
    if (f.open(IO_ReadOnly)) {
      f.readLine(pbuf,127); kdir = QString::fromLatin1(pbuf).simplifyWhiteSpace();
      f.readLine(pbuf,127); learndir = QString::fromLatin1(pbuf).simplifyWhiteSpace();
      f.close();
    }
    if (kdir.isEmpty()) kdir=QString(kdirdefault).prepend(home);
    if (learndir.isEmpty()) learndir=QString(learndirdefault).prepend(home);
    
    setIcon( QPixmap( QString("kangoroo32.png").prepend(kdir)));    
    
    KangBaseLayout = new QVBoxLayout( this ); 
    KangBaseLayout->setSpacing( 0 );
    KangBaseLayout->setMargin( 0 );
        
    vbox = new QVBoxLayout; 
    vbox->setSpacing( 0 );
    vbox->setMargin( 0 );
        
    hbox = new QHBoxLayout; 
    hbox->setSpacing( 4 );
    hbox->setMargin( 2 );

    qlabel = new QLabel( QString::fromUtf8(
     "\xe4\xb8\x80\xe4\xb8\x80\xe4\xb8\x80\xe4\xb8\x80\xe4\xb8\x80\xe4\xb8\x80\xe4\xb8\x80\xe4\xb8\x80"),
      this);
    hbox->addWidget( qlabel );
    
    query = new KangLineEdit( this, "query" );
    hbox->addWidget( query );

    clear = new KangPushButton( this, "clear" );
    clear->setText( tr( "cl&ear" ) );
    hbox->addWidget( clear );
    clear->setAccel( CTRL+Key_E );
    vbox->addLayout( hbox );
        
    hbox = new QHBoxLayout; 
    hbox->setSpacing( 4 );
    hbox->setMargin( 2 );
    
    search = new KangPushButton( this, "search" );
    search->setText( tr( "&search" ) );
    hbox->addWidget( search );
    search->setAccel( CTRL+Key_S );

    memo = new KangPushButton( this, "memo" );
    memo->setText( tr( "&memo" ) );
    hbox->addWidget( memo );
    memo->setAccel( CTRL+Key_M );

    kanji = new KangPushButton( this, "kanji" );
    kanji->setText( tr( "&Kanji" ) );
    hbox->addWidget( kanji );
    kanji->setAccel( CTRL+Key_K );

    train = new KangPushButton( this, "train" );
    train->setText( tr( "&train" ) );
    hbox->addWidget( train );
    train->setAccel( CTRL+Key_T );

    cpaste = new KangPushButton( this, "cpaste" );
    cpaste->setText( tr( "cp&&p" ) );
    hbox->addWidget( cpaste );
    
    vbox->addLayout( hbox );
    
    res = new KangBrowser( kdir, this, "res" );
    
    pic = new Kanjipic( kdir.append("kpic").append(slash), res, this, "pic");
    res->setKang(this,pic);    
    
    hbox = new QHBoxLayout; 
    hbox->setSpacing( 4 );
    hbox->setMargin( 2 );
    hbox->addWidget( pic );
    hbox->addLayout( vbox );
    
    KangBaseLayout->addLayout( hbox );
    KangBaseLayout->addWidget( res );
        
    // tab order
    setTabOrder( query, search );
    setTabOrder( search, memo );
    setTabOrder( memo, kanji );
    setTabOrder( kanji, train );
    setTabOrder( train, cpaste );
    setTabOrder( cpaste, clear );
    
    query->setKangBrowser( res );
    clear->setKangBrowser( res );
    search->setKangBrowser( res );
    memo->setKangBrowser( res );
    kanji->setKangBrowser( res );
    train->setKangBrowser( res );
    cpaste->setKangBrowser( res );

    mainMenu = new QMenuBar(this);
    learnfileMenu = new QPopupMenu();
    
    listLF = QDir(learndir,"*.utf8").entryList();
    int id=0;
    if (listLF.isEmpty()) listLF.append("learn.utf8");
    for (QStringList::Iterator it = listLF.begin(); it != listLF.end(); it++ ) {
      learnfileMenu->insertItem(*it, id++);
      (*it).prepend(learndir);
    }    
    learnfileMenu->setItemChecked(0,true);
    jLF=0;
    
    settingsMenu = new QPopupMenu();
    
    settingsMenu->insertItem("memo: all", 0);
    settingsMenu->insertItem("memo: without *", 1);
    settingsMenu->insertItem("memo: with *", 2);
    settingsMenu->setItemChecked(2,true);
    sMemo = menuCodes[2];
        
    settingsMenu->insertSeparator();
    
    settingsMenu->insertItem("font: normal", 3);
    settingsMenu->insertItem("font: large", 4);
    settingsMenu->insertItem("font: extra large", 5);
    settingsMenu->setItemChecked(3+sFont,true);    
    
    settingsMenu->insertSeparator();
    
    settingsMenu->insertItem("display KanjiBB first", 6);
    sKanjiBBfirst = 0;
    
    settingsMenu->insertSeparator();
    
    settingsMenu->insertItem("full-search: common only", 7);
    settingsMenu->setItemChecked(7,true);
    sSearchFullDefault = 1;
    
    settingsMenu->insertSeparator();
    
    settingsMenu->insertItem("max #results: 20", 8);
    settingsMenu->insertItem("max #results: 50", 9);
    settingsMenu->insertItem("max #results: 100", 10);
    settingsMenu->setItemChecked(9,true);
    maxResults = menuCodes[9];
    
    trainMenu = new QPopupMenu();
    
    trainMenu->insertItem("Vocabulary: by Kanji", 0);
    trainMenu->insertItem("Vocabulary: by Kana", 1);
    trainMenu->insertItem("Vocabulary: by Translation", 2);
    trainMenu->insertItem("Kanji: by Segmentation", 3);
    trainMenu->insertItem("Kanji: by Kanji", 4);
    trainMenu->insertItem("Kanji: by Keyword", 5);
    trainMenu->setItemChecked(2,true);
    sTrain = 2;    
    
    trainMenu->insertSeparator();
    trainMenu->insertItem("Kanji: marked only", 6);
    trainMenu->setItemChecked(6,true);
    sKTrMarked=1;
    
    trainMenu->insertSeparator();
    trainMenu->insertItem("Kanji: also building blocks", 7);
    trainMenu->setItemChecked(7,true);
    sKTrBB=1;
       
    naviMenu = new QPopupMenu();
    naviMenu->insertItem( "&Back",  this, SLOT(clickBack()), CTRL+Key_B , 10);
    naviMenu->insertItem( "&Forward",  this, SLOT(clickNext()), CTRL+Key_F , 11);
    naviMenu->insertSeparator();
    naviMenu->insertItem("List annotated Kanji", this, SLOT(clickAnnotatedKanji()));
    naviMenu->insertSeparator();
    naviMenu->insertItem( "&Quit",  this, SLOT(close()));
    
    helpMenu = new QPopupMenu();
    helpMenu->insertItem( "&Help",  this, SLOT(dispHelp()));
    helpMenu->insertSeparator();
    helpMenu->insertItem( "&About",  this, SLOT(dispAbout()));
    
    mainMenu->insertItem( "&Navigation", naviMenu, 0, 0 );
    mainMenu->insertItem( "&LearnFile", learnfileMenu, 1, 1 );
    mainMenu->insertItem( "&Settings", settingsMenu, 2, 2 );
    mainMenu->insertItem( "&Train", trainMenu, 3, 3 );
    mainMenu->insertItem( "&Help", helpMenu, 4, 4 );
    
//     mainMenu->setSeparator( QMenuBar::InWindowsStyle );
    
    KangBaseLayout->setMenuBar( mainMenu );
    
    mainMenu->setItemEnabled(10,false);
    mainMenu->setItemEnabled(11,false);
    
    if (!res->bCorrectInit) {
      res->setText("Could not find resources, please make sure that your installation is correct!");
      return;
    }

    connect(query, SIGNAL(returnPressed()), this, SLOT(hitEnter()));
    
    connect(search, SIGNAL(clicked()), this, SLOT(hitEnter()));
    connect(memo, SIGNAL(clicked()  ), this, SLOT(clickMemo()));
    connect(kanji, SIGNAL(clicked()), this, SLOT(clickKanji()));
    connect(train, SIGNAL(clicked()), this, SLOT(clickTrain()));
    connect(cpaste, SIGNAL(clicked()), this, SLOT(clickCP()));
    connect(clear, SIGNAL(clicked()), this, SLOT(clickClear()));
    
    connect(settingsMenu, SIGNAL(activated(int)), this, SLOT(ssMenu(int)));
    connect(trainMenu, SIGNAL(activated(int)), this, SLOT(trMenu(int)));
    connect(learnfileMenu, SIGNAL(activated(int)), this, SLOT(lfMenu(int)));
    
//     setTabOrder( clear, query );
    res->setFocusPolicy(QWidget::NoFocus);
    pic->setFocusPolicy(QWidget::NoFocus);
    
    if (!res->readLearnfile(listLF.first())) dispAbout();
}

Kangoroo::~Kangoroo() {}

void Kangoroo::keyPressEvent ( QKeyEvent * e ) {
  if (e->key()==Key_F11) { // Zaurus menu key !!
    mainMenu->setFocus();
    mainMenu->activateItemAt(0);
  } else QWidget::keyPressEvent(e);
}

void Kangoroo::showEvent ( QShowEvent* ) {  
  if (res->kanjiWidth==1) { 
    res->kanjiWidth=(qlabel->width()>>3);
    res->calcLineLength();
    qlabel->setText("Query:");
  }  
}  

void Kangoroo::closeEvent ( QCloseEvent* e ) {
  QString s,x;
  QFile f(resfilename);
  if (f.open(IO_WriteOnly)) {
    s.setNum(size().width()); s.append("\n");
    x.setNum(size().height()); s.append(x).append("\n");
    x.setNum(sFont); s.append(x).append("\n");
    f.writeBlock(s.latin1(),s.length());
    f.close();
  }
  e->accept();
  qApp->quit();
}

void Kangoroo::enableBackButton(bool b) { naviMenu->setItemEnabled(10,b); }

void Kangoroo::enableNextButton(bool b) { naviMenu->setItemEnabled(11,b); }

void Kangoroo::setQueryFocus() { query->setFocus(); query->selectAll(); }

void Kangoroo::hitEnter() {
  QString s = QString("e");
  s.append(query->text());
  res->setSource(s);
}

void Kangoroo::clickMemo() {
  QString s = QString("m");
  s.append(query->text());
  res->setSource(s);
}

void Kangoroo::clickKanji() {
  QString s = QString("k");
  s.append(query->text());
  res->setSource(s);
}

void Kangoroo::clickTrain() {
  QString s = QString("t");
  res->setSource(s);
}

void Kangoroo::dispHelp() {res->dispHelp();}

void Kangoroo::dispAbout() {res->dispAbout();}

void Kangoroo::clickAnnotatedKanji() {res->setSource("A");}

void Kangoroo::clickBack() {res->backward();}

void Kangoroo::clickNext() {res->next();}

void Kangoroo::clickCP() {
  if (res->hasSelectedText()) {
    QString s = res->selectedText();
    s.replace( QRegExp("<[^>]*>"), "");
    query->insert(s); 
    res->copy();
  }
}

void Kangoroo::clickClear() { query->setText(QString("")); query->setFocus(); }

void Kangoroo::trMenu(int id) {
  if (id<0) return;
  if (id<6) {
    if (id!=sTrain) {
      trainMenu->setItemChecked(sTrain,false);
      trainMenu->setItemChecked(id,true);
      sTrain=id;
    }
  } else if (id==6) {
    sKTrMarked=!sKTrMarked;
    trainMenu->setItemChecked(6,sKTrMarked);    
  } else {
    sKTrBB=!sKTrBB;
    trainMenu->setItemChecked(7,sKTrBB);    
  }
}  

void Kangoroo::ssMenu(int id) { 
  int s= menuCodes[id];
  if (id<3) {
    if (s!=sMemo) {
      settingsMenu->setItemChecked(mcMemo[sMemo],false);
      settingsMenu->setItemChecked(mcMemo[s],true);
      sMemo = s;
    }  
  } else if (id<6) {
    if (s!=sFont) {
      settingsMenu->setItemChecked(sFont+3,false);
      settingsMenu->setItemChecked(s+3,true);
      sFont = s;
      res->refreshDisplay();
    }  
  } else if (id==6) {  
    sKanjiBBfirst=!sKanjiBBfirst;
    settingsMenu->setItemChecked(6,sKanjiBBfirst);
  } else if (id==7) {  
    sSearchFullDefault=!sSearchFullDefault;
    settingsMenu->setItemChecked(7,sSearchFullDefault);
  } else {
    if (s!=maxResults) {
      settingsMenu->setItemChecked(maxResults==20?8:(maxResults==50?9:10),false);
      settingsMenu->setItemChecked(id,true);
      maxResults = s;
    } 
  }
}

void Kangoroo::lfMenu(int id) { 
  if (id!=jLF) {
    learnfileMenu->setItemChecked(jLF,false);
    learnfileMenu->setItemChecked(id,true);
    jLF=id;
    res->writeLearnfile();
    res->readLearnfile(listLF[jLF]);
  }
}
