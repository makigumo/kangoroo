
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

#include <stdlib.h>

#include <qtl.h>

#include "kangbrowser.h"
#include "kangoroo.h"
#include "kanjipic.h"
#include "keydlg.h"

#define HISTLENGTH 10

#include "MersenneTwister.h"

static const QString STERN=QString::fromUtf8("\xEF\xBC\x8A");
static QString FRAGEZEICHEN=QString::fromUtf8("\xEF\xBC\x9F");
static QString BLANK=QString::fromUtf8("[\xE3\x80\x80-\xE3\x80\x82\xE3\x80\x88-\xE3\x80\x8F]");
static QString BLANKWO=QString::fromUtf8("[\xE3\x80\x80-\xE3\x80\x82\xE3\x80\x88-\xE3\x80\x8F\xE3\x82\x92]");
static QString WO=QString::fromUtf8("\xE3\x82\x92");

static MTRand rnd;

static char nkg[]="(no keyword given)";

static double fontMultiplyer[] = {1.0, 1.3, 1.6};

static char about[] = "<h2>\xE6\xBC\xA2\xE8\xAA\x9E\xE5\x8A\xB4 -- Kangoroo</h2>Japanese dictionary with Kanji browsing.<br>Type your query (with Kana, Kanji, or Romaji) and press enter.<br>Click on the Kanji to follow links (try this one: \xE6\xBC\xA2).<br>Kanji are segmented into building blocks in the upper left: click on the building blocks to follow links.<br><br><font color=#FF0000>Credits:</font><br>The Kangoroo software is only a small part of the whole! Many thanks to the authors and maintainers of the dictionaries, who have made this project possible:<br><font color=#0000FF>Jim Breen</font>, author of EDict (Japanese-English dictionary)<br><font color=#0000FF>Ulrich Apel</font>, author and coordinator of WaDoku (Japanese-German dictionary)<br>and all other contributors to the dictionaries!<br><br>Many thanks also to <br><font color=#0000FF>James Heisig</font> who proposed to segment Kanji in order to memorize them<br><font color=#0000FF>Anne Poland</font> who did the present segmentation.<br><br><br><hr>Kangoroo v1.0, 2007 Jan Poland";


KangBrowser::KangBrowser( const QString &kdir, QWidget* parent,  const char* name) : QTextBrowser( parent, name) {
  kanjiWidth=1;  
  ichi = QString::fromUtf8("\xe4\xb8\x80").at(0);  
  history = new QString[HISTLENGTH];
  histp=0; histplast=0;
  setTextFormat(RichText);
  setLinkUnderline(false);
  lineLength=26;
  int j;
  char pbuf[16];
  bMMalloc=false;
  curKcode=0;
  
  bCorrectInit = true;
  dicfile.setName(QString("dic.utf8").prepend(kdir));  
  bCorrectInit &= dicfile.open(IO_ReadOnly);
  dicfile.close();
  defile.setName(QString("de.idx").prepend(kdir));
  bCorrectInit &= defile.open(IO_ReadOnly);
  defile.close();
  kanafile.setName(QString("kana.idx").prepend(kdir));
  bCorrectInit &= kanafile.open(IO_ReadOnly);
  kanafile.close();
  kanjifile.setName(QString("kanji.idx").prepend(kdir));
  bCorrectInit &= kanjifile.open(IO_ReadOnly);
  kanjifile.close();
  kdicfile.setName(QString("kkdic.idx").prepend(kdir));
  bCorrectInit &= kdicfile.open(IO_ReadOnly);
  kdicfile.close();
  QFile f(QString("index.nfo").prepend(kdir));
  bCorrectInit &= f.open(IO_ReadOnly);  
  if (bCorrectInit) {
    f.readLine(pbuf,15); dicPsize = QString::fromLatin1(pbuf).toInt();
    f.readLine(pbuf,15); dicsize = QString::fromLatin1(pbuf).toInt();
    f.readLine(pbuf,15); deMS = QString::fromLatin1(pbuf).toInt();
    f.readLine(pbuf,15); kanaMS = QString::fromLatin1(pbuf).toInt();
    f.readLine(pbuf,15); kanjiMS = QString::fromLatin1(pbuf).toInt();
    f.readLine(pbuf,15); kdicMS = QString::fromLatin1(pbuf).toInt();
    f.readLine(pbuf,15); kindexMS = QString::fromLatin1(pbuf).toInt();
    f.readLine(pbuf,15); ksegMS = QString::fromLatin1(pbuf).toInt();
    f.readLine(pbuf,15);
    indexRe = QRegExp(QString::fromLatin1(pbuf).simplifyWhiteSpace().prepend("\\s").append("\\d+\\s"));
    maxLL=1020;
    while (f.readLine(pbuf,15)>0) {j=QString::fromLatin1(pbuf).toInt(); if (j>maxLL) maxLL=j;}
    maxLL+=4;
    f.close();
    
    bMMalloc=true;
    pline = new char[maxLL];
    pzeile = new char[maxLL];
    pMde = new int[deMS<<1];
    pMkana = new int[kanaMS<<1];
    pMkanji = new int[kanjiMS<<1];
    pMkdic = new int[kdicMS<<1];
    pMkindex = new int[kindexMS<<1];
    pMkseg = new int[ksegMS<<1];
    pKeyC = new int[ksegMS];
    nkey=0;
    bCorrectInit &= readMeta(QString("de.meta").prepend(kdir),pMde,deMS,0); 
    bCorrectInit &= readMeta(QString("kana.meta").prepend(kdir),pMkana,kanaMS,1); 
    bCorrectInit &= readMeta(QString("kanji.meta").prepend(kdir),pMkanji,kanjiMS,1);
    bCorrectInit &= readMeta(QString("kkdic.meta").prepend(kdir),pMkdic,kdicMS,1);
    bCorrectInit &= readMeta(QString("kindex.meta").prepend(kdir),pMkindex,kindexMS,-1);
    bCorrectInit &= readMeta(QString("kseg.meta").prepend(kdir),pMkseg,ksegMS,-1);
  } 
  helpfile.setName(QString("kangoroo.utf8").prepend(kdir));  
  
  f.setName(QString("blacklist.utf8").prepend(kdir));
  if (f.open(IO_ReadOnly)) {
    while (f.readLine(pline,maxLL)>0) blacklist.append(QString::fromUtf8(pline).stripWhiteSpace());
    f.close();
  }
}

