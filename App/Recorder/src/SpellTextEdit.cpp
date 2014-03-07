/*Owner & Copyrights: Vance King Saxbe. A.*//* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*/
#include "SpellTextEdit.h"
#include <QTextCursor>
#include <QTextBlock>
#include <QMenu>
#include <QContextMenuEvent>
#include <QFileInfo>
#include <QTextCodec>
#include <QSettings>
#include <QTextStream>
#include <iostream>
#include <QMessageBox>
#include <QFile>
#include <QPushButton>
#include <QMessageBox>
#include <QScrollBar>
#include <QDebug>


SpellTextEdit::SpellTextEdit(QWidget *parent,QString SpellDic)
: QTextEdit(parent)
{
	createActions();
	//this->setWindowTitle(QApplication::translate(tr("Evidencer"), tr("Spellcheck"), 0, QApplication::UnicodeUTF8));
	QScrollBar *bar = this->verticalScrollBar();
	bar->setValue(bar->maximum());
	// create misspell actions in context menu
	spell_dic=SpellDic.left(SpellDic.length()-4);
	pChecker =new Hunspell(spell_dic.toLatin1()+".aff",spell_dic.toLatin1()+".dic");

	QFileInfo fi(SpellDic);//Read the dictionary for spell check
	if (!(fi.exists() && fi.isReadable())){
		delete pChecker;
		pChecker=0;
	}
	// get user config dictionary
	/*QSettings setting;
	QString filePath=QFileInfo(setting.fileName()).absoluteFilePath();
	filePath=filePath+"/User_"+QFileInfo(spell_dic.toLatin1()+".dic").fileName();
	std::cout << qPrintable(filePath) << std::endl;
	fi=QFileInfo(filePath);
	if (fi.exists() && fi.isReadable()){
		pChecker->add_dic(filePath.toLatin1());
	}
	else filePath="";*/
	this->clear();
	addedWords.clear();

 	this->setFont(QFont("Arial",11));
	this->viewport()->setFont(QFont("Arial",11));
	this->setCurrentFont(QFont("Arial",11));
	this->setFontFamily("Arial");
	this->setFontPointSize(11);
	this->document()->setDefaultFont(QFont("Arial",11));
}

void SpellTextEdit::keyPressEvent(QKeyEvent *e)
{
	if (e->key()== Qt::Key_Enter ||  e->key()== Qt::Key_Return)
	{
		e->ignore();
		return;
	}
	else
	{
		QTextEdit::keyPressEvent(e);
	}
}
void SpellTextEdit::CursorPositionChanged()
{
	if(this->textCursor().block().position()<curPosition)
	{
		
		this->setReadOnly(true);
	}
	else if(this->textCursor().currentTable()!=0)
	{
		this->setReadOnly(false);
	}
	else
	{
		this->setReadOnly(true);
	return;
	}
}

/*Function for apply spell */



// //code repayed by midhun on 9th mar 2011 ,remove this comment if not working... :)
// void SpellTextEdit::Spellapply()
// {
// 	QString text=this->toPlainText (); 
// 	QStringList list;
// 	list=text.split("\n");
// 	SpeakerColumn=list[3];
// 	NoteColumn=list[4];
// 	emit applySpellCheck(SpeakerColumn,NoteColumn);
// 	SpeakerColumn.clear();
// 	NoteColumn.clear();
// }



/*Destructor*/

SpellTextEdit::~SpellTextEdit()
{

	if(pChecker)delete pChecker;
	// write user dictionary
// 	QSettings setting;
// 	QString fileName=QFileInfo(setting.fileName()).absoluteFilePath();
// 	fileName=fileName+"/User_"+QFileInfo(spell_dic.toLatin1()+".dic").fileName();
// 	std::cout << qPrintable(fileName) << std::endl;
// 	QFile file(fileName);
// 	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
// 	{
// 	     QTextStream in(&file);
// 	     in.readLine();
// 	     while (!in.atEnd()) {
// 	         QString line = in.readLine();
// 	         if(!addedWords.contains(line)) addedWords << line;
// 	     }
// 	     file.close();
// 	}
// 	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
// 	{
// 		std::cout << "write" << std::endl;
// 
// 		QTextStream out(&file);
// 		QByteArray encodedString;
// 		QString spell_encoding=QString(pChecker->get_dic_encoding());
// 		QTextCodec *codec = QTextCodec::codecForName(spell_encoding.toLatin1());
// 		out << addedWords.count() << "\n";
// 		foreach(QString elem, addedWords){
// 			encodedString = codec->fromUnicode(elem);
// 			out << encodedString.data() << "\n";
// 			std::cout << encodedString.data() << std::endl;
// 		}
// 	}
}

/*Function for set the Dictionary*/

