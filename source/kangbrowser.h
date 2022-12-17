
/* 
  Kangoroo v1.0 Feb 2007 Jan Poland
  
  Licence: GNU GPL v2
  
  WARNING: This source code has been made available to the public, in order to
  increase the amount of available source code for the Sharp Zaurus, and in order
  to allow interested individuals to customize and improve the software.
  THE SOURCE CODE IS NEITHER WELL STRUCTURED NOR DOCUMENTED!

*/

#ifndef KANGBROWSER_H
#define KANGBROWSER_H

#include <qtextbrowser.h>
#include <qvaluelist.h>
#include <qfile.h>
#include <qregexp.h>
#include <qstringlist.h>

class Kangoroo;
class Kanjipic;

class KangBrowser : public QTextBrowser
{ 
    Q_OBJECT
    
    friend class KangLineEdit;
    friend class KangPushButton;

public:        
    
    KangBrowser( const QString &kdir, QWidget* parent = 0, const char* name = 0);
    ~KangBrowser();
    
    void setSource( const QString &s);
    
    void setKang( Kangoroo *p, Kanjipic *pp);
    
    bool readLearnfile( QString &fname);
    void writeLearnfile();
    
    void backward();
    void next();
    
    void dispHelp();
    void dispAbout();
    
    void displayText( QString out);
    void refreshDisplay( void);
    void calcLineLength( void);
    
    int kanjiWidth;
    bool bCorrectInit;
    
protected:
    void resizeEvent ( QResizeEvent *e);
    
private:
    int lineLength;    
    QChar ichi;
    Kangoroo* pKang;
    Kanjipic* pPic;
    char* pline;
    char* pzeile;    
    QFile dicfile, defile, kanafile, kanjifile, kdicfile, helpfile;
    QString learnfilename;
    QValueList<int> idz;
    QValueList<int> lfidz;
    int *pKeyC;
    int nkey;
    QStringList keystr;
    int lfcount[2];
    int dicsize, dicPsize;
    int deMS,kanaMS,kanjiMS,kdicMS,kindexMS,ksegMS;
    QString* history;
    QString curKseg;
    int curKcode;
    QChar curKanji;
    int histp, histplast;
    bool bMMalloc;
    int *pMde, *pMkana, *pMkanji, *pMkdic, *pMkindex, *pMkseg;
    int maxLL;
    QStringList blacklist;
    QRegExp indexRe;
    
    QString dispIdz();    
    QString dispLine(int idx, uint maxchar, const QString &rstr);
    void completeLine(QString &line, int idx, bool bP, const QString &rstr);
    QString dispFirstLine(int p, const QString &type, char c, QString q);
    
    void addLinks(QString &out, const QString &idxs);

    bool readMeta(const QString &fname, int *pM, int MS, int jap);
    int bsearchMeta(int *pM, int MS, int key);
            
    void getIdzIndexFile( const QString &qkey, char com, const QString &qmatch, const QString &qhelp);
    void getIdzLine(QString line, char com, const QString &qmatch, const QString &qhelp);
    void getIdzLF(int mask, const QString &q, char com, int pmax, const QString &qhelp);
    void getIdzFull(const QString &q, char com, const QString &qhelp);
    
    void postprocessIdz();

    bool matchLine(const QRegExp &re, const QRegExp &rehelp);
    
    bool searchBlacklist(const QRegExp &re);
    
    void updateUI();
    void addToHistory(QString &s);
    
    void showAnnotatedKanji();
    void showKeyDlg();
    
//     static const char* const sTypes[];

};

#endif // KANGBROWSER_H