KangBrowser::~KangBrowser() {
  writeLearnfile();
  delete []history;  
  if (bMMalloc) {
    delete []pKeyC;
    delete []pMde;
    delete []pMkana;
    delete []pMkanji;
    delete []pMkdic;
    delete []pMkindex;
    delete []pMkseg;
    delete []pline;
    delete []pzeile;
  }
}

bool KangBrowser::searchBlacklist(const QRegExp &re) {
  bool bFound=false;
  for ( QStringList::Iterator it=blacklist.begin(); it!=blacklist.end() && !bFound; it++ )
    bFound = (re.match(*it)>=0);
  return bFound;  
}

bool KangBrowser::readMeta(const QString &fname, int *pM, int MS, int jap) {
  int i,j;
  QString l;
  QFile f(fname);
  if (!f.open(IO_ReadOnly)) return false;
  for (i=0;i<MS;i++) {
    f.readLine(pline,maxLL);
    if (jap<0) {
      l=QString::fromLatin1(pline); j=l.find('\t');
      *(pM++)=l.left(j).toInt();
      *(pM++)=QString::fromLatin1(pline+j+1).toInt();
    } else {
      if (jap) *(pM++)=QString::fromUtf8(pline).at(0).unicode();
          else *(pM++)=(int)pline[0]*256+(int)pline[1];
      *(pM++)=QString::fromLatin1(pline+3+jap).toInt();
    }  
  }
  f.close();
  return true;
}

int KangBrowser::bsearchMeta(int *pM, int MS, int key) {
  int a=0, b=MS-1, c;
  if (pM[0]==key) return pM[1];
  if (pM[b<<1]==key) return pM[(b<<1)+1];
  while (b-a>1) {
    c=(b+a)>>1;
    if (pM[c<<1]==key) return pM[(c<<1)+1];
    if (pM[c<<1]>key) b=c; else a=c;
  }
  return -1;
}
    

void KangBrowser::resizeEvent ( QResizeEvent *e) { 
  QTextBrowser::resizeEvent(e);
  calcLineLength();
}

void KangBrowser::showAnnotatedKanji()
{
  QString out,nn;
  int kp, n=0;
  QChar k;
  for (kp=0;kp<nkey;kp++) {
    k=keystr[kp][0];
    if (k!='-') {
      if (n++>0) out.append(' ');
      out.append(k);
    }
  }
  addLinks(out,"");
  nn.setNum(n);
  out.prepend(" annotated Kanji:<br>\n").prepend(nn).prepend("#-#");
  displayText(out);
  addToHistory(out);
}

void KangBrowser::showKeyDlg()
{
  if (curKcode>0) {
    QString keys;
    int i;
    for (i=0;i<nkey;i++) if ((pKeyC[i]>>1)==curKcode) break;
    if (i<nkey) {keys=keystr[i]; keys.remove(0,1);}
    KeyDlg keyDlg(pKang,keys,(i<nkey)?(pKeyC[i]&1):1);
    keyDlg.exec();
    if(keyDlg.result()==QDialog::Accepted) {
      keys=keyDlg.le->text();
      int bCk=keyDlg.cb->isChecked();
      if (keys.isEmpty()) {
        if (i<nkey) {
          keystr.remove(keystr.at(i));
          nkey--;
          for (;i<nkey;i++) pKeyC[i]=pKeyC[i+1];
        } else return;
      } else {
        keys.prepend(curKanji);
        if (i>=nkey) {
          pKeyC[nkey++]=(curKcode<<1)+bCk;
          keystr.append(keys);
        } else {
          pKeyC[i]=(curKcode<<1)+bCk;
          keystr.insert(keystr.remove(keystr.at(i)),keys); // keystr[i]=keys;
        }
      }  
      if (histp>0) {
        i=history[histp-1].find("<a href=\"!\">");          
        if (i>=0) {
          i+=12;
          int j=history[histp-1].find("</a>",i);
          if (j>=0) {
            if (keys.isEmpty()) keys=nkg; else keys.remove(0,1).append('"').prepend('"');
            history[histp-1].replace(i,j-i,keys);
            displayText(history[histp-1]);
          }
        }
      }
    }
  }  
}

