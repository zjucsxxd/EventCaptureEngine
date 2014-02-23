/* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*/#include "languagemenu.h"

LanguageMenu::LanguageMenu(QWidget *parent)
	: QAction(parent)
{
	connect(this, SIGNAL(triggered(bool)),this,SLOT(actionLanguage_clicked(bool)));
}
void LanguageMenu::actionLanguage_clicked(bool staus)
{
	emit LAction(this);
}
LanguageMenu::~LanguageMenu()
{

}
