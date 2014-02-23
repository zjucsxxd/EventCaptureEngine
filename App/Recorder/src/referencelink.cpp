/* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*/#include "referencelink.h"
#include "eventeditor.h"
#include "eventretriever.h"
#include "linkdetails.h"
#include <QDesktopServices>
#include <QUrl>

ReferenceLink::ReferenceLink(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.btnOk, SIGNAL(clicked()),this,SLOT(btnOk_Clicked()));
	connect(ui.btnTest, SIGNAL(clicked()),this,SLOT(btnTest_Clicked()));
}

void ReferenceLink::setTabIndex(int index_)
{
	tabIndex=index_;
	exec();
}
void ReferenceLink::setRowTime(QString time)
{
	currentRowTime=time;
	

}
void ReferenceLink::btnOk_Clicked()
{
	LinkDetails *b=static_cast<LinkDetails*>(this->parent());
	if (tabIndex==0)
	{	
		EventEditor *editor=new EventEditor(this);
		editor->attachLink(ui.txtURL->text(),ui.txtDescription->text(),currentRowTime);
	}
	else
	{
		EventRetriever *retriever=new EventRetriever(this);
		retriever->attachLink(ui.txtURL->text(),ui.txtDescription->text(),currentRowTime);
	}

	b->loadLinkDetails(ui.txtURL->text(),ui.txtDescription->text());
	ui.txtURL->clear();
	ui.txtDescription->clear();
	
}

void ReferenceLink::btnTest_Clicked()
{
	QString url=ui.txtURL->text();
	QDesktopServices::openUrl(QUrl("http://"+url));

}
ReferenceLink::~ReferenceLink()
{

}