bool SpellTextEdit::setDict(const QString SpellDic)
{

		if(SpellDic!=""){
			//mWords.clear();
			spell_dic=SpellDic.left(SpellDic.length()-4);
			delete pChecker;
			pChecker =new  Hunspell(spell_dic.toLatin1()+".aff",spell_dic.toLatin1()+".dic");
		}
		else spell_dic="";

		QFileInfo fi(SpellDic);
		if (!(fi.exists() && fi.isReadable())){
				delete pChecker;
				pChecker=0;
			}

		// get user config dictionary
		QSettings setting;
		QString filePath=QFileInfo(setting.fileName()).absoluteFilePath();
		filePath=filePath+"/User_"+QFileInfo(spell_dic.toLatin1()+".dic").fileName();
/*		std::cout << qPrintable(filePath) << std::endl;*/
		fi=QFileInfo(filePath);
		if (fi.exists() && fi.isReadable()){
			pChecker->add_dic(filePath.toLatin1());
		}
		else filePath="";
		if(spell_dic!="")
		{
				highlighter = new Highlighter(document(),SpellDic,true);
		}
		return (spell_dic!="");
}

/*Function for create actions*/

void SpellTextEdit::createActions() {
	for (int i = 0; i < MaxWords; ++i) {
		misspelledWordsActs[i] = new QAction(this);
		misspelledWordsActs[i]->setVisible(false);
		connect(misspelledWordsActs[i], SIGNAL(triggered()), this, SLOT(correctWord()));
	}
}

/*Function for get the corrent word*/

void SpellTextEdit::correctWord() {
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
	{
		QString replacement = action->text();
		QTextCursor cursor = cursorForPosition(lastPos);
		//QTextCursor cursor = textCursor();
		QString zeile = cursor.block().text();
		cursor.select(QTextCursor::WordUnderCursor);
		cursor.deleteChar();
		cursor.insertText(replacement);
	}
}

/*Function for populate correct values in to the context menu event*/

void SpellTextEdit::contextMenuEvent(QContextMenuEvent *event)
{
     QMenu *menu = createStandardContextMenu();
     lastPos=event->pos();
     QTextCursor cursor = cursorForPosition(lastPos);
     QString zeile = cursor.block().text();
     int pos = cursor.columnNumber();
     int end = zeile.indexOf(QRegExp("\\W+"),pos);
     int begin = zeile.lastIndexOf(QRegExp("\\W+"),pos);
     zeile=zeile.mid(begin+1,end-begin-1);
     QStringList liste = getWordPropositions(zeile);
     if (!liste.isEmpty())
     {
	     menu->addSeparator();
	  //   QAction *a;
// 	     a = menu->addAction(tr("Add .."), this, SLOT(slot_addWord()));
// 	     a = menu->addAction(tr("Ignore .."), this, SLOT(slot_ignoreWord()));
	     for (int i = 0; i < qMin(int(MaxWords),liste.size()); ++i) {
	    	    misspelledWordsActs[i]->setText(liste.at(i).trimmed());
	    	    misspelledWordsActs[i]->setVisible(true);
	     		menu->addAction(misspelledWordsActs[i]);
	     }

     } // if  misspelled
     menu->exec(event->globalPos());
     delete menu;
}

/*Function to get word proposition*/

QStringList SpellTextEdit::getWordPropositions(const QString word)
 {
	 QStringList wordList;
	 if(pChecker){
		 QByteArray encodedString;
		 QString spell_encoding=QString(pChecker->get_dic_encoding());
		 QTextCodec *codec = QTextCodec::codecForName(spell_encoding.toLatin1());
		 encodedString = codec->fromUnicode(word);
		 bool check = pChecker->spell(encodedString.data());
		 if(!check){
			 char ** wlst;
			 int ns = pChecker->suggest(&wlst,encodedString.data());
			 if (ns > 0)
			 {
				 for (int i=0; i < ns; i++)
				 {
					 wordList.append(codec->toUnicode(wlst[i]));
					 //free(wlst[i]);
				 }
				 //free(wlst);
				 pChecker->free_list(&wlst, ns);
			 }// if ns >0
		 }// if check
	 }
	 return wordList;
 }

/*Function to add word */

void SpellTextEdit::slot_addWord()
{
    QTextCursor cursor = cursorForPosition(lastPos);
    QString zeile = cursor.block().text();
    int pos = cursor.columnNumber();
    int end = zeile.indexOf(QRegExp("\\W+"),pos);
    int begin = zeile.left(pos).lastIndexOf(QRegExp("\\W+"),pos);
    zeile=zeile.mid(begin+1,end-begin-1);
    std::cout << qPrintable(zeile) << std::endl;
    QByteArray encodedString;
    QString spell_encoding=QString(pChecker->get_dic_encoding());
    QTextCodec *codec = QTextCodec::codecForName(spell_encoding.toLatin1());
    encodedString = codec->fromUnicode(zeile);
    pChecker->add(encodedString.data());
    addedWords.append(zeile);
    emit addWord(zeile);
}

/*Function to ignore word */

void SpellTextEdit::slot_ignoreWord()
{
	QTextCursor cursor = cursorForPosition(lastPos);
	QString zeile = cursor.block().text();
	int pos = cursor.columnNumber();
	int end = zeile.indexOf(QRegExp("\\W+"),pos);
	int begin = zeile.left(pos).lastIndexOf(QRegExp("\\W+"),pos);
	zeile=zeile.mid(begin+1,end-begin-1);
	QByteArray encodedString;
	QString spell_encoding=QString(pChecker->get_dic_encoding());
	QTextCodec *codec = QTextCodec::codecForName(spell_encoding.toLatin1());
	encodedString = codec->fromUnicode(zeile);
	pChecker->add(encodedString.data());
	emit addWord(zeile);
}
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/