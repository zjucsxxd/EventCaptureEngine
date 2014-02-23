/* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*/#include "goto.h"
#include "dlcrmain.h"

#include <QRegExp>
#include <QValidator>

Goto::Goto(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.btnGoto, SIGNAL(clicked()),this,SLOT(btnGoto_Clicked()));

	QRegExp numberField("^[0-9]+$");
	QValidator *numberValidator = new QRegExpValidator(numberField, this);

	ui.txtGoto->setValidator(numberValidator);
}

void Goto::btnGoto_Clicked()
{
	DLCRMain *m=static_cast<DLCRMain*>(this->parent());

	if (tabIndex==1)
	{
		m->toGoToRetrieverCount(ui.txtGoto->text().toInt());
	}
	else
	{
		m->toGoToEditorCount(ui.txtGoto->text().toInt());
	}
	
	close();	
}

void Goto::setTabIndex(int ind_)
{
	tabIndex=ind_;
	exec();
}

Goto::~Goto()
{

}