void KangBrowser::setSource( const QString &s)
{  
  char com=s.at(0).latin1();
  
  if (com=='A') {showAnnotatedKanji(); return; }
  if (com=='!') {showKeyDlg(); return; }
  
  curKcode=0;
  curKanji='-';
  
  displayText("searching..."); qApp->processEvents(); 
  dicfile.open(IO_ReadOnly);
  
  QString out, qKanji(""), rstr("");
  int idx, j, u;
  bool bKIdx=false;
  QValueListIterator<int> it;
  QString q = QString(s).remove(0,1);
  q.replace( QRegExp(STERN), "*");
  q.replace( QRegExp(FRAGEZEICHEN), "?");
  q.replace( QRegExp(BLANK), " ");
  for (j=0;j<(int)q.length();j++) {
    u=q[j].unicode();
    if (!((u>=(int)'0' && u<=(int)'9') || (u>=(int)'A' && u<=(int)'Z') ||
          (u>=(int)'a' && u<=(int)'z') || u>127 || u==(int)'*' || u==(int)'?')) q.replace(j,1," ");
  }        
  q = q.simplifyWhiteSpace();  
  if (com=='k' || com=='l' || com=='*' || com=='x' || com=='f' || com=='s' || com=='z' || com=='Z') {
    if (com=='k') {
      if (q[0].unicode()>=128) {qKanji=q.left(1); q.remove(0,1);} else bKIdx=true;
    } else if (q[0].latin1()=='r') {rstr.append('r'); q.remove(0,1);}
    if (com!='s' && com!='z' && com!='Z' && !bKIdx && QRegExp("\\d+").match(q)>=0) {
      idx=q.toInt();      
      dicfile.at(idx>>2); dicfile.readLine(pline,maxLL);
      if ((com=='l' && (idx&1)==0) || (com=='*' && (idx&2)==0)  || (com=='x' && (idx&3)>0)) {
        if (com=='x') out.append("Removed from memo ("); else out.append("Just memorized (");
        out.append(learnfilename.right(learnfilename.length()-learnfilename.findRev('/')-1));
        out.append("):<br>\n");
        int idxnew = (idx&(-4))+(com!='x')+(com=='*');
        bool bChanged=false;
        for (it=lfidz.begin();it!=lfidz.end() && (*it>>2)<=(idx>>2); it++) if ((*it>>2)==(idx>>2)) {
          lfcount[((*it)&3)-1]--;
          if ((idxnew&3)>0) { lfcount[(idxnew&3)-1]++; *it=idxnew; } 
            else lfidz.remove(it);
          bChanged=true;
          break;          
        }
        if (!bChanged && com!='x') {lfidz.insert(it,idxnew); lfcount[(idxnew&3)-1]++;}
        for (it=idz.begin();it!=idz.end(); it++) if ((*it>>2)==(idx>>2)) *it=idxnew;
        idx=idxnew;
      }
      out.append(dispLine(idx,9999,rstr));
      out.append("<hr>\n");
    }
    if (com=='k' || com=='s' || com=='z' || com=='Z') {
      if (com=='s' || com=='z' || com=='Z') idx=bsearchMeta(pMkseg,ksegMS,q.toInt()); else
      if (!qKanji.isEmpty()) idx=bsearchMeta(pMkdic,kdicMS,qKanji[0].unicode()); 
        else idx=bsearchMeta(pMkindex,kindexMS,q.toInt());
      if (idx>=0) {
        
        kdicfile.open(IO_ReadOnly);
        kdicfile.at(idx); kdicfile.readLine(pline,maxLL);        
        QString line(QString::fromUtf8(pline).simplifyWhiteSpace());
        kdicfile.readLine(pline,maxLL);
        QString idzline(QString::fromUtf8(pline).simplifyWhiteSpace());
        kdicfile.readLine(pline,maxLL);
        curKseg=QString(QString::fromUtf8(pline).simplifyWhiteSpace());
        kdicfile.readLine(pline,maxLL);
        QString kbb(QString::fromUtf8(pline).simplifyWhiteSpace());
        kdicfile.close();
        
        curKcode=pPic->setK(curKseg);
        
        if (com=='z' || com=='Z') {
          if (com=='z') out.append("mark removed"); else out.append("marked for training");
          out.append("<br>\n<hr>\n");
          for (j=0;j<nkey;j++) if ((pKeyC[j]>>1)==curKcode) {pKeyC[j]=(curKcode<<1)+(com=='Z'); break;}
        }        
        
        if (!kbb.isEmpty()) {
          kbb.prepend("Building block in: ");
          addLinks(kbb,"");
        }
        
        int kp;
        for (kp=0;kp<nkey;kp++) if ((pKeyC[kp]>>1)==curKcode) break;
        
        if (!line.isEmpty()) {
          qKanji=line.left(1);
          curKanji=line[0];
          j = line.find( indexRe );
          if (j>=0) {
            int i = j+3;
            while (line.at(i).latin1()!=' ') i++;
            line.insert(i,"</font>");
            line.remove(j+1,1);
            line.insert(j+1,"<font color=#FF0000>");
          }
          if (curKcode>0) {
            line.insert(2,"</a> ");
            if (kp>=nkey) line.insert(2,nkg); else {
              QString keys(keystr[kp]);
              keys.remove(0,1);
              line.insert(2,'"');line.insert(2,keys);line.insert(2,'"');
            }  
            line.insert(2,"<a href=\"!\">");
          }
          line.insert(2,"</font>");
          line.prepend("<font size=+2>");
          out.append(line);
          out.append("<hr>\n");
        } else {
          curKanji=QChar('-');
          kbb.prepend(")!<br>\n").prepend(curKseg.left(curKseg.find(' '))).prepend("<br>\nThis is no Kanji, but a building block (ref.no.");
          kbb.prepend("</a>");
          if (kp>=nkey) kbb.prepend(nkg); else {
            QString keys(keystr[kp]);
            keys.remove(0,1);
            kbb.prepend('"');kbb.prepend(keys);kbb.prepend('"');
          }  
          kbb.prepend("<a href=\"!\">");
        }
        
        if (!kbb.isEmpty() && pKang->sKanjiBBfirst) {
          out.append(kbb);
          if (!idzline.isEmpty()) out.append("<hr>\n");
        }
        
        idz.clear();
        
        if (!idzline.isEmpty()) {
          getIdzLine(idzline,com,"","");
          postprocessIdz();
        
          out.append(dispFirstLine(idz.count(),QString("Kanji"),'0',qKanji));
          out.append(dispIdz());
        }  
        
        if (!kbb.isEmpty() && !pKang->sKanjiBBfirst) {
          if (!idzline.isEmpty()) out.append("<hr>\n");
          out.append(kbb);          
        }
        
      } else {
        out.append("No such Kanji, sorry!<br>\n");
      }      
    } else {
      if (rstr.isEmpty()) out.append(dispIdz());
    }
  } else if (com=='t') {
    if (pKang->sTrain<3) {
      curKseg=QString(""); pPic->setK(curKseg);
      int lfc;
      if (pKang->sMemo==3) j = lfc=lfcount[0]+lfcount[1]; else lfc=lfcount[pKang->sMemo-1];
      if (lfc==0) out.append("No vocabulary to train!"); else {
        j = rnd.randInt(lfc-1)+1;
        for (it=lfidz.begin();it!=lfidz.end(); it++) if (*it&pKang->sMemo) if (--j==0) break;
        idx=*it;
        dicfile.at(idx>>2); dicfile.readLine(pline,maxLL);
        QString line(QString::fromUtf8(pline));
        QString idxs; idxs.setNum(idx);
        int psep = line.find('/');
        int jP = line.find("/(P)/");
        bool bP = false;
        if (jP>=0) {line = line.left(jP); bP=true; }
        line.replace( QRegExp("/\\s*$"), "");
        line.replace( QRegExp("<"), "&lt;");
        line.replace( QRegExp(">"), "&gt;");  
        if (pKang->sTrain<2) {
          line=line.left(psep);
          psep = line.find('[');
          if (psep>=0 && pKang->sTrain==0) line=line.left(psep); else
          if (psep>=0) {line.remove(0,psep+1); line.replace( QRegExp("\\]"), "");}
        } else line.remove(0,psep+1);
        
        line = line.simplifyWhiteSpace();
        completeLine(line, idx, bP, QString("r"));
        line.append(" ? <a href=\"fr").append(idxs).append("\">solution&gt;&gt;!</a>");
        out.append(line);
      }
    } else {
      int J=0;
      for (j=0;j<nkey;j++) 
        J+=((pKang->sKTrMarked==0 || (pKeyC[j]&1)) && (pKang->sKTrBB==1 || keystr[j][0]!='-'));
      if (J==0) {
        out.append("No Kanji to train!"); 
        curKseg=QString(""); pPic->setK(curKseg);
      } else {
        J = rnd.randInt(J-1)+1;
        for (j=0;j<nkey;j++) 
          if ((pKang->sKTrMarked==0 || (pKeyC[j]&1)) && (pKang->sKTrBB==1 || keystr[j][0]!='-')) 
            {J--; if (J==0) break;}
        QString idxs; idxs.setNum(pKeyC[j]>>1);
        if (pKang->sTrain==3 || (pKang->sTrain==4 && keystr[j][0]=='-')) {
          curKseg=idxs; pPic->setK(curKseg);
        } else {
          curKseg=QString(""); pPic->setK(curKseg);
          if (pKang->sTrain==4) {
            out.append("<font size=+2>").append(keystr[j][0]).append("</font> ");
          } else {
            QString keys(keystr[j]);
            keys.remove(0,1);            
            out.append(keys).append(' ');
          }  
        }  
        out.append("? <a href=\"s").append(idxs).append("\">solution&gt;&gt;!</a>");
        out.append("<br><hr><a href=\"");
        if (pKeyC[j]&1) out.append("z").append(idxs).append("\">remove mark</a>");
          else out.append("Z").append(idxs).append("\">add mark for training</a>");
      }  
    }
  } else {
    curKseg=QString(""); pPic->setK(curKseg);
    QString q1(q);   
    if (!q.isEmpty()) {
      q1.append(' ').prepend(' ');
      q1.replace( QRegExp(" "), " * ");
    }  
    
    int j, score=0, sc1, score2=0;
    bool bMatchWo=false;
    QString qqq(q), qhelp, qkc;
    qqq.replace( QRegExp("\\*"), " ");
    qqq.replace( QRegExp("\\?"), " ");
    qqq = qqq.simplifyWhiteSpace().append(' ');
    while ((j=qqq.find(' '))>=0) {
      qkc=qqq.left(j).lower(); sc1=qkc.length();
      if (qkc[0].unicode()>127) sc1<<=1;
      if (sc1>score2) {score2=sc1; qhelp=qkc;}
      qqq.remove(0,j+1);
    }
    
    if (com=='m') {
      idz.clear();
      getIdzLF(pKang->sMemo,q1,com,999,qhelp);
      out.append(dispFirstLine(idz.count(),QString("memo"),'0'+(char)pKang->sMemo,q));
      out.append(dispIdz());
    } else {
      QString qkey, qq(q);
      if (qq.length()>1 && qq.find(WO)>=0) {bMatchWo=true; qq.replace(QRegExp(WO)," ");}
      qq.append(' ');
      while ((j=qq.find(' '))>=0) {
        qkc=qq.left(j).lower();
        if (qkc[0].unicode()>127) sc1=qkc.length()<<1; else
        if (qkc.length()>=2 && qkc[0].latin1()!='*' && qkc[0].latin1()!='?' && 
            qkc[1].latin1()!='*' && qkc[1].latin1()!='?') sc1=qkc.length(); else sc1=0;
        if (sc1>0) {    
          qkc.replace( QRegExp("\\*"), ".*");
          qkc.replace( QRegExp("\\?"), ".?");
          qkc.prepend('^').append('$');
          if (qkc.find('*')<0 && qkc.find('?')<0) sc1+=1000;
          if (!searchBlacklist(QRegExp(qkc,false))) sc1+=2000;
        }  
        if (sc1>score) {score=sc1; qkey=qkc;}
        qq.remove(0,j+1);
      }
                  
      char ccom[2]={'a','c'}; 
      if (score>0) {
        if (q.find(' ')<0 && !bMatchWo) q1="";
        if (com=='e') com=ccom[0];
        idz.clear();
        qkey.remove(qkey.length()-1,1);qkey.append('\t');
        if (!q.isEmpty()) getIdzIndexFile(qkey,com,q1,qhelp);
        postprocessIdz();
        out.append(dispFirstLine(idz.count(),"index",com,q));
        out.append(dispIdz());
      } else {
        if (com=='e') com=ccom[pKang->sSearchFullDefault];
        idz.clear();
        getIdzLF(3,q1,com,pKang->maxResults,qhelp);
        getIdzFull(q1,com,qhelp);
        out.append(dispFirstLine(idz.count(),"full",com,q));
        out.append(dispIdz());
      }
    }  
  }  
  
  dicfile.close();
  
  displayText(out);
  addToHistory(out.prepend('#').prepend(curKseg).prepend(curKanji).prepend('#'));  
}

