/*Owner & Copyrights: Vance King Saxbe. A.*/
/*
*
* Copyright (c) 2007-2008 Softnotions Technologies Pvt Ltd. All Rights Reserved
* This software is the confidential and proprietary information of Softnotions Technologies Pvt Ltd.
*("Confidential Information"). You shall not disclose such Confidential Information 
*and shall use it only in accordance with the terms of the license agreement you 
*entered into with Softnotions.
*
*/


/*
* History
* @Author : Softnotions Technologies
* Dated   : 11-2-2010
* version : 5.3.1
*/
#ifndef SPELLTEXTEDIT_H_
#define SPELLTEXTEDIT_H_

#include <QTextEdit>
#include <QAction>
#include <QContextMenuEvent>
#include "hunspell/hunspell.hxx"
#include "highlighter.h"
#include <QTextTable>
#include <QTextTableFormat>
#include <QTextCursor>
#include <QTime>
#include <QLocale>
#include <QGridLayout>

class SpellTextEdit : public QTextEdit
{
	Q_OBJECT

//Public Variables

public:
    SpellTextEdit(QWidget *parent = 0,QString SpellDic="");
	~SpellTextEdit();
	
	QTextTable *table;
	QLocale con;
	QTextCursor cursor;
	QTextTableFormat tableFormat;
	QVector<QTextLength> constraints;
	QTextCharFormat format;
	QTextCharFormat boldFormat;
	int i;
	int curPosition;
	QGridLayout *mainLayout;
	Highlighter *highlighter;

//Public Functions

	QStringList getWordPropositions(const QString word);
	bool setDict(const QString SpellDic);
	void insertitem(QString,QString);
	void Spellapply();
	void CursorPositionChanged();

//Signals

signals:
	void addWord(QString word);
	void checkedText();
	void applySpellCheck(QString,QString);

//Protected Functions

protected:
	void createActions();
    void contextMenuEvent(QContextMenuEvent *event);
	void keyPressEvent(QKeyEvent *e);

//Private slot functions

private slots:
	void correctWord();
	void slot_addWord();
	void slot_ignoreWord();
	
//Private Variables

private:
    enum { MaxWords = 5 };
    QAction *misspelledWordsActs[MaxWords];

    QString spell_dic;
    Hunspell *pChecker;

    QPoint lastPos;

    QStringList addedWords;
};

#endif /*SPELLTEXTEDIT_H_*/
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/