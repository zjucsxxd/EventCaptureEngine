/* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*/#include "find.h"
#include "eventeditor.h"
Find::Find(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.btnFind, SIGNAL(clicked()),this,SLOT(btnFind_Clicked()));
}

void Find::btnFind_Clicked()
{
	 findstr=ui.txtFind->text();
	 close();	
}

QString Find::findText()
{
	return findstr;
}
Find::~Find()
{

}