void KangBrowser::displayText( QString out) {
  QString s;
  if (out.latin1()[0]=='#') {
    int p=out.find('#',2);
    curKanji=out[1];
    curKseg=out.mid(2,p-2);
    out.remove(0,p+1);
    curKcode=pPic->setK(curKseg);
  }
  s.setNum(pKang->sFont);
  s.prepend("<font size=+").append(">");  
  out.prepend(s).append("</font>");
  setText(out);  
}

void KangBrowser::refreshDisplay() {
  calcLineLength();
  if (histp>0) displayText(history[histp-1]);
}

void KangBrowser::calcLineLength() {
  lineLength=(int)((double)width()/(double)kanjiWidth/fontMultiplyer[pKang->sFont])-5;
}

void KangBrowser::setKang( Kangoroo* p, Kanjipic* pp) {pKang=p; pPic=pp;}

void KangBrowser::getIdzIndexFile( const QString &qkey, char com, const QString &qmatch, const QString &qhelp) { 
  QRegExp re = QRegExp(qkey,false);
  uint i;
  int idx, MS=kanaMS, key;
  int *pM=pMkana;
  bool bSearching=true, bJap=true, bWildcard=(qkey.find('*')>=0 || qkey.find('?')>=0);
  QFile* pf=&kanafile;
  QString line;  
  
  if (qkey[1].latin1()) {
    pM = pMde; MS=deMS;
    pf=&defile;
    bJap=false;
    key = (int)qkey[1].latin1()*256+(int)qkey[2].latin1();
  } else {
    for (i=1;i<qkey.length()-1;i++) if (qkey[i]>=ichi) {pM = pMkanji; MS=kanjiMS; pf=&kanjifile;}
    key=qkey[1].unicode();
  }  
  idx = bsearchMeta(pM,MS,key);
  if (idx>=0) {
    pf->open(IO_ReadOnly);
    pf->at(idx);
    while (pf->readLine(pline,maxLL)>0 && bSearching) {
      line = QString::fromUtf8(pline);
      if ( (!bJap && key!=((int)line[0].latin1()*256+(int)line[1].latin1())) ||
            (bJap && key!=line[0].unicode()) ) bSearching=false; else
      if (re.match(line)>=0) {
        bSearching=bWildcard;
        line.remove(0,line.find('\t')+1);
        getIdzLine(line,com,qmatch,qhelp);
      }  
    }    
    pf->close();
  }  
}

