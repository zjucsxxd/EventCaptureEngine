/* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*/#include "replace.h"
#include "eventeditor.h"
#include "textedit.h"
Replace::Replace(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.btnReplace, SIGNAL(clicked()),this,SLOT(btnReplace_Clicked()));
}

void Replace::loadSelectedText(QString selString)
{
	ui.txtSelected->setText(selString);
	//exec();	
}
void Replace::btnReplace_Clicked()
{
	txtToReplace=ui.txtReplace->text();
	searchStr=ui.txtSelected->text();
	close();	

}
QString Replace::replaceTxt()
{
	return txtToReplace;
	
}
QString Replace::searchTxt()
{
	return searchStr;

}
Replace::~Replace()
{

}
