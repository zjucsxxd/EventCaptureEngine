/*Owner & Copyrights: Vance King Saxbe. A.*//* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*/#include "uispellchecker.h"

#include "SpellTextEdit.h"
#include "dlcrmain.h"

#include <QTableWidgetItem>

SpellTextEdit *editor;

SpellChecker::SpellChecker(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	
	editor=new SpellTextEdit(ui.frameEditor);
	logSession=LoginUserSession::session();
	ui.gridLayout_2->addWidget(editor, 0, 0, 1, 1);

	connect(ui.btnApply, SIGNAL(clicked()), this,SLOT(btnApply_clicked()));
}

void SpellChecker::setText(QString str_)
{	
	editor->setText(str_);	
	if(logSession->getLanguageIndex()==0) editor->setDict("en_GB.dic");
	else editor->setDict("ar_AE.dic");
}
void SpellChecker::btnApply_clicked()
{
	DLCRMain *dlcrs=static_cast<DLCRMain*>(this->parent());
	
	if(dlcrs->ui.tabEvents->currentIndex()==0)
	{
		dlcrs->ui.activeEditor->ui.tblEventEditor->currentItem()->setText(editor->toHtml());
	}
	else
	{		
		dlcrs->ui.activeRetriever->ui.tblEventRetriever->currentItem()->setText(editor->toHtml());		
	}

	this->close();
}
SpellChecker::~SpellChecker()
{

}
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/