void KangBrowser::getIdzLine(QString line, char com, const QString &qmatch, const QString &qhelp) {
  QValueListIterator<int> it=lfidz.begin();
  int idx;
  uint i;
  QRegExp re(qmatch,false,true), rehelp(qhelp,false);
  line = line.simplifyWhiteSpace().append(" ");
  
  for (;line.length()>0 && (i=line.find(QRegExp("\\s")));line.remove(0,i+1)) {
    idx = line.left(i).toInt();
    if (com=='c' && idx>=dicPsize) break;
    idx<<=2;
    
    if (!qmatch.isEmpty()) {
      dicfile.at((idx)>>2); dicfile.readLine(pline,maxLL);
      if (!matchLine(re,rehelp)) continue;
    }
    
    while (it!=lfidz.end() && *it<idx) it++;
    if (it!=lfidz.end() && *it<idx+4) idx=*it;
    idz.append(idx);
  }
}

void KangBrowser::getIdzLF(int mask, const QString &q, char com, int pmax, const QString &qhelp) {  
  QValueListIterator<int> it;
  QRegExp re(q,false,true), rehelp(qhelp,false);
  int p = idz.count();
  it=lfidz.begin();
  for (it=lfidz.begin();it!=lfidz.end() && p<pmax && (com!='c' || ((*it)>>2)<dicPsize); it++) {
    if ((*it)&mask) {
      if (q.isEmpty()) {idz.append(*it); p++;} else {
        dicfile.at((*it)>>2); dicfile.readLine(pline,maxLL);
        if (matchLine(re,rehelp)) {idz.append(*it); p++;}
      }
    }  
  }
}

void KangBrowser::getIdzFull(const QString &q, char com, const QString &qhelp) {  
  QValueListIterator<int> it=lfidz.begin();
  QRegExp re(q,false,true), rehelp(qhelp,false);
  int idx=0, p = idz.count(), pgoal=1, pgoalidx=(com=='c'?dicPsize:dicsize)/20;  
  dicfile.at(0);
  QString s;
  displayText("searching... 0%"); qApp->processEvents();
  while (p<pKang->maxResults && dicfile.readLine(pline,maxLL)>0 ) {
    if ((idx>>2)>=pgoalidx) {
      s.sprintf("searching... %i%%",pgoal*5); displayText(s); qApp->processEvents();
      pgoalidx=(com=='c'?dicPsize:dicsize)*(++pgoal)/20;
    }
    if (com=='c' && (idx>>2)>=dicPsize) break;
    while (it!=lfidz.end() && *it<idx) it++;
    if (!(it!=lfidz.end() && *it<idx+4) && matchLine(re,rehelp)) {idz.append(idx); p++;}
    idx=(dicfile.at()<<2); 
  }
}

void KangBrowser::postprocessIdz() {
  QValueListIterator<int> it;
  int c=0,preidx=-1;
  int minint=(int)((((uint)-1)>>1)+1);
  for (it=idz.begin();it!=idz.end(); it++) if (*it&3) *it+=minint;
  qHeapSort(idz);  
  it=idz.begin();
  while (it!=idz.end() && c<pKang->maxResults) {
    if (*it<0) *it-=minint;
    if ((*it>>2)==(preidx>>2)) it=idz.remove(it); else {
      preidx=*it;
      it++;c++;
    }
  }  
  while (it!=idz.end()) it=idz.remove(it);
}

