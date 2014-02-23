/* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*/#include "bookreference.h"
#include "eventeditor.h"
#include "eventretriever.h"
#include "bookdetails.h"
#include "referencebookutil.h"
#include <QCompleter>
BookReference::BookReference(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	logSession=LoginUserSession::session();
	connect(ui.btnAdd, SIGNAL(clicked()),this,SLOT(btnAdd_Clicked()));
}

void BookReference::setTabIndex(int index_)
{
	tabIndex=index_;
	completer();
	exec();
}
void BookReference::setRowTime(QString time)
{
	currentRowTime=time;
}
void BookReference::btnAdd_Clicked()
{
	BookDetails *b=static_cast<BookDetails*>(this->parent());

	if (tabIndex==0)
	{	
		EventEditor *editor=new EventEditor(this);
		editor->attachBook(ui.txtBookName->text(),ui.txtAuthorName->text(),ui.txtBookDescription->text(),currentRowTime,ui.txtEdition->text());
	}
	else
	{
 		EventRetriever *retriever=new EventRetriever(this);
 		retriever->attachBook(ui.txtBookName->text(),ui.txtAuthorName->text(),ui.txtBookDescription->text(),currentRowTime,ui.txtEdition->text());
	}
	b->loadBookDetails(ui.txtBookName->text(),ui.txtAuthorName->text(),ui.txtEdition->text(),ui.txtBookDescription->text());
	ui.txtBookName->clear();
	ui.txtAuthorName->clear();
	ui.txtBookDescription->clear();
	ui.txtEdition->clear();	
}
void BookReference::completer()
{
	ReferenceBookUtil *rbook=new ReferenceBookUtil(this); 

	QList<ReferenceBookinfo> wordList=rbook->selectAllReferenceBook(logSession->getDriverName());
	QStringList nameList,authorList;
	foreach(ReferenceBookinfo rinfo,wordList)
	{
		nameList<<rinfo.getBookName();
		authorList<<rinfo.getAuthor();
	}
	QCompleter *nameCompleter = new QCompleter(nameList,NULL);
	nameCompleter->setCompletionMode(QCompleter::PopupCompletion);
	nameCompleter->setCaseSensitivity(Qt::CaseInsensitive);
	ui.txtBookName->setCompleter(nameCompleter);

	QCompleter *authorCompleter = new QCompleter(authorList,NULL);
	authorCompleter->setCompletionMode(QCompleter::PopupCompletion);
	authorCompleter->setCaseSensitivity(Qt::CaseInsensitive);
	ui.txtAuthorName->setCompleter(authorCompleter);
}
BookReference::~BookReference()
{

}
