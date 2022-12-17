
/* 
  Kangoroo v1.0 Feb 2007 Jan Poland
  
  Licence: GNU GPL v2
  
  WARNING: This source code has been made available to the public, in order to
  increase the amount of available source code for the Sharp Zaurus, and in order
  to allow interested individuals to customize and improve the software.
  THE SOURCE CODE IS NEITHER WELL STRUCTURED NOR DOCUMENTED!

*/

#ifndef KANG_H
#define KANG_H
#include "kangbrowser.h"
#include "kanjipic.h"

#include <qmenubar.h>
#include <qstringlist.h>
#include <qvariant.h>
#include <qwidget.h>
#include <qevent.h>

class QLabel;
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class KangLineEdit;
class KangPushButton;

class Kangoroo : public QWidget
{ 
    Q_OBJECT

public:
    int sMemo;
    int sTrain;
    int sKTrMarked;
    int sKTrBB;
    int sFont;
    int sSearchFullDefault;
    int sKanjiBBfirst;
    int maxResults;
    
    KangBrowser* res;
    Kanjipic* pic;
    QMenuBar* mainMenu;
    QPopupMenu* learnfileMenu;
    QPopupMenu* settingsMenu;
    QPopupMenu* trainMenu;
    QPopupMenu* naviMenu;
    QPopupMenu* helpMenu;
    
    Kangoroo( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~Kangoroo();
    
    void enableBackButton(bool b);
    void enableNextButton(bool b);
    void setQueryFocus();
    
    QLabel *qlabel;
    KangLineEdit* query;
    KangPushButton* clear;
    KangPushButton* search;
    KangPushButton* memo;
    KangPushButton* kanji;
    KangPushButton* train;
    KangPushButton* cpaste;

protected:
    void showEvent ( QShowEvent* e);
    void closeEvent ( QCloseEvent* e);
    void keyPressEvent ( QKeyEvent* e);

    QVBoxLayout* KangBaseLayout;
    
private slots:
    void hitEnter();
    void clickMemo();
    void clickKanji();
    void clickTrain();
    void clickBack();
    void clickNext();
    void clickCP();
    void clickClear();
    
    void clickAnnotatedKanji();
    
    void dispHelp();
    void dispAbout();
    
    void ssMenu(int id);
    void trMenu(int id);
    void lfMenu(int id);
    
private:    
    QStringList listLF;
    QString resfilename;
    int jLF;
};

#endif // KANG_H