bool KangBrowser::matchLine(const QRegExp &re, const QRegExp &rehelp) {
  int p, pp=0;
  bool bSpace=true;
  char c;
  pzeile[pp++]=' ';pzeile[pp++]=' ';
  for (p=0; (c=pline[p]); p++) {
    if (c==(char)0xe3 && pline[p+1]==(char)0x80 && 
          (((unsigned char)pline[p+2]>=(unsigned char)0x80 && pline[p+2]<=(char)0x82) ||
           (pline[p+2]>=(char)0x88 && pline[p+2]<=(char)0x8F))) {
      if (!bSpace) {pzeile[pp++]=' ';pzeile[pp++]=' '; bSpace=true;}
      p+=2;
    } else
    if ((c>0 && c<'0') || (c>'9' && c<'A') || (c>'Z' && c<'a') || (c>'z' && c<127)) {
      if (!bSpace) {pzeile[pp++]=' ';pzeile[pp++]=' '; bSpace=true;}
    } else {
      pzeile[pp++]=c;
      bSpace=false;
    }
  }
  pzeile[pp]=0;
  if (QString::fromUtf8(pzeile).find(rehelp)<0) return false;
  return (QString::fromUtf8(pzeile).find(re)>=0);
} 

QString KangBrowser::dispIdz() {
  QString out;
  QValueListIterator<int> it;
  for (it=idz.begin();it!=idz.end(); it++) {
    dicfile.at((*it)>>2); dicfile.readLine(pline,maxLL);
    out.append(dispLine(*it,idz.count()==1?9999:lineLength,QString("")));
  }
  return out;
}

QString KangBrowser::dispLine(int idx, uint maxchar, const QString &rstr) {
  QString line(QString::fromUtf8(pline));
  QString idxs;
  idxs.setNum(idx);
  int psep = line.find('/');
  int j, japlength;
  int jP = line.find("/(P)/");
  bool bP = false, bCut=false;
  if (jP>=0) {line = line.left(jP); bP=true; }
  line.replace( QRegExp("/\\s*$"), "");
  line = line.simplifyWhiteSpace();
  
  QString jap = line.left(psep).simplifyWhiteSpace();
  japlength = jap.length();
  if (jap.find('[')>=0) japlength--;
  line.remove(0,psep+1);
  
  if ((line.length()>>1)+japlength>maxchar) {
    bCut=true;
    int brackpos=jap.find('(');    
    if (brackpos>=0) {
      int kpos=jap.find(curKanji);
      if (curKanji<ichi || kpos<brackpos)
        jap.replace( QRegExp("\\s*\\([^\\)]*\\)"), "");
      else {
        int kpos1=kpos; 
        while (jap[kpos1]>0 && jap[kpos1]!=' ' && jap[kpos1]!=','  && jap[kpos1]!=')') kpos1++;
        while (kpos>=0 && jap[kpos]!=' ' && jap[kpos]!=','  && jap[kpos]!='(') kpos--;
        kpos++;
        QString repstr=jap.mid(kpos,kpos1-kpos); repstr.prepend('/');
        jap.replace( QRegExp("\\s*\\([^\\)]*\\)"), repstr);
      }  
    }  
    japlength = jap.length();
    if (jap.find('[')>=0) japlength--;
    line.replace( QRegExp("\\([^\\)]*\\)"), "");
    line.replace( QRegExp("\\[[^\\]]*\\]"), "");
    line.replace( QRegExp("\\{[^\\}]*\\}"), "");
    line.replace( QRegExp("\\.\\.+"), " ");
    line.replace( QRegExp("\\."), "/");
    line.replace( QRegExp("/\\s+"), "/");
    line.replace( QRegExp("\\s+/"), "/");
    line.replace( QRegExp("/+"), "/");
    line = line.simplifyWhiteSpace();
    line.replace( QRegExp("^/"), "");
    line.replace( QRegExp("/$"), "");
    if ((line.length()>>1)+japlength>maxchar-1) {
      j=(maxchar-1-japlength)<<1;
      line = line.left(j>=0?j:0);
      j = line.findRev('/');
      if (j>=0) line = line.left(j);
    }    
  } else if (maxchar==9999) {
    line.replace( QRegExp("/\\s+"), "/");
    line.replace( QRegExp("\\s+/"), "/");
    line.replace( QRegExp("^/"), "");
    line.replace( QRegExp("/$"), "");
    line.replace( QRegExp("/"), " / ");
  }
  
  line.replace( QRegExp("<"), "&lt;");
  line.replace( QRegExp(">"), "&gt;");  
  if (bCut) line.append("<a href=\"f").append(rstr).append(idxs).append("\">&gt;&gt;</a>");
  
  j = jap.find('[');
  if (j>=0) {
    jap.remove(j,1);
    if (bP) jap.insert(j,"<font color=#906090>"); else jap.insert(j,"<font color=#808080>");
    j = jap.find(']');
    if (j>=0) {jap.remove(j,1); jap.insert(j,"</font>");} else jap.append("</font>");
    addLinks(jap,idxs);    
  } else {
    if (bP) jap.prepend("<font color=#906090>"); else jap.prepend("<font color=#808080>");
    jap.append("</font>");
  }    
  
  completeLine(line, idx, bP, rstr);
  
  jap.append(" ");
  line.prepend(jap).append("<br>\n");
  return line;
}

void KangBrowser::completeLine(QString &line, int idx, bool bP, const QString &rstr) {
  QString idxs;
  idxs.setNum(idx);
  if (idx&2) line.prepend("<font color=#C08000>");  else
  if (idx&1) line.prepend("<font color=#FF0000>"); else
  if (bP) line.prepend("<font color=#00FF00>"); 
  if ((idx&2) || (idx&1) || bP) {
    line.append("</font>");
    line.replace( QRegExp("\\[D\\]"), "<font color=#000000>[D]</font>");
    line.replace( QRegExp("\\[E\\]"), "<font color=#000000>[E]</font>");
  } else {
    line.replace( QRegExp("\\[D\\]"), "<font color=#00FF00>[D]</font>");
    line.replace( QRegExp("\\[E\\]"), "<font color=#00FF00>[E]</font>");
  }
  
  line.append(" |");
  if ((idx&1)==0) line.append("<a href=\"l").append(rstr).append(idxs).append("\">").append( QString::fromUtf8("\xEF\xBC\xAD")).append("</a>|");
  if ((idx&2)==0) line.append("<a href=\"*").append(rstr).append(idxs).append("\">").append( QString::fromUtf8("\xEF\xBC\x8A")).append("</a>|");  
  if ((idx&3)!=0) line.append("<a href=\"x").append(rstr).append(idxs).append("\">").append( QString::fromUtf8("\xEF\xBC\xB8")).append("</a>|");  
}

QString KangBrowser::dispFirstLine(int p, const QString &type, char c, QString q) {
  QString out;
  out.setNum(p); if (p==pKang->maxResults || p==999) out.append("+");  
  out.append(" matches (").append(type);
  switch (c) {
    case 'c': out.append(", common"); break;
    case 'a': out.append(", all"); break;
    case '1': out.append(": without *"); break;
    case '2': out.append(": with *"); break;
  }  
  out.append(") for \"").append(q).append("\"<br>\n");
  return out;
}  

bool KangBrowser::readLearnfile( QString &fname) {
  learnfilename = fname;
  QFile f(fname);
  int j, star, k;
  bool bImport=false;
  QValueListIterator<int> i,ii;
  lfidz.clear();
  lfcount[0]=0; lfcount[1]=0;
  if (f.open(IO_ReadOnly)) {
    if (f.readLine(pline,maxLL)>0) {
      bImport=true;
      if (strncmp(pline,"Kangoroo v",10)==0) {
        *pline=0;
        j=10; while (pline[j] && pline[j]!=' ') j++;
        pline[j++]=0;
        float ver=atof(pline+10);
        if (strncmp(pline+j,"learn file ",11)==0 && ver>=1.0) {
          int len=atoi(pline+j+11);
          if (len==dicsize) bImport=false;
        }  
      }
    }
    if (bImport) {
    
      displayText("importing from previous learn file, please wait..."); qApp->processEvents(); 
  
      int idx, key, imp=0;
      bool bKanji, bSearching, bFound;
      QFile *pf;
      QString qstr, qkey, qstr0, out;
      kanafile.open(IO_ReadOnly);
      kanjifile.open(IO_ReadOnly);
      dicfile.open(IO_ReadOnly);
      QString line, lll;
      QRegExp re,reidx;
      
      while (1) {
        bFound=false;
        lll=QString::fromUtf8(pline);
        star = (lll.find( '*')>=0);
        qstr=lll.replace(QRegExp("\\([^\\(]\\)"),"");
        j=qstr.find('/');
        if (j>=0) qstr.remove(j,9999);
        qstr0=qstr.simplifyWhiteSpace();
        j=qstr.find(']');
        if (j>=0) qstr.remove(j,9999);
        qstr.replace(QRegExp("^\\.+"),"");qstr.replace(QRegExp("\\[\\.+"),"[");
        qstr.replace(QRegExp("\\.+$"),"");qstr.replace(QRegExp("\\.+\\["),"[");
        qstr.replace(QRegExp("\\.+\\s")," ");
        qstr.replace(QRegExp("\\."),"\\.");
        j=qstr.find('[');
        if (j>=0) {
          qkey=qstr.left(j).simplifyWhiteSpace();
          qstr.replace(QRegExp("\\s+\\["),"\\s[^\\/\\[]*\\[");
          qstr.prepend("^[^\\/\\[]*\\s").append("\\]\\s");
        } else if (!qstr.isEmpty()) {
          qstr=qstr.simplifyWhiteSpace();
          qkey=qstr;
          qstr.prepend("^\\s").append("\\s");
        }
        qkey=qkey.replace( QRegExp(BLANKWO), " ").simplifyWhiteSpace();
        j=qkey.find(' ');
        if (j>=0) qkey=qkey.left(j);        
        
        if (!qstr.isEmpty() && !qkey.isEmpty()) {
          //printf("%s *** %s\n",qkey.utf8().data(),qstr.utf8().data());
          qkey.prepend('^').append('\t');
          reidx=QRegExp(qkey);
          re=QRegExp(qstr);
          
          bKanji=false; pf=&kanafile;
          
          for (j=1;j<(int)qkey.length()-1;j++) if (qkey[j]>=ichi) {pf=&kanjifile; bKanji=true; break;}
          key=qkey[1].unicode();
          if (bKanji) idx=bsearchMeta(pMkanji,kanjiMS,key); else idx=bsearchMeta(pMkana,kanaMS,key);
        
          if (idx>=0) {
            pf->at(idx);
            bSearching=true;
            while (pf->readLine(pline,maxLL)>0 && bSearching) {
              line = QString::fromUtf8(pline);
              if ( key!=line[0].unicode() ) bSearching=false; else
              if (reidx.match(line)>=0) {                
                line.remove(0,line.find('\t')+1);
                pline[0]=' ';
                for (;line.length()>0 && (j=line.find(QRegExp("\\s"))) && bSearching;line.remove(0,j+1)) {
                  idx = line.left(j).toInt();
                  dicfile.at(idx); dicfile.readLine(pline+1,maxLL-1);
                  lll=QString::fromUtf8(pline);
                  lll.replace(QRegExp("[\\(\\),\\/]")," ");
                  lll.replace(QRegExp("\\s\\.+")," ");lll.replace(QRegExp("\\[\\.+"),"[");
                  lll.replace(QRegExp("\\.+\\/"),"/");lll.replace(QRegExp("\\.+\\["),"[");
                  lll.replace(QRegExp("\\.+\\s")," ");lll.replace(QRegExp("\\.+\\]"),"]");
                  if (re.match(lll)>=0) {
                    bSearching=false; bFound=true; imp++; lfidz.append((idx<<2)+1+star);
                  }
                }
                bSearching=false;
              }  
            }    
          }
        } 
        
        if (!bFound && !qstr0.isEmpty()) out.prepend("</a><br>\n").prepend(qstr0).prepend("\">").prepend(qstr0).prepend("<a href=\"e");
        if (f.readLine(pline,maxLL)<=0) break;
      }
      
      kanafile.close();
      kanjifile.close();
      dicfile.close();
      
      if (!out.isEmpty()) out.prepend("Couldn't automatically match the following items:<br>\n");
      qstr0.setNum(imp);
      out.prepend(" items successfully imported from previous learn file.</font><br>\n").prepend(qstr0).prepend("#-#<font color=#0000FF>");
      displayText(out);
      addToHistory(out);
      
    } else {
      while (f.readLine(pline,maxLL)>0) {
        QString line(QString::fromUtf8(pline));
        j = line.find( '/');
        if (j>=0) {
          line.remove(0,j+1);
          star = (line.find( '*')>=0);
          j = line.find( '/');
          if (j>=0) {
            lfidz.append( (line.remove(0,j+1).stripWhiteSpace().toInt()<<2)+1+star);
            lfcount[star]++;
          }  
        }
      }
    }  
    f.close();
    qHeapSort( lfidz);
    for (i=lfidz.begin(),i++;i!=lfidz.end();) {
      ii = i; ii--; if ((*ii>>2)==(*i>>2)) i=lfidz.remove(i); else i++;
    }
  }
  
  keystr.clear();
  nkey=0;
  QString keyfilename(learnfilename);
  keyfilename.append(".kk");
  f.setName(keyfilename);
  if (f.open(IO_ReadOnly)) {
    f.readLine(pline,maxLL);
    while (f.readLine(pline,maxLL)>0) {
      pline[4]=0;
      k=atoi(pline);
      if (k>0) {
        pKeyC[nkey++]=k;
        pline[4+strlen(pline+5)]=0;
        keystr.append(QString::fromUtf8(pline+5));
      }  
    }  
    f.close();    
  }
  
  return bImport; 
}

void KangBrowser::writeLearnfile() {
  if (learnfilename.isEmpty()) return;
  QFile f(learnfilename);
  int pd, p=0, idx, i;
  bool bCp=true;
  QCString idxs;
  QValueListIterator<int> it;
  if (f.open(IO_WriteOnly)) {
    sprintf(pzeile,"Kangoroo v1.0 learn file %i\n",dicsize);
    f.writeBlock(pzeile,strlen(pzeile));
    dicfile.open(IO_ReadOnly);
    for (it=lfidz.begin();it!=lfidz.end(); it++) {
      idx=(*it)>>2; idxs.setNum(idx);
      dicfile.at(idx); dicfile.readLine(pline,maxLL);
      for (pd=0,p=0; pline[p] && pline[p]!='/'; p++) {
        if (pline[p]=='(') bCp=false;
        if (bCp) pzeile[pd++]=pline[p];
        if (pline[p]==')') bCp=true;
      }
      p=pd;
      if (p>=0) {                              // always true!
        pzeile[p++]='/'; pzeile[p++]=' ';
        if (*it&2) pzeile[p++]='*';
        pzeile[p++]=' '; pzeile[p++]='/'; pzeile[p++]=' ';
        memmove(pzeile+p,idxs.data(),idxs.length());
        p+=idxs.length();
        pzeile[p++]='\n';
        f.writeBlock(pzeile,p);
      }  
    }
    dicfile.close();
    f.close();
  }
  
  QString keyfilename(learnfilename);
  keyfilename.append(".kk");
  f.setName(keyfilename);
  if (f.open(IO_WriteOnly)) {
    sprintf(pzeile,"Kangoroo v1.0 keyword list\n");
    f.writeBlock(pzeile,strlen(pzeile));
    for (i=0;i<nkey;i++) {
      sprintf(pzeile,"%.4i ",pKeyC[i]);
      memmove(pzeile+5,keystr[i].utf8(),keystr[i].utf8().length());
      p=5+keystr[i].utf8().length();
      pzeile[p++]='\n';
      f.writeBlock(pzeile,p);
    }
    f.close();
  }
}

void KangBrowser::addToHistory(QString &s) {
  if (histp>=HISTLENGTH) {
    for (int j=0;j<HISTLENGTH-1;j++) history[j]=history[j+1];
    histp--;
  }
  history[histp++]=s;
  histplast=histp;  
  updateUI();
}  

void KangBrowser::backward() {
  if (histp<2) return;
  displayText(history[(--histp)-1]);
  updateUI();
}

void KangBrowser::next() {
  if (histplast<=histp) return;
  displayText(history[histp++]);
  updateUI();
}

void KangBrowser::updateUI() {
  pKang->enableBackButton(histp>=2);
  pKang->enableNextButton(histplast>histp);
  pKang->setQueryFocus();
}

void KangBrowser::addLinks(QString &out, const QString &idxs) {
  int j;
  for (j=out.length()-1;j>=0;j--) if (out[j]>=ichi) {
    out.insert(j+1,"</a>");
    out.insert(j,QString("<a href=\"k").append(out[j]).append(idxs).append("\">"));
  }
}

void KangBrowser::dispAbout() {
  QString out(QString::fromUtf8(about));
  addLinks(out,"");
  out.prepend("#-#");
  displayText(out);
  addToHistory(out);
}

void KangBrowser::dispHelp() {  
  QString out,l;
  if (helpfile.open(IO_ReadOnly)) {
    if (helpfile.readLine(pline,maxLL)>0)
      out.append("<h2>").append(QString::fromUtf8(pline)).append("</h2>");
    helpfile.readLine(pline,maxLL);  
    while (helpfile.readLine(pline,maxLL)>0) {
      l=QString::fromUtf8(pline).stripWhiteSpace();
      if (l[l.length()-1].latin1()==':') l.prepend("<font color=#FF0000>").append("</font>");
      out.append(l).append("<br>\n");
    }
    helpfile.close();
    out.replace( QRegExp( " \""), " <font color=#008000>;");
    out.replace( QRegExp( "\\(\""), "(<font color=#008000>;");
    out.replace( QRegExp( "\""), ";</font>");
    out.replace( QRegExp( ";"), "\"");
    addLinks(out,"");
  } else out="Cannot read help file kangoroo.utf8";
  out.prepend("#-#");
  displayText(out);
  addToHistory(out);
}
